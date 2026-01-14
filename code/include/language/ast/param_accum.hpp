#ifndef language_ast_param_accum_hpp
#define language_ast_param_accum_hpp

#include "core/types.hpp"
#include "language/syntax/parameter.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {



	// -- T R I G  A C C U M --------------------------------------------------

	struct trig_accum final {


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = as::trig_accum;

			/* value type */
			using type = mx::u8;


			// -- public constants --------------------------------------------

			/* id */
			static constexpr pa::id id = pa::trig;

			/* is discrete */
			static constexpr bool is_discrete = true;

			/* is continuous */
			static constexpr bool is_continuous = false;

			/* default trigger */
			static constexpr mx::u8 default_value = 0U;

			/* min */
			static constexpr mx::u8 min = 0U;

			/* max */
			static constexpr mx::u8 max = 1U;


			// -- public members ----------------------------------------------

			mx::u8 value;
			mx::u8 edge;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			trig_accum(void) noexcept
			: value{0U}, edge{0U} {
			}

			/* accumulate */
			auto accumulate(const mx::u8 v, const bool e) noexcept -> void {
				value |= v;
				 //edge  = static_cast<mx::u8>(edge || e);
				edge = static_cast<mx::u8>((edge && value) || (e && v)); // MUST BE CHECKED !!!!
			}

			/* merge */
			auto merge(const self& other) noexcept -> void {
				value |= other.value;
				 //edge |= other.edge;
				//edge = static_cast<mx::u8>(edge || other.edge);
				//edge = static_cast<mx::u8>((edge || other.edge) && value);
				edge = static_cast<mx::u8>((edge && value) || (other.edge && other.value)); // MUST BE CHECKED !!!!

			}


	}; // struct trig_accum



	// -- N O T E  A C C U M --------------------------------------------------

	struct note_accum final {


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = as::note_accum;

			/* value type */
			using type = mx::u8;


			mx::u8 states[128U];
			mx::u8 active[128U];
			mx::u8 count;
			mx::u8 edge;


			// -- public constants --------------------------------------------

			/* id */
			static constexpr pa::id id = pa::note;

			/* is discrete */
			static constexpr bool is_discrete = true;

			/* is continuous */
			static constexpr bool is_continuous = false;

			/* default note */
			static constexpr type default_value = 60U;

			/* min */
			static constexpr type min = 0U;

			/* max */
			static constexpr type max = 127U;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			note_accum(void) noexcept
			: states{}, active{}, count{0U}, edge{0U} {
			}

			/* accumulate */
			auto accumulate(const mx::u8 n, const bool e) noexcept -> void {

				if (states[n] != 0U)
					return;
				states[n] = 1U;
				active[count] = n;
				++count;

				edge = static_cast<mx::u8>(edge || e);
			}

			/* merge */
			auto merge(const self& other) noexcept -> void {
				for (mx::u8 i = 0U; i < other.count; ++i) {
					const auto n = other.active[i];
					if (states[n] != 0U)
						continue;
					states[n] = 1U;
					active[count] = n;
					++count;
				}
				edge |= other.edge;
			}

			auto ensure_default(void) noexcept -> void {
				if (count != 0U)
					return;
				states[default_value] = 1U;
				active[0U] = default_value;
				count = 1U;
			}

	}; // struct note_accum


	// -- V E L O  A C C U M --------------------------------------------------

	struct velo_accum {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = as::velo_accum;

			/* value type */
			using type = mx::u8;


			// -- public constants --------------------------------------------

			/* id */
			static constexpr pa::id id = pa::velo;

			/* is discrete */
			static constexpr bool is_discrete = false;

			/* is continuous */
			static constexpr bool is_continuous = true;

			/* default velocity */
			static constexpr type default_value = 255U;

			/* min */
			static constexpr type min = 0U;

			/* max */
			static constexpr type max = 127U;


			mx::usz total;
			mx::usz count;
			mx::u8   edge;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			velo_accum(void) noexcept
			: total{0U}, count{0U}, edge{0U} {
			}

			/* accumulate */
			auto accumulate(const mx::u8 v, const bool e) noexcept -> void {
				total += v;
				++count;
				edge = static_cast<mx::u8>(edge || e);
			}

			/* merge */
			auto merge(const self& other) noexcept -> void {
				total += other.total;
				count += other.count;
				edge  |= other.edge;
			}

			/* value */
			auto value(void) const noexcept -> type {
				return count != 0U
					? static_cast<type>(total / count)
					: default_value;
			}

			//auto ensure_default(void) noexcept -> void {
			//	if (count != 0U)
			//		return;
			//	total = default_value;
			//	count = 1U;
			//}

	}; // struct velo_accum


	// -- C H A N  A C C U M --------------------------------------------------

	struct chan_accum {


		public:
			
			// -- public types ------------------------------------------------

			/* self type */
			using self = as::chan_accum;

			/* value type */
			using type = mx::u8;


			// -- public constants --------------------------------------------

			/* id */
			static constexpr pa::id id = pa::chan;

			/* is discrete */
			static constexpr bool is_discrete = true;

			/* is continuous */
			static constexpr bool is_continuous = false;

			/* default channel */
			static constexpr type default_value = 0U;

			/* min */
			static constexpr type min = 0U;

			/* max */
			static constexpr type max = 15U;


			mx::u8 states[16U];
			mx::u8 active[16U];
			mx::u8 count;
			mx::u8 edge;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			chan_accum(void) noexcept
			: states{}, active{}, count{0U}, edge{0U} {
			}

			/* accumulate */
			auto accumulate(const mx::u8 c, const bool e) noexcept -> void {

				if (states[c] != 0U)
					return;

				states[c] = 1U;
				active[count] = c;
				++count;

				edge = static_cast<mx::u8>(edge || e);
			}

			/* merge */
			auto merge(const self& other) noexcept -> void {
				for (mx::u8 i = 0U; i < other.count; ++i) {
					const auto c = other.active[i];
					if (states[c] != 0U)
						continue;
					states[c] = 1U;
					active[count] = c;
					++count;
				}
				edge |= other.edge;
			}


			auto ensure_default(void) noexcept -> void {
				if (count != 0U)
					return;

				states[default_value] = 1U;
				active[0U] = default_value;
				count = 1U;
			}

	}; // struct chan_accum


	// -- O C T A  A C C U M --------------------------------------------------

	struct octa_accum final {


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = as::octa_accum;

			/* value type */
			using type = mx::i8;


			// -- public constants --------------------------------------------

			/* id */
			static constexpr pa::id id = pa::octa;

			/* is discrete */
			static constexpr bool is_discrete = false;

			/* is continuous */
			static constexpr bool is_continuous = true;

			/* default octave */
			static constexpr type default_value = 0;

			/* positive limit */
			static constexpr type max = 127 / 12;

			/* negative limit */
			static constexpr type min = -128 / 12;


			type    sum;
			mx::u8 edge;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			octa_accum(void) noexcept
			: sum{0}, edge{0U} {
			}

			/* accumulate */
			auto accumulate(const mx::i8 o, const bool e) noexcept -> void {
				_sum(o);
				edge = static_cast<mx::u8>(edge || e);
			}

			/* merge */
			auto merge(const self& other) noexcept -> void {
				_sum(other.sum);
				edge |= other.edge;
			}

			/* sum */
			auto _sum(const mx::i8 o) noexcept -> void {

				// check for overflow
				if      (o > 0 && sum > (max - o))
					sum = max;
				else if (o < 0 && sum < (min - o))
					sum = min;
				else
					sum += o;
			}

			/* value */
			auto value(void) const noexcept -> type {
				// check for overflow?
				return static_cast<type>(sum);
			}

			/* offset */
			auto offset(void) const noexcept -> mx::i8 {
				return sum * 12;
			}

			/* apply on */
			auto apply_on(const mx::u8 nt) const noexcept -> mx::u8 {

				const auto oc = offset();

				// positive overflow
				if (oc > 0) {
					if (nt > (127 - oc))
						return 127;
					else
						return nt + static_cast<mx::u8>(oc);
				}
				// negative overflow
				else if (oc < 0) {
					mx::u8 abs_oc = static_cast<mx::u8>(-oc);
					if (nt < abs_oc)
						return 0;
					else
						return nt - abs_oc;
				}
				return nt;
			}

	}; // struct octa_accum


	// -- S E M I  A C C U M --------------------------------------------------

	struct semi_accum final {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = as::semi_accum;

			/* value type */
			using type = mx::i8;


			type    sum;
			mx::u8 edge;


			// -- public constants --------------------------------------------

			/* id */
			static constexpr pa::id id = pa::semi;

			/* is discrete */
			static constexpr bool is_discrete = false;

			/* is continuous */
			static constexpr bool is_continuous = true;

			/* default semitone */
			static constexpr type default_value = 0;

			/* positive limit */
			static constexpr type max = +127;

			/* negative limit */
			static constexpr type min = -128;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			semi_accum(void) noexcept
			: sum{0}, edge{0U} {
			}

			/* accumulate */
			auto accumulate(const mx::i8 s, const bool e) noexcept -> void {
				_sum(s);
				edge = static_cast<mx::u8>(edge || e);
			}

			/* merge */
			auto merge(const self& other) noexcept -> void {
				_sum(other.sum);
				edge |= other.edge;
			}

			/* sum */
			auto _sum(const mx::i8 s) noexcept -> void {
				// check for overflow
				if      (s > 0 && sum > (max - s))
					sum = max;
				else if (s < 0 && sum < (min - s))
					sum = min;
				else
					sum += s;
			}

			/* value */
			auto value(void) const noexcept -> type {
				return sum;
			}

			/* apply on */
			auto apply_on(const mx::u8 nt) const noexcept -> mx::u8 {

				// positive overflow
				if (sum > 0) {
					if (nt > (127 - static_cast<mx::u8>(sum)))
						return 127;
					else
						return nt + static_cast<mx::u8>(sum);
				}
				// negative overflow
				else if (sum < 0) {
					mx::u8 abs_sm = static_cast<mx::u8>(-sum);
					if (nt < abs_sm)
						return 0;
					else
						return nt - abs_sm;
				}
				return nt;
			}

	}; // struct semi_accum


	// -- G A T E  A C C U M --------------------------------------------------

	struct gate_accum final {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = as::gate_accum;

			/* value type */
			using type = mx::u8;


			// -- public constants --------------------------------------------

			/* id */
			static constexpr pa::id id = pa::gate;

			/* is discrete */
			static constexpr bool is_discrete = false;

			/* is continuous */
			static constexpr bool is_continuous = true;

			/* default gate */
			static constexpr type default_value = 10U;

			/* min */
			static constexpr type min = 0U;

			/* max */
			static constexpr type max = 127U;


			mx::usz total;
			mx::usz count;
			mx::u8   edge;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			gate_accum(void) noexcept
			: total{0U}, count{0U}, edge{0U} {
			}

			/* accumulate */
			auto accumulate(const type v, const bool e) noexcept -> void {
				total += v;
				++count;
				edge = static_cast<mx::u8>(edge || e);
			}

			/* merge */
			auto merge(const self& other) noexcept -> void {
				total += other.total;
				count += other.count;
				edge  |= other.edge;
			}


			/* value */
			auto value(void) const noexcept -> type {
				return count != 0U
					? static_cast<type>(total / count)
					: default_value;
			}

			//auto ensure_default(void) noexcept -> void {
			//	if (count != 0U)
			//		return;
			//	total = default_value;
			//	count = 1U;
			//}

	}; // struct gate_accum


	// -- P R O B  A C C U M --------------------------------------------------

	struct prob_accum final {

		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self = as::prob_accum;

			/* value type */
			using type = mx::u8;


			// -- public constants --------------------------------------------

			/* id */
			static constexpr pa::id id = pa::prob;

			/* is discrete */
			static constexpr bool is_discrete = false;

			/* is continuous */
			static constexpr bool is_continuous = true;

			/* default probability */
			static constexpr type default_value = 100U;

			/* min */
			static constexpr type min = 0U;

			/* max */
			static constexpr type max = 100U;


			mx::u8 total;
			mx::u8 count;
			mx::u8 edge;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			prob_accum(void) noexcept
			: total{0U}, count{0U}, edge{0U} {
			}

			/* accumulate */
			auto accumulate(const type v, const bool e) noexcept -> void {
				total += v;
				++count;
				edge = static_cast<mx::u8>(edge || e);
			}

			/* merge */
			auto merge(const self& other) noexcept -> void {
				total += other.total;
				count += other.count;
				edge  |= other.edge;
			}

			/* value */
			auto value(void) const noexcept -> type {
				return count != 0U
					? static_cast<type>(total / count)
					: default_value;
			}

			//auto ensure_default(void) noexcept -> void {
			//	if (count != 0U)
			//		return;
			//	total = default_value;
			//	count = 1U;
			//}

	}; // struct prob_accum



	/* is param accum
		concept to check if a type is a parameter accumulator */
	template <typename T>
	concept is_param_accum = mx::is_one_of<T, as::note_accum, as::trig_accum,
											  as::velo_accum, as::chan_accum,
											  as::octa_accum, as::semi_accum,
											  as::gate_accum, as::prob_accum>;

} // namespace as

#endif // language_ast_param_accum_hpp
