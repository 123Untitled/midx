#ifndef ml_language_state_machine_hpp
#define ml_language_state_machine_hpp

#include "types.hpp"
#include <vector>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- forward declarations ------------------------------------------------

	/* parser context */
	class parser_context;


	// -- S T A T E  M A C H I N E --------------------------------------------
	//
	//class state_machine {
	//
	//
	//	private:
	//
	//		// -- private types -----------------------------------------------
	//
	//		/* self type */
	//		using self = ml::state_machine;
	//
	//
	//	public:
	//
	//		// -- public lifecycle --------------------------------------------
	//
	//		/* default constructor */
	//		state_machine(void) noexcept = default;
	//
	//		/* copy constructor */
	//		state_machine(const self&) noexcept = default;
	//
	//		/* move constructor */
	//		state_machine(self&&) noexcept = default;
	//
	//		/* destructor */
	//		virtual ~state_machine(void) noexcept = default;
	//
	//
	//		// -- public assignment operators ---------------------------------
	//
	//		/* copy assignment operator */
	//		auto operator=(const self&) noexcept -> self& = default;
	//
	//		/* move assignment operator */
	//		auto operator=(self&&) noexcept -> self& = default;
	//
	//
	//		// -- public interface --------------------------------------------
	//
	//		/* parse */
	//		virtual auto parse(ml::parser_context&) -> void = 0;
	//
	//}; // class state_machine
	//
	//
	//
	//class machine_stack final {
	//
	//	private:
	//
	//		// -- private types -----------------------------------------------
	//
	//		/* self type */
	//		using self = ml::machine_stack;
	//
	//
	//		// -- private members ---------------------------------------------
	//
	//		/* stack */
	//		std::vector<ml::state_machine*> _stack;
	//
	//
	//	public:
	//
	//		// -- public lifecycle --------------------------------------------
	//
	//		/* default constructor */
	//		machine_stack(void) noexcept
	//		: _stack{} {
	//		}
	//
	//		/* deleted copy constructor */
	//		machine_stack(const self&) = delete;
	//
	//		/* move constructor */
	//		machine_stack(self&&) noexcept = default;
	//
	//		/* destructor */
	//		~machine_stack(void) noexcept {
	//			for (auto* sm : _stack)
	//				delete sm;
	//		}
	//
	//
	//		// -- public assignment operators ---------------------------------
	//
	//		/* deleted copy assignment operator */
	//		auto operator=(const self&) -> self& = delete;
	//
	//		/* move assignment operator */
	//		auto operator=(self&&) noexcept -> self& = default;
	//
	//
	//		// -- public methods ----------------------------------------------
	//
	//		/* push */
	//		template <typename T, typename... Tp>
	//		auto push(Tp&&... args) -> void {
	//			static_assert(std::derived_from<T, ml::state_machine>,
	//						  "T must be derived from ml::state_machine");
	//			_stack.push_back(new T{std::forward<Tp>(args)...});
	//		}
	//
	//		/* pop */
	//		auto pop(void) noexcept -> void {
	//			delete _stack.back();
	//			_stack.pop_back();
	//		}
	//
	//		/* top */
	//		auto top(void) -> ml::state_machine& {
	//			return *_stack.back();
	//		}
	//
	//		/* empty */
	//		auto empty(void) const noexcept -> bool {
	//			return _stack.empty();
	//		}
	//
	//}; // class machine_stack
	//

} // namespace ml

#endif // ml_language_state_machine_hpp




	//template <typename T>
	//class state_machine_logic {
	//	private:
	//		ml::transition<T>* _tr;
	//
	//	public:
	//
	//		state_machine_logic(void) noexcept
	//		: _tr{&static_cast<T*>(this)->initial_transition()} {
	//		}
	//
	//		auto step(const ml::u8 c) -> void {
	//			//auto& sm = T::state_machine;
	//			//auto& ct = T::char_table;
	//			//_tr = &sm[_tr->state][ct[c]];
	//			_tr = &static_cast<T*>(this)->next_transition();
	//			_tr->execute(*static_cast<T*>(this));
	//		}
	//
	//}; // class state_machine_logic


