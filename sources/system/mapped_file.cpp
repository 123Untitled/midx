#include "system/mapped_file.hpp"
#include "diagnostics/system_error.hpp"

#include <sys/mman.h>
#include <sys/stat.h>


// -- M A P P E D  F I L E ----------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
ml::mapped_file::mapped_file(void) noexcept
: _mapping{MAP_FAILED}, _size{0U} {
}

/* fd constructor */
ml::mapped_file::mapped_file(const int fd)
: _mapping{MAP_FAILED} /* uninitialized */ {

	// get file size
	struct ::stat st{};

	if (::fstat(fd, &st) == -1)
		throw ml::system_error{"fstat"};

	_size = static_cast<ml::usz>(st.st_size);

	if (_size == 0U)
		return;

	// map file
	_mapping = ::mmap(nullptr, _size, PROT_READ, MAP_PRIVATE, fd, 0);

	// check errors
	if (_mapping == MAP_FAILED)
		throw ml::system_error{"mmap"};
}

/* move constructor */
ml::mapped_file::mapped_file(self&& other) noexcept
: _mapping{other._mapping}, _size{other._size} {

	other._mapping = MAP_FAILED;
	other._size    = 0U;
}

/* destructor */
ml::mapped_file::~mapped_file(void) noexcept {
	if (_mapping != MAP_FAILED)
		static_cast<void>(::munmap(_mapping, _size));
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto ml::mapped_file::operator=(self&& other) noexcept -> self& {

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
auto ml::mapped_file::begin(void) const noexcept -> const ml::u8* {
	return static_cast<const ml::u8*>(_mapping);
}

/* end */
auto ml::mapped_file::end(void) const noexcept -> const ml::u8* {
	return static_cast<const ml::u8*>(_mapping) + _size;
}

/* size */
auto ml::mapped_file::size(void) const noexcept -> ml::usz {
	return _size;
}
