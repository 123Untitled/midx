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

		/* value iterator as friend */
		friend class value_iterator;

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


			/* block order */
			std::vector<ml::usz> _block_order;


		public:

			// -- public lifecycle --------------------------------------------

			tree(void)
			: _blocks{},
			  _params{},
			  _values{},
			  _block_order{} {


				  _blocks.reserve( 16U);
				  _params.reserve( 64U);
				  _values.reserve(512U);

				  _block_order.reserve(_blocks.capacity());
			}



			// -- public modifiers --------------------------------------------

			/* new block */
			auto new_block(tk::token& tk) -> void {
				_blocks.emplace_back(tk, _params.size());
			}

			/* new param */
			auto new_param(tk::token& tk) -> void {
				const sp::id spec_id = _blocks.back().spec_id();
				_params.emplace_back(spec_id, tk,
									_values.size());
				++(_blocks.back().pc);
			}

			/* new value */
			auto new_value(tk::token& tk) -> void {
				_values.emplace_back(tk);
				++(_params.back().vc);
			}

			/* remove last block */
			auto remove_last_block(void) noexcept -> void {

				const auto& b = _blocks.back();
					  auto pi = b.ps;
				const auto pe = b.pc + pi;

				ml::usz vc = 0U;
				for (; pi < pe; ++pi) {
					const auto& param = _params[pi];
					vc += param.vc;
				}

				const ml::usz p_resize = _params.size() - b.pc;
				const ml::usz v_resize = _values.size() - vc;

				_blocks.pop_back();
				_params.resize(p_resize);
				_values.resize(v_resize);
			}


			auto clear(void) noexcept -> void {
				_blocks.clear();
				_params.clear();
				_values.clear();

				_block_order.clear();
			}


			auto sort(void) noexcept -> void {

				_block_order.resize(_blocks.size());
				for (ml::usz i = 0U; i < _blocks.size(); ++i)
					_block_order[i] = i;

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

			/* last block */
			auto last_block(void) noexcept -> as::block& {
				return _blocks.back();
			}

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
							v.debug();
						}
					}
				}
			}

	}; // class tree

} // namespace as

#endif // language_ast_tree_hpp
