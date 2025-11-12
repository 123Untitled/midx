#ifndef language_syntax_layout_hpp
#define language_syntax_layout_hpp

#include "language/ast/tree.hpp"
#include "language/tokens.hpp"
#include "language/identifier_map.hpp"
#include "language/diagnostic.hpp"
#include "data/model.hpp"
#include "language/atomic_parser.hpp"



// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {

	class builder;





	class builder {

		public:

			using fn_block = auto (sx::builder::*)(as::block_view&) -> void;
			using fn_param = auto (sx::builder::*)(as::param_view&) -> void;

			mx::model*      _model;
			as::tree*       _tree;
			an::diagnostic* _diag;

			std::vector<mx::usz> _remap;
			sx::identifier_map _id_map;

			sx::atomic_parser _atom_parser;


			auto clear(void) noexcept -> void {
				_remap.clear();
				_id_map.clear();
			}

			auto build(as::tree& tree, mx::model& model, an::diagnostic& diag) -> void {

				_model = &model;
				_tree  = &tree;
				_diag  = &diag;

				clear();

				 _remap.resize(tree.num_blocks());


				for (auto bv : tree.blocks()) {

					auto& b = bv.block();

					if (b.spec_id() == sp::id::invalid)
						continue;

					// skip anonymous
					if (b.is_anonymous() == false) {

						// insert block identifiers
						if (_id_map.insert(b, bv.bi()) == false) {
							diag.push_error("duplicate identifier", b.identifier());
							b.identifier().id = tk::invalid;
						}
					}

					// dispatch block builder
					auto fnb = block_dispatch[static_cast<mx::usz>(b.spec_id())];
					(this->*fnb)(bv);
				}

				_tree->debug();
				_model->debug();
			}


			auto bool_parse(const as::param_view& pv, mx::sequence& seq) -> void {
			}


			template <sp::id ID>
			auto build_block_atomic(as::block_view& bv) -> void {

				// tr, nt, ga, vl, oc, se, ch, pr
				const auto idx = _model->new_sequence();
				auto& seq = _model->get_sequence(idx);
				_remap[bv.bi()] = idx;

				// loop over params
				for (const auto& pv : bv.params()) {

					auto& p = pv.param();

					if (p.param_id() == pa::id::invalid)
						continue;

					switch (p.param_id()) {

						case pa::id::seq:
							_atom_parser.template parse<ID>(pv, seq, *_diag);
							break;
						case pa::id::rpt:
							bool_parse(pv, seq);
							break;
						case pa::id::lnk:
							bool_parse(pv, seq);
							break;
						default:
							break;
					}
				}
			}


			auto build_block_track(as::block_view& bv) -> void {

				const auto idx = _model->new_track();
				auto& track = _model->get_track(idx);
				_remap[bv.bi()] = idx;

				// loop over params
				for (const auto& pv : bv.params()) {

					auto& p = pv.param();

					if (p.param_id() == pa::id::invalid)
						continue;

					if (pv.num_values() != 2U) {
						_diag->push_error("expected reference", p.token());
						continue;
					}

					auto it = pv.begin();

					if (it[0U].token().id != tk::ampersand
					 || it[1U].token().id != tk::text) {
						_diag->push_error("expected reference", it[0U].token());
						continue;
					}

					mx::usz i;
					// search identifier
					const sp::id sid = _id_map.find(p, it[1U], i);

					if (sid == sp::id::invalid) {
						_diag->push_error("unknown identifier", it[1U].token());
						it[1U].token().id = tk::invalid;
						continue;
					}

					auto stype = mx::to_seq_type[static_cast<mx::usz>(sid)];
					track.set_index(stype, _remap[i]);
				}

			}

			auto build_block_pattern(as::block_view& bv) -> void {
			}

			auto build_block_song(as::block_view& bv) -> void {
			}

			/* invalid block */
			auto build_block_invalid(as::block_view& bv) -> void {
			}



			static constexpr fn_block block_dispatch[static_cast<mx::usz>(sp::id::count)] {
				&sx::builder::build_block_atomic<sp::id::trig>,
				&sx::builder::build_block_atomic<sp::id::note>,
				&sx::builder::build_block_atomic<sp::id::gate>,
				&sx::builder::build_block_atomic<sp::id::velo>,
				&sx::builder::build_block_atomic<sp::id::octa>,
				&sx::builder::build_block_atomic<sp::id::semi>,
				&sx::builder::build_block_atomic<sp::id::chan>,
				&sx::builder::build_block_atomic<sp::id::prob>,
				&sx::builder::build_block_track,
				&sx::builder::build_block_pattern,
				&sx::builder::build_block_song,
				&sx::builder::build_block_invalid
			};

			static constexpr fn_param param_dispatch[static_cast<mx::usz>(pa::id::count)] {
			};


	}; // class builder



} // namespace sx

#endif // language_syntax_syntax_layout_hpp
