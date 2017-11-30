// -----------------------------------------------
// File : Actor.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <memory>
#include <list>
#include <functional>
#include <algorithm>

class Actor {
public:
	Actor() : m_enable(true) {}
	virtual ~Actor() = default;
	virtual void update() = 0;
	virtual void draw() = 0;
	void kill() { m_enable = false; }
	bool isEnabled() const { return m_enable; }
private:
	bool m_enable;
};

template <typename Type>
class ActorManager {
public:
	ActorManager() = default;
	virtual ~ActorManager() = default;

	void add(std::shared_ptr<Type> actor)
	{
		m_actors.push_back(actor);
	}

	void clear()
	{
		m_actors.clear();
	}

	int size()
	{
		return m_actors.size();
	}

	void updateAll()
	{
		for (auto& actor : m_actors) {
			actor->update();
		}
		m_actors.erase(std::remove_if(m_actors.begin(), m_actors.end(),
			[](std::shared_ptr<Type> actor) { return !actor->isEnabled(); }), m_actors.end());
	}

	void drawAll()
	{
		for (const auto& actor : m_actors) {
			actor->draw();
		}
	}

	void sort(std::function<bool(std::shared_ptr<Type>, std::shared_ptr<Type>)> rule)
	{
		m_actors.sort(rule);
	}

	typename std::list<std::shared_ptr<Type>>::const_iterator begin() { return m_actors.begin(); }
	typename std::list<std::shared_ptr<Type>>::const_iterator end() { return m_actors.end(); }
private:
	std::list<std::shared_ptr<Type>> m_actors;
};
