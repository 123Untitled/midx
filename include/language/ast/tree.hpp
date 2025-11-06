#ifndef language_ast_tree_hpp
#define language_ast_tree_hpp

#include "language/ast/block.hpp"
#include "language/ast/param.hpp"
#include "language/ast/value.hpp"

#include "language/ast/block_iterator.hpp"


#include <vector>


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- T R E E -------------------------------------------------------------

	class tree final {


		// -- friends ---------------------------------------------------------

		/* value_view as friend */
		friend class value_view;

		/* param_view as friend */
		friend class param_view;

		/* block_view as friend */
		friend class block_view;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = as::tree;


			// -- private members ---------------------------------------------

			/* blocks */
			std::vector<as::block> _blocks;

			/* params */
			std::vector<as::param> _params;

			/* values */
			std::vector<as::value> _values;

			/* temporary storage during parsing */
			std::vector<as::block> _blocks_tmp;
			std::vector<as::param> _params_tmp;
			std::vector<as::value> _values_tmp;


		public:

			// -- public lifecycle --------------------------------------------

			tree(void)
			: _blocks{},
			  _params{},
			  _values{},

			  _blocks_tmp{},
			  _params_tmp{},
			  _values_tmp{} {

				  _blocks.reserve( 16U);
				  _params.reserve( 64U);
				  _values.reserve(512U);

				  _blocks_tmp.reserve( 4U);
				  _params_tmp.reserve(16U);
				  _values_tmp.reserve(64U);
			}

			auto new_block(void) -> void {
				//std::cout << "\x1b[33mNEW BLOCK\x1b[0m\n";
				_blocks_tmp.emplace_back(_params_tmp.size());
			}

			auto new_param(tk::token* tk) -> void {
				_params_tmp.emplace_back(tk, _values_tmp.size());
				++(_blocks_tmp.back()._params_count);
			}

			auto new_value(tk::token* tk) -> void {
				_values_tmp.emplace_back(tk);
				++(_params_tmp.back()._values_count);
			}

			auto new_nested_block(void) -> void {
				//std::cout << "\x1b[33mNEW NESTED BLOCK\x1b[0m\n";
				_blocks_tmp.emplace_back(_params_tmp.size());
				// specifier from param
				_blocks_tmp.back().specifier(_params_tmp.back());
			}

			auto new_nested_value(const ml::usz n) -> void {
				_values_tmp.emplace_back(n);
				++(_params_tmp.back()._values_count);
			}

			auto remove_last_block(void) noexcept -> void {
				//std::cout << "BEFORE REMOVE LAST BLOCK:\n";
				//std::cout << " size of tmp_blocks: " << _blocks_tmp.size() << "\n";
				//std::cout << " size of tmp_params: " << _params_tmp.size() << "\n";
				//std::cout << " size of tmp_values: " << _values_tmp.size() << "\n";
				//std::cout << "REMOVE LAST BLOCK\n";
				const auto& b = _blocks_tmp.back();
					  auto pi = b._params_start;
				const auto pe = pi + b._params_count;

				ml::usz vc = 0U;
				for (; pi < pe; ++pi) {
					const auto& param = _params_tmp[pi];
					vc += param._values_count;
				}

				const ml::usz p_resize = _params_tmp.size() - b._params_count;
				const ml::usz v_resize = _values_tmp.size() - vc;

				_blocks_tmp.pop_back();
				_params_tmp.resize(p_resize);
				_values_tmp.resize(v_resize);

				//std::cout << "AFTER REMOVE LAST BLOCK:\n";
				//std::cout << " size of tmp_blocks: " << _blocks_tmp.size() << "\n";
				//std::cout << " size of tmp_params: " << _params_tmp.size() << "\n";
				//std::cout << " size of tmp_values: " << _values_tmp.size() << "\n";
			}



			auto flush(void) -> void {

				// copy block
				auto& b = _blocks.emplace_back(_blocks_tmp.back());

				ml::usz params_resize = _params_tmp.size() - b._params_count;
				ml::usz values_resize = _values_tmp.size();

				// get params tmp start
					  ml::usz pi = b._params_start;
				const ml::usz pe = b._params_count + pi;

				// assign new params start
				b._params_start = _params.size();

				//std::cout << "FLUSH BLOCK:\n";
				//std::cout << " size of tmp_blocks: " << _blocks_tmp.size() << "\n";
				//std::cout << " size of tmp_params: " << _params_tmp.size() << "\n";
				//std::cout << " size of tmp_values: " << _values_tmp.size() << "\n";
				//
				//std::cout << " PARAMS_START: " << b._params_start << "\n";
				//std::cout << " PARAMS_COUNT: " << b._params_count << "\n";

				for (; pi < pe; ++pi) {

					const auto& p = _params_tmp[pi];

					_params.emplace_back(
						// copy token
						p._token,
						// new values start
						_values.size(),
						// values count
						p._values_count
					);

					values_resize -= p._values_count;

						  ml::usz vi = p._values_start;
					const ml::usz ve = p._values_count + vi;
					for (; vi < ve; ++vi)
						_values.emplace_back(_values_tmp[vi]);
				}

				// remove last tmp block
				_blocks_tmp.pop_back();
				// resize tmp vectors
				_params_tmp.resize(params_resize);
				_values_tmp.resize(values_resize);

				// handle nested block index
				if (!_blocks_tmp.empty()) {
					++(_params_tmp.back()._values_count);
					_values_tmp.emplace_back(_blocks.size() - 1U);
				}
			}

			auto last_block(void) noexcept -> as::block& {
				return _blocks_tmp.back();
			}

			auto clear(void) noexcept -> void {
				_blocks.clear();
				_params.clear();
				_values.clear();

				_blocks_tmp.clear();
				_params_tmp.clear();
				_values_tmp.clear();
			}


			// -- public iterators --------------------------------------------

			/* begin */
			auto begin(void) const noexcept -> as::block_iterator {
				return as::block_iterator{*this, 0U};
			}

			/* end */
			auto end(void) const noexcept -> as::block_iterator {
				return as::block_iterator{*this, _blocks.size()};
			}


			// -- public accessors --------------------------------------------

			/* blocks */
			auto blocks(void) const noexcept -> const self& {
				return *this;
			}


			/* debug */
			auto debug(void) const -> void {

				const auto& t = *this;

				for (const auto& b : t.blocks()) {
					b.block().debug();

					for (const auto& p : b.params()) {
						p.param().debug();

						for (const auto& v : p.values()) {
							v.value().debug();
						}
					}
				}
			}

	}; // class tree

} // namespace as

#endif // language_ast_tree_hpp
