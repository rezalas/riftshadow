#include <stdlib.h>
#include <string>
#include <unordered_map>

#include "riftpath.h"

#ifndef RIFT_DEFAULT_ROOT
	#define RIFT_DEFAULT_ROOT ".."
#endif

namespace
{
	// Worked out once and then reused.  A trailing slash is removed so that
	// joining it with a suffix that starts with one cannot produce "//".
	const std::string &ResolveRoot()
	{
		static std::string root = []() {
			const char *fromEnv = getenv("RIFT_ROOT");
			std::string resolved = fromEnv != nullptr ? fromEnv : RIFT_DEFAULT_ROOT;

			while (resolved.size() > 1 && resolved.back() == '/')
				resolved.pop_back();

			return resolved;
		}();

		return root;
	}
}

const char *rift_path(const char *suffix)
{
	// Callers keep the pointer, so every distinct suffix is resolved once and
	// kept.  The map is node based, so a rehash does not move what was handed
	// out earlier.
	static std::unordered_map<std::string, std::string> resolved;

	std::string key = suffix != nullptr ? suffix : "";
	auto found = resolved.find(key);

	if (found != resolved.end())
		return found->second.c_str();

	auto added = resolved.emplace(key, ResolveRoot() + key);

	return added.first->second.c_str();
}
