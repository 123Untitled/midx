#ifndef data_node_hpp
#define data_node_hpp

#include "core/types.hpp"
#include "time/signature.hpp"
#include "language/tokens.hpp"
#include <vector>
#include <iostream>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {

	// *2 tempo time 2
	// /4 tempo divide by 4
	// A B C D E values
	// ( ) permutations -> A (B C) : A B A C,   A (B (C D)) : A B A C A B A D

	// exemple *2 A (B | *3 (C >< D E) /4 F) G

	// exemple *1 A (B | *2 (C D E))


	// A (B C | D E) F G

	enum class node_type : mx::u8 {
		value,
		tempo,
		container,
		parallel,
		crossfade,
		permute
	};

	class ast_node final {
		public:
			mx::node_type type;
			tk::raw::token* token;
			std::vector<mx::ast_node> childs;
	};


	class node {

		public:

			using unique = std::unique_ptr<mx::node>;

			virtual ~node(void) noexcept = default;

			virtual auto duration(void) const noexcept -> mx::u64 = 0;
			virtual auto evaluate(const mx::u64, const mx::i8) const noexcept -> mx::i8 = 0;
			virtual auto debug(const mx::u64 = 0U) const noexcept -> void = 0;

			mx::u64 _duration;

	}; // class node


	class container : public mx::node {

		protected:

			std::vector<mx::node::unique> _childs;

		public:
			~container(void) noexcept override = default;


			auto add(mx::node::unique&& child) noexcept -> void {
				_childs.emplace_back(std::move(child));
			}

			auto childs(void) noexcept -> std::vector<mx::node::unique>& {
				return _childs;
			}

			auto childs(void) const noexcept -> const std::vector<mx::node::unique>& {
				return _childs;
			}

			auto duration(void) const noexcept -> mx::u64 override {
				mx::u64 total = 0U;
				for (const auto& child : _childs)
					total += child->duration();
				return total;
			}

			auto evaluate(mx::u64 time, const mx::i8 def) const noexcept -> mx::i8 override {
				for (const auto& child : _childs) {
					const auto dur = child->duration();
					if (time < dur)
						return child->evaluate(time, def);
					time -= dur;
				}
				return def;
			}

			auto debug(const mx::u64 indent = 0U) const noexcept -> void override {
				const std::string indent_str(indent, ' ');
				std::cout << indent_str << "container (duration: " << duration() << ")\n";
				for (const auto& child : _childs) {
					child->debug(indent + 2U);
				}
			}

	}; // class container


	class value final : public mx::node {

		private:
			mx::i8 _value;

		public:
			value(const mx::i8 v) noexcept
			: _value{v} {
			}

			auto duration(void) const noexcept -> mx::u64 override final {
				return 1U;
			}
			auto evaluate(const mx::u64, const mx::i8) const noexcept -> mx::i8 override final {
				return _value;
			}

			auto debug(const mx::u64 indent = 0U) const noexcept -> void override {
				const std::string indent_str(indent, ' ');
				std::cout << indent_str << "value: " << static_cast<int>(_value) << "\n";
			}

	}; // class value


	class permute : public mx::container {


		public:

			auto duration(void) const noexcept -> mx::u64 override final {
				return _childs.size(); // not valid
			}

			auto evaluate(const mx::u64 time, const mx::i8 def) const noexcept -> mx::i8 override final {
				return def;
			}

			auto debug(const mx::u64 indent = 0U) const noexcept -> void override {
				const std::string indent_str(indent, ' ');
				std::cout << indent_str << "permute (duration: " << duration() << ")\n";
				for (const auto& child : _childs) {
					child->debug(indent + 2U);
				}
			}

	}; // class permute



	class tempo final : public mx::container {

		private:

			mx::signature _signature;


		public:

			auto duration(void) const noexcept -> mx::u64 override final {
				return mx::container::duration()
					 * _signature.modulus();
			}

			auto debug(const mx::u64 indent = 0U) const noexcept -> void override {
				const std::string indent_str(indent, ' ');
				std::cout << indent_str << "tempo x" << _signature.numerator() << "/" << _signature.denominator()
						  << " (duration: " << duration() << ")\n";
				for (const auto& child : _childs) {
					child->debug(indent + 2U);
				}
			}

	}; // class tempo


	class parallel final : public mx::container {

		public:

			auto duration(void) const noexcept -> mx::u64 override final {
				mx::u64 max = 0U;
				for (const auto& child : _childs) {
					const auto dur = child->duration();
					if (dur > max)
						max = dur;
				}
				return max;
			}

			auto evaluate(const mx::u64 time, const mx::i8 def) const noexcept -> mx::i8 override final {
				mx::i8 result = def;
				for (const auto& child : _childs) {
					const auto dur = child->duration();
				}
				return result;
			}

			auto debug(const mx::u64 indent = 0U) const noexcept -> void override {
				const std::string indent_str(indent, ' ');
				std::cout << indent_str << "parallel (duration: " << duration() << ")\n";
				for (const auto& child : _childs) {
					child->debug(indent + 2U);
				}
			}

	}; // class parallel


	class crossfade final : public mx::node {

		private:

			mx::node::unique _from;
			mx::node::unique _to;

		public:

			crossfade(mx::node::unique&& from, mx::node::unique&& to) noexcept
			: _from{std::move(from)}
			, _to{std::move(to)} {
			}


			auto duration(void) const noexcept -> mx::u64 override final {
				return std::max(_from->duration(), _to->duration());
			}

			auto evaluate(const mx::u64 time, const mx::i8 def) const noexcept -> mx::i8 override final {
				return def; // not implemented
			}

			auto debug(const mx::u64 indent = 0U) const noexcept -> void override {
				const std::string indent_str(indent, ' ');
				std::cout << indent_str << "crossfade (duration: " << duration() << ")\n";
				std::cout << indent_str << " From:\n";
				_from->debug(indent + 2U);
				std::cout << indent_str << " To:\n";
				_to->debug(indent + 2U);
			}

	}; // class crossfade


	class root final {

		private:

			std::unique_ptr<mx::container> _container;


		public:

			root(void) noexcept
			: _container{std::make_unique<mx::container>()} {
			}

			auto evaluate(const mx::u64 time, const mx::i8 def) const noexcept -> mx::i8 {

				if (!_container)
					return def;

				const auto dur = _container->duration();

				if (dur == 0U)
					return def;

				const auto t = time % dur;
				return _container->evaluate(t, def);
			}

			auto root_node(void) noexcept -> mx::container* {
				return _container.get();
			}

			auto debug(void) const noexcept -> void {
				std::cout << "Root Node:\n";
				if (_container)
					_container->debug(2U);
				else
					std::cout << "  (empty)\n";
			}
	};

	using atomic_type = mx::root;

} // namespace mx

#endif // data_node_hpp
