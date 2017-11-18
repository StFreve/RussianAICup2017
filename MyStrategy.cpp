#include "MyStrategy.h"

#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;
MyStrategy::MyStrategyPtr MyStrategy::this_ptr = NULL;
void MyStrategy::move( const Player& me, const World& world, const Game& game, Move& move ) {
    if ( world.getTickIndex() == 0 )
	{
        init( me, world, game );
		movesQueue_ = aiController_->get_first_moves();
    }

	if( me.getRemainingActionCooldownTicks() == 0 )
	{
        update( me, world, game );
        if(movesQueue_.empty())
		{
			movesQueue_ += aiController_->get_next_moves();
		}
		if(!movesQueue_.empty())
		{
			move = movesQueue_.pop();
		}
	}
    
}

VehicleControllerPtr MyStrategy::getVehicleController()
{
    return this->vehicleController_;
}

MoveControllerPtr MyStrategy::getMoveController()
{
    return this->moveController_;
}

AIControllerPtr MyStrategy::getAIController()
{
    return aiController_;
}

MyStrategy::MyStrategyPtr MyStrategy::get_instance()
{
    return this_ptr;
}

void MyStrategy::init( const model::Player & me, const model::World & world, const model::Game & game )
{
    moveController_.reset( new MoveController( me, world, game ) );
    vehicleController_.reset( new VehicleController( me, world, game ) );
	aiController_.reset( new AIController(me, world, game) );

}

MyStrategy::MyStrategy()
{
    this_ptr = this;
}

void MyStrategy::update(const model::Player& me, const model::World& world, const model::Game& game)
{
	vehicleController_->update( me, world, game );
	moveController_->update( me, world, game );
	aiController_->update(me, world, game );
}
