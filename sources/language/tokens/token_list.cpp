#include "language/tokens/token_list.hpp"
#include <iostream>
#include <iomanip>


// -- T O K E N S -------------------------------------------------------------

// -- public modifiers --------------------------------------------------------

/* push raw token */
auto tk::tokens::push_raw_token(const tk::raw::token& token) -> void {
	_raw.push_back(token);
}

/* push map token */
auto tk::tokens::push_map_token(const tk::raw::token& token) -> void {
	_raw.push_back(token);
	_map.push_back(_raw.size() - 1U);
}

/* push syn token */
auto tk::tokens::push_syn_token(const tk::syn::token& token) -> void {
	_syn.push_back(token);
}

/* push syn token */
auto tk::tokens::push_syn_token(const tk::id id,
								const tk::const_map_iterator& start,
								const mx::usz count) -> void {

	const tk::syn::token t{
		id, start.index(), count
	};

	_syn.emplace_back(t);
}


// -- public methods ----------------------------------------------------------

/* raw tokens */
auto tk::tokens::raw_tokens(void) noexcept -> tk::raw_iterator {
	return tk::raw_iterator{*this};
}

/* const raw tokens */
auto tk::tokens::raw_tokens(void) const noexcept -> tk::const_raw_iterator {
	return tk::const_raw_iterator{*this};
}

/* map tokens */
auto tk::tokens::map_tokens(void) noexcept -> tk::map_iterator {
	return tk::map_iterator{*this};
}

/* const map tokens */
auto tk::tokens::map_tokens(void) const noexcept -> tk::const_map_iterator {
	return tk::const_map_iterator{*this};
}

/* syn tokens */
auto tk::tokens::syn_tokens(void) noexcept -> tk::syn_iterator {
	return tk::syn_iterator{*this};
}

/* const syn tokens */
auto tk::tokens::syn_tokens(void) const noexcept -> tk::const_syn_iterator {
	return tk::const_syn_iterator{*this};
}


// -- public methods ----------------------------------------------------------

/* debug */
auto tk::tokens::debug(void) const -> void {

	//const mx::u8* buffer_start = br.begin;
	std::cout << "\x1b[36m--- TOKEN LIST DEBUG ---\x1b[0m\n";

	mx::usz max_name = 0U;
	//for (mx::u8 i = 0U; i <= tk::end_of_tokens; ++i) {
	//	const auto len = std::strlen(tk::token_names[i]);
	//	max_name = (len > max_name) ? len : max_name;
	//}
	// add escape sequence \x1b[36m and \x1b[0m length
	max_name += std::strlen("\x1b[36m") + std::strlen("\x1b[0m");
	max_name += 20;

	mx::usz max_token = 0U;
	for (mx::usz i = 0; i < _raw.size(); ++i) {
		const auto& t = _raw[i];
		const auto len = t.lexeme.size;
		max_token = (len > max_token) ? len : max_token;
	}

	mx::usz i = 0U;
	for (const tk::raw::token& t : self::map_tokens()) {

		// get token name
		const char* name = tk::raw::names[static_cast<unsigned>(t.id)];
		const auto  len  = std::strlen(name);

		std::cout << std::right << std::setw(4) << i << ": "
				  << "\x1b[35m" << name << "\x1b[0m"
				  << std::setw(static_cast<int>(max_name) - static_cast<int>(len))
				  << "-";
		std::cout << " \"" << t.lexeme << "\"";

        std::cout << '\n';
		++i;
    }

    std::cout << "\x1b[36m------------------------\x1b[0m\n";
}


auto tk::tokens::debug_syn(void) const -> void {

	std::cout << "\x1b[36m--- SYN TOKEN LIST DEBUG ---\x1b[0m\n";

	mx::usz i = 0U;
	for (const auto& sv : self::syn_tokens()) {

		// get token name
		const char* name = tk::syn::names[sv.token().id];
		const auto  len  = std::strlen(name);

		std::cout << std::right << std::setw(4) << i << ": "
				  << tk::syn::colors[sv.token().id] << name << "\x1b[0m"
				  << std::setw(static_cast<int>(20U) - static_cast<int>(len))
				  << "-";
		std::cout << " [";
		mx::usz j = 0U;
		for (const auto& mt : sv) {
			if (j > 0U)
				std::cout << ", ";
			std::cout << "\"" << mt.lexeme << "\"";
			++j;
		}
		std::cout << "]\n";
	}

	std::cout << "\x1b[36m----------------------------\x1b[0m\n";
}



