#pragma once


#if defined(DEBUG) || defined(_DEBUG)
#	define THROW_IF_DEBUG
#else
#	define THROW_IF_DEBUG noexcept
#endif
