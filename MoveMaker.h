#pragma once
#include "model/Move.h"
#include "model/World.h"
#include "model/Game.h"
#include "MovesQueue.h"
#include "VehicleController.h"
#include <queue>
#include <list>
#include <vector>
#include <set>
#include <memory>
#include <mutex>
#include <thread>

class MoveMaker {
public:
    typedef VehicleController::Point              Point;
    typedef std::pair<int, int>                   Direction;
    typedef model::Move                           Move;
public:
    MoveMaker( const model::Player& me, const model::World& world, const model::Game& game );
   
    MovesQueue move_selected_in_direction( Direction direction );
    MovesQueue move_all_in_direction( Direction direction);
    MovesQueue move_in_direction( VehicleController::VehicleID vehicleID, Direction direction );
    MovesQueue move_in_direction( Point leftUp, Point rightDown, Direction direction );
    MovesQueue move_in_direction( VehicleController::VehicleType vehicleType, Direction direction );
    MovesQueue move_in_direction( int groupID, Direction direction );
   
    MovesQueue move_selected_to_point( Direction direction );
    MovesQueue move_all_to_point( Point point );
    MovesQueue move_to_point( VehicleController::VehicleID vehicleID, Point point );
    MovesQueue move_to_point( Point leftUp, Point rightDown, Point point );
    MovesQueue move_to_point( VehicleController::VehicleType vehicleType, Point point );
    MovesQueue move_to_point( int groupID, Point point );

    inline Direction get_direction( Point start, Point end );
};

typedef std::shared_ptr<MoveMaker> MoveMakerPtr;