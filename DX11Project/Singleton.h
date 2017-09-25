// -----------------------------------------------
// File : Singleton.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

template<class Type>
class Singleton {
public:
	static Type* getInstance()
	{
		static Type* instance;

		if (instance == nullptr) {
			instance = new Type();
		}
		return instance;
	}
protected:
	Singleton() = default;
	virtual ~Singleton() = default;
private:
	Singleton(const Singleton&) = delete;
	Singleton& operator= (const Singleton&) = delete;
};
