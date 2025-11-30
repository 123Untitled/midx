#pragma once

#include "core/utility/type_operations.hpp"
#include "core/string/chars_segment.hpp"
#include "core/type_traits/index_sequence.hpp"

#include <concepts>
#include "core/config/class.hpp"
#include "core/type_traits/type_at.hpp"

#include <iostream>

// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	/* chars encodable
	 * This concept checks if a type can be encoded to characters using the
	 * chars_segment class. It requires that the type can be converted to
	 * a chars_segment and that the segment can provide its size and encode
	 * method.
	 */
	template <typename C, typename T>
	concept chars_encodable = requires(T&& t, const ms::chars_segment<C, T>& segment,
										typename ms::char_traits<C>::pointer dst) {

		// construct constraint
		{
			ms::chars_segment<C, T>{ms::forward<T>(t)}
		} -> mx::is_same<ms::chars_segment<C, T>>;

		// size constraint
		{
			segment.size()
		} -> std::convertible_to<typename ms::char_traits<C>::size_type>;

		// encode constraint
		{
			segment.encode(dst)
		} -> std::convertible_to<typename ms::char_traits<C>::size_type>;

		// encode (range) constraint
		{
			segment.encode(dst, dst)
		} -> std::convertible_to<typename ms::char_traits<C>::size_type>;
	};


	/* nothrow chars encodable
	 * This concept checks if a type can be encoded to characters without throwing
	 * exceptions. It requires that the type can be converted to a chars_segment
	 * and that the segment can provide its size and encode method without throwing.
	 */
	template <typename C, typename T>
	concept nothrow_chars_encodable = ms::chars_encodable<C, T> &&
		requires(T&& t, const ms::chars_segment<C, T>& segment,
				 typename ms::char_traits<C>::pointer dst) {

		// size constraint
		{ segment.size() } noexcept;

		// encode constraint
		{ segment.encode(dst) } noexcept;

		// encode (range) constraint
		{ segment.encode(dst, dst) } noexcept;


		// + add noexcept construct
	};



	// -- T O  C H A R S  E N C O D E R ---------------------------------------

	template <typename C, typename... Tp>
	class chars_encoder final {


		// -- assertions ------------------------------------------------------

		/* requires character type and encodable types */
		static_assert((ms::chars_encodable<C, ms::remove_cvr<Tp>> && ...),
					  "All arguments must be encodable to characters");

		/* requires nothrow encodable types */
		static_assert((ms::nothrow_chars_encodable<C, ms::remove_cvr<Tp>> && ...),
					  "All arguments must be nothrow encodable to characters");


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ms::chars_encoder<C, Tp...>;

			/* traits type */
			using traits = ms::char_traits<C>;

			/* sequence type */
			using make_sequence = ms::make_index_sequence<sizeof...(Tp)>;

			/* type at */
			template <unsigned I>
			using type_at = ms::type_at<I, Tp...>;

			/* segment type */
			template <typename T>
			using segment_type = ms::chars_segment<C, ms::remove_cvr<T>>;

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
				: segment{ms::forward<U>(arg)} {
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
				: element<Is, Tp>{ms::forward<Ts>(args)}... {
				}

			};


			template <unsigned I>
			static constexpr auto get(const storage<make_sequence>& _storage) noexcept -> const segment_at<I>& {
				return static_cast<const element_at<I>&>(_storage).segment;
			}


			// -- private members ---------------------------------------------

			/* storage for chars_segment */
			storage<make_sequence> _storage;


			// -- private methods ---------------------------------------------

			/* size */
			template <unsigned... Is>
			constexpr auto _size(ms::index_sequence<Is...>) const noexcept -> traits::size_type {

				// total size
				typename traits::size_type total = 0U;

				// calculate total size
				((total += self::get<Is>(_storage).size()), ...);

				return total;
			}

			/* encode */
			template <unsigned... Is>
			constexpr auto _encode(ms::index_sequence<Is...>, traits::pointer dst) const noexcept -> traits::size_type {

				// current offset
				typename traits::size_type offset = 0U;

				// encode each segment to the destination
				((offset += self::get<Is>(_storage).encode(dst + offset)), ...);

				return offset; // return total size written
			}




			/* encode (range) */
			template <unsigned I> requires (I < sizeof...(Tp) - 1U)
			constexpr auto _encode(traits::pointer begin, traits::pointer end) const noexcept -> traits::size_type {

				const auto written = self::get<I>(_storage).encode(begin, end);

				begin += written;
				return (begin != end)
					? written + self::_encode<I + 1U>(begin, end)
					: written;
			}

			template <unsigned I> requires (I == sizeof...(Tp) - 1U)
			constexpr auto _encode(traits::pointer begin, traits::pointer end) const noexcept -> traits::size_type {
				return self::get<I>(_storage).encode(begin, end);
			}


			template <typename... Ts>
			static constexpr bool is_single_self =
				(sizeof...(Tp) == 1U) && (mx::is_same<self, ms::remove_cvr<Ts>> && ...);


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			chars_encoder(void) = delete;

			/* variadic constructor */
			template <typename... Ts> requires (not is_single_self<Ts...>)
											&& (sizeof...(Ts) > 0U)
			constexpr explicit chars_encoder(Ts&&... args) noexcept
			: _storage{ms::forward<Ts>(args)...} {
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
			constexpr auto size(void) const noexcept -> traits::size_type {
				return self::_size(make_sequence{});
			}

			/* encode */
			constexpr auto encode(traits::pointer dst) const noexcept -> traits::size_type {
				return self::_encode(make_sequence{}, dst);
			}

			/* encode (range) */
			constexpr auto encode(traits::pointer begin,
								  traits::pointer end) const noexcept -> traits::size_type {
				return self::_encode<0U>(begin, end);
			}

	}; // class chars_encoder


	/* empty specialization
	 * This specialization is used when no arguments are provided to the
	 * chars_encoder. It provides a default empty implementation.
	 */
	template <typename C>
	class chars_encoder<C> final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ms::chars_encoder<C>;

			/* traits type */
			using traits = ms::char_traits<C>;


		public:

			// -- public lifecycle --------------------------------------------

			/* noexcept default class */
			noexcept_default_class(chars_encoder);


			// -- public methods ----------------------------------------------

			constexpr auto size(void) const noexcept -> traits::size_type {
				return 0U; // nothing to encode
			}

			constexpr auto encode(traits::pointer) const noexcept -> traits::size_type {
				return 0U; // nothing to encode
			}

			constexpr auto encode(traits::pointer, traits::pointer) const noexcept -> traits::size_type {
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

		template <typename C, typename... Tp>
		struct is_chars_encoder<ms::chars_encoder<C, Tp...>> final {
			static constexpr bool value = true;
			non_instantiable(is_chars_encoder);
		};
	}

	/* is chars_encoder */
	template <typename T>
	concept is_chars_encoder = ms::impl::is_chars_encoder<ms::remove_cvr<T>>::value;

} // namespace ms
