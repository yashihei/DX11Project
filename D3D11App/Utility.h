#pragma once

#include <string>
#include <WinNls.h>
#include <vector>
#include <stdexcept>

template <class Type> inline const Type& Clamp(const Type& x, const Type& min, const Type& max) {
	return (x < min) ? min : ((max < x) ? max : x);
}

template <class Type> inline Type Wrap(const Type& x, const Type& min, const Type& max) {
	const Type n = (x - min) % (max - min);
	return (n >= 0) ? (n + min) : (n + max);
}

template <class Type> inline bool InRange(const Type& x, const Type& min, const Type& max) {
	return (min <= x) && (x <= max);
}

template <class Container, class Pred> inline void Erase_if(Container& c, Pred pred) {
	c.erase(std::remove_if(c.begin(), c.end(), pred), c.end());
}

template<class T, class F> inline void Each(const T &v, const F &f) {
	std::for_each(v.begin(), v.end(), f);
}

//from:http://nekko1119.hatenablog.com/entry/2017/01/02/054629
inline std::wstring s2ws(const std::string& src) {
	auto const dest_size = ::MultiByteToWideChar(CP_ACP, 0U, src.data(), -1, nullptr, 0U);
    std::vector<wchar_t> dest(dest_size, L'\0');
    if (::MultiByteToWideChar(CP_ACP, 0U, src.data(), -1, dest.data(), dest.size()) == 0) {
        throw std::system_error{static_cast<int>(::GetLastError()), std::system_category()};
    }
    return std::wstring(dest.data());
}

inline std::string ws2s(const std::wstring& src) {
    auto const dest_size = ::WideCharToMultiByte(CP_ACP, 0U, src.data(), -1, nullptr, 0, nullptr, nullptr);
    std::vector<char> dest(dest_size, '\0');
    if (::WideCharToMultiByte(CP_ACP, 0U, src.data(), -1, dest.data(), dest.size(), nullptr, nullptr) == 0) {
        throw std::system_error{static_cast<int>(::GetLastError()), std::system_category()};
    }
    return std::string(dest.data());
}

inline std::string GetDirPath(const std::string& filePath) {
	auto const dest_size = filePath.size();
	std::vector<char> dest(dest_size, '\0');
	_splitpath_s(filePath.c_str(), NULL, 0, dest.data(), dest_size, NULL, 0, NULL, 0);
	return std::string(dest.data());
}
