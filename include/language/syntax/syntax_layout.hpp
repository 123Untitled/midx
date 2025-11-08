#ifndef language_syntax_layout_hpp
#define language_syntax_layout_hpp

#include "language/ast/tree.hpp"
#include "language/tokens.hpp"
#include "language/identifier_map.hpp"
#include "language/diagnostic.hpp"
#include "data/model.hpp"
#include "atoi.hpp"



// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {

	class builder {

		public:

			using fn_block = auto (sx::builder::*)(as::block_view&) -> void;
			using fn_param = auto (sx::builder::*)(as::param_view&) -> void;

			as::tree* _tree;
			std::vector<mx::usz> _remap;
			sx::identifier_map _id_map;
			mx::model _model;
			an::diagnostic* _diag;


			auto build(as::tree& tree, an::diagnostic& diag) -> void {

				_tree = &tree;
				_diag = &diag;
				 _model.clear();
				 _remap.clear();
				_id_map.clear();
				 _remap.resize(tree.num_blocks());


				for (auto bv : tree.blocks()) {


					auto& b         = bv.block();
					const auto spid = b.spec_id();

					// skip anonymous
					if (spid != sp::id::invalid && b.is_anonymous() == false) {

						// insert block identifiers
						if (_id_map.insert(b, bv.bi()) == false) {
							diag.push_error("duplicate identifier", b.identifier());
							b.identifier().id = tk::invalid;
						}
					}

					// dispatch block builder
					auto fnb = block_dispatch[static_cast<ml::usz>(spid)];
					(this->*fnb)(bv);
				}

				// debug tree
				tree.debug();
			}


			template <sp::id ID>
			auto build_block_atomic(as::block_view& bv) -> void {
				// tr, nt, ga, vl, oc, se, ch, pr
				const auto idx = _model.new_sequence();
				_remap[bv.bi()] = idx;

				// loop over params
				for (const auto& pv : bv.params()) {

					auto& p = pv.param();

					if (p.param_id() == pa::id::invalid) {
						_diag->push_error("invalid parameter", p.token());
						continue;
					}

					switch (p.param_id()) {

						case pa::id::seq: {
							// loop over values
							for (const auto& vv : pv.values()) {

								auto& v = vv.value();

								if (v.is_nested()) {
									p.token().id = tk::invalid;
									_diag->push_error("nested block not allowed here", p.token());
									continue;
								}

								mx::i8 n = ml::to_integer<>(v.token(), *_diag);
								std::cout << (int)n << "\n";
							}

							break;
						}
						case pa::id::rpt: {
							break;
						}
						case pa::id::lnk: {
							break;
						}
						default: {
							continue;
						}
					}
				}
			}


			auto build_block_track(as::block_view& bv) -> void {
			}

			auto build_block_pattern(as::block_view& bv) -> void {
			}
			auto build_block_song(as::block_view& bv) -> void {
			}

			/* invalid block */
			auto build_block_invalid(as::block_view& bv) -> void {
				//_diag->push_error("invalid specifier", bv.block().specifier());
			}



			static constexpr fn_block block_dispatch[static_cast<ml::usz>(sp::id::count)] {
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

			static constexpr fn_param param_dispatch[static_cast<ml::usz>(pa::id::count)] {
			};


	}; // class builder



} // namespace sx

#endif // language_syntax_syntax_layout_hpp
