// -----------------------------------------------
// File : Singleton.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

namespace hks {

template <class Type>
class Singleton {
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static Type& instance()
	{
		static Type instance;
		return instance;
	}
protected:
	Singleton() = default;
	virtual ~Singleton() = default;
};

} // namespace hks
