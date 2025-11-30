#ifndef language_ast_hpp
#define language_ast_hpp

#include "language/ast/node.hpp"
#include "language/ast/nodes.hpp"

#include "language/tokens/tokens.hpp"

#include "midi/midi_engine.hpp"

#include "language/ast/storage.hpp"

#include "language/ast/play_ctx.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	auto highlight(std::string&,
				   const tk::const_token_view&,
				   const char*) -> void;



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

			std::vector<mx::frac> _fracs;


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






			auto play_permutation(play_ctx&) const -> void;
			auto play_parallel(play_ctx&) const -> void;
			auto play_crossfade(play_ctx&) const -> void;
			auto play_track(play_ctx&) const -> void;
			auto play_atomic(play_ctx&) const -> void;
			auto play_tempo(play_ctx&) const -> void;
			auto play_modulo(play_ctx&) const -> void;
			auto play_group(play_ctx&) const -> void;
			auto play_group2(play_ctx&) const -> void;
			auto play_reference(play_ctx&) const -> void;
			auto play_program(play_ctx&) const -> void;

			auto play_parameter(play_ctx&) const -> void;

			auto dispatch_play(play_ctx&) const -> void;


		public:

			tk::tokens* tokens;

			auto play(std::string&, mx::midi_engine&,
										  const mx::frac&) const -> void;


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

				const mx::usz offset = mx::align_up<T>(_nodes.size());
				constexpr auto  size = sizeof(T);

				// allocate space
				_nodes.resize(offset + size);

				// construct in place
				::new (&_nodes[offset]) T{args...};

				return offset;
			}

			/* frac start
			   get the start index for frac storage */
			auto frac_start(void) const noexcept -> mx::usz {
				return _fracs.size();
			}

			/* push frac
			   push a frac onto the frac storage */
			auto push_frac(const mx::frac& f) -> void {
				_fracs.push_back(f);
			}

			/* frac at
			   access frac by direct index */
			auto frac_at(const mx::usz index) const /*noexcept*/ -> const mx::frac& {
				if (index >= _fracs.size()) {
					std::cout << "Frac index out of bounds: " << index
							  << " >= " << _fracs.size() << '\n';
					throw std::runtime_error{"Arena frac index out of bounds"};
				}
				return _fracs[index];
			}

			/* resize fracs
			   resize the frac storage to a given size */
			auto resize_fracs(const mx::usz size) -> void {
				_fracs.resize(size);
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
				_stack.push_back(index);
			}

			/* push
			   push a range of pending node indices onto the stack
			   to be flushed later */
			auto push(const as::remap_range& range) -> void {

				const auto size = _stack.size();
				_stack.resize(size + range.count);

				for (mx::usz i = 0U; i < range.count; ++i)
					_stack[size + i] = _remap[range.start + i];
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
							   const mx::usz node) -> void {
				// manual version
				const auto size = range.count;
				range.count += 1U;
				const auto start = _remap.size();
				_remap.resize(start + range.count);
				for (mx::usz i = 0U; i < size; ++i)
					_remap[start + i] = _remap[range.start + i];
				_remap[start + size] = node;
				range.start = start;
			}


		public:

			/* extend range of
			   append an element to a contiguous range
			   of a node by direct index */
			auto extend_range_of(const mx::usz node, const mx::usz elem) -> void {
				_extend_range(range_of(node), elem);
			}

			/* extend group
			   extend a group node by another node */
			auto extend_group(const mx::usz group, const mx::usz add) -> void {

				if (!is_group(group))
					throw std::runtime_error{"extend_group: node is not a group"};

				// get group node
				auto& g = node<as::group>(group);

				// get elem header
				const auto& h = header(add);

				g.header.dur += h.dur;

				// reduce duration
				g.header.dur.reduce();

				_extend_range(g.range, add);
			}

			/* extend parameter
			   extend a parameter node by another node */
			auto extend_parameter(const mx::usz param, const mx::usz add) -> void {

				if (!is_parameter(param))
					throw std::runtime_error{"extend_parameter: node is not a parameter"};

				// get parameter node
				auto& p = node<as::parameter>(param);

				// get elem header
				const auto& h = header(add);

				p.header.dur += h.dur;

				// reduce duration
				p.header.dur.reduce();

				_extend_range(p.range, add);
			}

			/* sum fracs
			   sum the fracs of all nodes in a range */
			//auto sum_fracs(const as::remap_range& range) const -> mx::frac


			/* make group with
			   create a new group node with given nodes */
			template <typename... Tp>
			auto make_group_with(const Tp&... nodes) -> mx::usz {

				const as::header* hs[] { &header(nodes)... };
				mx::usz dur = 0U;
				for (const auto h : hs)
					dur += h->steps;

				return make_node<as::group>(make_range(nodes...), dur);
			}

			/* sum duration
			   sum the durations of all nodes in a range */
			auto sum_duration(const as::remap_range& range) const -> mx::frac {

				mx::frac sum;

				for (mx::usz i = 0U; i < range.count; ++i) {
					const auto& h = remap_header(range.start + i);
					sum += h.dur;
				}
				return sum;
			}

			/* product duration
			   compute the product of durations of all nodes in a range */
			auto product_duration(const as::remap_range& range) const -> mx::frac {
				mx::frac prod{1,1};

				for (mx::usz i = 0U; i < range.count; ++i) {
					const auto& h = remap_header(range.start + i);
					prod *= h.dur;
				}
				return prod;
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

			auto max_range_dur(const as::remap_range& range) const -> mx::frac {

				mx::frac max;

				for (mx::usz i = 0U; i < range.count; ++i) {
					const auto& h = remap_header(range.start + i);
					if (h.dur > max)
						max = h.dur;
				}

				return max;
			}

			/* lcm range
			   compute the lcm of durations of all nodes in a range */
			auto lcm_range(const as::remap_range& range) const -> mx::frac {

				mx::frac lcm;
				if (range.count == 0U)
					return lcm;

				// get first duration
				//const auto& h0 = remap_header(range.start);
				//lcm = h0.dur;
				lcm = remap_header(range.start).dur;

				for (mx::usz i = 1U; i < range.count; ++i) {
					const auto& h = remap_header(range.start + i);
					lcm = mx::lcm_frac(lcm, h.dur);
				}

				return lcm;
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
						absorb_range(left, right);
						const auto& range = range_of(left);
						//lh.dur  = max_range_dur(range);
						lh.dur  = mx::lcm_frac(lh.dur, rh.dur);
						return left;
					}
					else {
						extend_range_of(left, right); // append right to left
						//lh.dur = rh.dur > lh.dur ? rh.dur : lh.dur;
						lh.dur = mx::lcm_frac(lh.dur, rh.dur);
						return left;
					}
				}

				// right is parallel
				if (rh.type == as::type::parallel) {
					extend_range_of(right, left); // append left to right
					//rh.dur = lh.dur > rh.dur ? lh.dur : rh.dur;
					rh.dur = mx::lcm_frac(lh.dur, rh.dur);
					return right;
				}

				const auto range = make_range(left, right);
				//auto dur = lh.dur > rh.dur ? lh.dur : rh.dur;
				auto dur = mx::lcm_frac(lh.dur, rh.dur);

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


			/* make group
			   create a new group node with given arguments */
			template <typename... Tp>
			auto make_group(const Tp&... args) -> mx::usz {
				return this->make_node<as::group>(args...);
			}

			/* make parameter
			   create a new parameter node with given arguments */
			template <typename... Tp>
			auto make_parameter(const Tp&... args) -> mx::usz {
				return this->make_node<as::parameter>(args...);
			}

			/* make tempo
			   create a new tempo node with given arguments */
			template <typename... Tp>
			auto make_tempo(const Tp&... args) -> mx::usz {
				return this->make_node<as::tempo>(args...);
			}


			/* make atomics
			   create a new atomic values node with given arguments */
			template <typename... Tp>
			auto make_atomics(const Tp&... args) -> mx::usz {
				return this->make_node<as::atomics>(args...);
			}

			/* make refs
			   create a new references node with given arguments */
			template <typename... Tp>
			auto make_refs(const Tp&... args) -> mx::usz {
				return this->make_node<as::references>(args...);
			}

			/* make crossfade
			   create a new crossfade node with given arguments */
			auto make_crossfade(const mx::usz left, const mx::usz right) -> mx::usz {
							
				// get headers
				const auto& lh = header(left);
				const auto& rh = header(right);

				// compute lcm duration
				const auto dur = mx::lcm_frac(lh.dur, rh.dur);


				auto c =  this->make_node<as::crossfade>(
						left, right, dur
				);

				return c;
			}


			// -- public accessors --------------------------------------------

			/* is group */
			auto is_group(const mx::usz index) /*noexcept*/ -> bool {
				return header(index).type == as::type::group;
			}

			/* is parameter */
			auto is_parameter(const mx::usz index) /*noexcept*/ -> bool {
				return header(index).type == as::type::parameter;
			}

			/* is collapsable */
			auto is_collapsable(const mx::usz index) /*noexcept*/ -> bool {
				const auto t = header(index).type;
				return t == as::type::group
					|| t == as::type::permutation;
			}

			/* is atomics */
			auto is_atomics(const mx::usz index) /*noexcept*/ -> bool {
				return header(index).type == as::type::atomics;
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
					case as::type::atomics:
						return "atomics";
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
			friend auto make_node(const Ts&...) -> as::tree::make_result<T>;



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


	inline as::tree* tree_locator = nullptr;



	/* make node
	   create a new node in the arena
	   and return its index and reference */
	template <typename T, typename... Ts>
	auto make_node(const Ts&... args) -> as::tree::make_result<T> {

		const auto offset = mx::align_up<T>(tree_locator->_nodes.size());

		// allocate space
		tree_locator->_nodes.resize(offset + sizeof(T));

		// pointer
		void* ptr = tree_locator->_nodes.data() + offset;

		// construct in place
		::new (ptr) T{args...};

		return as::tree::make_result<T>{
			offset,
			*reinterpret_cast<T*>(ptr)
		};
	}

	/* make group
	   create a new group node with given two nodes */
	inline auto make_group(const mx::usz a, const mx::usz b) -> mx::usz {
		return tree_locator->make_node<as::group>(
			   tree_locator->make_range(a, b),
			  (tree_locator->header(a).dur
			 + tree_locator->header(b).dur).reduce()
		);
	}

	/* is group
	   check if a node is a group */
	inline auto is_group(const mx::usz index) /*noexcept*/ -> bool {
		return tree_locator->header(index).type == as::type::group;
	}





	struct printer {


		private:

			const as::tree& _tree;
			const tk::tokens& _tokens;

			printer(const as::tree& t,
					const tk::tokens& tokens)
			: _tree{t}, _tokens{tokens} {
			}


		public:

			static auto run(const as::tree& tree, const mx::usz root, const tk::tokens& tokens) -> void {
				std::cout << "\nAST Printer:\n";
				as::printer{tree, tokens}.print_node(root, {}, true);
			}

			auto node_name(const as::header& h) const -> std::string {
				switch (h.type) {
					case as::type::program:      return "PROGRAM";
					case as::type::group:        return "\x1b[90m☐\x1b[0m group";
					case as::type::track:        return "\x1b[90m♬\x1b[0m track";
					case as::type::parameter:    return "\x1b[90m⚙\x1b[0m param";
					case as::type::tempo:        return "\x1b[90m↻\x1b[0m tempo";
					case as::type::modulo:       return "\x1b[90m⧗\x1b[0m modulo";
					case as::type::atomics:      return "\x1b[90m↘\x1b[0m []";
					case as::type::references:   return "\x1b[90m⚭\x1b[0m refs";
					case as::type::parallel:     return "\x1b[90m☰\x1b[0m parallel";
					case as::type::crossfade:    return "\x1b[90m⇋\x1b[0m crossfade";
					case as::type::permutation:  return "\x1b[90m☷\x1b[0m permutation";
					default:                     return "unknown";
				}
			}


			void print_node(mx::usz index,
					const std::vector<bool>& prefix,
					const bool last) {

				// get children
				const auto children = get_children(index);
				const auto& h = _tree.header(index);


				std::cout << "\x1b[90m";
				// Draw the prefix bars
				for (mx::usz i = 0; i < prefix.size(); ++i) {
					if (prefix[i])
						std::cout << "│  ";
					else
						std::cout << "   ";
				}

				std::cout << (last ? "└─ " : "├─ ");

				//std::cout << (last ? "╰─" : "├─");
				//if (children.empty())
				//	std::cout << "── ";
				//else
				//	std::cout << "─╮ ";

				std::cout << "\x1b[0m" << node_name(h) << " ";
				std::cout << "\x1b[34m"
						  << h.dur.num
						  << "\x1b[90m/\x1b[33m"
						  << h.dur.den
						  << "\x1b[0m";

				if (h.type == as::type::atomics) {

					std::cout << " \x1b[90m[\x1b[32m";
					const auto& n = _tree.node<as::atomics>(index);
					const auto count = n.header.dur.num;
					for (mx::usz i = 0; i < count; ++i) {
						const auto v = _tree.value_at(n.value_start + i);
						std::cout << static_cast<mx::i32>(v) << (i + 1 < count ? " " : "");
					}
					std::cout << "\x1b[90m]\x1b[0m\n";

					return;
				}
				std::cout << "\n";


				// For each child, update prefix and recurse
				for (mx::usz i = 0; i < children.size(); ++i) {

					bool is_last = (i == children.size() - 1);

					auto new_prefix = prefix;
					new_prefix.push_back(!last);  // continue vertical line if parent not last

					print_node(children[i], new_prefix, is_last);
				}
			}


			auto get_children(mx::usz index) -> std::vector<mx::usz> {

				std::vector<mx::usz> out;

				const auto& h = _tree.header(index);

				switch (h.type)
				{
					case as::type::program:
					case as::type::group:
					case as::type::parallel:
					case as::type::permutation:
						{
							const auto& n = _tree.node<as::group>(index);
							for (mx::usz i = n.range.start; i < n.range.end(); ++i)
								out.push_back(_tree.remap_index(i));
							break;
						}

					case as::type::track:
						{
							const auto& n = _tree.node<as::track>(index);
							for (mx::usz i = 0; i < pa::max_params; ++i)
								if (n.params[i])
									out.push_back(n.params[i]);
							break;
						}

					case as::type::parameter:
						{
							const auto& n = _tree.node<as::parameter>(index);
							for (mx::usz i = n.range.start; i < n.range.end(); ++i)
								out.push_back(_tree.remap_index(i));
							break;
						}

					case as::type::tempo: {
						const auto& n = _tree.node<as::tempo>(index);
						out.push_back(n.child);
						break;
					}

					case as::type::modulo: {
						const auto& n = _tree.node<as::modulo>(index);
						out.push_back(n.child);
						break;
					}

					case as::type::references: {
						const auto& n = _tree.node<as::references>(index);
						for (mx::usz i = 0; i < n.count; ++i)
							out.push_back(_tree.ref_at(n.ref_start + i));
						break;
					}

					case as::type::crossfade: {
						const auto& n = _tree.node<as::crossfade>(index);
						out.push_back(n.left);
						out.push_back(n.right);
						break;
					}

						// atomic / permutation : pas d’enfants
					default: break;
				}

				return out;
			}

	};


} // namespace as

#endif // language_ast_hpp
