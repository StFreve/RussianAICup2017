#include "MoveMaker.h"
#include "MyStrategy.h"
#include <map>

MoveMaker::MoveMaker( const model::Player & me, const model::World & world, const model::Game & game )
{
}

MovesQueue MoveMaker::move_selected_in_direction( Direction direction )
{
    Move moveAction;
    moveAction.setAction( model::ActionType::MOVE );
    moveAction.setX( direction.first );
    moveAction.setY( direction.second );
    return moveAction;
}

MovesQueue MoveMaker::move_all_in_direction( Direction direction )
{
    return MyStrategy::get_instance()->getVehicleController()->select_all() +
        move_selected_in_direction( direction );
}

MovesQueue MoveMaker::move_in_direction( VehicleController::VehicleID vehicleID, Direction direction )
{
    return MyStrategy::get_instance()->getVehicleController()->select( vehicleID ) +
        move_selected_in_direction( direction );
}

MovesQueue MoveMaker::move_in_direction( Point leftUp, Point rightDown, Direction direction )
{
    return MyStrategy::get_instance()->getVehicleController()->select( leftUp, rightDown ) +
        move_selected_in_direction( direction );
}

MovesQueue MoveMaker::move_in_direction( VehicleController::VehicleType vehicleType, Direction direction )
{
    return MyStrategy::get_instance()->getVehicleController()->select( vehicleType ) +
        move_selected_in_direction( direction );
}

MovesQueue MoveMaker::move_in_direction( int groupID, Direction direction )
{
    return MyStrategy::get_instance()->getVehicleController()->select( groupID ) +
        move_selected_in_direction( direction );
}

MovesQueue MoveMaker::move_all_to_point( Point point )
{

    return move_all_in_direction( get_direction( MyStrategy::get_instance()->getVehicleController()->get_center_all(), point ) );
}

MovesQueue MoveMaker::move_to_point( VehicleController::VehicleID vehicleID, Point point )
{
    return move_all_in_direction( get_direction( MyStrategy::get_instance()->getVehicleController()->get_center( vehicleID ), point ) );
}

MovesQueue MoveMaker::move_to_point( Point leftUp, Point rightDown, Point point )
{
    return move_all_in_direction( get_direction( MyStrategy::get_instance()->getVehicleController()->get_center( leftUp, rightDown ), point ) );
}

MovesQueue MoveMaker::move_to_point( VehicleController::VehicleType vehicleType, Point point )
{
    return move_all_in_direction( get_direction( MyStrategy::get_instance()->getVehicleController()->get_center( vehicleType ), point ) );
}

MovesQueue MoveMaker::move_to_point( int groupID, Point point )
{
    return move_all_in_direction( get_direction( MyStrategy::get_instance()->getVehicleController()->get_center( groupID ), point ) );
}

inline MoveMaker::Direction MoveMaker::get_direction( Point start, Point end )
{
    return{ end.first - start.first, end.second - start.second };
}
