#include "AIStrategy.h"
#include "MyStrategy.h"
AIStrategy::AIStrategy()
    : strategyFinished_( false )
{

}

bool AIStrategy::finished() const
{
    return strategyFinished_;
}

void AIStrategy::set_finished()
{
    strategyFinished_ = true;
}

