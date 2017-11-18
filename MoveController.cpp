#include "MoveController.h"
#include "MyStrategy.h"
#include <map>

MoveController::MoveController( const model::Player & me, const model::World & world, const model::Game & game )
{
}

MovesQueue MoveController::move_in_direction_selected( Direction direction, double maxSpeed )
{
    Move moveAction;
    moveAction.setAction( model::ActionType::MOVE );
    moveAction.setX( direction.first );
    moveAction.setY( direction.second );
	moveAction.setMaxSpeed( maxSpeed );
    return moveAction;
}

MovesQueue MoveController::move_all_in_direction( Direction direction, double maxSpeed )
{
	MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select_all();
	return mq + move_in_direction_selected(direction, maxSpeed);
}

MovesQueue MoveController::move_selected_to_point(Point point, double maxSpeed )
{
	return move_in_direction_selected( get_direction( MyStrategy::get_instance()->getVehicleController()->get_balanced_center_selected(), point ), maxSpeed );
}

MovesQueue MoveController::move_all_to_point( Point point, double maxSpeed )
{
    return move_all_in_direction( get_direction( MyStrategy::get_instance()->getVehicleController()->get_balanced_center(), point ), maxSpeed );
}

MovesQueue MoveController::scale_selected( Point point, double factor, double maxSpeed )
{
    Move moveAction;
    moveAction.setAction( model::ActionType::SCALE );
    moveAction.setX( point.first );
    moveAction.setY( point.second );
    moveAction.setFactor( factor );
    moveAction.setMaxSpeed( maxSpeed );
    return moveAction;
}

MovesQueue MoveController::scale_all( Point point, double factor, double maxSpeed )
{
    MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select_all();
    return mq + scale_selected( point, factor, maxSpeed );
}

MoveController::Direction MoveController::get_direction( Point start, Point end )
{
    return{ end.first - start.first, end.second - start.second };
}

MovesQueue MoveController::rotate_selected(Point point, double angle, double maxSpeed, double maxAngularSpeed)
{
	Move moveAction;
	moveAction.setAction(model::ActionType::ROTATE);
	moveAction.setX(point.first);
	moveAction.setY(point.second);
	moveAction.setAngle(angle * PI / 180 );
	moveAction.setMaxSpeed(maxSpeed);
	moveAction.setMaxAngularSpeed(maxAngularSpeed);
	return moveAction;
}

MovesQueue MoveController::rotate_all( Point point, double angle, double maxSpeed, double maxAngularSpeed )
{
    MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select_all();
    return mq + rotate_selected( point, angle, maxSpeed, maxAngularSpeed );
}
