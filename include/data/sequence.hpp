#ifndef data_sequence_hpp
#define data_sequence_hpp

#include <vector>
#include "time/signature.hpp"
#include "language/tokens.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- S E Q U E N C E -----------------------------------------------------

	template <ml::u8 DEFAULT>
	class sequence {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::sequence<DEFAULT>;


		protected:

			// -- protected structs -------------------------------------------

			struct slot final {
				public:
					ml::u8 value;
					ml::signature sign;
					tk::token* token;
					ml::usz depth;
			};


			// -- protected members -------------------------------------------

			/* sequence */
			std::vector<slot> _sequence;

			/* current */
			slot* _current;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			sequence(void) noexcept
			: _sequence{},
			  _current{nullptr} {
			}

			/* copy constructor */
			sequence(const self&) = default;

			/* move constructor */
			sequence(self&&) noexcept = default;

			/* destructor */
			~sequence(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) noexcept -> std::vector<slot>& {
				return _sequence;
			}

			/* const data */
			auto data(void) const noexcept -> const std::vector<slot>& {
				return _sequence;
			}

			/* current */
			auto current(void) const noexcept -> const slot& {
				return _current;
			}


			// -- public modifiers --------------------------------------------

			/* push */
			auto push(const ml::u8 value) -> void {
				_sequence.emplace_back(value);
				_current = value; // ? for bug but not fixed
			}

			/* repush */
			//auto repush(void) -> void {
			//	_sequence.emplace_back(
			//			_sequence.empty() ?
			//				D : _sequence.back()
			//	);
			//	_current = _sequence.back(); // ? for bug but not fixed
			//}

			/* reset */
			auto reset(void) noexcept -> void {
				_sequence.clear();
				//_current = def; // this fixes the bug when reparse (nano)
				// but not fix when delete (vim)
			}


			// -- public methods ----------------------------------------------

			/* next */
			auto next(const ml::u64& timeline) noexcept -> bool {

				_current = _sequence[timeline % _sequence.size()];

				// check if signature is time
				//if (_signature.is_time(timeline) == false)
				//	return false;
				//
				//if (_sequence.empty() == true)
				//	return true;
				//
				//// get count from timeline beginning
				//const auto count = _signature.count(timeline);
				//
				//// get current value
				//_current = _sequence[count % _sequence.size()];

				return true;
			}

	}; // class sequence


	// -- N O T E -------------------------------------------------------------

	class note final : public ml::sequence<60U> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::note;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			note(void) noexcept = default;

			/* copy constructor */
			note(const self&) = default;

			/* move constructor */
			note(self&&) noexcept = default;

			/* destructor */
			~note(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class note


	// -- T R I G -------------------------------------------------------------

	class trig final : public ml::sequence<0U> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::trig;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			trig(void) noexcept = default;

			/* copy constructor */
			trig(const self&) = default;

			/* move constructor */
			trig(self&&) noexcept = default;

			/* destructor */
			~trig(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class trig



	// -- G A T E -------------------------------------------------------------

	class gate final : public ml::sequence<100U> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::gate;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			gate(void) noexcept = default;

			/* copy constructor */
			gate(const self&) = default;

			/* move constructor */
			gate(self&&) noexcept = default;

			/* destructor */
			~gate(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class gate


	// -- V E L O C I T Y -----------------------------------------------------

	class velocity final : public ml::sequence<127U> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::velocity;


		public:
			
			// -- public lifecycle --------------------------------------------

			/* default constructor */
			velocity(void) noexcept = default;

			/* copy constructor */
			velocity(const self&) = default;

			/* move constructor */
			velocity(self&&) noexcept = default;

			/* destructor */
			~velocity(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class velocity


	// -- C H A N N E L -------------------------------------------------------

	class channel final : public ml::sequence<0U> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::channel;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			channel(void) noexcept = default;

			/* copy constructor */
			channel(const self&) = default;

			/* move constructor */
			channel(self&&) noexcept = default;

			/* destructor */
			~channel(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class channel



} // namespace ml

#endif // data_sequence_hpp
