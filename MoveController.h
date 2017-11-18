#pragma once
#include "Controller.h"
#include "MovesQueue.h"
#include "VehicleController.h"

#include <queue>
#include <list>
#include <vector>
#include <set>

class MoveController : public Controller {
public:
    typedef Controller::Point					  Point;
    typedef std::pair<int, int>                   Direction;
    typedef model::Move                           Move;
public:
    MoveController( const model::Player& me, const model::World& world, const model::Game& game );

    MovesQueue move_in_direction_selected( Direction direction, double maxSpeed = 0.0 );
    MovesQueue move_all_in_direction( Direction direction, double maxSpeed = 0.0);

	template<typename... Params>
	MovesQueue move_in_direction(Direction direction, Params... parameters)
	{
		MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select(parameters...);
		return mq + move_in_direction_selected(direction, 0.0);
	}
	template<typename... Params>
	MovesQueue move_in_direction(Direction direction, double maxSpeed, Params... parameters)
	{
		MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select(parameters...);
		return mq + move_in_direction_selected(direction, maxSpeed);
	}
   


    MovesQueue move_selected_to_point( Point point, double maxSpeed = 0.0 );
    MovesQueue move_all_to_point( Point point, double maxSpeed = 0.0 );

    template<typename... Params>
	MovesQueue move_to_point( Point point, Params... parameters)
	{
		MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select(parameters...);
		return mq + move_selected_to_point(point, 0.0);
	}
	template<typename... Params>
	MovesQueue move_to_point(Point point, double maxSpeed, Params... parameters)
	{
		MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select(parameters...);
		return mq + move_selected_to_point(point, maxSpeed);
	}



	MovesQueue rotate_selected(Point point, double angle, double maxSpeed = 0.0, double maxAngularSpeed = 0.0);
	MovesQueue rotate_all(Point point, double angle, double maxSpeed = 0.0, double maxAngularSpeed = 0.0);
	
    template<typename... Params>
	MovesQueue rotate(Point point, double angle, Params... parameters)
	{
		MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select(parameters...);
		return mq + rotate_selected(point, angle, 0.0, 0.0);
	}
	template<typename... Params>
	MovesQueue rotate(Point point, double angle, double maxSpeed, Params... parameters)
	{
		MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select(parameters...);
		return mq + rotate_selected(point, angle, maxSpeed, 0.0);
	}
	template<typename... Params>
	MovesQueue rotate(Point point, double angle, double maxSpeed, double maxAngularSpeed, Params... parameters)
	{
		MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select(parameters...);
		return mq + rotate_selected(point, angle, maxSpeed, maxAngularSpeed);
	}
   
    MovesQueue scale_selected( Point point, double factor, double maxSpeed = 0.0 );
    MovesQueue scale_all( Point point, double factor, double maxSpeed = 0.0);

    template<typename... Params>
    MovesQueue scale( Point point, double factor, Params... parameters )
    {
        MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select( parameters... );
        return mq + scale_selected( point, factor, 0.0);
    }
    template<typename... Params>
    MovesQueue scale( Point point, double factor, double maxSpeed, Params... parameters )
    {
        MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select( parameters... );
        return mq + scale_selected( point, factor, maxSpeed );
    }

    Direction get_direction( Point start, Point end );
};

typedef std::shared_ptr<MoveController> MoveControllerPtr;