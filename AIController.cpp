#include "AIController.h"
#include "MyStrategy.h"
#include "Strategies.h"
AIController::AIController( const model::Player & me, const model::World & world, const model::Game & game )
{
    player_ = &me;
    world_ = &world;
    game_ = &game;
    add_strategy( AIStrategyPtr( new DefaultStrategy() ) );
}

void AIController::update( const model::Player & me, const model::World & world, const model::Game & game )
{
    player_ = &me;
    world_ = &world;
    game_ = &game;
    tickCount = game.getTickCount();
}

MovesQueue AIController::move()
{
    if ( curentStrategy_ != nullptr && curentStrategy_->finished() )
    {
        finishedStrategies_.insert( curentStrategy_ );
        strategies_.erase( curentStrategy_ );
    }
    if ( curentStrategy_ == nullptr )
    {
        for ( auto strategy : strategies_ )
        {
            if ( strategy->available() )
            {
                curentStrategy_ = strategy;
                break;
            }
        }
    }
    return curentStrategy_->get_next_move();
}

void AIController::add_strategy( AIStrategyPtr strategy )
{
    strategies_.insert( strategy );
}

const model::Player & AIController::getPlayerInfo()
{
    return *player_;
}

const model::World & AIController::getWorldInfo()
{
    return *world_;
}

const model::Game & AIController::getGamerInfo()
{
    return *game_;
}