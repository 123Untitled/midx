#ifndef language_ast_hash_run_hpp
#define language_ast_hash_run_hpp

#include "math.hpp"
#include <unordered_map>


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- H A S H  R U N ------------------------------------------------------

	struct hash_run final {

		using map_type = std::unordered_map<mx::usz, mx::frac>;
		using self = as::hash_run;

		map_type _m1;
		map_type _m2;
		map_type* _old;
		map_type* _now;

		hash_run(void) noexcept
		: _m1{}, _m2{},
		  _old{&_m1},
		  _now{&_m2} {
		}

		auto swap_now(void) noexcept -> void {
			auto* tmp = _old;
			_old = _now;
			_now = tmp;
			_now->clear();
		}

		auto clear(void) noexcept -> void {
			_old->clear();
			_now->clear();
		}

		auto hash_combine(mx::u64 h, mx::u64 v) noexcept -> mx::u64 {
			h ^= v + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2);
			return h;
		}

		//auto operator[](const mx::usz h) noexcept -> mx::frac& {
		//	return (*_now)[h];
		//}
		auto operator[](const mx::usz h) noexcept -> mx::frac& {
			return (*_old)[h];
		}

		auto find(const mx::usz h) const noexcept -> typename map_type::const_iterator {
			return _old->find(h);
		}

		auto end(void) const noexcept -> typename map_type::const_iterator {
			return _old->end();
		}

		auto old_size(void) const noexcept -> mx::usz {
			return _old->size();
		}
	};

} // namespace as

#endif // language_ast_hash_run_hpp
