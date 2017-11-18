#pragma once

#include "Controller.h"
#include "MovesQueue.h"
#include "AIStrategy.h"

class AIController : public Controller {
private:
    typedef std::set<AIStrategyPtr> StrategySet;
public:
	AIController(const model::Player & me, const model::World & world, const model::Game & game);
	void update(const model::Player & me, const model::World & world, const model::Game & game);

    MovesQueue move();
    void  add_strategy( AIStrategyPtr strategy );
    const model::Player&  getPlayerInfo();
    const model::World&  getWorldInfo();
    const model::Game&  getGamerInfo();
private:
    int  step;
    int  tickCount;

    const model::Player*    player_;
    const model::World*     world_;
    const model::Game*      game_;
    StrategySet             strategies_;
    StrategySet             finishedStrategies_;
    AIStrategyPtr           curentStrategy_;
};

typedef std::shared_ptr<AIController> AIControllerPtr;