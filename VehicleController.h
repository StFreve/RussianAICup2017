#pragma once
#include "Controller.h"

#include "MovesQueue.h"

#include <map>
#include <list>
#include <set>
#include <iterator>
#include <algorithm>

class VehicleController :public Controller {
private:
    struct CountingIterator
        : std::iterator<std::output_iterator_tag, int>
    {
        template <typename T>
        CountingIterator& operator=( T&& ) {
            ++count;
            return *this;
        }

        CountingIterator& operator*() { return *this; }
        CountingIterator& operator++() { return *this; }
        CountingIterator& operator++( int ) { return *this; }

        operator int() { return count; }

        int count = 0;
    };
public:
    typedef model::Vehicle                        Vehicle;
    typedef model::VehicleUpdate                  VehicleUpdate;
    typedef model::VehicleType                    VehicleType;
    typedef std::vector<VehicleUpdate>            VehicleUpdates;
    typedef std::vector<int>                      VehicleGroups;

    typedef std::shared_ptr<model::Vehicle>       VehiclePtr;
    typedef long long                             VehicleID;
    class Vehicles : public std::set<VehicleID> {
    public:
        Vehicles( VehicleID rhvid ) { this->insert( rhvid ); }
        Vehicles() { }
        Vehicles& operator+=( const Vehicles& rhvs ) { if ( this->empty() ) *this = rhvs; else this->insert( rhvs.begin(), rhvs.end() ); return *this; }
        Vehicles& operator+=( VehicleID rhvid ) { this->insert( rhvid ); return *this; }
        Vehicles operator+( const Vehicles& rhvs ) const { if ( this->empty() ) return rhvs; Vehicles newvs = *this; return newvs += rhvs; }
        Vehicles operator+( VehicleID rhvid ) const { Vehicles newvs = *this; return newvs += rhvid; }

        Vehicles& operator-=( const Vehicles& rhvs ) {
            if ( this->empty() ) return *this;
            Vehicles result;
            std::set_difference( this->begin(), this->end(), rhvs.begin(), rhvs.end(), std::inserter( result, result.begin() ) );
            this->swap( result );
            return *this;
        }
        Vehicles& operator-=( VehicleID rhvid ) { this->erase( rhvid ); return *this; }
        Vehicles operator-( const Vehicles& rhvs ) const { Vehicles newvs = *this; return newvs -= rhvs; }
        Vehicles operator-( VehicleID rhvid ) const { Vehicles newvs = *this; return newvs -= rhvid; }
    };

    typedef std::map<VehicleID, VehiclePtr>       VehicleByID;
    typedef std::vector<std::vector<VehicleID> >  VehicleByXY;
    typedef std::vector<Vehicles>                 VehiclesByType;
    typedef std::vector<Vehicles>                 VehiclesByGroup;

    typedef Controller::Point		              Point;
public:
    VehicleController( const model::Player& me, const model::World& world, const model::Game& game );
    void        update( const model::Player& me, const model::World& world, const model::Game& game );

    MovesQueue  select_all();
    MovesQueue  select( VehicleID vehicleID, bool clean = true );
    MovesQueue  select( Point leftUp, Point rightDown, bool clean = true );
    MovesQueue  select( Point leftUp, Point rightDown, VehicleType vehicleType, bool clean = true );
    MovesQueue  select( VehicleType vehicleType, bool clean = true );
    MovesQueue  select( int groupID, bool clean = true );

    MovesQueue  assign_all( int groupID );
    MovesQueue  assign_selected( int groupID );
    template<typename... Params>
    MovesQueue  assign( int groupID, Params... parameters )
    {
        MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select( parameters... );
        return mq + assign_selected( int groupID );
    }

    MovesQueue  dismiss_all( int groupID );
    MovesQueue  dismiss_selected( int groupID );
    template<typename... Params>
    MovesQueue  dismiss( int groupID, Params... parameters )
    {
        MovesQueue mq = MyStrategy::get_instance()->getVehicleController()->select( parameters... );
        return mq + dismiss_selected( int groupID );
    }

    MovesQueue  disband( int groupID );

    MovesQueue  deselect_all();
    MovesQueue  deselect( VehicleID vehicleID );
    MovesQueue  deselect( Point leftUp, Point rightDown );
    MovesQueue  deselect( Point leftUp, Point rightDown, VehicleType vehicleType );
    MovesQueue  deselect( VehicleType vehicleType );
    MovesQueue  deselect( int groupID );

    bool        is_selected_all();
    bool        is_selected( VehicleID vehicleID, bool only = true );
    bool        is_selected( Point leftUp, Point rightDown, bool only = true );
    bool        is_selected( Point leftUp, Point rightDown, VehicleType vehicleType, bool only = true );
    bool        is_selected( VehicleType vehicleType, bool only = true );
    bool        is_selected( int groupID, bool only = true );
    bool		is_selected( const Vehicles& vehicles, bool only = true );

    bool        is_deselected_all();
    bool        is_deselected( VehicleID vehicleID );
    bool        is_deselected( Point leftUp, Point rightDown );
    bool        is_deselected( Point leftUp, Point rightDown, VehicleType vehicleType );
    bool        is_deselected( VehicleType vehicleType );
    bool        is_deselected( int groupID );
    bool		is_deselected( const Vehicles& vehicles );

    Vehicles    get_selected();
    Vehicles    get_moving();
    Vehicles    get_all();
    VehiclePtr  get( VehicleID vehicleID );
    Vehicles    get( Point leftUp, Point rightDown );
    Vehicles    get( Point leftUp, Point rightDown, VehicleType vehicleType );
    Vehicles    get( VehicleType vehicleType );
    Vehicles    get( int groupID );


    Point       get_balanced_center_selected();
    Point       get_balanced_center();
    Point       get_balanced_center( VehicleID vehicleID );
    Point       get_balanced_center( const Vehicles& vehicles );
    template<typename... Params>
    Point       get_balanced_center( Params... parameters )
    {
        return  get_balanced_center( get( parameters... ) );
    }

    Point       get_center_selected();
    Point       get_center();
    Point       get_center( VehicleID vehicleID );
    Point       get_center( const Vehicles& vehicles );
    template<typename... Params>
    Point       get_center( Params... parameters )
    {
        return  get_center( get( parameters... ) );
    }

    bool        is_moving_selected();
    bool        is_moving();
    bool        is_moving( VehicleID vehicleID );
    bool        is_moving( const Vehicles& vehicles );
    template<typename... Params>
    bool        is_moving( Params... parameters )
    {
        return  is_moving( get( parameters... ) );
    }

    Vehicles    enemy_get_moving();
    Vehicles    enemy_get_all();
    VehiclePtr  enemy_get( VehicleID vehicleID );
    Vehicles    enemy_get( Point leftUp, Point rightDown );
    Vehicles    enemy_get( Point leftUp, Point rightDown, VehicleType vehicleType );
    Vehicles    enemy_get( VehicleType vehicleType );
    Vehicles    enemy_get( int groupID );


    Point       enemy_get_balanced_center();
    Point       enemy_get_balanced_center( VehicleID vehicleID );
    Point       enemy_get_balanced_center( const Vehicles& vehicles );
    template<typename... Params>
    Point       enemy_get_balanced_center( Params... parameters )
    {
        return  enemy_get_balanced_center( enemy_get( parameters... ) );
    }

    Point       enemy_get_center();
    Point       enemy_get_center( VehicleID vehicleID );
    Point       enemy_get_center( const Vehicles& vehicles );
    template<typename... Params>
    Point       enemy_get_center( Params... parameters )
    {
        return  enemy_get_center( enemy_get( parameters... ) );
    }

    bool        enemy_is_moving();
    bool        enemy_is_moving( VehicleID vehicleID );
    bool        enemy_is_moving( const Vehicles& vehicles );
    template<typename... Params>
    bool        enemy_is_moving( Params... parameters )
    {
        return  enemy_is_moving( enemy_get( parameters... ) );
    }


    void        reset_selected();
private:
    inline bool  vehicle_is_in_rectangle( const Vehicle& vehicle, Point leftUp, Point rightDown );
private:
    VehicleByID     vehicles_;
    VehiclesByGroup groups_;
    Vehicles        selected_vehicles_;
    Vehicles        moving_vehicles_;
    VehicleByID     enemy_vehicles_;
    Vehicles        enemy_moving_vehicles_;

    const double    worldWidth;
    const double    worldHeight;
};

typedef std::shared_ptr<VehicleController> VehicleControllerPtr;