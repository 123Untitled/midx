#ifndef core_string_chars_encoder_hpp
#define core_string_chars_encoder_hpp

#include "core/utility/type_operations.hpp"
#include "core/string/chars_segment.hpp"
#include "core/type_traits/index_sequence.hpp"

//#include "core/config/class.hpp"
#include "core/type_traits/type_at.hpp"
#include <concepts>



// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* chars encodable
	 * This concept checks if a type can be encoded to characters using the
	 * chars_segment class. It requires that the type can be converted to
	 * a chars_segment and that the segment can provide its size and encode
	 * method.
	 */
	template <typename T>
	concept chars_encodable = requires(T&& t, const mx::chars_segment<T>& segment, char* dst) {

		// construct constraint
		{
			mx::chars_segment<T>{mx::forward<T>(t)}
		} -> mx::is_same<mx::chars_segment<T>>;

		// size constraint
		{
			segment.size()
		} -> std::convertible_to<mx::usz>;

		// encode constraint
		{
			segment.encode(dst)
		} -> std::convertible_to<typename mx::usz>;

		// encode (range) constraint
		{
			segment.encode(dst, dst)
		} -> std::convertible_to<mx::usz>;
	};


	/* nothrow chars encodable
	 * This concept checks if a type can be encoded to characters without throwing
	 * exceptions. It requires that the type can be converted to a chars_segment
	 * and that the segment can provide its size and encode method without throwing.
	 */
	template <typename T>
	concept nothrow_chars_encodable = mx::chars_encodable<T> &&
		requires(T&& t, const mx::chars_segment<T>& segment, char* dst) {

		// size constraint
		{ segment.size() } noexcept;

		// encode constraint
		{ segment.encode(dst) } noexcept;

		// encode (range) constraint
		{ segment.encode(dst, dst) } noexcept;


		// + add noexcept construct
	};



	// -- T O  C H A R S  E N C O D E R ---------------------------------------

	template <typename... Tp>
	class chars_encoder final {


		// -- assertions ------------------------------------------------------

		/* requires character type and encodable types */
		static_assert((mx::chars_encodable<mx::remove_cvr<Tp>> && ...),
					  "All arguments must be encodable to characters");

		/* requires nothrow encodable types */
		static_assert((mx::nothrow_chars_encodable<mx::remove_cvr<Tp>> && ...),
					  "All arguments must be nothrow encodable to characters");


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::chars_encoder<Tp...>;

			/* sequence type */
			using make_sequence = ms::make_index_sequence<sizeof...(Tp)>;

			/* type at */
			template <unsigned I>
			using type_at = ms::type_at<I, Tp...>;

			/* segment type */
			template <typename T>
			using segment_type = mx::chars_segment<mx::remove_cvr<T>>;

			/* segment at */
			template <unsigned I>
			using segment_at = segment_type<type_at<I>>;



			// -- private structs ---------------------------------------------

			template <size_t I, typename T>
			struct element {
				segment_type<T> segment;
				element(void) = delete;
				template <typename U>
				constexpr explicit element(U&& arg) noexcept
				: segment{mx::forward<U>(arg)} {
				}
			};

			/* element at */
			template <unsigned I>
			using element_at = element<I, type_at<I>>;


			template <typename>
			struct storage;

			template <unsigned... Is>
			struct storage<ms::index_sequence<Is...>> final : element<Is, Tp>... {
				storage(void) = delete;
				template <typename... Ts>
				constexpr explicit storage(Ts&&... args) noexcept
				: element<Is, Tp>{mx::forward<Ts>(args)}... {
				}

			};


			template <unsigned I>
			static constexpr auto get(const storage<make_sequence>& st) noexcept -> const segment_at<I>& {
				return static_cast<const element_at<I>&>(st).segment;
			}


			// -- private members ---------------------------------------------

			/* storage for chars_segment */
			storage<make_sequence> _storage;


			// -- private methods ---------------------------------------------

			/* size */
			template <unsigned... Is>
			constexpr auto _size(ms::index_sequence<Is...>) const noexcept -> mx::usz {

				// total size
				mx::usz total = 0U;

				// calculate total size
				((total += self::get<Is>(_storage).size()), ...);

				return total;
			}

			/* encode */
			template <unsigned... Is>
			constexpr auto _encode(ms::index_sequence<Is...>, char* dst) const noexcept -> mx::usz {

				// current offset
				mx::usz offset = 0U;

				// encode each segment to the destination
				((offset += self::get<Is>(_storage).encode(dst + offset)), ...);

				return offset; // return total size written
			}




			/* encode (range) */
			template <unsigned I> requires (I < sizeof...(Tp) - 1U)
			constexpr auto _encode(char* begin, char* end) const noexcept -> mx::usz {

				const auto written = self::get<I>(_storage).encode(begin, end);

				begin += written;
				return (begin != end)
					? written + self::_encode<I + 1U>(begin, end)
					: written;
			}

			template <unsigned I> requires (I == sizeof...(Tp) - 1U)
			constexpr auto _encode(char* begin, char* end) const noexcept -> mx::usz {
				return self::get<I>(_storage).encode(begin, end);
			}


			template <typename... Ts>
			static constexpr bool is_single_self =
				(sizeof...(Tp) == 1U) && (mx::is_same<self, mx::remove_cvr<Ts>> && ...);


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			chars_encoder(void) = delete;

			/* variadic constructor */
			template <typename... Ts> requires (not is_single_self<Ts...>)
											&& (sizeof...(Ts) > 0U)
			constexpr explicit chars_encoder(Ts&&... args) noexcept
			: _storage{mx::forward<Ts>(args)...} {
			}

			/* copy constructor */
			constexpr chars_encoder(const self&) noexcept = default;

			/* move constructor */
			constexpr chars_encoder(self&&) noexcept = default;

			/* destructor */
			constexpr ~chars_encoder(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			constexpr auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			constexpr auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			/* size */
			constexpr auto size(void) const noexcept -> mx::usz {
				return self::_size(make_sequence{});
			}

			/* encode */
			constexpr auto encode(char* dst) const noexcept -> mx::usz {
				return self::_encode(make_sequence{}, dst);
			}

			/* encode (range) */
			constexpr auto encode(char* begin,
								  char* end) const noexcept -> mx::usz {
				return self::_encode<0U>(begin, end);
			}

	}; // class chars_encoder


	/* empty specialization
	 * This specialization is used when no arguments are provided to the
	 * chars_encoder. It provides a default empty implementation.
	 */
	template <>
	class chars_encoder<> final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::chars_encoder<>;


		public:

			// -- public lifecycle --------------------------------------------

			/* noexcept default class */
			noexcept_default_class(chars_encoder);


			// -- public methods ----------------------------------------------

			constexpr auto size(void) const noexcept -> mx::usz {
				return 0U; // nothing to encode
			}

			constexpr auto encode(char*) const noexcept -> mx::usz {
				return 0U; // nothing to encode
			}

			constexpr auto encode(char*, char*) const noexcept -> mx::usz {
				return 0U; // nothing to encode
			}

	}; // class chars_encoder



	// -- I S  C H A R S  E N C O D E R ---------------------------------------

	namespace impl {

		template <typename T>
		struct is_chars_encoder final {
			static constexpr bool value = false;
			non_instantiable(is_chars_encoder);
		};

		template <typename... Tp>
		struct is_chars_encoder<mx::chars_encoder<Tp...>> final {
			static constexpr bool value = true;
			non_instantiable(is_chars_encoder);
		};
	}

	/* is chars_encoder */
	template <typename T>
	concept is_chars_encoder = mx::impl::is_chars_encoder<mx::remove_cvr<T>>::value;

} // namespace mx

#endif // core_string_chars_encoder_hpp
