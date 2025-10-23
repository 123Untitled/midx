#ifndef ml_language_action_table_hpp
#define ml_language_action_table_hpp

#include "char_type.hpp"
#include "state_driver.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	using sd = ml::state_driver;


	template <sd::action_type...>
	struct end_of_chunk final {
		constexpr end_of_chunk(void) noexcept = default;
	};

	template <sd::action_type...>
	struct end_of_file final {
		constexpr end_of_file(void) noexcept = default;
	};
	struct eof final {
		constexpr eof(void) noexcept = default;
	};
	struct eoc final {
		constexpr eoc(void) noexcept = default;
	};


	template <ml::u8, ml::u8>
	struct range final {
		constexpr range(void) noexcept = default;
	};

	template <ml::u8...>
	struct set final {
		constexpr set(void) noexcept = default;
	};

	template <typename...>
	struct on_chars final {
		constexpr on_chars(void) noexcept = default;
	};

	//template <typename, typename>
	template <typename, sd::action_type...>
	struct add final {
		constexpr add(void) noexcept = default;
	};


	using whitespace = ml::set<' ', '\t'>;
	using control  = ml::on_chars<ml::range<0x00, 0x20>, ml::set<0x7f>>;
	using extended = ml::on_chars<ml::range<0x80, 0xff>>;



	// -- A C T I O N  T A B L E ----------------------------------------------

	class action_table final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::action_table;


			// -- public constants --------------------------------------------

			/* table size */
			static constexpr ml::usz _tsize = ml::global_char::size;



			// -- private members ---------------------------------------------

			/* action table */
			sd::action_type _table[_tsize];

			/* end of chunk */
			sd::action_type _eoc;

			/* end of file */
			sd::action_type _eof;




			template <typename T, sd::action_type... As>
			struct add_helper final {


				// -- on_chars extractor --------------------------------------

				template <typename>
				struct on_chars_extractor;

				template <typename... Tp>
				struct on_chars_extractor<ml::on_chars<Tp...>> final {


					template <typename>
					struct dispatch;

					template <ml::u8... Cs>
					struct dispatch<ml::set<Cs...>> final {

						static consteval auto fill(self& s) noexcept -> void {
							((s._table[ml::global_char::table[Cs]] = &sd::call<As...>), ...);
						}
					};

					template <ml::u8 S, ml::u8 E>
					struct dispatch<ml::range<S, E>> final {

						static consteval auto fill(self& s) noexcept -> void {
							for (ml::u8 c = S; c < E; ++c)
								s._table[ml::global_char::table[c]] = &sd::call<As...>;
						}
					};

					template <>
					struct dispatch<ml::eoc> final {

						static consteval auto fill(self& s) noexcept -> void {
							s._eoc = &sd::call<As...>;
						}
					};

					template <>
					struct dispatch<ml::eof> final {

						static consteval auto fill(self& s) noexcept -> void {
							s._eof = &sd::call<As...>;
						}
					};


					static consteval auto extract(self& s) noexcept -> void {
						(dispatch<Tp>::fill(s), ...);
					}
				};

				template <ml::u8... Cs>
				struct on_chars_extractor<ml::set<Cs...>> final {

					static consteval auto extract(self& s) noexcept -> void {
						((s._table[ml::global_char::table[Cs]] = &sd::call<As...>), ...);
					}
				};

				template <ml::u8 S, ml::u8 E>
				struct on_chars_extractor<ml::range<S, E>> final {

					static consteval auto extract(self& s) noexcept -> void {
						for (ml::u8 c = S; c < E; ++c)
							s._table[ml::global_char::table[c]] = &sd::call<As...>;
					}
				};


				static consteval auto fill(self& s) noexcept -> void {
					on_chars_extractor<T>::extract(s);
				}

			}; // struct add_helper


		public:


			template <typename T, sd::action_type... As>
			consteval auto dispatch(const ml::add<T, As...>&) noexcept -> void {
				add_helper<T, As...>::fill(*this);
			}

			template <sd::action_type... As>
			consteval auto dispatch(const ml::end_of_chunk<As...>&) noexcept -> void {
				_eoc = &sd::call<As...>;
			}

			template <sd::action_type... As>
			consteval auto dispatch(const ml::end_of_file<As...>&) noexcept -> void {
				_eof = &sd::call<As...>;
			}

			consteval auto dispatch(const self& other) noexcept -> void {
				// copy table
				for (ml::usz i = 0U; i < _tsize; ++i)
					_table[i] = other._table[i];
				_eoc = other._eoc;
				_eof = other._eof;
			}


		public:


			template <typename... Tp>
			constexpr explicit action_table(const Tp&... args) noexcept
			: /* uninitialized */
			  _eoc{&sd::call<>},
			  _eof{&sd::call<>} {

				// initialize table
				for (auto& a : _table)
					a = &sd::call<>;

				((dispatch(args)), ...);
			}


			auto execute(sd& driver, const ml::u8 byte) const /* noexcept */-> void {
				const auto ct = ml::global_char::table[byte];
				(driver.*_table[ct])();
			}
			auto end_of_chunk(sd& driver) const /* noexcept */ -> void {
				(driver.*_eoc)();
			}
			auto end_of_file(sd& driver) const /* noexcept */ -> void {
				(driver.*_eof)();
			}

	}; // class action_table


	/* make table */
	template <typename... Tp>
	consteval auto make_table(void) noexcept -> ml::action_table {
		return ml::action_table{Tp{}...};
	}

} // namespace ml

#endif //ml_language_action_table_hpp
