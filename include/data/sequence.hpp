#ifndef data_sequence_hpp
#define data_sequence_hpp

#include <vector>
#include "time/signature.hpp"
#include "language/tokens.hpp"
#include "language/syntax/specifier.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- constants -----------------------------------------------------------

	enum seq_type : mx::usz {
		TR, // trigger
		NT, // note
		GA, // gate
		VL, // velocity
		OC, // octave
		SE, // semitone
		CH, // channel
		PR, // probability
		seq_count,
		invalid_seq = seq_count
	};

	constexpr mx::i8 seq_defs[seq_type::seq_count] {
		0,    // TR
		60,   // NT
		100,  // GA
		127,  // VL
		0,    // OC
		0,    // SE
		0,    // CH
		100   // PR
	}; 

	constexpr mx::seq_type to_seq_type[static_cast<mx::usz>(sp::id::count)] {
		seq_type::TR, // trig
		seq_type::NT, // note
		seq_type::GA, // gate
		seq_type::VL, // velo
		seq_type::OC, // octa
		seq_type::SE, // semi
		seq_type::CH, // chan
		seq_type::PR  // prob
	};



	struct seq_slot final {
		public:
			mx::i8 value;
			mx::signature sign;
			tk::raw::token* token;
			mx::usz depth;
	};


	// -- S E Q U E N C E -----------------------------------------------------

	class sequence {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::sequence;


			// -- private members ---------------------------------------------

			/* sequence */
			std::vector<mx::seq_slot> _sequence;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			sequence(void) noexcept
			: _sequence{} {
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
			auto data(void) noexcept -> std::vector<mx::seq_slot>& {
				return _sequence;
			}

			/* const data */
			auto data(void) const noexcept -> const std::vector<mx::seq_slot>& {
				return _sequence;
			}

			/* is empty */
			auto empty(void) const noexcept -> bool {
				return _sequence.empty();
			}


			// -- public modifiers --------------------------------------------

			/* push */
			auto push(const mx::i8 value, tk::raw::token& tk, const mx::signature& sign) -> void {

				const auto repeat = sign.modulus();

				for (mx::usz i = 0U; i < repeat; ++i) {

					_sequence.emplace_back(
						mx::seq_slot{
							.value = value,
							.sign  = sign,
							.token = &tk,
							.depth = 0U
						}
					);
				} 
			}

			/* push */
			auto push_trig(const mx::i8 value, tk::raw::token& tk, const mx::signature& sign) -> void {

				const auto repeat = sign.modulus();

				_sequence.emplace_back(
						mx::seq_slot{
						.value = value,
						.sign  = sign,
						.token = &tk,
						.depth = 0U
						}
				);

				for (mx::usz i = 1U; i < repeat; ++i) {

					_sequence.emplace_back(
						mx::seq_slot{
							.value = 0,
							.sign  = sign,
							.token = &tk,
							.depth = 0U
						}
					);
				} 
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
			auto next(const mx::u64& timeline) const noexcept -> const mx::seq_slot& {

				return _sequence[timeline % _sequence.size()];

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
			}

	}; // class sequence

} // namespace mx

#endif // data_sequence_hpp
