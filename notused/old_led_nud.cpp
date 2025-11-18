
//template <pr::level L>
//auto pr::parser::old_nud_value(const mx::usz left) -> mx::usz {

	//return 0U;
	//if (!left)
	//	left = as::make_unique<as::n_ary>();
	//
	//// dynamic cast to n_ary
	//auto* nary = dynamic_cast<as::n_ary*>(left.get());
	//if (!nary) {
	//	auto promoted = as::make_unique<as::n_ary>();
	//	promoted->add_child(std::move(left));
	//	left = std::move(promoted);
	//	nary = static_cast<as::n_ary*>(left.get());
	//}
	//
	//
	//nary->add_child(as::make_unique<as::leaf>(_it.token()));
	//debug("NUD value", _it);
	//
	//while (++_it != _end) {
	//
	//	if (_it.token().id != tk::decimal)
	//		break;
	//
	//	debug("NUD value", _it);
	//	nary->add_child(as::make_unique<as::leaf>(_it.token()));
	//}
	//
	//return left;
//}


// OLD GROUP NUD
    //return left;

	//debug("NUD group", _it);
	//const auto it = _it;
	//
	//// consume opening parenthesis
	//++_it;
	//
	//// parse inner expression
 //   auto inside = parse_expr<L>(pr::precedence::none);
	//
 //   if (_it == _end || _it.token().id != tk::priority_close) {
 //       error("Unclosed group", it);
 //       return inside;
 //   }
	//
	//debug("NUD group", _it);
	//
	//// consume closing parenthesis
	//++_it;
	//
	//if (!left)
	//	return inside;
	//
	//if (!inside) // 0 ()
	//	return left;
	//
	// // left existe déjà → séquence
 //   auto *ary = dynamic_cast<as::n_ary*>(left.get());
 //   if (!ary) {
 //       auto promoted = as::make_unique<as::n_ary>();
 //       promoted->add_child(std::move(left));
 //       left = std::move(promoted);
 //       ary = static_cast<as::n_ary*>(left.get());
 //   }
	//
	//ary->add_child(std::move(inside));
	//
 //   return left;
}



template <pr::level L>
auto pr::parser::nud_permutation(const mx::usz left) -> mx::usz {

	return 0U;
	//debug("NUD permutation", _it);
	//const auto it = _it;
	//
	//// consume opening bracket
	//++_it;
	//
	//// parse inner expression
 //   auto inside = parse_expr<L>(pr::precedence::none);
	//
 //   if (_it == _end || _it.token().id != tk::permutation_close) {
 //       error("Unclosed permutation", it);
 //       return inside; // HERE NOT SURE ??? left ???
 //   }
	//
	//debug("NUD group", _it);
	//
	//// consume closing parenthesis
	//++_it;
	//
	// // make permutation node
 //   auto perm = std::make_unique<as::permutation>();
 //   perm->add_child(std::move(inside));
	//
	////if (!left)
	////	return inside;
	//
	//
	//// if left already exists → sequence
 //   if (!left)
 //       return perm;
	//
 //   auto seq = left.get();
 //   auto* ary = dynamic_cast<as::n_ary*>(seq);
 //   if (!ary) {
 //       auto promoted = std::make_unique<as::n_ary>();
 //       promoted->add_child(std::move(left));
 //       ary = promoted.get();
 //       left = std::move(promoted);
 //   }
	//
 //   ary->add_child(std::move(perm));
 //   return left;

}



template <pr::level L>
auto pr::parser::led_parallel(const mx::usz left) -> mx::usz {
	++_it;
	return 0U;

	//debug("LED parallel", _it);
	//
	//const auto& tk = _it.token();
	//
	//do { // skip consecutive parallel operators
	//	++_it;
	//} while (_it != _end && _it.token().id == tk::parallel);
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
}




template <pr::level L>
auto pr::parser::led_crossfade(const mx::usz left) -> mx::usz {
	++_it;
	return 0U;

	//debug("LED crossfade", _it);
	//
	//const auto& tk = _it.token();
	//
	//do { // skip consecutive crossfade operators
	//	++_it;
	//} while (_it != _end && _it.token().id == tk::crossfade);
	//
	//if (_it == _end)
	//	return left;
	//
	//auto right = parse_expr<L>(pr::pre_of<L>(tk));
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
}










	template <typename T>
	using unique = std::unique_ptr<T>;

	using std::make_unique;

	class node {


		public:

			using unique = std::unique_ptr<as::node>;

			node(void) noexcept = default;

			virtual ~node(void) noexcept = default;

			virtual auto debug_node(const char* type, const mx::usz indent = 0U) const noexcept -> void {
				print_indent(indent);
				std::cout << type << ' ';
			}

			static auto print_indent(const mx::usz indent) noexcept -> void {
				std::cout << "\x1b[90m| ";
				for (mx::usz i = 0U; i < indent; ++i)
					std::cout << "--";
				std::cout << "\x1b[0m" << ((indent > 0U) ? " " : "");
			}

			virtual auto debug(const mx::usz indent = 0U) const noexcept -> void = 0;
	};





	class tokenable : public as::node {
		protected:

			const tk::token* _token;

		public:
			tokenable(void) = delete;
			tokenable(const tk::token& t) noexcept
			: as::node{}, _token{&t} {
			}

			auto debug_node(const char* type, const mx::usz indent = 0U) const noexcept -> void override {
				as::node::debug_node(type, indent);
				std::cout << *_token  << '\n';
			}
	};


	class leaf : public as::tokenable {
		public:
			leaf(const tk::token& t) noexcept
			: as::tokenable{t} {
			}
			auto debug(const mx::usz indent = 0U) const noexcept -> void override {
				as::tokenable::debug_node("leaf", indent);
			}
	};


	class binary final : public as::tokenable {

		private:

			as::node::unique _left;
			as::node::unique _right;

		public:
			binary(as::node::unique&& left,
				   as::node::unique&& right,
				   const tk::token& t) noexcept
			: as::tokenable{t},
			  _left{std::move(left)},
			  _right{std::move(right)} {
			}

			auto debug(const mx::usz indent = 0U) const noexcept -> void override {
				as::tokenable::debug_node("binary", indent);
				_left->debug(indent + 2U);
				_right->debug(indent + 2U);
			}
	};


	class n_ary final : public as::node {
		private:

			std::vector<as::node::unique> _children;

		public:

			n_ary(void) noexcept
			: as::node{} {
			}

			auto add_child(as::node::unique&& child) -> void {
				_children.emplace_back(std::move(child));
			}

			auto debug(const mx::usz indent = 0U) const noexcept -> void override {
				debug_node("n_ary", indent);
				std::cout << '\n';
				for (const auto& c : _children)
					c->debug(indent + 2U);
			}
			auto children(void) noexcept -> std::vector<as::node::unique>& {
				return _children;
			}
	};

	class permutation final : public as::node {
		private:

			std::vector<as::node::unique> _children;

		public:

			permutation(void) noexcept
			: as::node{} {
			}

			auto add_child(as::node::unique&& child) -> void {
				_children.emplace_back(std::move(child));
			}

			auto debug(const mx::usz indent = 0U) const noexcept -> void override {
				debug_node("permutation", indent);
				std::cout << '\n';
				for (const auto& c : _children)
					c->debug(indent + 2U);
			}
			auto children(void) noexcept -> std::vector<as::node::unique>& {
				return _children;
			}
	};
class parallel final : public as::tokenable {

        std::vector<as::node::unique> _children;

    public:
        parallel(const tk::token& t) noexcept
        : as::tokenable{t} {
        }

        auto add_child(as::node::unique&& child) -> void {
            _children.emplace_back(std::move(child));
        }

        auto children() noexcept -> std::vector<as::node::unique>& {
            return _children;
        }

        auto debug(const mx::usz indent = 0U) const noexcept -> void override {
            as::tokenable::debug_node("parallel", indent);
            for (const auto& c : _children)
                c->debug(indent + 2U);
        }
    };


	class unary : public as::tokenable {
		private:
			as::node::unique _operand;
		public:
			unary(as::node::unique&& operand,
				  const tk::token& t) noexcept
			: as::tokenable{t},
			  _operand{std::move(operand)} {
			}


			auto debug(const mx::usz indent = 0U) const noexcept -> void override {
				debug_node("unary", indent);
				_operand->debug(indent + 2U);
			}
	};













	class expression final : public as::node {
		private:

			std::vector<as::node::unique> _nodes;

		public:
			expression(void) noexcept
			: as::node{}, _nodes{} {
			}

			auto debug(const mx::usz indent = 0U) const noexcept -> void override {
				//debug_node("expression", indent);
				//std::cout << '\n';
				//for (const auto& s : _nodes)
				//	s->debug(indent + 2U);
			}

			auto add_node(as::node::unique&& seq) -> void {
				_nodes.emplace_back(std::move(seq));
			}
	};



	//class sequence final : public as::node {
	//	private:
	//
	//		std::vector<as::node::unique> _elements;
	//
	//	public:
	//		sequence(void) noexcept
	//		: as::node{} {
	//		}
	//
	//		auto add_element(as::node::unique&& element) -> void {
	//			_elements.emplace_back(std::move(element));
	//		}
	//
	//		auto debug(const mx::usz indent = 0U) const noexcept -> void override {
	//			debug_node("sequence", indent);
	//			std::cout << '\n';
	//			for (const auto& e : _elements)
	//				e->debug(indent + 2U);
	//
	//		}
	//		auto children(void) noexcept -> std::vector<as::node::unique>& {
	//			return _elements;
	//		}
	//};
	class assignment final : public as::node {

		private:

			std::vector<const tk::token*> _idents;
			as::node::unique _expr;

		public:
			assignment(void) noexcept
			: as::node{}, _expr{} {
			}

			auto add_identifier(const tk::token& tk) -> void {
				_idents.emplace_back(&tk);
			}
			auto set_expression(as::node::unique&& expr) -> void {
				_expr = std::move(expr);
			}

			auto debug(const mx::usz indent = 0U) const noexcept -> void override {
				//debug_node("assignment", indent);
				//std::cout << '\n';
				//for (mx::usz i = 0U; i < indent; ++i)
				//	std::cout << "-- ";
				//for (const auto& tk : _idents) {
				//	std::cout << *tk << ' ';
				//}
				//std::cout << '\n';
				//if (_expr)
				//	_expr->debug(indent + 2U);
			}
	};

} // namespace as
