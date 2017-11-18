#include "AIController.h"
#include "MyStrategy.h"

AIController::AIController( const model::Player & me, const model::World & world, const model::Game & game )
{
    step = 0;
}

void AIController::update( const model::Player & me, const model::World & world, const model::Game & game )
{
    tickCount = game.getTickCount();
}

MovesQueue AIController::get_next_moves()
{
    VehicleControllerPtr vc = MyStrategy::get_instance()->getVehicleController();
    MoveControllerPtr mc = MyStrategy::get_instance()->getMoveController();
    MovesQueue mq;
    if ( step == 1 )
    {
        ++step;
        mq += vc->select( model::VehicleType::HELICOPTER, false );
        mq += vc->select( model::VehicleType::FIGHTER, false );
        mq += vc->assign_selected( 1 );
    }
    else if ( step == 4 )
    {
        mq += vc->select( 1 );
    }
    else if ( step > 7 )
    {
        mq += mc->move_selected_to_point( vc->enemy_get_balanced_center(), 0.8 );
    }
    if ( ( tickCount % 10 == 0 ) && !vc->is_moving() )
    {
        if ( step == 2 )
        {
            ++step;
            mq += mc->scale_selected( vc->get_center_selected(), 2 );
        }
        else if ( step == 3 )
        {
            ++step;
            mq += mc->move_in_direction( { 200, 0 }, model::VehicleType::FIGHTER );
        }
        else if ( step == 5 )
        {
            ++step;
            mq += mc->rotate_selected( vc->get_balanced_center_selected(), 45 );
        }
        else if ( step == 6 )
        {
            ++step;
            mq += mc->scale_selected( vc->get_center_selected(), 0.1 );
        }
        else
        {
            ++step;
        }
    }
    
    return mq;
}

MovesQueue AIController::get_first_moves()
{
    ++step;
    VehicleControllerPtr vc = MyStrategy::get_instance()->getVehicleController();
    MoveControllerPtr mc = MyStrategy::get_instance()->getMoveController();
    MovesQueue mq;
    mq += mc->move_to_point( { 120,210 }, model::VehicleType::FIGHTER );
    mq += mc->move_to_point( { 220,200 }, model::VehicleType::HELICOPTER );
    return mq;
}