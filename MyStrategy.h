#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include "Strategy.h"
#include <queue>
#include <thread>
#include <mutex>
#include "VehicleController.h"
#include "MoveController.h"
#include "AIController.h"

class MyStrategy : public Strategy {
public:
    typedef MyStrategy* MyStrategyPtr;
private:
    MovesQueue				movesQueue_;
    MoveControllerPtr		moveController_;
    VehicleControllerPtr	vehicleController_;
	AIControllerPtr			aiController_;

    static MyStrategyPtr this_ptr;
public:
    MyStrategy();

    void move(const model::Player& me, const model::World& world, const model::Game& game, model::Move& move) override;
    void init( const model::Player& me, const model::World& world, const model::Game& game );
	void update(const model::Player& me, const model::World& world, const model::Game& game);

    VehicleControllerPtr getVehicleController();
    MoveControllerPtr getMoveController();
	AIControllerPtr getAIController();

    static MyStrategyPtr get_instance();
};

#endif
