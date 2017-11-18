#pragma once
#include "model/Move.h"
#include <list>
class MovesQueue : public std::list<model::Move> {
    typedef model::Move Move;
public:
    MovesQueue() {}
    MovesQueue( const Move& move ) { this->push( move ); }
    MovesQueue& operator+=( MovesQueue& rhmq ) { this->splice( this->end(), rhmq ); return *this; }
    MovesQueue& operator+=( const Move& rhm ) { this->push( rhm ); return *this; }
    MovesQueue operator+( MovesQueue& rhmq ) const { MovesQueue newmq = *this; return newmq += rhmq; }
    MovesQueue operator+( const Move& rhm ) const { MovesQueue newmq = *this; return newmq += rhm; }
   
    inline Move& push( const Move& move ) { this->push_back( move ); return this->back(); }
    inline Move pop() { Move move = this->front(); this->pop_front(); return move; }
};