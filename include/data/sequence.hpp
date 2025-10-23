#ifndef midilang_data_sequence_hpp
#define midilang_data_sequence_hpp

#include <vector>
#include "time/signature.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- S E Q U E N C E -----------------------------------------------------

	template <typename T, T D>
	class sequence {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::sequence<T, D>;


			// -- private members ---------------------------------------------

		protected:

			/* signature */
			ml::signature _signature;

			/* sequence */
			std::vector<T> _sequence;

			/* current */
			T _current;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			sequence(void) noexcept
			: _signature{}, _sequence{}, _current{D} {
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

			/* signature */
			auto signature(void) noexcept -> ml::signature& {
				return _signature;
			}

			/* const signature */
			auto signature(void) const noexcept -> const ml::signature& {
				return _signature;
			}

			/* data */
			auto data(void) noexcept -> std::vector<T>& {
				return _sequence;
			}

			/* const data */
			auto data(void) const noexcept -> const std::vector<T>& {
				return _sequence;
			}

			/* current */
			auto current(void) const noexcept -> const T& {
				return _current;
			}


			// -- public modifiers --------------------------------------------

			/* push */
			auto push(const T& value) -> void {
				_sequence.emplace_back(value);
				_current = value; // ? for bug but not fixed
			}

			/* repush */
			auto repush(void) -> void {
				_sequence.emplace_back(
						_sequence.empty() ?
							D : _sequence.back()
				);
				_current = _sequence.back(); // ? for bug but not fixed
			}

			/* reset */
			auto reset(void) noexcept -> void {
				_sequence.clear();
				//_current = def; // this fixes the bug when reparse (nano)
				// but not fix when delete (vim)
			}


			// -- public methods ----------------------------------------------

			/* next */
			auto next(const ml::u64& timeline) noexcept -> bool {

				// check if signature is time
				if (_signature.is_time(timeline) == false)
					return false;

				if (_sequence.empty() == true)
					return true;

				// get count from timeline beginning
				const auto count = _signature.count(timeline);

				// get current value
				_current = _sequence[count % _sequence.size()];

				return true;
			}

	}; // class sequence


	// -- N O T E S -----------------------------------------------------------

	class notes final : public ml::sequence<ml::u8, 60U> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::notes;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			notes(void) noexcept = default;

			/* copy constructor */
			notes(const self&) = default;

			/* move constructor */
			notes(self&&) noexcept = default;

			/* destructor */
			~notes(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class notes


	// -- T R I G G E R S -----------------------------------------------------

	class triggers final : public ml::sequence<bool, false> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::triggers;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			triggers(void) noexcept = default;

			/* copy constructor */
			triggers(const self&) = default;

			/* move constructor */
			triggers(self&&) noexcept = default;

			/* destructor */
			~triggers(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class triggers



	// -- G A T E S -----------------------------------------------------------

	class gates final : public ml::sequence<ml::u8, 100U> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::gates;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			gates(void) noexcept = default;

			/* copy constructor */
			gates(const self&) = default;

			/* move constructor */
			gates(self&&) noexcept = default;

			/* destructor */
			~gates(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class gates


	class velocities final : public ml::sequence<ml::u8, 127U> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::velocities;


		public:
			
			// -- public lifecycle --------------------------------------------

			/* default constructor */
			velocities(void) noexcept = default;

			/* copy constructor */
			velocities(const self&) = default;

			/* move constructor */
			velocities(self&&) noexcept = default;

			/* destructor */
			~velocities(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class velocities


	class channels final : public ml::sequence<ml::u8, 0U> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::channels;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			channels(void) noexcept = default;

			/* copy constructor */
			channels(const self&) = default;

			/* move constructor */
			channels(self&&) noexcept = default;

			/* destructor */
			~channels(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class channels



} // namespace ml

#endif // midilang_data_sequence_hpp
