// -- L X  N A M E S P A C E --------------------------------------------------

namespace lx {


	// -- forward declarations ------------------------------------------------

	class table;
	class lexer;
	class context;
	using action = auto (lexer::*)(void) -> void;




	// -- L E X E R -----------------------------------------------------------

	class lexer final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = lx::lexer;


		private:

			// -- private members ---------------------------------------------


			const ml::u8* _it;

			const ml::u8* _end;

			const ml::u8* _checkpoint;

			ml::uint _line;

			ml::uint _column;

			/* state */
			const lx::table* _state;

			std::vector<tk::token> _tokens;

			std::vector<std::string> _errors;


			auto _atoi(void) noexcept -> ml::i8;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			lexer(void) noexcept;


			// -- public methods ----------------------------------------------

			auto lex(lx::context&) -> void;
			auto lex2(lx::context&) -> void;


			/* call actions */
			template <lx::action...>
			auto call(void) -> void;


			/* switch state */
			template <typename>
			auto switch_state(void) noexcept -> void;

			/* push state */
			template <typename>
			auto push_state(void) -> void;

			/* pop state */
			auto pop_state(void) -> void;


			/* checkpoint */
			auto checkpoint(void) noexcept -> void;

			/* newline */
			auto newline(void) noexcept -> void;

			/* count */
			auto count(void) noexcept -> void;

			/* reset count */
			auto reset_count(void) noexcept -> void;


			/* push token */
			template <tk::is_token_class, ml::uint = 0U>
			auto push_token(void) -> void;

			template <tk::is_token_class>
			auto push_token2(void) -> void;

			template <tk::is_token_class, typename... Tp>
			auto push_token(const Tp&...) -> void;

			template <tk::is_token_class>
			auto push_token_byte(void) -> void;

			template <tk::is_token_class>
			auto push_byte_token(void) -> void;

			/* push error */
			template <ml::literal>
			auto push_error(void) -> void;


			auto end_of_file(void) -> void;


	}; // class lexer

} // namespace lx

auto lx::lexer::lex(lx::context& ctx) -> void {
	std::cout << "\x1b[32m--- parsing started ---\x1b[0m\n";

	_it  = ctx.it();
	_end = ctx.end();

	for (; _it < _end; ++_it) {
		++_column; // maybe use arithmetic to increment column only on certain chars
		_state->execute(*this, *_it);
	}
	_state->end_of_file(*this);


	for (const auto& t : _tokens) {
		std::cout << "token: " << (unsigned)t.type << " [" << t.line << "] -> ";
		for (const ml::u8* p = t.begin; p < t.end; ++p) {
			//std::cout << *p;
			std::cout << ml::dbc{*p};
		}
		std::cout << "\n";
	}

	for (const auto& e : _errors) {
		std::cout << e << '\n';
	}

}

template <lx::action... As>
auto lx::lexer::call(void) -> void {
	((this->*As)(), ...);
}


/* switch state */
template <typename S>
auto lx::lexer::switch_state(void) noexcept -> void {
	//static_assert(lx::is_state<S>, "S must be a state");
	// assume stack is never empty
	_state = &S::table;
	std::cout << "switch to state: " << S::name << "\n";
}

/* push state */
//template <typename S>
//auto lx::lexer::push_state(void) -> void {
//	_stack.push_back(&S::table);
//	std::cout << "push state: " << S::name << "\n";
//}

/* pop state */
//auto lx::lexer::pop_state(void) -> void {
//	// assume stack is never empty
//	_stack.pop_back();
//	std::cout << "pop state " << "\n";
//}


/* checkpoint */
auto lx::lexer::checkpoint(void) noexcept -> void {
	_checkpoint = _it;
	//std::cout << "checkpoint at: " << ml::dbc{*_it} << "\n";
}

/* newline */
auto lx::lexer::newline(void) noexcept -> void {
	_line += 1U;
	_column = 0U;
	//std::cout << "newline\n";
}


/* push token */
template <tk::is_token_class T, ml::uint SHIFT>
auto lx::lexer::push_token(void) -> void {

	//std::cout << "push token: " << static_cast<unsigned>(T::id) << '\n';

	_tokens.emplace_back(
		tk::token{
			T::id,
			_line,
			_checkpoint,
			_it + SHIFT
		}
	);
}

template <tk::is_token_class T>
auto lx::lexer::push_token2(void) -> void {
	_tokens.emplace_back(
		tk::token{T::id, _line, _checkpoint, _it}
	);
}

template <tk::is_token_class T, typename... Tp>
auto lx::lexer::push_token(const Tp&... args) -> void {
	_tokens.emplace_back(
		tk::token{
			T::id,
			args...
		}
	);
}

template <tk::is_token_class T>
auto lx::lexer::push_token_byte(void) -> void {
	_tokens.emplace_back(
		T::id, _line, _it, _it + 1U
	);
}

template <tk::is_token_class T>
auto lx::lexer::push_byte_token(void) -> void {
	_tokens.emplace_back(
		T::id, _line, _it, _it + 1U
	);
}

/* push error */
template <ml::literal E>
auto lx::lexer::push_error(void) -> void {

	std::string err;
	err.append("\x1b[31merror\x1b[0m:");
	err.append(std::to_string(_line));
	err.append(":");
	err.append(std::to_string(_column));
	err.append(": ");
	err.append(E.data);

	_errors.emplace_back(std::move(err));
}

