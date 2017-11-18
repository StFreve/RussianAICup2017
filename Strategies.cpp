#include "Strategies.h"
#include "MyStrategy.h"

DefaultStrategy::DefaultStrategy()
    : step( 0 )
{

}

bool DefaultStrategy::available()
{
    return true;
}

MovesQueue DefaultStrategy::get_next_move()
{
    VehicleController& vc = *MyStrategy::get_instance()->getVehicleController();
    MoveController& mc = *MyStrategy::get_instance()->getMoveController();
    AIController& ac = *MyStrategy::get_instance()->getAIController();
    MovesQueue mq;
    if ( finished() )
        return mq;

    if ( step == 0 )
    {
        ++step;
        mq += mc.move_to_point( { 120,203 }, model::VehicleType::FIGHTER );
        mq += mc.move_to_point( { 220,200 }, model::VehicleType::HELICOPTER );
        mq += vc.select( model::VehicleType::HELICOPTER, false );
        mq += vc.select( model::VehicleType::FIGHTER, false );
        mq += vc.assign_selected( 1 );
    }
    else if ( step > 5 )
    {
        mq += mc.move_selected_to_point( vc.enemy_get_balanced_center(), 0.8 );
    }
    else if ( ( ac.getWorldInfo().getTickCount() % 10 == 0 ) && !vc.is_moving() )
    {
        if ( step == 1 )
        {
            ++step;
            mq += mc.scale_selected( vc.get_center_selected(), 2.0 );
        }
        else if ( step == 2 )
        {
            ++step;
            mq += mc.move_in_direction( { 200, 0 }, model::VehicleType::FIGHTER );
            mq += vc.select( 1 );
        }
        else if ( step == 3 )
        {
            ++step;
            mq += mc.rotate_selected( vc.get_balanced_center_selected(), 45 );
        }
        else if ( step == 4 )
        {
            ++step;
            mq += mc.scale_selected( vc.get_center_selected(), 0.1 );
        }
        else if ( step == 5 )
        {
            ++step;
        }
    }

    return mq;
}
