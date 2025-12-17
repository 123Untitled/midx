#ifndef core_platform_hpp
#define core_platform_hpp


#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)

#	if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#		define midx_endian_little
	
#	elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#		define midx_endian_big
#	endif

#else
	#error "Your compiler doesn't seem to define byte order macros, which are required to know the endianness of your target platform"
#endif


#endif // core_platform_hpp
