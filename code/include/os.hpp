#ifndef os_hpp
#define os_hpp


#if defined(__APPLE__) || defined(__MACH__)
	#define midx_macos
#elif defined(__linux__)
	#define midx_linux
#endif


#endif // os_hpp
