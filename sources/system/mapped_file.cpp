#include "system/mapped_file.hpp"
#include "diagnostics/system_error.hpp"

#include <sys/mman.h>
#include <sys/stat.h>


// -- M A P P E D  F I L E ----------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::mapped_file::mapped_file(void) noexcept
: _mapping{MAP_FAILED}, _size{0U} {
}

/* fd constructor */
mx::mapped_file::mapped_file(const int fd)
: _mapping{MAP_FAILED} /* uninitialized */ {

	// get file size
	struct ::stat st{};

	if (::fstat(fd, &st) == -1)
		throw mx::system_error{"fstat"};

	_size = static_cast<mx::usz>(st.st_size);

	if (_size == 0U)
		return;

	// map file
	_mapping = ::mmap(nullptr, _size, PROT_READ, MAP_PRIVATE, fd, 0);

	// check errors
	if (_mapping == MAP_FAILED)
		throw mx::system_error{"mmap"};
}

/* move constructor */
mx::mapped_file::mapped_file(self&& other) noexcept
: _mapping{other._mapping}, _size{other._size} {

	other._mapping = MAP_FAILED;
	other._size    = 0U;
}

/* destructor */
mx::mapped_file::~mapped_file(void) noexcept {
	if (_mapping != MAP_FAILED)
		static_cast<void>(::munmap(_mapping, _size));
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto mx::mapped_file::operator=(self&& other) noexcept -> self& {

	if (this == &other)
		return *this;

	if (_mapping != MAP_FAILED)
		static_cast<void>(::munmap(_mapping, _size));

	_mapping = other._mapping;
	_size    = other._size;
	other._mapping = MAP_FAILED;
	other._size    = 0U;

	return *this;
}


// -- public accessors --------------------------------------------------------

/* begin */
auto mx::mapped_file::begin(void) const noexcept -> const mx::u8* {
	return static_cast<const mx::u8*>(_mapping);
}

/* end */
auto mx::mapped_file::end(void) const noexcept -> const mx::u8* {
	return static_cast<const mx::u8*>(_mapping) + _size;
}

/* size */
auto mx::mapped_file::size(void) const noexcept -> mx::usz {
	return _size;
}
