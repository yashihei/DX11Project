// -----------------------------------------------
// File : Random.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <random>

namespace hks {

using RNG = std::mt19937;

// 整数型 -> int_distirbution、浮動小数点型 -> real_distribution
template <class Type>
using distribution_type =
	std::conditional_t<std::is_floating_point<Type>::value,
	std::uniform_real_distribution<Type>,
	std::uniform_int_distribution<Type>>;

inline RNG& getRNG()
{
	static std::random_device rd;
	static RNG rng(rd());
	return rng;
}

inline void Reseed(const RNG::result_type seed)
{
	getRNG().seed(seed);
}

// [0 - 1.0]
inline double Random()
{
	return getRNG()() / static_cast<double>(0xFFffFFff);
}

// [min - max]
template <class Type>
inline Type Random(const Type& min, const Type& max)
{
	return distribution_type<Type>(min, max)(getRNG());
}

// [0 - max]
template <class Type>
inline Type Random(const Type& max)
{
	return Random<Type>(0, max);
}

// true or false
inline bool RandomBool()
{
	return std::bernoulli_distribution()(getRNG());
}

template <class Iterator>
inline auto RandomSelect(Iterator begin, Iterator end)
{
	assert(begin != end);
	// random(イテレータの長さ-1)だけイテレータを進める
	std::advance(begin, Random(std::distance(begin, end) - 1));
	return *begin;
}

template <class Container>
inline auto RandomSelect(const Container& c)
{
	return RandomSelect(std::begin(c), std::end(c));
}

template <class Type>
inline Type RandomSelect(std::initializer_list<Type> ilist)
{
	return RandomSelect(std::begin(ilist), std::end(ilist));
}

} // namespace hks
