#ifndef language_ast_hpp
#define language_ast_hpp

#include "language/tokens.hpp"



// -- A S T  N A M E S P A C E ------------------------------------------------

namespace ast {


	// -- forward declarations ------------------------------------------------

	class block;


	// -- V A L U E -----------------------------------------------------------

	class value final {

		private:

			enum : ml::usz {
				vl_nested,
				vl_token
			} _type;

			union {
				ml::usz   nested; // block index
				tk::token* value;
			} _data;


		public:

			// -- public lifecycle --------------------------------------------


			explicit value(const ml::usz n) noexcept
			: _type{vl_nested}, _data{.nested = n} {
			}

			explicit value(tk::token* v) noexcept
			: _type{vl_token}, _data{.value = v} {
			}


			// -- public methods ----------------------------------------------

			auto debug(void) const -> void {
				if (_type == vl_token)
					std::cout << *(_data.value) << " ";
				else
					std::cout << "[nested block #" << _data.nested << "] ";
			}

	}; // class value


	// -- P A R A M -----------------------------------------------------------

	class param final {


		private:

			// -- private members ---------------------------------------------

			tk::token* _name;
			std::vector<ast::value> _values;


		public:

			// -- public lifecycle --------------------------------------------

			explicit param(tk::token* n) noexcept
			: _name{n}, _values{} {
			}


			// -- public methods ----------------------------------------------

			auto new_value(const ml::usz n) -> void {
				_values.emplace_back(n);
			}
			auto new_value(tk::token* v) -> void {
				_values.emplace_back(v);
			}

			auto remove_last_value(void) -> void {
				if (_values.empty()) {
					throw std::runtime_error("no values in parameter");
				}
				_values.pop_back();
			}

			auto debug(void) const -> void {
				std::cout << "    PARAM: ";
				if (_name == nullptr) {
					std::cout << "//////\n";
				} else {
					std::cout << *_name << "\n";
				}
				for (const auto& v : _values) {
					std::cout << "        VALUE: ";
					v.debug();
					std::cout << "\n";
				}
			}

	}; // class param


	// -- B L O C K -----------------------------------------------------------

	class block final {


		private:

			// -- private members ---------------------------------------------

			tk::token* _name;

			enum : ml::u8 {
				id_anonymous,
				id_token
			} _id_type;

			union {
				ml::usz anonymous;
				tk::token* token;
			} _id;

			std::vector<ast::param> _params;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			block(void) noexcept
			: _name{nullptr}, _id_type{id_anonymous}, _id{0U}, _params{} {
			}


			// -- public methods ----------------------------------------------

			auto set_name(tk::token* n) noexcept -> void {
				_name = n;
			}

			auto set_anonymous(const ml::usz id) noexcept -> void {
				_id_type = id_anonymous;
				_id.anonymous = id;
			}

			auto set_id(tk::token* t) noexcept -> void {
				_id_type = id_token;
				_id.token = t;
			}

			auto new_param(tk::token* name) -> void {
				_params.emplace_back(name);
			}

			auto last_param(void) -> ast::param& {
				if (_params.empty()) {
					throw std::runtime_error("no parameters in block");
				}
				return _params.back();
			}

			auto remove_last_value(void) -> void {
				if (_params.empty()) {
					throw std::runtime_error("no parameters in block");
				}
				auto& p = _params.back();
				p.remove_last_value();
			}

			auto debug(void) const -> void {

				std::cout << "BLOCK: ";
				if (_name == nullptr) {
					std::cout << "////// ";
				} else {
					std::cout << *_name << " ";
				}
				if (_id_type == id_anonymous) {
					std::cout << "__anon_" << _id.anonymous;
				} else {
					std::cout << *(_id.token);
				}
				std::cout << "\n";

				for (const auto& p : _params) {
					p.debug();
				}
				// debug output
			}
	};


	// other approaches:
	/*
	   struct block {

	   }

		std::vector<ast::block> _blocks;
		std::vector<ast::param> _params;
		std::vector<ast::value> _values;


	*/




} // namespace ast

#endif // language_ast_hpp
