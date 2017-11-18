#pragma once

#include "Controller.h"
#include  "MovesQueue.h"

class AIController : public Controller {
public:
	AIController(const model::Player & me, const model::World & world, const model::Game & game);
	void update(const model::Player & me, const model::World & world, const model::Game & game);

	MovesQueue get_first_moves();
	MovesQueue get_next_moves();
private:
    int  step;
    int  tickCount;
    Controller::Point direction;
};

typedef std::shared_ptr<AIController> AIControllerPtr;