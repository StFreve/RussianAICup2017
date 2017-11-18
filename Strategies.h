#pragma once
#include "AIStrategy.h"

class DefaultStrategy : public AIStrategy {
public:
    DefaultStrategy();
    // Inherited via AIStrategy
    virtual bool available() override;
    virtual MovesQueue get_next_move() override;
private:
    int step;
};