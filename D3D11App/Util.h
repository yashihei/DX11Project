#pragma once

#include <WinBase.h>

template <typename Type> inline void OutputDebugValue(Type& value) {
#if defined(DEBUG) || defined(_DEBUG)
	OutputDebugString(std::to_string(value).c_str());
	OutputDebugString("\n");
#endif
}
