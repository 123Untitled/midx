#include <iostream>
#include "application.hpp"

namespace mx {

	class node {
		public:
			virtual ~node(void) noexcept = default;
			virtual auto duration(void) const noexcept -> mx::u64 = 0;
			virtual auto evaluate(const mx::u64, const mx::i8) const noexcept -> mx::i8 = 0;

			auto evaluate_node(const mx::u64 ticks, const mx::i8 def) const noexcept -> mx::i8 {
				const auto dur = this->duration();
				if (dur == 0U)
					return def;
				const auto t = ticks % dur;
				return this->evaluate(t, def);
			}

	}; // class node


	class concat final : public mx::node {

		private:
			std::vector<mx::i8> _values;

		public:
			auto duration(void) const noexcept -> mx::u64 override final {
				return static_cast<mx::u64>(_values.size());
			}

			auto evaluate(const mx::u64 time, const mx::i8) const noexcept -> mx::i8 override final {
				return _values[time];
			}

	}; // class concat


	class permute : public mx::node {

		private:
			std::vector<std::unique_ptr<mx::node>> _childs;

		public:
			auto duration(void) const noexcept -> mx::u64 override final {
				return 0U;
			}
			auto evaluate(const mx::u64 time, const mx::i8 def) const noexcept -> mx::i8 override final {
				return def;
			}

	}; // class permute


	class tempo : public mx::node {

		private:
			mx::signature _signature;
			std::vector<std::unique_ptr<mx::node>> _childs;

		public:

			auto duration(void) const noexcept -> mx::u64 override final {
				mx::u64 total = 0U;
				for (const auto& child : _childs)
					total += child->duration();
				return total / _signature.modulus();
			}

			auto evaluate(const mx::u64 time, const mx::i8 def) const noexcept -> mx::i8 override final {
				mx::u64 t = time;
				for (const auto& child : _childs) {
					const auto dur = child->duration();
					if (t < dur)
						return child->evaluate(t, def);
					t -= dur;
				}
				return def;
			}

	}; // class tempo
}



auto main(int ac, char** av) -> int {

	mx::tempo t;

	try {
		mx::application::run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
