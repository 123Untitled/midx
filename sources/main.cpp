#include <iostream>
#include "application.hpp"

#include <stdio.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <sys/ioctl.h>

namespace mx {

	auto tcgetpgrp(int fd) -> ::pid_t {
		pid_t pgid = 0;
		if (::ioctl(fd, TIOCGPGRP, &pgid) != 0)
			throw mx::system_error("ioctl");
		return pgid;
	}

	auto tcgetsid(const int fd) -> ::pid_t {
		const auto pgrp = mx::tcgetpgrp(fd);
		::pid_t sid = ::getsid(pgrp);

		dprintf(fd, "sid: %d, pgrp: %d\n", sid, pgrp);
		if (sid == -1 && errno == ESRCH) {
			errno = ENOTTY;
			throw mx::system_error("getsid");
		}
		
		return sid;
	}
}

int check_control_term(void) {

	const int fd = ::open("/dev/tty", O_RDWR);
	if (fd == -1)
		throw mx::system_error("open");


	constexpr int fds[3U] { STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO };

	constexpr const char* descriptor[3U] { "stdin", "stdout", "stderr" };

	// get the calling process ID
	const pid_t pid = ::getpid();
	// get the current process session ID
	const pid_t sid = ::getsid(pid);
	// get the process group ID of the session
	// for which the terminal specified by fildes is the controlling terminal.
	//const pid_t cid = tcgetsid(STDOUT_FILENO);

	int status = 0;

	for (::size_t x : { 0U, 1U, 2U }) {
		if (not ::isatty(fds[x])) {
			status = -1;
			dprintf(fd, "[%s] is not a tty.\n", descriptor[x]);
			continue;
		}
		if (sid != mx::tcgetsid(fds[x])) {
			status = -1;
			dprintf(fd, "[%s] is not the controlling terminal.\n", descriptor[x]);
		}
		//dprintf(fd, "sid: %d, tcsid: %d\n", sid, tcgetsid(fds[x]));
	}

	::close(fd);
	return status;

	//std::cout << "terminal control session id: " << npid << std::endl;
	//std::cout << "current pid: " << pid << std::endl;
	//std::cout << "current process session id: " << sid << std::endl;

}


auto main(int ac, char** av) -> int {

	try {
	if (check_control_term() != 0) {
		std::cerr << "\x1b[31merror\x1b[0m -> no controlling terminal.\r\n";
		return 1;
	}
	else {
		std::cout << "\x1b[32minfo\x1b[0m -> controlling terminal confirmed.\r\n";
	}
	}
	catch (const std::exception& e) {
		std::cerr << "\x1b[31mexception\x1b[0m -> " << e.what() << "\r\n";
		return 1;
	}
	return 0;

	//mx::storage<mx::u16, mx::usz> s{{2U, 1U}};
	//
	//s.ptr_of<0>()[0U] = 42U;
	//s.ptr_of<0>()[1U] = 84U;
	//*s.ptr_of<mx::usz>() = 123U;
	//s.count_of<mx::u16>(); // 2
	//
	//
	//s.debug();
	//
	//return 0;

	//try {
	//mx::midi m;
	//mx::midi_event mev{
	//	.channel = 0,
	//	.note	= 60,
	//	.velocity = 127,
	//	.gate	= 100
	//};
	//sleep(1);
	////char c;
	////::read(0, &c, 1); // wait for input
	//
	//mx::host_time lt{static_cast<mx::usz>(mx::timebase::ms_to_ticks(1000U))};
	//mx::host_time ts = mx::host_time::now() + lt;
	//std::cout << ts << "\r\n";
	//
	//m.note_on(mev, ts);
	//m.flush();
	//// sleep 100 ms
	//usleep(100'000);
	//m.cancel();
	//
	//
	//sleep(3);
	//}
	//catch (const std::exception& e) {
	//	std::cerr << "\x1b[31mexception\x1b[0m -> " << e.what() << "\r\n";
	//	return 1;
	//}
	//
	//
	//return 0;



	//MIDIFlushOutput
	try {

		mx::application::run();
	}
	catch (const std::exception& e) {
		std::cerr << "\x1b[31mexception\x1b[0m -> " << e.what() << "\r\n";
		return 1;
	}

	return 0;
}
