#ifndef core_config_class_hpp
#define core_config_class_hpp


#define noexcept_default_class(T) \
	T(void) noexcept = default; \
	T(const T&) noexcept = default; \
	T(T&&) noexcept = default; \
	~T(void) noexcept = default; \
	auto operator=(const T&) noexcept -> T& = default; \
	auto operator=(T&&) noexcept -> T& = default

#define non_instantiable_class(T) \
	T(void) = delete; \
	T(const T&) = delete; \
	T(T&&) = delete; \
	~T(void) = delete; \
	auto operator=(const T&) -> T& = delete; \
	auto operator=(T&&) -> T& = delete


#define default_noexcept_moveable_class(type) \
	type(type&&) noexcept = default; \
	auto operator=(type&&) noexcept -> type& = default

#define uncopyable_class(type) \
	type(const type&) = delete; \
	auto operator=(const type&) -> type& = delete



	#define noexcept_default_movable(T) \
		T(T&&) noexcept = default; \
		auto operator=(T&&) noexcept -> T& = default

	#define noexcept_default_copyable(T) \
		T(const T&) noexcept = default; \
		auto operator=(const T&) noexcept -> T& = default

	#define noexcept_default_movable_copyable(T) \
		noexcept_default_movable(T); \
		noexcept_default_copyable(T)

	#define non_copyable(T) \
		T(const T&) = delete; \
		auto operator=(const T&) -> T& = delete

	#define non_movable(T) \
		T(T&&) = delete; \
		auto operator=(T&&) -> T& = delete

	#define non_copyable_movable(T) \
		non_movable(T); \
		non_copyable(T)


#endif // core_config_class_hpp
