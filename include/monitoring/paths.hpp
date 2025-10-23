#ifndef midilang_paths_hpp
#define midilang_paths_hpp

#include "system/unix_descriptor.hpp"
#include "system/file.hpp"
#include "diagnostics/runtime_error.hpp"
#include "diagnostics/system_error.hpp"

#include <sys/stat.h>

#include <string>
#include <iostream>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- P A T H S -----------------------------------------------------------

	class paths final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::paths;


			// -- private members ---------------------------------------------

			/* path */
			char* _path;

			/* parent */
			char* _parent;


		public:

			// -- public lifecycle --------------------------------------------

			/* path constructor */
			paths(const char* path)
			: _path{nullptr}, _parent{nullptr} {

				if (path == nullptr)
					throw ml::runtime_error{"path is null"};

				::size_t path_len = ::strlen(path);

				if (path_len == 0U)
					throw ml::runtime_error{"path is empty"};


				::size_t len = 0U;

				// check if path is relative
				if (*path != '/') {

					_path = ::getcwd(nullptr, 0);

					if (_path == nullptr)
						throw ml::system_error{"getcwd"};

					const auto cwd_len = ::strlen(_path);

					if (cwd_len == 0U)
						throw ml::runtime_error{"getcwd: empty current working directory"};

					if (*_path != '/')
						throw ml::runtime_error{"getcwd: current working directory is not absolute"};

					len = cwd_len + 1U + path_len;

					char* ptr = static_cast<char*>(::realloc(_path, len + 1U));

					if (ptr == nullptr) {
						::free(_path);
						throw ml::system_error{"realloc"};
					}

					_path = ptr;

					ptr += cwd_len;
					ptr[0U] = '/';
					++ptr;
					::memcpy(ptr, path, path_len + 1U);
				}
				else {

					_path = static_cast<char*>(::malloc(path_len + 1U));
					if (_path == nullptr)
						throw ml::system_error{"malloc"};

					::memcpy(_path, path, path_len + 1U);

					len = path_len;
				}



				// skip root slash
				::size_t r = 1U, w = 1U;

				while (r < len) {

					// skip repeated separators
					while (r < len && _path[r] == '/')
						++r;

					// break if end of path
					if (r >= len)
						break;

					// find segment end
					::size_t s = r;

					while (r < len && _path[r] != '/')
						++r;

					size_t seg_len = r - s;

					// empty segment
					if (seg_len == 0U)
						continue;

					// skip '.'
					if (seg_len == 1U && _path[s] == '.')
						continue;

					// skip '..'
					else if (seg_len == 2U && _path[s] == '.' && _path[s + 1U] == '.') {

						if (w == 1U)
							continue;
						// back to previous directory
						--w; // ignore last '/'
						while (w > 0U && _path[w - 1U] != '/')
							--w;
					}

					else {
						// write '/'
						if (w == 0U || _path[w - 1U] != '/')
							_path[w++] = '/';

						::memmove(_path + w, _path + s, seg_len);
						w += seg_len;
					}
				}

				// remove last separator
				if (w > 1U && _path[w - 1U] == '/')
					--w;

				// null terminate path
				_path[w] = '\0';

				len = w;


				// -- check file existence and extension ----------------------

				// check for regular file
				struct ::stat st;
				if (::stat(_path, &st) == -1) {
					::free(_path);
					throw ml::system_error{"stat"};
				}

				if ((st.st_mode & S_IFMT) != S_IFREG) {
					::free(_path);
					throw ml::runtime_error{"path is not a regular file"};
				}


				const char* ext = ::strrchr(_path, '.');
				if (ext == nullptr || ::strcmp(ext, ".ml") != 0) {
					::free(_path);
					throw ml::runtime_error{"path must have .ml extension"};
				}


				const char* slash = ::strrchr(_path, '/');

				if (slash == nullptr) {
					::free(_path);
					throw ml::runtime_error{"invalid path"};
				}

				if (slash == _path) {
					_parent = static_cast<char*>(::malloc(2U));
					if (_parent == nullptr) {
						::free(_path);
						throw ml::system_error{"malloc"};
					}
					_parent[0U] = '/';
					_parent[1U] = '\0';
				}
				else {
					const auto parent_len = static_cast<::size_t>(slash - _path);

					_parent = static_cast<char*>(::malloc(parent_len + 1U));
					if (_parent == nullptr) {
						::free(_path);
						throw ml::system_error{"malloc"};
					}

					::memcpy(_parent, _path, parent_len);
					_parent[parent_len] = '\0';
				}

				std::cout << "path: \x1b[32m" << _path << "\x1b[0m\n";
				std::cout << "parent: \x1b[32m" << _parent << "\x1b[0m\n";
			}

			/* deleted copy constructor */
			paths(const self&) = delete;

			/* move constructor */
			paths(self&& other) noexcept
			: _path{other._path}, _parent{other._parent} {
				other._path = nullptr;
				other._parent = nullptr;
			}

			/* destructor */
			~paths(void) noexcept {
				if (_path != nullptr)
					::free(_path);
				if (_parent != nullptr)
					::free(_parent);
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				if (this == &other)
					return *this;

				if (_path != nullptr)
					::free(_path);

				if (_parent != nullptr)
					::free(_parent);

				_path = other._path;
				_parent = other._parent;
				other._path = nullptr;
				other._parent = nullptr;

				return *this;
			}


			// -- public accessors --------------------------------------------

			/* file */
			auto file(void) const noexcept -> const char* {
				return _path;
			}

			/* folder */
			auto folder(void) const noexcept -> const char* {
				return _parent;
			}

	}; // class paths

} // namespace ml

#endif // midilang_paths_hpp
