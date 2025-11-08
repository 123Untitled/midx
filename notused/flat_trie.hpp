
	using uint = unsigned int;

	constexpr auto common_prefix_length(const char* a, const char* b) noexcept -> size_type {
		// N = min size to iterate on N
		size_type n = 0U;
		while (a[n] && b[n] && (a[n] == b[n]))
			++n;
		return n;
	}

	constexpr auto strlen(const char* s) noexcept -> size_type {
		size_type n = 0U;
		while (s[n])
			++n;
		return n;
	}

	constexpr auto compute_trie_sizes(const char* const* words, const size_type nwords) noexcept -> std::pair<size_type, size_type> {
		if (nwords == 0U)
			return {0U, 1U};

		size_type edges   = ::strlen(words[0U]);
		for (size_type i = 1U; i < nwords; ++i) {
			const size_type cpl = common_prefix_length(words[i - 1U], words[i]);
			edges += (sx::strlen(words[i]) - cpl);
		}

		size_type offsets = edges + 1U;
		return {edges, offsets};
	}

	//template <size_type NEDGES, size_type NOFFSETS>
	class flat_trie final {

		private:

			struct edge final {
				char        c;
				size_type   dest;
			};


			//const edge        _edges[NEDGES];
			//const size_type _offsets[NOFFSETS];
			std::vector<edge> _edges;
			std::vector<size_type> _offsets;

		public:

			flat_trie(const char* const* words, const size_type nwords) noexcept
			: _edges{}, _offsets{} {

				auto sizes = sx::compute_trie_sizes(words, nwords);
				_edges.resize(sizes.first);
				_offsets.resize(sizes.second);


				size_type next_offset = 1U; // 0 is root

				#define MAX_WORD_LENGTH 64
				size_type stack[MAX_WORD_LENGTH];
				stack[0U] = 0U;
				size_type sp = 1U;
				size_type edge_index = 0U;

				const char* prev = "";

				for (size_type i = 0U; i < nwords; ++i) {

					const auto cpl = common_prefix_length(prev, words[i]);

					while (sp > (cpl + 1U)) {

						auto& e = _edges[stack[--sp]];
						--sp;
						e.dest = next_offset;
					}
				}
			}

	}; // class flat_trie


