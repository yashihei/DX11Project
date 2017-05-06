#pragma once

#include <WinBase.h>
#include <boost/format.hpp>

inline void Log(boost::format &bfmt) {
#if defined(DEBUG) || defined(_DEBUG)
	OutputDebugString(bfmt.str().c_str());
#endif
}

template<class First, class... Rest>
inline void Log(boost::format &bfmt, const First& first, const Rest&... rest) {
	Log(bfmt % first, rest...);
}

template<class... Rest>
inline void Log(const std::string& fmt, const Rest&... rest) {
	boost::format bfmt(fmt);
	Log(bfmt, rest...);
}
