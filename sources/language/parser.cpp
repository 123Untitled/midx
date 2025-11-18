#include "language/parser.hpp"
#include "language/tokens/tokens.hpp"
#include "language/diagnostic.hpp"
#include "language/ast/tree.hpp"

#include <iostream>
#include "language/parser/table.hpp"


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
pr::parser::parser(void)
: _tokens{nullptr},
  _diag{nullptr},
  _it{},
  _end{}
{
}


// -- public methods ----------------------------------------------------------

auto pr::parser::parse(tk::tokens&   tokens,
					   as::tree&       tree,
					   an::diagnostic& diag) -> void {

	// get references
	_tokens = &tokens;
	_tree   = &tree;
	_diag   = &diag;

	_arena.clear();

	// initialize iterators
	const auto fi = tokens.filtered();
	_it	 = fi.begin();
	_end = fi.end();

	_depth = 0U;

	// parse scope
	if (_it == _end) {
		std::cout << "No tokens to parse\n";
		return;
	}

	auto root = parse_expr<pr::level::block>(pr::precedence::none);

	if (!root) {
		std::cout << "No expression parsed\n";
		return;
	}
	else {
		std::cout << "Parsed expression:\n";
	}

	const auto& n = _arena.node(root);

	as::debug::run(_arena, n);
}

auto dbg_token(const char* msg, const tk::token& t) -> void {
	std::cout << msg << ": " << t << ' ' << &t << '\n';
}


// -- private methods ---------------------------------------------------------

auto pr::parser::error(const char* msg) -> void {

	if (_it == _end) {
		_diag->push(msg, tk::range{});
		return;
	}

	tk::token_view tv = *_it;
	if (tv.num_chunks() == 0U)
		throw std::runtime_error{"Token has no chunks"};

	//_diag->push(msg, tv.first_chunk().range);
	_diag->push(msg, tk::range{});
}

auto pr::parser::error(const char* msg,
					   const tk::iterator& it) -> void {

	it.token().id = tk::invalid;
	tk::token_view tv = *it;
	_diag->push(msg, tv.first_chunk().range);
}




/* parse expr */
template <pr::level L>
auto pr::parser::parse_expr(const pr::precedence min_pre) -> mx::usz {

	std::cout << "\x1b[35mEntering\x1b[0m parse_expr " << _depth++ << '\n';

	mx::usz left = 0U;

	while (_it != _end) {

		auto& tk = _it.token();
		auto nud = pr::nud_of<L>(tk);
		std::cout << "\x1b[32mNUD\x1b[0m loop\n";


		// no NUD found, go to LED phase
		if (!nud) {
			//std::cout << " No NUD found\n";
			break;
		}

		if (pr::can_start<L>(tk) == false) {
			error("Token cannot start an expression", _it);
			return 0U;
			break;
		}

		//std::cout << " Found NUD\n";
		// consume token in nud function
		//const auto node = (this->*nud)(std::move(left));
		//left = node != 0U ? node : left;
		left = (this->*nud)(left);
	}


	// loop for led
	while (_it != _end) {
		std::cout << "\x1b[31mLED\x1b[0m loop\n";

		const auto& tk = _it.token();
		const auto led = pr::led_of<L>(tk);
		const auto pre = pr::pre_of<L>(tk);

		if (!led) {
			//std::cout << " No LED found\n";
			break;
		}

		if (pre <= min_pre)
			break;

		// appelle le handler LED
		left = (this->*led)(left);

		if (!left) {
			//std::cout << " LED returned null\n";
			return 0U;
		}

		// here check if left and right are compatible ?

	}
	//std::cout << "Returning from parse_expr\n";
	return left;
}

auto debug(const char* msg, const tk::iterator& it) -> void {
	//return;
	std::cout << msg << ": " << *it << "\n";
}


// -- N U D S -----------------------------------------------------------------


template <pr::level L>
mx::usz pr::parser::nud_value(const mx::usz left) {

	// new marker
    _arena.mark();

	// left exists
    if (left) {

		// is sequence
		if (std::holds_alternative<as::sequence>(_arena.node(left))) {
			const auto& seq = std::get<as::sequence>(_arena.node(left));
			_arena.push(seq.start, seq.count);
		}
		// is not sequence
		else { _arena.push(left); }
	}

	// push initial leaf
	_arena.push(_arena.make_node<as::leaf>(_it.token()));

	// consume other values
	while (++_it != _end && _it.token().id == tk::decimal) {
		_arena.push(
			_arena.make_node<as::leaf>(_it.token())
		);
	}

	if (left == 0U || !std::holds_alternative<as::sequence>(_arena.node(left))) {
		const auto [start, count] = _arena.flush();
		return _arena.make_node<as::sequence>(start, count);
	}

	auto& seq = std::get<as::sequence>(_arena.node(left));
	auto [start,count] = _arena.flush();
	seq.start = start;
	seq.count = count;
	return left;
}



// -- N U D S -----------------------------------------------------------------


/* nud group */
template <pr::level L>
auto pr::parser::nud_group(const mx::usz left) -> mx::usz {

	debug("NUD group", _it);
	const auto it = _it;

	// consume opening parenthesis
	++_it;

	// parse inner expression
    const auto inside = parse_expr<L>(pr::precedence::none);

	if (_it != _end && _it.token().id == tk::priority_close) {
		debug("NUD group", _it);
		++_it; // consume closing parenthesis
	}
	else error("Unclosed group", it);

	if (!inside) // 0 ()
		return left;

	if (!left)
		return inside;

	// check if left is sequence
	if (as::node_is<as::sequence>(_arena, left))
		return _arena.append_to_sequence(left, inside);

	// else make new sequence
	return _arena.merge_as_sequence(left, inside);
}



/* nud permutation */
template <pr::level L>
auto pr::parser::nud_permutation(const mx::usz left) -> mx::usz {

	debug("NUD permutation", _it);
	const auto it = _it;

	// consume opening bracket
	++_it;

	// parse inner expression
    const auto inside = parse_expr<L>(pr::precedence::none);


	if (_it != _end && _it.token().id == tk::permutation_close) {
		debug("NUD permutation", _it);
		++_it; // consume closing permutation
	}
	else error("Unclosed permutation", it);

	if (!inside) // 0 []
		return left;

	 // make permutation node
	mx::usz perm;
	auto& pref = as::make_node<as::permutation>(_arena, perm);

	// check if inside is sequence
	if (as::node_is<as::sequence>(_arena, inside)) {
		const auto& seq = std::get<as::sequence>(_arena.node(inside));
		pref.start = seq.start;
		pref.count = seq.count;
	}
	else {
		// need remap index here
		pref.start = _arena.new_remap(inside);
		pref.count = 1U;
	}

	if (!left)
		return perm;

	// check if left is sequence
	if (as::node_is<as::sequence>(_arena, left))
		return _arena.append_to_sequence(left, perm);

	// else make new sequence
	return _arena.merge_as_sequence(left, perm);
}


/* nud parameter */
template <pr::level L>
auto pr::parser::nud_parameter(const mx::usz left) -> mx::usz {

	if constexpr (L == pr::level::block) {
		// create track node
		// ...
		// handle left
		// left maybe 0, a sequence, a prefix tempo, etc.
	}


	++_it;
	return 0U;
}

/* nud tempo */
template <pr::level L>
auto pr::parser::nud_tempo(mx::usz left) -> mx::usz {

	// consume tempo token
    ++_it;

    // new context: mark children
    _arena.mark();

    // parse everything that belongs to this tempo block
    mx::usz inside = parse_expr<L>(pr::precedence::tempo);

    // tempo closes automatically when parse_expr returns
    auto [start, count] = _arena.flush();

    //mx::usz tempo_idx = _arena.make_node<ast::tempo>(tvalue, start, count);

    // merge with left
    //if (left == 0)
    //    return tempo_idx;
    //
    //if (as::node_is<ast::sequence>(_arena.node(left)))
    //    return _arena.append_to_sequence(left, tempo_idx);
    //
    //return _arena.merge_as_sequence(left, tempo_idx);
	return 0;
}


// -- L E D S -----------------------------------------------------------------

/* led crossfade */
template <pr::level L>
auto pr::parser::led_crossfade(const mx::usz left) -> mx::usz {

	debug("LED crossfade", _it);

	const auto& tk = _it.token();

	do { // skip consecutive crossfade operators
		++_it;
	} while (_it != _end && _it.token().id == tk::crossfade);

	if (_it == _end)
		return left;

	const auto right = parse_expr<L>(pr::pre_of<L>(tk));

	return left == 0U ? right :
		 (right == 0U ? left :
		   _arena.make_node<as::crossfade>(
			   _arena.new_remap(left),
			   _arena.new_remap(right)
		   ));
}

template <pr::level L>
auto pr::parser::led_tracksep(const mx::usz left) -> mx::usz {
	++_it;
	return 0U;

	//debug("LED tracksep", _it);
	//
	//const auto& tk = _it.token();
	//
	//do { // skip consecutive parallel operators
	//	++_it;
	//} while (_it != _end && _it.token().id == tk::block_start);
	//
	//if (_it == _end)
	//	return left;
	//
 //   auto right = parse_expr<L>(pr::pre_of<L>(tk));
	//
	//
	//if (left == nullptr)
	//	return right;
	//
	//if (right == nullptr)
	//	return left;
	//
	//return as::make_unique<as::binary>(
	//	std::move(left),
	//	std::move(right),
	//	tk
	//);
	//return left;
}


/* led parallel */
template <pr::level L>
auto pr::parser::led_parallel(const mx::usz left) -> mx::usz {

	debug("LED parallel", _it);

	const auto& tk = _it.token();

	do { // skip consecutive parallel operators
		++_it;
	} while (_it != _end && _it.token().id == tk::parallel);

	if (_it == _end)
		return left;

    auto right = parse_expr<L>(pr::pre_of<L>(tk));

	if (left == 0U)
		return right;

	if (right == 0U)
		return left;

	return _arena.make_node<as::parallel>(
			_arena.new_remap(left),
			_arena.new_remap(right)
	);
}





//auto pr::parser::led_parallel(as::node::unique&& left) -> as::node::unique {
//
//    debug("LED parallel", _it);
//
//    const auto& tk = _it.token(); // token de l'opérateur |
//
//    // 1) Skip des || consécutifs (ton comportement permissif)
//    do {
//        ++_it;
//    } while (_it != _end && _it.token().id == tk::parallel);
//
//    // Pas de RHS → on ne fabrique rien de spécial
//    if (_it == _end)
//        return left;
//
//    // 2) Parser la partie droite avec la bonne précédence
//    auto right = parse_expr(pr::pre_of(tk));
//
//    // Si vraiment rien à droite → on garde juste la gauche
//    if (!right)
//        return left;
//
//    // 3) Si pas de gauche (cas comme `(| 2 3)`), on renvoie juste right
//    if (!left)
//        return right;
//
//    // 4) Fusion dans un nœud parallel
//
//    // Si la gauche est déjà un parallel, on le réutilise
//    if (auto* par = dynamic_cast<as::parallel*>(left.get())) {
//
//        // Optionnel : si la droite est aussi un parallel (parenthèses), on la "flatten"
//        if (auto* par_right = dynamic_cast<as::parallel*>(right.get())) {
//            auto& children = par_right->children();
//            for (auto& child : children) {
//                par->add_child(std::move(child));
//            }
//            children.clear();
//        } else {
//            par->add_child(std::move(right));
//        }
//
//        return left;
//    }
//
//    // Sinon on crée un nouveau parallel et on y met left + right
//    auto par = as::make_unique<as::parallel>(tk);
//
//    par->add_child(std::move(left));
//
//    if (auto* par_right = dynamic_cast<as::parallel*>(right.get())) {
//        auto& children = par_right->children();
//        for (auto& child : children) {
//            par->add_child(std::move(child));
//        }
//        children.clear();
//    } else {
//        par->add_child(std::move(right));
//    }
//
//    return par;
//}

