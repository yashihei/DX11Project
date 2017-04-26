#pragma once

#include <WinBase.h>
#include <string>
#include <codecvt>

template <typename Type> inline void OutputDebugValue(Type& value) {
#if defined(DEBUG) || defined(_DEBUG)
	OutputDebugString(std::to_string(value).c_str());
	OutputDebugString("\n");
#endif
}

template <typename Type> inline const Type& Clamp(const Type& x, const Type& min, const Type& max) {
	return (x < min) ? min : ((max < x) ? max : x);
}

template <typename Type> inline Type Wrap(const Type& x, const Type& min, const Type& max) {
	const Type n = (x - min) % (max - min);
	return (n >= 0) ? (n + min) : (n + max);
}

template <typename Type> inline bool InRange(const Type& x, const Type& min, const Type& max) {
	return (min <= x) && (x <= max);
}

template <typename Container, typename Pred> inline void Erase_if(Container& c, Pred pred) {
	c.erase(std::remove_if(c.begin(), c.end(), pred), c.end());
}

inline std::wstring s2ws(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.from_bytes(str);
}

inline std::string ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
}
