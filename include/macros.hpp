#ifndef ml_macros_hpp


#define noexcept_default_class(class_name) \
	class_name(void) noexcept = default; \
	~class_name(void) noexcept = default; \
	class_name(const class_name&) noexcept = default; \
	class_name(class_name&&) noexcept = default; \
	auto operator=(const class_name&) noexcept -> class_name& = default; \
	auto operator=(class_name&&) noexcept -> class_name& = default


#define non_instantiable_class(class_name) \
	class_name(void) = delete; \
	~class_name(void) = delete; \
	class_name(const class_name&) = delete; \
	class_name(class_name&&) = delete; \
	auto operator=(const class_name&) -> class_name& = delete; \
	auto operator=(class_name&&) -> class_name& = delete

#endif // ml_macros_hpp
