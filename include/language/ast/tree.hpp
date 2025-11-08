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

		/* block_iterator as friend */
		friend class block_iterator;

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

			/* block order */
			std::vector<ml::usz> _block_order;


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

				  _block_order.reserve(_blocks.capacity());
			}

			auto new_block(void) -> void {
				//std::cout << "\x1b[33mNEW BLOCK\x1b[0m\n";
				_blocks_tmp.emplace_back(_params_tmp.size());
			}

			auto new_param(tk::token& tk) -> void {
				const sp::id spec_id = _blocks_tmp.back().spec_id();
				_params_tmp.emplace_back(
						spec_id, tk,
						_values_tmp.size());
				++(_blocks_tmp.back().pc);
			}

			auto new_value(tk::token* tk) -> void {
				_values_tmp.emplace_back(tk);
				++(_params_tmp.back().vc);
			}

			auto new_nested_block(void) -> void {
				//std::cout << "\x1b[33mNEW NESTED BLOCK\x1b[0m\n";
				// specifier from param
				_blocks_tmp.emplace_back(_params_tmp.size(),
										 _params_tmp.back());
			}

			/* remove last block */
			auto remove_last_block(void) noexcept -> void {

				const auto& b = _blocks_tmp.back();
					  auto pi = b.ps;
				const auto pe = b.pc + pi;

				ml::usz vc = 0U;
				for (; pi < pe; ++pi) {
					const auto& param = _params_tmp[pi];
					vc += param.vc;
				}

				const ml::usz p_resize = _params_tmp.size() - b.pc;
				const ml::usz v_resize = _values_tmp.size() - vc;

				_blocks_tmp.pop_back();
				_params_tmp.resize(p_resize);
				_values_tmp.resize(v_resize);
			}



			auto flush(void) -> void {

				// insert block index in order vector
				_block_order.emplace_back(_blocks.size());

				// copy block
				auto& b = _blocks.emplace_back(_blocks_tmp.back());

				ml::usz params_resize = _params_tmp.size() - b.pc;
				ml::usz values_resize = _values_tmp.size();

				// get params tmp start
					  ml::usz pi = b.ps;
				const ml::usz pe = b.pc + pi;

				// assign new params start
				b.ps = _params.size();

				for (; pi < pe; ++pi) {

					const auto& p = _params_tmp[pi];

					_params.emplace_back(p);
					// update param values start
					_params.back().vs = _values.size();

					//_params.emplace_back(
					//	// copy token
					//	p._token,
					//	// new values start
					//	_values.size(),
					//	// values count
					//	p._values_count
					//);

					values_resize -= p.vc;

						  ml::usz vi = p.vs;
					const ml::usz ve = p.vc + vi;
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
					++(_params_tmp.back().vc);
					_values_tmp.emplace_back(_blocks.size() - 1U);
				}

			}

			auto dbg(void) const -> void {
				std::cout << "BLOCKS[" << _blocks.size()
					<< "][" << _blocks_tmp.size()
					<< "]TMP PARAMS[" << _params.size()
					<< "][" << _params_tmp.size() << "]TMP VALUES[" << _values.size()
					<< "][" << _values_tmp.size() << "]TMP\n";
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

				_block_order.clear();
			}


			auto sort(void) noexcept -> void {

				// insertion sort
				for (ml::usz i = 1U; i < _blocks.size(); ++i) {
					const auto key = _block_order[i];
					const auto key_spec = _blocks[key].spec_id();

					ml::usz j = i;
					while (j > 0U && _blocks[_block_order[j - 1U]].spec_id()
									> key_spec) {
						_block_order[j] = _block_order[j - 1U];
						--j;
					}
					_block_order[j] = key;
				}
			}


			// -- public iterators --------------------------------------------

			/* begin */
			auto begin(void) noexcept -> as::block_iterator {
				return as::block_iterator{*this, 0U};
			}

			/* end */
			auto end(void) noexcept -> as::block_iterator {
				return as::block_iterator{*this, _blocks.size()};
			}


			// -- public accessors --------------------------------------------

			/* blocks */
			auto blocks(void) noexcept -> self& {
				return *this;
			}

			/* number of blocks */
			auto num_blocks(void) const noexcept -> ml::usz {
				return _blocks.size();
			}

			/* block at */
			auto block_at(const ml::usz bi) noexcept -> as::block& {
				return _blocks[bi];
			}



			/* debug */
			auto debug(void) -> void {

				auto& t = *this;

				for (auto b : t.blocks()) {
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
