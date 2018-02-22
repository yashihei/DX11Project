// -----------------------------------------------
// File : EffectBase.h
// Copyright (c) 2018 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

class EffectBase {
public:
	EffectBase();
	virtual ~EffectBase();
	virtual void apply() = 0;
private:
};
