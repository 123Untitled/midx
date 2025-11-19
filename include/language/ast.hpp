#ifndef language_ast_hpp
#define language_ast_hpp

#include "language/ast/node.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- A R E N A -----------------------------------------------------------

	class arena final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = as::arena;


			// -- private members ---------------------------------------------

			std::vector<as::node> _nodes;
			std::vector<mx::usz>  _remap;
			std::vector<mx::usz>  _stack;
			std::vector<mx::usz>  _marks;


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


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			arena(void) noexcept
			: _nodes{}, _remap{},
			  _stack{}, _marks{} {

				static_cast<void>(
					make_node(as::type::dummy)
				);
			}

			/* destructor */
			~arena(void) noexcept = default;


			/* make node
			   create a new node in the arena
			   and return its index */
			template <typename... Ts>
			auto make_node(Ts&&... args) -> mx::usz {
				const mx::usz index = _nodes.size();
				static_cast<void>(
						_nodes.emplace_back(std::forward<Ts>(args)...));
				return index;
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
			auto push(const mx::usz start,
					  const mx::usz count) -> void {

				const auto size = _stack.size();
				_stack.resize(size + count);

				for (mx::usz i = 0U; i < count; ++i)
					_stack[size + i] = _remap[start + i];
			}

			/* clear
			   reset the arena to an empty state */
			auto clear(void) noexcept -> void {

				_nodes.clear(); _remap.clear();
				_stack.clear(); _marks.clear();

				static_cast<void>(
					make_node(as::type::dummy)
				);

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
			auto flush(void) -> std::pair<mx::usz, mx::usz> {

				const auto mark = _pop_mark();

				const mx::usz start = _remap.size();
				const mx::usz count = _stack.size() - mark;

				_remap.resize(start + count);

				for (mx::usz i = 0U; i < count; ++i)
					_remap[start + i] = _stack[mark + i];

				_stack.resize(mark);

				return {start, count};
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
			auto remap(const mx::usz index) /*noexcept*/ -> as::node& {

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

				return _nodes[_remap[index]];
			}

			/* remap
			   access const node by remapped index */
			auto remap(const mx::usz index) const /*noexcept*/ -> const as::node& {

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

				return _nodes[_remap[index]];
			}

			/* node
			   access node by direct index */
			auto node(const mx::usz index) /*noexcept*/-> as::node& {
				if (index >= _nodes.size())
					throw std::runtime_error{"Arena node index out of bounds"};
				return _nodes[index];
			}

			/* node
			   access const node by direct index */
			auto node(const mx::usz index) const /*noexcept*/ -> const as::node& {
				if (index >= _nodes.size())
					throw std::runtime_error{"Arena node index out of bounds"};
				return _nodes[index];
			}


			/* append to sequence */
			auto append_to_sequence(const mx::usz left, const mx::usz elem)
				/* noexcept */
				-> mx::usz {

				// assume left is sequence
				auto& seq = node(left);

				mark();

				// push all children of to sequence
				push(seq.start, seq.count);
				// push new element
				push(elem);

				// flush
				const auto [start, count] = flush();
				seq.start = start;
				seq.count = count;

				return left;
			}

			/* merge as sequence */
			auto merge_as_sequence(const mx::usz left, const mx::usz right) -> mx::usz {

				const auto start = _remap.size();
				_remap.resize(start + 2U);

				_remap[start + 0U] = left;
				_remap[start + 1U] = right;

				return make_node(
					as::type::sequence,
					start, 2U
				);
			}


			// -- public accessors --------------------------------------------

			/* is sequence */
			auto is_sequence(const mx::usz index) /*noexcept*/ -> bool {
				return node(index).type == as::type::sequence;
			}

			/* is leaf */
			auto is_leaf(const mx::usz index) /*noexcept*/ -> bool {
				return node(index).type == as::type::leaf;
			}

			/* is track */
			auto is_track(const mx::usz index) /*noexcept*/ -> bool {
				return node(index).type == as::type::track;
			}

	}; // class arena


	inline auto what_is(const as::node& n) -> const char* {
		switch (n.type) {
			case as::type::dummy:       return "dummy";
			case as::type::permutation: return "permutation";
			case as::type::track:       return "track";
			case as::type::parallel:    return "parallel";
			case as::type::crossfade:   return "crossfade";
			case as::type::parameter:   return "parameter";
			case as::type::sequence:    return "sequence";
			case as::type::leaf:        return "leaf";
			default:                    return "unknown";
		}
	}


	struct debug final {


		private:

			using self = as::debug;

			const as::arena& _arena;
			const mx::usz _indent;

			auto _pad(void) const -> void {
				std::cout << "\x1b[90m| ";
				for (mx::usz i = 0U; i < _indent; ++i)
					std::cout << "--";
				std::cout << "\x1b[0m" << ((_indent > 0U) ? " " : "");
			}


			auto _visit(const mx::usz index) -> void {
				self d{_arena, _indent + 2U};
				d._switch(_arena.remap(index));
			}


			debug(const as::arena& a, const mx::usz ind = 0U) noexcept
			: _arena{a}, _indent{ind} {
			}

		public:


			static auto run(const as::arena& a, const as::node& n) -> void {
				as::debug{a}._switch(n);
			}


			auto _switch(const as::node& n) -> void {

				switch (n.type) {

					case as::type::dummy: {
						_pad();
						std::cout << "dummy\n";
						break;
					}

					case as::type::permutation: {
						_pad();
						std::cout << "permutation:\n";

						for (mx::usz i = 0U; i < n.count; ++i)
							_visit(n.start + i);
						break;
					}

					case as::type::track: {
						_pad();
						std::cout << "track:\n";

						for (mx::usz i = 0U; i < n.count; ++i)
							_visit(n.start + i);
						break;
					}

					case as::type::parallel: {
						_pad();
						std::cout << "parallel:\n";

						_visit(n.start);
						_visit(n.count);
						break;
					}

					case as::type::crossfade: {
						_pad();
						std::cout << "crossfade:\n";
						_visit(n.start);
						_visit(n.count);
						break;
					}

					case as::type::parameter: {
						_pad();
						std::cout << "parameter: " << *(n.token) << '\n';

						for (mx::usz i = 0U; i < n.count; ++i)
							_visit(n.start + i);
						break;
					}

					case as::type::sequence: {
						_pad();
						std::cout << "sequence:\n";

						for (mx::usz i = 0U; i < n.count; ++i)
							_visit(n.start + i);
						break;
					}

					case as::type::leaf: {
						_pad();
						std::cout << "leaf: " << *(n.token) << '\n';
						break;
					}

					default:
						throw std::runtime_error{"Unknown node type in debug"};
				}
			}


	}; // struct debug

} // namespace as

#endif // language_ast_hpp
