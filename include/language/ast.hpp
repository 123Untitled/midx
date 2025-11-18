#ifndef language_ast_hpp
#define language_ast_hpp

#include "core/types.hpp"
#include "language/tokens/def.hpp"

#include <variant>


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	struct dummy final {};


	struct permutation final {
		mx::usz start;
		mx::usz count;
	};

	struct track final {
		mx::usz start;
		mx::usz count;
	};


	struct parallel final {
		mx::usz left;
		mx::usz right;
	};

	struct crossfade final {
		mx::usz left;
		mx::usz right;
	};


	struct parameter final {
		const tk::token* token;
		mx::usz index;
	};

	struct sequence final {
		mx::usz start;
		mx::usz count;
	};

	struct leaf final {
		const tk::token* token;
		leaf(const tk::token& t) noexcept
		: token{&t} {
		}
	};



	using node = std::variant<
		as::dummy,
		as::permutation,
		as::parallel,
		as::crossfade,
		as::parameter,
		as::sequence,
		as::leaf
	>;



	// -- A R E N A -----------------------------------------------------------

	class arena final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = as::arena;


			// -- private members ---------------------------------------------

			std::vector<as::node> _nodes;
			std::vector<mx::usz>   _remap;
			std::vector<mx::usz>   _stack;
			std::vector<mx::usz>   _marks;


			// -- private methods ---------------------------------------------

			/* pop mark
			   pop the last mark from the mark stack
			   and return its value */
			auto _pop_mark(void) -> mx::usz {
				if (_marks.empty())
					throw std::runtime_error{"No marks to pop"};
				const auto mark = _marks.back();
				_marks.pop_back();
				return mark;
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			arena(void) noexcept = default;

			/* destructor */
			~arena(void) noexcept = default;


			/* make node
			   create a new node in the arena
			   and return its index */
			template <typename T, typename... Ts>
			auto make_node(Ts&&... args) -> mx::usz {
				const mx::usz index = _nodes.size();
				_nodes.emplace_back(T{std::forward<Ts>(args)...});
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
				for (mx::usz i = 0U; i < count; ++i)
					_stack.emplace_back(_remap[start + i]);
			}

			/* clear
			   reset the arena to an empty state */
			auto clear(void) noexcept -> void {
				_nodes.clear(); _remap.clear();
				_stack.clear(); _marks.clear();
				static_cast<void>(
						self::make_node<as::dummy>()
				);
			}

			/* mark
			   push a mark onto the mark stack
			   to remember the current stack size */
			auto mark(void) -> void {
				_marks.emplace_back(_stack.size());
			}

			/* flush
			   flush all pending nodes since the last mark
			   into the remap, returning the start index
			   and count of flushed nodes */
			auto flush(void) -> std::pair<mx::usz, mx::usz> {

				const auto mark = self::_pop_mark();

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

				auto& seq = std::get<as::sequence>(self::node(left));

				self::mark();

				// push all children of to sequence
				self::push(seq.start, seq.count);
				// push what
				self::push(elem);

				// flush
				const auto [start, count] = self::flush();
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
				return self::make_node<as::sequence>(start, 2U);
			}

			template <typename T, typename... Ts>
			friend auto make_node(self&, mx::usz&, Ts&&...) -> T&;

	}; // class arena


	/* node is */
	template <typename T>
	auto node_is(const as::arena& a, const mx::usz index) /*noexcept*/ -> bool {
		const auto& n = a.node(index);
		return std::holds_alternative<T>(n);
	}

	/* make node */
	template <typename T, typename... Ts>
	auto make_node(as::arena& a, mx::usz& i, Ts&&... args) -> T& {
		i = a._nodes.size();
		auto& n = a._nodes.emplace_back(T{std::forward<Ts>(args)...});
		return std::get<T>(n);
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
				std::visit(self{_arena, _indent + 2U}, _arena.remap(index));
			}


			debug(const as::arena& a, const mx::usz ind = 0U) noexcept
			: _arena{a}, _indent{ind} {
			}

		public:


			static auto run(const as::arena& a, const as::node& n) -> void {
				std::visit(as::debug{a}, n);
			}

			auto operator()(const as::dummy& d) -> void {
				_pad();
				std::cout << "dummy\n";
			}

			auto operator()(const as::permutation& g) -> void {
				_pad();
				std::cout << "permutation:\n";

				for (mx::usz i = 0U; i < g.count; ++i)
					_visit(g.start + i);
			}

			auto operator()(const as::parallel& p) -> void {
				_pad();
				std::cout << "parallel:\n";

				_visit(p.left);
				_visit(p.right);
			}
			auto operator()(const as::crossfade& c) -> void {
				_pad();
				std::cout << "crossfade:\n";
				_visit(c.left);
				_visit(c.right);
			}
			auto operator()(const as::parameter& p) -> void {
				_pad();
				std::cout << "parameter: " << *(p.token) << '\n';
			}
			auto operator()(const as::sequence& s) -> void {
				_pad();
				std::cout << "sequence:\n";

				for (mx::usz i = 0U; i < s.count; ++i)
					_visit(s.start + i);
			}
			auto operator()(const as::leaf& l) -> void {
				_pad();
				std::cout << "leaf: " << *(l.token) << '\n';
			}

	}; // struct debug


} // namespace as

#endif // language_ast_hpp
