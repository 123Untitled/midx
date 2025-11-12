#include "time/realtime.hpp"
#include "time/timebase.hpp"

#include <mach/mach.h>


/* realtime */
auto mx::realtime(const mx::i64& nano_clock) noexcept -> bool {

	// policy data
	::thread_time_constraint_policy_data_t policy {

		/* This is the nominal amount of time between separate
		// processing arrivals, specified in absolute time units.
		// A value of 0 indicates that there is no inherent periodicity
		// in the computation.
		*/  //policy.period		= 50 * msToAbsolute; // 50 ms periodicity - time between
		.period = static_cast<mx::u32>(nano_clock * mx::timebase::nano_to_absolute()),
		// chatGPR recommendation
		/* Le champ period de la structure thread_time_constraint_policy_data_t spécifie la durée nominale entre les arrivées de traitement séparées, en unités de temps absolues. Cela signifie que cette valeur indique la fréquence à laquelle le thread en cours est censé être exécuté.
		Par exemple, si la valeur de period est définie sur 50 * msToAbsolute, cela signifie que le thread en cours est censé être exécuté toutes les 50 millisecondes.
		Une valeur de 0 pour period indique qu'il n'y a pas de périodicité inhérente dans le calcul du thread en cours. Cela signifie que le thread n'a pas de fréquence d'exécution fixe et peut être exécuté à n'importe quel moment.
		*/


		/* This is the nominal amount of computation
		// time needed during a separate processing arrival, specified
		// in absolute time units.
		*/ //policy.computation	= 1 * msToAbsolute; // 1 ms of work
		.computation = static_cast<mx::u32>(1 * mx::timebase::ms_to_absolute()), // 1 ms of work
		//Il est également important de tenir compte de la précision de l'horloge requise par votre application. Si une précision élevée est requise, une valeur de computation plus grande peut être nécessaire pour garantir que le thread a suffisamment de temps pour effectuer tous les calculs nécessaires. Cependant, si une précision moins élevée est acceptable, une valeur de computation plus petite peut être utilisée pour réduire la charge de travail du processeur.

		/* This is the maximum amount of real time that
		// may elapse from the start of a separate processing arrival
		// to the end of computation for logically correct functioning,
		// specified in absolute time units.  Must be (>= computation).
		// Note that latency = (constraint - computation).
		*/ //policy.constraint	= 2 * msToAbsolute;
		.constraint = static_cast<mx::u32>(1 * mx::timebase::ms_to_absolute()),

		/* This indicates that the computation may be
		// interrupted, subject to the constraint specified above.
		*/
		.preemptible = true
	};


	// get current thread
	const auto self_thread = ::mach_thread_self();

	// set thread policy
	return ::thread_policy_set(self_thread,
							   // real-time behavior
							   THREAD_TIME_CONSTRAINT_POLICY,
							   // policy data
							   reinterpret_cast<::thread_policy_t>(&policy),
							   // policy count
							   THREAD_TIME_CONSTRAINT_POLICY_COUNT)
		!= KERN_SUCCESS;
}
