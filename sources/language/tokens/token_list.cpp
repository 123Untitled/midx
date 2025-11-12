#include "language/tokens/token_list.hpp"
#include <iostream>
#include <iomanip>


// -- L I S T  ----------------------------------------------------------------


// -- public modifiers --------------------------------------------------------

/* push token */
auto tk::token_list::push_token(const tk::token& token) -> void {
	_tokens.push_back(token);
}

/* push parse token */
auto tk::token_list::push_parse_token(const tk::token& token) -> void {
	_tokens.push_back(token);
	_parse_indices.push_back(_tokens.size() - 1U);
}


// -- public methods ----------------------------------------------------------

/* begin */
auto tk::token_list::begin(void) noexcept -> tk::iterator {
	return tk::iterator{*this};
}

/* end */
auto tk::token_list::end(void) noexcept -> tk::iterator {
	return tk::iterator{*this, _parse_indices.size()};
}


/* debug */
auto tk::token_list::debug(const mx::byte_range& br) -> void {




	const mx::u8* buffer_start = br.begin;
	std::cout << "\x1b[36m--- TOKEN LIST DEBUG ---\x1b[0m\n";

	mx::usz max_name = 0U;
	for (mx::u8 i = 0U; i <= tk::parameter; ++i) {
		const auto len = std::strlen(tk::token_names[i]);
		max_name = (len > max_name) ? len : max_name;
	}
	// add escape sequence \x1b[36m and \x1b[0m length
	max_name += std::strlen("\x1b[36m") + std::strlen("\x1b[0m");

	mx::usz max_token = 0U;
	for (mx::usz i = 0; i < _tokens.size(); ++i) {
		const auto& t = _tokens[i];
		const auto len = t.lexeme.size;
		max_token = (len > max_token) ? len : max_token;
	}




	//for (mx::usz i = 0; i < _tokens.size(); ++i) {
	mx::usz i = 0;
	for (auto& t : *this) {
        //const auto& t = _tokens[i];

        // Extraire le texte du token


		auto len = std::strlen(tk::token_names[static_cast<unsigned>(t.id)]);

		std::cout << std::right << std::setw(4) << i << ": "
				  << "\x1b[35m" << tk::token_names[static_cast<unsigned>(t.id)] << "\x1b[0m"
				  << std::setw(static_cast<int>(max_name) - static_cast<int>(len))
				  << "-";
		std::cout << " \"" << t.lexeme << "\"";

        std::cout << '\n';
		++i;
    }

    std::cout << "\x1b[36m------------------------\x1b[0m\n";
}
