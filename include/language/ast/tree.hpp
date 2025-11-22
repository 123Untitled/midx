#ifndef language_ast_hpp
#define language_ast_hpp

#include "language/ast/node.hpp"
#include "language/tokens/tokens.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- T R E E -------------------------------------------------------------

	class tree final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = as::tree;


			// -- private members ---------------------------------------------

			std::vector<mx::u8>  _nodes;
			std::vector<mx::usz> _remap;
			std::vector<mx::usz> _stack;
			std::vector<mx::usz> _marks;
			std::vector<mx::usz> _refs;
			std::vector<mx::i8>  _values;


			// -- private methods ---------------------------------------------

			/* pop mark
			   pop the last mark from the mark stack
			   and return its value */
			auto _pop_mark(void) /*noexcept*/ -> mx::usz {
				if (_marks.empty())
					throw std::runtime_error{"No marks to pop"};
				const auto mark = _marks.back();
				_marks.pop_back();
				return mark;
			}


			/* align up
			   align a size up to the next multiple of align */
			template <typename T>
			static auto _align_up(const mx::usz size) noexcept -> mx::usz {
				constexpr mx::usz align = std::alignment_of<T>::value;
				return (size + (align - 1U)) & ~(align - 1U);
			}


			struct frame final {
				mx::usz node;
				mx::usz tick;
				mx::usz num;
				mx::usz den;
				frame(void) noexcept
				: node{0U}, tick{0U}, num{1U}, den{1U} {
				}

				frame(const mx::usz nod,
					  const mx::usz tic,
					  const mx::usz num,
					  const mx::usz den) noexcept
				: node{nod}, tick{tic}, num{num}, den{den} {
				}
			};

			struct play_ctx final {
				mx::usz absolute;
				std::stringstream& hi;
				std::vector<frame> stack;
				frame fr;


				play_ctx(std::stringstream& h, const mx::usz ticks)
				: absolute{ticks}, hi{h}, stack{}, fr{} {
				}

				auto push(const mx::usz node,
						  const mx::usz tick) -> void {
					stack.emplace_back(node, tick, fr.num, fr.den);
				}
				auto push(const mx::usz node,
						  const mx::usz tick,
						  const mx::usz num,
						  const mx::usz den) -> void {
					stack.emplace_back(node, tick, num, den);
				}

				auto pop(void) -> void {
					fr = stack.back();
					stack.pop_back();
				}
			};


			auto play_parallel(play_ctx&) const -> void;
			auto play_crossfade(play_ctx&) const -> void;
			auto play_track(play_ctx&) const -> void;
			auto play_atomic(play_ctx&) const -> void;
			auto play_tempo(play_ctx&) const -> void;
			auto play_group(play_ctx&) const -> void;
			auto play_parameter(play_ctx&) const -> void;
			auto play_reference(play_ctx&) const -> void;


		public:

			tk::tokens* tokens;

			auto play(std::stringstream&, const mx::usz) const -> void;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			tree(void) noexcept
			: _nodes{}, _remap{},
			  _stack{}, _marks{},
			  _refs{}, _values{} {
			}

			/* destructor */
			~tree(void) noexcept = default;


			/* make node
			   create a new node in the arena
			   and return its index */
			//template <typename... Ts>
			//auto make_node(Ts&&... args) -> mx::usz {
			//	const mx::usz index = _nodes.size();
			//	static_cast<void>(
			//			_nodes.emplace_back(std::forward<Ts>(args)...));
			//	return index;
			//}

			template <typename T, typename... Ts>
			auto make_node(const Ts&... args) -> mx::usz {

				const mx::usz offset = _align_up<T>(_nodes.size());
				constexpr auto  size = sizeof(T);

				// allocate space
				_nodes.resize(offset + size);

				// construct in place
				::new (&_nodes[offset]) T{args...};

				return offset;
			}

			/* value start
			   get the start index for value storage */
			auto value_start(void) const noexcept -> mx::usz {
				return _values.size();
			}

			/* push value
			   push a value onto the value storage */
			auto push_value(const mx::i8 value) -> void {
				_values.push_back(value);
			}

			/* ref start
			   get the start index for reference storage */
			auto ref_start(void) const noexcept -> mx::usz {
				return _refs.size();
			}

			/* push ref
			   push a reference onto the reference storage */
			auto push_ref(const mx::usz ref) -> void {
				_refs.push_back(ref);
			}

			/* ref at
			   access ref by direct index */
			auto ref_at(const mx::usz index) const /*noexcept*/ -> mx::usz {
				if (index >= _refs.size()) {
					std::cout << "Ref index out of bounds: " << index
							  << " >= " << _refs.size() << '\n';
					throw std::runtime_error{"Arena ref index out of bounds"};
				}
				return _refs[index];
			}

			/* value
			   access value by direct index */
			auto value_at(const mx::usz index) const /*noexcept*/ -> mx::i8 {
				if (index >= _values.size()) {
					std::cout << "Value index out of bounds: " << index
							  << " >= " << _values.size() << '\n';
					throw std::runtime_error{"Arena value index out of bounds"};
				}
				return _values[index];
			}


			/* header
			   access header by direct index */
			auto header(const mx::usz index) const /*noexcept*/ -> const as::header& {
				if (index + sizeof(as::header) > _nodes.size())
					throw std::runtime_error{"Arena header index out of bounds"};
				return *reinterpret_cast<const as::header*>(&_nodes[index]);
			}

			auto header(const mx::usz index) /*noexcept*/ -> as::header& {
				if (index + sizeof(as::header) > _nodes.size())
					throw std::runtime_error{"Arena header index out of bounds"};
				return *reinterpret_cast<as::header*>(&_nodes[index]);
			}

			/* remap header
			   access header by remapped index */
			auto remap_header(const mx::usz index) const /*noexcept*/ -> const as::header& {
				return remap<as::header>(index);
			}

			/* range of */
			auto range_of(const mx::usz index) /*noexcept*/ -> as::remap_range& {

				if (index + sizeof(as::header) > _nodes.size())
					throw std::runtime_error{"Arena header index out of bounds"};

				as::header* h = reinterpret_cast<as::header*>(&_nodes[index]);

				if ((h->type != as::type::group)
				 && (h->type != as::type::parallel))
					throw std::runtime_error{"Arena range_of: node is not range type"};

				return *reinterpret_cast<as::remap_range*>(h + 1U);
			}


			/* push
			   push a pending node index onto the stack
			   to be flushed later */
			auto push(const mx::usz index) -> void {
				_stack.emplace_back(index);
			}

			/* push
			   push a range of pending node indices onto the stack
			   to be flushed later */
			auto push(const as::remap_range& range) -> void {

				std::cout << "Pushing range start=" << range.start
						  << " count=" << range.count << '\n';
				const auto size = _stack.size();
				_stack.resize(size + range.count);

				for (mx::usz i = 0U; i < range.count; ++i)
					_stack[size + i] = _remap[range.start + i];
				std::cout << "Pushed\n";
			}

			/* clear
			   reset the arena to an empty state */
			auto clear(void) noexcept -> void {
				_nodes.clear();  _remap.clear();
				_stack.clear();  _marks.clear();
				 _refs.clear(); _values.clear();
			}


			/* mark
			   push a mark onto the mark stack
			   to remember the current stack size */
			auto mark(void) -> void {
				_marks.push_back(_stack.size());
			}

			/* flush
			   flush all pending nodes since the last mark
			   into the remap, returning the start index
			   and count of flushed nodes */
			auto flush(void) -> as::remap_range {

				const auto mark = _pop_mark();

				const mx::usz start = _remap.size();
				const mx::usz count = _stack.size() - mark;

				_remap.resize(start + count);

				for (mx::usz i = 0U; i < count; ++i)
					_remap[start + i] = _stack[mark + i];

				_stack.resize(mark);

				return as::remap_range{start, count};
			}

			/* new remap
				remap a single node index */
			auto new_remap(const mx::usz index) -> mx::usz {

				const auto start = _remap.size();
				_remap.push_back(index);
				return start;
			}

			/* remap
			   access node by remapped index */
			template <typename T>
			auto remap(const mx::usz index) /*noexcept*/ -> T& {
				const auto idx = remap_index(index);
				return *reinterpret_cast<T*>(&_nodes[idx]);
			}

			/* remap
			   access const node by remapped index */
			template <typename T>
			auto remap(const mx::usz index) const /*noexcept*/ -> const T& {
				const auto idx = remap_index(index);
				return *reinterpret_cast<const T*>(&_nodes[idx]);
			}

			/* remap index
			   access node by remapped index */
			auto remap_index(const mx::usz index) const /*noexcept*/ -> mx::usz {

				if (index >= _remap.size()) {
					std::cout << "Remapped index out of bounds: " << index
							  << " >= " << _remap.size() << '\n';
					throw std::runtime_error{"Arena remapped index out of bounds"};
				}

				if (_remap[index] >= _nodes.size()) {
					std::cout << "Remapped node index out of bounds: "
							  << _remap[index] << " >= " << _nodes.size() << '\n';
					throw std::runtime_error{"Arena remapped node index out of bounds"};
				}

				return _remap[index];
			}


			/* node
			   access node by direct index */
			template <typename T>
			auto node(const mx::usz index) /*noexcept*/-> T& {
				if (index >= _nodes.size())
					throw std::runtime_error{"Arena node index out of bounds"};
				return *reinterpret_cast<T*>(&_nodes[index]);
			}

			/* node
			   access const node by direct index */
			template <typename T>
			auto node(const mx::usz index) const /*noexcept*/ -> const T& {
				if (index >= _nodes.size())
					throw std::runtime_error{"Arena node index out of bounds"};
				return *reinterpret_cast<const T*>(&_nodes[index]);
			}

		private:

			/* extend range
			   append an element to a contiguous range */
			auto _extend_range(as::remap_range& range,
							   const mx::usz elem) -> void {

				mark();

				// push all children of to range
				push(range);
				// push new element
				push(elem);

				// flush and update range
				range = flush();
			}

		public:

			/* extend range of
			   append an element to a contiguous range
			   of a node by direct index */
			auto extend_range_of(const mx::usz node, const mx::usz elem) -> void {
				_extend_range(range_of(node), elem);
			}

			auto extend_group(const mx::usz n, const mx::usz elem) -> void {

				if (!is_group(n))
					throw std::runtime_error{"extend_group: node is not a group"};

				// get group node
				auto& g = node<as::group>(n);

				// get elem header
				const auto& eh = header(elem);

				std::cout << "EXTEND GROUP: extending group duration "
						  << g.header.duration << " + " << eh.duration << '\n';
				g.header.duration += eh.duration;

				std::cout << "EXTEND GROUP: new duration " << g.header.duration << '\n';

				_extend_range(g.range, elem);
				//_extend_range(range_of(n), elem);
			}


			/* make group with
			   create a new group node with given nodes */
			template <typename... Tp>
			auto make_group_with(const Tp&... nodes) -> mx::usz {

				const as::header* hs[] { &header(nodes)... };
				mx::usz dur = 0U;
				for (const auto h : hs)
					dur += h->duration;

				return make_node<as::group>(make_range(nodes...), dur);
			}


			/* make range
			   create a new remap range with given nodes */
			template <typename... Tp>
			auto make_range(const mx::usz index, const Tp&... args) -> as::remap_range {

				const auto start = _remap.size();
				_remap.resize(start + 1U + sizeof...(args));
				mx::usz i = start;
				_remap[i] = index;
				((_remap[++i] = args ), ...);

				return as::remap_range{start, sizeof...(args) + 1U};
			}

			auto max_dur_of_range(const as::remap_range& range) const -> mx::usz {

				mx::usz max = 0U;

				for (mx::usz i = 0U; i < range.count; ++i) {
					const auto& h = remap_header(range.start + i);
					if (h.duration > max)
						max = h.duration;
				}

				return max;
			}


			/* make parallel */
			auto make_parallel(const mx::usz left,
							   const mx::usz right) -> mx::usz {

				// assume left and right are valid nodes

				auto& lh = header(left);
				auto& rh = header(right);

				// left is parallel
				if (lh.type == as::type::parallel) {

					if (rh.type == as::type::parallel) {
						std::cout << "Absorbing parallel ranges\n";
						absorb_range(left, right);
					}
					else {
						std::cout << "Extending left parallel range\n";
						extend_range_of(left, right); // append right to left
					}

					const auto& range = range_of(left);
					lh.duration = max_dur_of_range(range);
					return left;
				}
				// right is parallel
				else if (rh.type == as::type::parallel) {
					std::cout << "Extending right parallel range\n";
					extend_range_of(right, left); // append left to right
					const auto& range = range_of(right);
					rh.duration = max_dur_of_range(range);
					return right;
				}

				std::cout << "Creating new parallel node\n";

				const auto range = make_range(left, right);
				const auto dur   = max_dur_of_range(range);

				// create new parallel node
				return make_node<as::parallel>(range, dur);
			}

			/* absorb range
			   absorb all nodes from a range into another range */
			auto absorb_range(const mx::usz to, const mx::usz from) -> void {

				auto& t = node<as::parallel>(to);
				auto& f = node<as::parallel>(from);

				mark();

				// push all children of left
				push(t.range);
				// push all children of right
				push(f.range);
				// flush and update left range
				t.range = flush();
			}


			// -- public accessors --------------------------------------------

			/* is group */
			auto is_group(const mx::usz index) /*noexcept*/ -> bool {
				return header(index).type == as::type::group;
			}

			/* is collapsable */
			auto is_collapsable(const mx::usz index) /*noexcept*/ -> bool {
				const auto t = header(index).type;
				return t == as::type::group
					|| t == as::type::permutation;
			}

			template <typename F, typename... Ts>
			auto for_each_header_in_range(const as::remap_range& range, F fn, const Ts&... args) -> void {
				mx::usz it  = range.start;
				mx::usz end = it + range.count;

				for (; it < end; ++it) {
					const auto& h = remap_header(it);
					fn(h, args...);
				}
			}


			inline auto what_is(const mx::usz index) const -> const char* {
				// get type from header
				const as::header& h = header(index);
				switch (h.type) {
					case as::type::program:
						return "program";
					case as::type::atomic_values:
						return "atomic_values";
					case as::type::references:
						return "references";
					case as::type::group:
						return "group";
					case as::type::parallel:
						return "parallel";
					case as::type::crossfade:
						return "crossfade";
					case as::type::track:
						return "track";
					case as::type::parameter:
						return "parameter";
					case as::type::permutation:
						return "permutation";
					case as::type::tempo:
						return "tempo";
					default:
						return "unknown";
				}
			}

			template <typename T>
			struct make_result final {
				mx::usz index;
				T& ref;
			};

			template <typename T, typename... Ts>
			friend auto make_node(as::tree&, const Ts&...) -> as::tree::make_result<T>;



			auto debug(void) const -> void {
				std::cout << "AST Tree Debug:\n";
				std::cout << " Nodes size:  " << _nodes.size() << " bytes\n";
				std::cout << " Remap size:  " << _remap.size() << " entries\n";
				std::cout << " Stack size:  " << _stack.size() << " entries\n";
				std::cout << " Marks size:  " << _marks.size() << " entries\n";
				std::cout << " Refs size:   " << _refs.size()  << " entries\n";
				std::cout << " Values size: " << _values.size() << " entries\n";
				std::cout << "End of AST Tree Debug\n\n";
			}

	}; // class arena


	/* make node
	   create a new node in the arena
	   and return its index and reference */
	template <typename T, typename... Ts>
	auto make_node(as::tree& t, const Ts&... args) -> as::tree::make_result<T> {

		const mx::usz offset = t.template _align_up<T>(t._nodes.size());

		// allocate space
		t._nodes.resize(offset + sizeof(T));

		// pointer
		void* ptr = t._nodes.data() + offset;

		// construct in place
		::new (ptr) T{args...};

		return as::tree::make_result<T>{
			offset,
			*reinterpret_cast<T*>(ptr)
		};
	}




	struct printer {


		private:

			const as::tree& _tree;
			const tk::tokens& _tokens;
			mx::usz _indent;


			printer(const as::tree& t,
					const tk::tokens& tokens)
			: _tree{t}, _tokens{tokens}, _indent{0U} {
			}


		public:




			static auto run(const as::tree& tree, const mx::usz root, const tk::tokens& tokens) -> void {
				std::cout << "\nAST Printer:\n";
				as::printer{tree, tokens}.print_node(root);
				std::cout << "End of AST\n\n";
			}


			auto pad(const char* type) const -> void {
				std::cout << "\x1b[90m| ";
				for (mx::usz i = 0U; i < _indent; ++i)
					std::cout << "----";
				std::cout << "\x1b[0m" << ((_indent > 0U) ? " " : "") << type;
			}

			void print_node(mx::usz index) {

				switch (_tree.header(index).type) {

					case as::type::program:
						print_program(index);
						break;

					case as::type::atomic_values:
						print_atomic_values(index);
						break;

					case as::type::references:
						print_references(index);
						break;

					case as::type::group:
						print_group(index);
						break;

					case as::type::parallel:
						print_parallel(index);
						break;

					case as::type::crossfade:
						print_crossfade(index);
						break;

					case as::type::track:
						print_track(index);
						break;

					case as::type::parameter:
						print_parameter(index);
						break;

					case as::type::permutation:
						print_permutation(index);
						break;

					case as::type::tempo:
						print_tempo(index);
						break;

					default:
						pad("unknown");
						break;
				}
			}

			class indent_guard {

				private:
					mx::usz& indent;

				public:
					indent_guard(mx::usz& ind)
					: indent{ind} {
						++indent;
					}

					~indent_guard(void) noexcept {
						--indent;
					}
			};


			auto print_range(const as::remap_range& range) -> void {
				mx::usz it  = range.start;
				mx::usz end = it + range.count;

				for (; it < end; ++it)
					print_node(_tree.remap_index(it));
			}


			auto print_program(const mx::usz index) -> void {
				const auto& n = _tree.node<as::program>(index);

				pad("program ");
				std::cout << n.header.duration << ":\n";

				indent_guard ig{_indent};
				print_range(n.range);
			}

			auto print_tempo(mx::usz index) -> void {
				const auto& n = _tree.node<as::tempo>(index);

				pad("tempo ");
				std::cout << n.header.duration << ":\n";

				indent_guard ig{_indent};
				print_node(n.child);
			}


			auto print_permutation(mx::usz index) -> void {
				const auto& n = _tree.node<as::permutation>(index);

				pad("permutation ");
				std::cout << n.header.duration << ":\n";

				indent_guard ig{_indent};
				print_range(n.range);
			}

			auto print_track(mx::usz index) -> void {
				const auto& n = _tree.node<as::track>(index);

				pad("track ");
				std::cout << n.header.duration << ":\n";

				indent_guard ig{_indent};
				for (mx::usz i = 0U; i < pa::max_params; ++i) {
					print_range(n.ranges[i]);
				}
			}

			auto print_parameter(mx::usz index) -> void {
				const auto& n = _tree.node<as::parameter>(index);

				pad("");
				std::cout << n.tv << " " << n.header.duration << ":\n";

				indent_guard ig{_indent};
				print_node(n.child);
			}

			auto print_group(mx::usz index) -> void {
				const auto& n = _tree.node<as::group>(index);

				pad("group ");
				std::cout << n.header.duration << ":\n";

				indent_guard ig{_indent};
				print_range(n.range);
			}

			auto print_parallel(mx::usz index) -> void {
				const auto& n = _tree.node<as::parallel>(index);

				pad("parallel ");
				std::cout << n.header.duration << ":\n";

				indent_guard ig{_indent};
				print_range(n.range);
			}

			auto print_crossfade(mx::usz index) -> void {
				const auto& n = _tree.node<as::crossfade>(index);

				pad("crossfade ");
				std::cout << n.header.duration << ":\n";

				indent_guard ig{_indent};
				print_node(n.left);
				print_node(n.right);
			}

			auto print_atomic_values(mx::usz index) -> void {
				const auto& n = _tree.node<as::atomic_values>(index);

				pad("atomic values ");
				std::cout << n.header.duration << ": ";
				for (mx::usz i = 0; i < n.header.duration; ++i) {
					//const auto vw = _tokens.filtered_view(n.token_start + i);
					//std::cout << vw << " ";
					const auto v = _tree.value_at(n.value_start + i);
					std::cout << static_cast<mx::i32>(v) << " ";
				}
				std::cout << "\n";
			}

			auto print_references(mx::usz index) -> void {
				const auto& n = _tree.node<as::references>(index);

				pad("references ");
				std::cout << n.header.duration << ":";
				if (n.count != 0U)
					std::cout << "\n";
				for (mx::usz i = 0U; i < n.count; ++i) {
					auto idx = _tree.ref_at(n.ref_start + i);
					indent_guard ig{_indent};
					print_node(idx);
				}
				//std::cout << "\n";
			}

	};


} // namespace as

#endif // language_ast_hpp
