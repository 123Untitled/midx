#ifndef language_ast_hpp
#define language_ast_hpp

#include "language/tokens.hpp"



// -- A S T  N A M E S P A C E ------------------------------------------------

namespace ast {


	// -- forward declarations ------------------------------------------------

	//class block;
	//
	//
	//// -- V A L U E -----------------------------------------------------------
	//
	//class value final {
	//
	//	private:
	//
	//		enum : ml::usz {
	//			vl_nested,
	//			vl_token
	//		} _type;
	//
	//		union {
	//			ml::usz   nested; // block index
	//			tk::token* value;
	//		} _data;
	//
	//
	//	public:
	//
	//		// -- public lifecycle --------------------------------------------
	//
	//
	//		explicit value(const ml::usz n) noexcept
	//		: _type{vl_nested}, _data{.nested = n} {
	//		}
	//
	//		explicit value(tk::token* v) noexcept
	//		: _type{vl_token}, _data{.value = v} {
	//		}
	//
	//
	//		// -- public methods ----------------------------------------------
	//
	//		auto debug(void) const -> void {
	//			if (_type == vl_token)
	//				std::cout << *(_data.value) << " ";
	//			else
	//				std::cout << "[nested block #" << _data.nested << "] ";
	//		}
	//
	//}; // class value
	//
	//
	//// -- P A R A M -----------------------------------------------------------
	//
	//class param final {
	//
	//
	//	private:
	//
	//		// -- private members ---------------------------------------------
	//
	//		tk::token* _name;
	//		std::vector<ast::value> _values;
	//
	//
	//	public:
	//
	//		// -- public lifecycle --------------------------------------------
	//
	//		explicit param(tk::token* n) noexcept
	//		: _name{n}, _values{} {
	//		}
	//
	//
	//		// -- public methods ----------------------------------------------
	//
	//		auto new_value(const ml::usz n) -> void {
	//			_values.emplace_back(n);
	//		}
	//		auto new_value(tk::token* v) -> void {
	//			_values.emplace_back(v);
	//		}
	//
	//		auto remove_last_value(void) -> void {
	//			if (_values.empty()) {
	//				throw std::runtime_error("no values in parameter");
	//			}
	//			_values.pop_back();
	//		}
	//
	//		auto debug(void) const -> void {
	//			std::cout << "    PARAM: ";
	//			if (_name == nullptr) {
	//				std::cout << "//////\n";
	//			} else {
	//				std::cout << *_name << "\n";
	//			}
	//			for (const auto& v : _values) {
	//				std::cout << "        VALUE: ";
	//				v.debug();
	//				std::cout << "\n";
	//			}
	//		}
	//
	//}; // class param
	//
	//
	//// -- B L O C K -----------------------------------------------------------
	//
	//class block final {
	//
	//
	//	private:
	//
	//		// -- private members ---------------------------------------------
	//
	//		tk::token* _name;
	//
	//		enum : ml::u8 {
	//			id_anonymous,
	//			id_token
	//		} _id_type;
	//
	//		union {
	//			ml::usz anonymous;
	//			tk::token* token;
	//		} _id;
	//
	//		std::vector<ast::param> _params;
	//
	//
	//	public:
	//
	//		// -- public lifecycle --------------------------------------------
	//
	//		/* default constructor */
	//		block(void) noexcept
	//		: _name{nullptr}, _id_type{id_anonymous}, _id{0U}, _params{} {
	//		}
	//
	//
	//		// -- public methods ----------------------------------------------
	//
	//		auto set_name(tk::token* n) noexcept -> void {
	//			_name = n;
	//		}
	//
	//		auto set_anonymous(const ml::usz id) noexcept -> void {
	//			_id_type = id_anonymous;
	//			_id.anonymous = id;
	//		}
	//
	//		auto set_id(tk::token* t) noexcept -> void {
	//			_id_type = id_token;
	//			_id.token = t;
	//		}
	//
	//		auto new_param(tk::token* name) -> void {
	//			_params.emplace_back(name);
	//		}
	//
	//		auto last_param(void) -> ast::param& {
	//			if (_params.empty()) {
	//				throw std::runtime_error("no parameters in block");
	//			}
	//			return _params.back();
	//		}
	//
	//		auto remove_last_value(void) -> void {
	//			if (_params.empty()) {
	//				throw std::runtime_error("no parameters in block");
	//			}
	//			auto& p = _params.back();
	//			p.remove_last_value();
	//		}
	//
	//		auto debug(void) const -> void {
	//
	//			std::cout << "BLOCK: ";
	//			if (_name == nullptr) {
	//				std::cout << "////// ";
	//			} else {
	//				std::cout << *_name << " ";
	//			}
	//			if (_id_type == id_anonymous) {
	//				std::cout << "__anon_" << _id.anonymous;
	//			} else {
	//				std::cout << *(_id.token);
	//			}
	//			std::cout << "\n";
	//
	//			for (const auto& p : _params) {
	//				p.debug();
	//			}
	//			// debug output
	//		}
	//};


	class value final {

		private:
		public:


			enum : ml::usz {
				vl_nested,
				vl_token
			} _type;

			union {
				ml::usz   nested; // block index
				tk::token* value;
			} _data;

			ml::usz _parent_param;

		public:

			// -- public lifecycle --------------------------------------------


			explicit value(const ml::usz bi, const ml::usz pp) noexcept
			: _type{vl_nested}, _data{.nested = bi}, _parent_param{pp} {
			}

			explicit value(tk::token* v, const ml::usz pp) noexcept
			: _type{vl_token}, _data{.value = v}, _parent_param{pp} {
			}


			// -- public methods ----------------------------------------------

			auto debug(void) const -> void {
				std::cout << "        " << _parent_param << " VALUE: ";
				if (_type == vl_token)
					std::cout << *(_data.value) << '\n';
				else
					std::cout << "[nested block #" << _data.nested << "]\n";
			}

	}; // class value


	// -- P A R A M -----------------------------------------------------------

	class param final {


		private:
		public:

			// -- private members ---------------------------------------------

			tk::token* _token;
			ml::usz _parent_block;


		public:

			// -- public lifecycle --------------------------------------------

			explicit param(tk::token* tk, const ml::usz pb) noexcept
			: _token{tk}, _parent_block{pb} {
			}


			// -- public methods ----------------------------------------------

			auto debug(void) const -> void {
				std::cout << "    " << _parent_block << " PARAM: ";
				if (_token == nullptr) {
					std::cout << "//////\n";
				} else {
					std::cout << *_token << "\n";
				}
			}

	}; // class param


	// -- B L O C K -----------------------------------------------------------

	class block final {


		private:

			// -- private members ---------------------------------------------

			tk::token* _specifier;

			enum : ml::u8 {
				id_anonymous,
				id_token
			} _id_type;

			union {
				ml::usz anonymous;
				tk::token* token;
			} _identifier;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			block(void) noexcept
			: _specifier{nullptr}, _id_type{id_anonymous}, _identifier{0U} {
			}


			// -- public methods ----------------------------------------------

			auto specifier(tk::token* tk) noexcept -> void {
				_specifier = tk;
			}

			auto anonymous(const ml::usz id) noexcept -> void {
				_id_type = id_anonymous;
				_identifier.anonymous = id;
			}

			auto identifier(tk::token* tk) noexcept -> void {
				_id_type = id_token;
				_identifier.token = tk;
			}


			auto debug(void) const -> void {

				std::cout << "BLOCK: ";
				if (_specifier == nullptr) {
					std::cout << "////// ";
				} else {
					std::cout << *_specifier << " ";
				}
				if (_id_type == id_anonymous) {
					std::cout << "__anon_" << _identifier.anonymous;
				} else {
					std::cout << *(_identifier.token);
				}
				std::cout << "\n";

				// debug output
			}
	};


	// other approaches:

		static std::vector<ast::block> blocks;
		static std::vector<ast::param> params;
		static std::vector<ast::value> values;
		static std::vector<ml::usz> params_order;
		static std::vector<ml::usz> values_order;


		inline auto clear(void) noexcept -> void {
			blocks.clear();
			params.clear();
			values.clear();
		}

		inline auto params_sort(void) -> void {
			params_order.resize(params.size());

			for (ml::usz i = 0U; i < params.size(); ++i) {
				params_order[i] = i;
			}
			//params_order[0U] = 0U;

			for (ml::isz i = 1U; i < (ml::isz)params.size(); ++i) {
				//params_order[i] = i;

				auto key = params_order[i];
				ml::isz j = i - 1U;

				while (j >= 0 && params[params_order[j]]._parent_block > params[key]._parent_block) {
					params_order[j + 1U] = params_order[j];
					--j;
				}

				params_order[j + 1U] = key;
			}
		}

		inline auto values_sort(void) -> void {
			values_order.resize(values.size());

			for (ml::usz i = 0U; i < values.size(); ++i) {
				values_order[i] = i;
			}
			//values_order[0U] = 0U;

			for (ml::isz i = 1U; i < (ml::isz)values.size(); ++i) {
				//values_order[i] = i;

				auto key = values_order[i];
				ml::isz j = i - 1U;

				while (j >= 0 && values[values_order[j]]._parent_param > values[key]._parent_param) {
					values_order[j + 1U] = values_order[j];
					--j;
				}

				values_order[j + 1U] = key;
			}
		}


		inline auto debug(void) -> void {

			ml::usz bi = 0U;
			ml::usz pi = 0U;
			ml::usz vi = 0U;

			for (; bi < blocks.size(); ++bi) {
				blocks[bi].debug();

				while (pi < params.size() && params[params_order[pi]]._parent_block == bi) {

					auto& p = params[params_order[pi]];
					p.debug();

					while (vi < values.size() && values[values_order[vi]]._parent_param == pi) {

						auto& v = values[values_order[vi]];
						v.debug();
						++vi;
					}
					++pi;
				}
			}
		}





} // namespace ast

#endif // language_ast_hpp
