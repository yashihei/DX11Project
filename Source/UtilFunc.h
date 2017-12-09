// -----------------------------------------------
// File : UtilFunc.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <algorithm>

namespace hks {

template <class Type>
inline const Type& Clamp(const Type& x, const Type& min, const Type& max)
{
	return (x < min) ? min : ((max < x) ? max : x);
}

template <class Type>
inline Type Wrap(const Type& x, const Type& min, const Type& max)
{
	const Type n = (x - min) % (max - min);
	return (n >= 0) ? (n + min) : (n + max);
}

template <class Type>
inline bool InRange(const Type& x, const Type& min, const Type& max)
{
	return (min <= x) && (x <= max);
}

template <class Container, class Pred>
inline void Erase_if(Container& c, Pred pred)
{
	c.erase(std::remove_if(c.begin(), c.end(), pred), c.end());
}

template <class T, class F>
inline void Each(const T& v, const F& f)
{
	std::for_each(v.begin(), v.end(), f);
}

} //namespace hks
