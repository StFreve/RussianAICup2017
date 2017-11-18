#pragma once
#include <functional>
#include "MovesQueue.h"
#include "VehicleController.h"
#include "MoveController.h"

class AIStrategy
{
private:
    bool strategyFinished_;
protected:
    void set_finished();

public:
    AIStrategy();
    virtual bool                available() = 0;
    virtual MovesQueue          get_next_move() = 0;
    bool                        finished() const;
};

typedef std::shared_ptr<AIStrategy> AIStrategyPtr;