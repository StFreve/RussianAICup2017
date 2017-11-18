#pragma once
#include "model/Move.h"
#include "model/World.h"
#include "model/Game.h"

#include <memory>
#include <mutex>
#include <thread>

class Controller {
public:
	// <x,y>
	typedef std::pair<double, double>             Point;
public:
	virtual void update(const model::Player & me, const model::World & world, const model::Game & game)	{}
};