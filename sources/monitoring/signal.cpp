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
: mx::watcher{}, _fds{-1, -1} {

	if (::pipe(_fds) == -1)
		throw mx::system_error{"pipe"};

	self::_wr = _fds[1U];

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

/* destructor */
mx::signal::~signal(void) noexcept {
	if (_fds[0U] != -1)
		static_cast<void>(::close(_fds[0U]));
	if (_fds[1U] != -1)
		static_cast<void>(::close(_fds[1U]));
}


static auto signal_str(const int sig) -> const char* {
	switch (sig) {
		case SIGHUP:    return "terminal line hangup";
		case SIGINT:    return "interrupt program";
		case SIGQUIT:   return "quit program";
		case SIGILL:    return "illegal instruction";
		case SIGTRAP:   return "trace trap";
		case SIGABRT:   return "abort program (formerly SIGIOT)";
		case SIGEMT:    return "emulate instruction executed";
		case SIGFPE:    return "floating-point exception";
		case SIGKILL:   return "kill program";
		case SIGBUS:    return "bus error";
		case SIGSEGV:   return "segmentation violation";
		case SIGSYS:    return "non-existent system call invoked";
		case SIGPIPE:   return "write on a pipe with no reader";
		case SIGALRM:   return "real-time timer expired";
		case SIGTERM:   return "software termination signal";
		case SIGURG:    return "urgent condition present on socket";
		case SIGSTOP:   return "stop (cannot be caught or ignored)";
		case SIGTSTP:   return "stop signal generated from keyboard";
		case SIGCONT:   return "continue after stop";
		case SIGCHLD:   return "child status has changed";
		case SIGTTIN:   return "background read attempted from control terminal";
		case SIGTTOU:   return "background write attempted to control terminal";
		case SIGIO:     return "I/O is possible on a descriptor (see fcntl(2))";
		case SIGXCPU:   return "cpu time limit exceeded (see setrlimit(2))";
		case SIGXFSZ:   return "file size limit exceeded (see setrlimit(2))";
		case SIGVTALRM: return "virtual time alarm (see setitimer(2))";
		case SIGPROF:   return "profiling timer alarm (see setitimer(2))";
		case SIGWINCH:  return "Window size change";
		case SIGINFO:   return "status request from keyboard";
		case SIGUSR1:   return "User defined signal 1";
		case SIGUSR2:   return "User defined signal 2";

		default:      return "UNKNOWN";
	}
}


/* ident */
auto mx::signal::ident(void) const noexcept -> int {
	return _fds[0U];
}


// -- public overrides --------------------------------------------------------

/* on event */
auto mx::signal::on_event(mx::application& app, const struct ::kevent& ev) -> void {

	std::cout << "event filter: " << ev.filter << std::endl;
	if ((ev.filter != EVFILT_READ))
		return;

	int sig = 0;

	const ::ssize_t res = ::read(_fds[0U], &sig, sizeof(int));

	if (res == -1)
		throw mx::system_error{"read"};

	::write(STDOUT_FILENO, "signal: ", 8U);
	const char* desc = signal_str(sig);
	::write(STDOUT_FILENO, desc, std::char_traits<char>::length(desc));
	::write(STDOUT_FILENO, "\n", 1U);

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

