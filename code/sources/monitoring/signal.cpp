#include "monitoring/signal.hpp"
#include "diagnostics/system_error.hpp"
#include "application.hpp"

#include <unistd.h>
#include <signal.h>
#include <sys/event.h>


// -- private static members --------------------------------------------------

/* write end */
int mx::signal::_wr = -1;


// -- private static methods --------------------------------------------------

/* signal handler */
auto mx::signal::_handler(const int sig) noexcept -> void {
	static_cast<void>(::write(self::_wr, &sig, sizeof(int)));
}


// -- public lifecycle --------------------------------------------------------

/* monitor constructor */
mx::signal::signal(const mx::monitor& monitor)
: mx::watcher{}, _pipe{mx::pipe()} {

	self::_wr = _pipe.wr();

	// subscribe to monitor
	monitor.add_read(*this);

	struct ::sigaction sa{};
	sa.sa_handler = self::_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	for (int i = 1; i < NSIG; ++i) {

		if (::sigaction(i, &sa, nullptr) == -1 && errno != EINVAL)
			throw mx::system_error{"sigaction"};
	}
}


enum : int {
	sighup,  sigint,    sigquit, sigill,
	sigtrap, sigabrt,   sigemt,  sigfpe,
	sigkill, sigbus,    sigsegv, sigsys,
	sigpipe, sigalrm,   sigterm, sigurg,
	sigstop, sigtstp,   sigcont, sigchld,
	sigttin, sigttou,   sigio,   sigxcpu,
	sigxfsz, sigvtalrm, sigprof, sigwinch,
	siginfo, sigusr1,   sigusr2,
	unknown,
	num_signals
};

constexpr mx::string_view descs[num_signals] {
	mx::string_view{"terminal line hangup"},
	mx::string_view{"interrupt program"},
	mx::string_view{"quit program"},
	mx::string_view{"illegal instruction"},
	mx::string_view{"trace trap"},
	mx::string_view{"abort program (formerly SIGIOT)"},
	mx::string_view{"emulate instruction executed"},
	mx::string_view{"floating-point exception"},
	mx::string_view{"kill program"},
	mx::string_view{"bus error"},
	mx::string_view{"segmentation violation"},
	mx::string_view{"non-existent system call invoked"},
	mx::string_view{"write on a pipe with no reader"},
	mx::string_view{"real-time timer expired"},
	mx::string_view{"software termination signal"},
	mx::string_view{"urgent condition present on socket"},
	mx::string_view{"stop (cannot be caught or ignored)"},
	mx::string_view{"stop signal generated from keyboard"},
	mx::string_view{"continue after stop"},
	mx::string_view{"child status has changed"},
	mx::string_view{"background read attempted from control terminal"},
	mx::string_view{"background write attempted to control terminal"},
	mx::string_view{"I/O is possible on a descriptor (see fcntl(2))"},
	mx::string_view{"cpu time limit exceeded (see setrlimit(2))"},
	mx::string_view{"file size limit exceeded (see setrlimit(2))"},
	mx::string_view{"virtual time alarm (see setitimer(2))"},
	mx::string_view{"profiling timer alarm (see setitimer(2))"},
	mx::string_view{"Window size change"},
	mx::string_view{"status request from keyboard"},
	mx::string_view{"User defined signal 1"},
	mx::string_view{"User defined signal 2"},
	mx::string_view{"unknown signal"}
};



static auto signal_str(const int sig) -> const mx::string_view& {
	switch (sig) {
		case SIGHUP:    return descs[sighup];
		case SIGINT:    return descs[sigint];
		case SIGQUIT:   return descs[sigquit];
		case SIGILL:    return descs[sigill];
		case SIGTRAP:   return descs[sigtrap];
		case SIGABRT:   return descs[sigabrt];
		case SIGEMT:    return descs[sigemt];
		case SIGFPE:    return descs[sigfpe];
		case SIGKILL:   return descs[sigkill];
		case SIGBUS:    return descs[sigbus];
		case SIGSEGV:   return descs[sigsegv];
		case SIGSYS:    return descs[sigsys];
		case SIGPIPE:   return descs[sigpipe];
		case SIGALRM:   return descs[sigalrm];
		case SIGTERM:   return descs[sigterm];
		case SIGURG:    return descs[sigurg];
		case SIGSTOP:   return descs[sigstop];
		case SIGTSTP:   return descs[sigtstp];
		case SIGCONT:   return descs[sigcont];
		case SIGCHLD:   return descs[sigchld];
		case SIGTTIN:   return descs[sigttin];
		case SIGTTOU:   return descs[sigttou];
		case SIGIO:     return descs[sigio];
		case SIGXCPU:   return descs[sigxcpu];
		case SIGXFSZ:   return descs[sigxfsz];
		case SIGVTALRM: return descs[sigvtalrm];
		case SIGPROF:   return descs[sigprof];
		case SIGWINCH:  return descs[sigwinch];
		case SIGINFO:   return descs[siginfo];
		case SIGUSR1:   return descs[sigusr1];
		case SIGUSR2:   return descs[sigusr2];

		default:      return descs[unknown];
	}
}


/* ident */
auto mx::signal::ident(void) const noexcept -> int {
	return _pipe.rd();
}


// -- public overrides --------------------------------------------------------

/* on event */
auto mx::signal::on_event(mx::application& app, const struct ::kevent& ev) -> void {

	if ((ev.filter != EVFILT_READ))
		return;

	int sig = 0;

	const ::ssize_t res = ::read(_pipe.rd(), &sig, sizeof(int));

	if (res == -1)
		throw mx::system_error{"read"};

	::write(STDOUT_FILENO, "signal: ", 8U);
	const auto& desc = signal_str(sig);
	::write(STDOUT_FILENO, desc.data(), desc.size());
	::write(STDOUT_FILENO, "\r\n", 2U);

	switch (sig) {

		case SIGINT:
		case SIGQUIT:
		case SIGTERM:
		case SIGABRT:
		case SIGILL:
		case SIGTRAP:
		case SIGFPE:
		case SIGBUS:
		case SIGSEGV:
		case SIGSYS:

		case SIGKILL:
		case SIGSTOP:

		case SIGHUP:
			app.exit();
			break;
		default:
	}

		//case SIGALRM:
		//case SIGVTALRM:
		//case SIGPROF:
		//case SIGXCPU:
		//case SIGXFSZ:
		//case SIGEMT:
		//
		//case SIGPIPE:
		//case SIGURG:
		//case SIGCHLD:
		//case SIGWINCH:
		//case SIGCONT:
		//case SIGTSTP:
		//case SIGTTIN:
		//case SIGTTOU:
		//case SIGINFO:
		//case SIGUSR1:
		//case SIGUSR2:
		//case SIGIO:
}

