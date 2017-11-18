#include "VehicleController.h"
#include <assert.h>
#include <algorithm>
using model::Move;
VehicleController::VehicleController( const model::Player & me, const model::World & world, const model::Game & game )
    : worldWidth( world.getWidth() )
    , worldHeight( world.getHeight() )
    , groups_( game.getMaxUnitGroup() + 1)
{
	auto& vehicles = world.getNewVehicles();
	for(auto& vehicle : vehicles)
	{
		if(vehicle.getPlayerId() == me.getId())
		{
            vehicles_[ vehicle.getId() ] = VehiclePtr( new Vehicle( vehicle ) );
		}
        else
        {
            enemy_vehicles_[vehicle.getId()] = VehiclePtr( new Vehicle( vehicle ) );
        }
	}
}

MovesQueue VehicleController::select_all()
{
    return select( { 0,0 }, { worldWidth, worldHeight } );
}

MovesQueue VehicleController::select( VehicleID vehicleID, bool clean )
{
    VehicleByID::iterator fit = vehicles_.find( vehicleID );
    assert( fit != vehicles_.end() );
    Vehicle& vehicle = *fit->second;
    return select( { vehicle.getX() , vehicle.getY() }, { vehicle.getX(), vehicle.getY() }, vehicle.getType(), clean );
}

MovesQueue VehicleController::select( Point leftUp, Point rightDown, bool clean )
{
	if(is_selected(leftUp, rightDown, clean))
		return MovesQueue();
	if ( clean ) selected_vehicles_.clear();
    for ( auto vehicleIt : vehicles_ )
    {
        Vehicle& vehicle = *vehicleIt.second;
        if ( vehicle_is_in_rectangle( vehicle, leftUp, rightDown ) )
            selected_vehicles_ += vehicleIt.first;
    }

    Move selectAction;
    selectAction.setAction( clean ? model::ActionType::CLEAR_AND_SELECT : model::ActionType::ADD_TO_SELECTION );
    selectAction.setLeft( leftUp.first );
    selectAction.setTop( leftUp.second );
    selectAction.setRight( rightDown.first );
    selectAction.setBottom( rightDown.second );
    return selectAction;
}

MovesQueue VehicleController::select( Point leftUp, Point rightDown, VehicleType vehicleType, bool clean )
{
	if(is_selected(leftUp, rightDown, vehicleType, clean))
		return MovesQueue();
	if ( clean ) selected_vehicles_.clear();
    for ( auto vehicleIt : vehicles_ )
    {
        Vehicle& vehicle = *vehicleIt.second;
        if ( vehicle_is_in_rectangle( vehicle, leftUp, rightDown ) && vehicle.getType() == vehicleType )
            selected_vehicles_ += vehicleIt.first;
    }

    Move selectAction;
    selectAction.setAction( clean ? model::ActionType::CLEAR_AND_SELECT : model::ActionType::ADD_TO_SELECTION );
    selectAction.setVehicleType( vehicleType );
    selectAction.setLeft( leftUp.first );
    selectAction.setTop( leftUp.second );
    selectAction.setRight( rightDown.first );
    selectAction.setBottom( rightDown.second );
    return selectAction;
}

MovesQueue VehicleController::select( VehicleType vehicleType, bool clean )
{
    return select( { 0,0 }, { worldWidth, worldHeight }, vehicleType, clean );;
}

MovesQueue VehicleController::select( int groupID, bool clean )
{
	if(is_selected(groupID, clean))
		return MovesQueue();
    if ( clean ) selected_vehicles_  = groups_[ groupID ];
    else         selected_vehicles_ += groups_[ groupID ];

    Move selectAction;
    selectAction.setAction( clean ? model::ActionType::CLEAR_AND_SELECT : model::ActionType::ADD_TO_SELECTION );
    selectAction.setGroup( groupID );
    return selectAction;
}

MovesQueue VehicleController::assign_all( int groupID )
{
    MovesQueue mq = select_all();
    return mq + assign_selected( groupID );
}

MovesQueue VehicleController::assign_selected( int groupID )
{
    groups_[ groupID ] += get_selected();
    Move assignAction;
    assignAction.setAction( model::ActionType::ASSIGN );
    assignAction.setGroup( groupID );
    return assignAction;
}

MovesQueue VehicleController::dismiss_all( int groupID )
{
    MovesQueue mq = select_all();
    return mq + dismiss_selected( groupID );
}

MovesQueue VehicleController::dismiss_selected( int groupID )
{
    groups_[ groupID ] -= get_selected();
    Move dismissAction;
    dismissAction.setAction( model::ActionType::DISMISS );
    dismissAction.setGroup( groupID );
    return dismissAction;
}

MovesQueue VehicleController::disband( int groupID )
{
    Move disbandAction;
    disbandAction.setAction( model::ActionType::DISBAND );
    disbandAction.setGroup( groupID );
    return disbandAction;
}

MovesQueue VehicleController::deselect_all()
{
    return deselect( { 0,0 }, { worldWidth, worldHeight } );
}

MovesQueue VehicleController::deselect( VehicleID vehicleID )
{
    VehicleByID::iterator fit = vehicles_.find( vehicleID );
    assert( fit != vehicles_.end() );
    Vehicle& vehicle = *fit->second;
    return deselect( { vehicle.getX(), vehicle.getY() }, { vehicle.getX(), vehicle.getY() }, vehicle.getType() );

}

MovesQueue VehicleController::deselect( Point leftUp, Point rightDown )
{
	if(is_deselected(leftUp, rightDown))
		return MovesQueue();
    Vehicles oldVehicles;
    oldVehicles.swap( selected_vehicles_ );
    for ( auto vehicleID : oldVehicles )
    {

        Vehicle& vehicle = *vehicles_[ vehicleID ];
        if ( !vehicle_is_in_rectangle( vehicle, leftUp, rightDown ) )
            selected_vehicles_ += vehicleID;
    }

    Move deselectAction;
    deselectAction.setAction( model::ActionType::DESELECT );
    deselectAction.setLeft( leftUp.first );
    deselectAction.setTop( leftUp.second );
    deselectAction.setRight( rightDown.first );
    deselectAction.setBottom( rightDown.second );
    return deselectAction;
}

MovesQueue VehicleController::deselect( Point leftUp, Point rightDown, VehicleType vehicleType )
{
	if(is_deselected(leftUp, rightDown, vehicleType))
		return MovesQueue(); 
	Vehicles oldVehicles;
    oldVehicles.swap( selected_vehicles_ );
    for ( auto vehicleID : oldVehicles )
    {

        Vehicle& vehicle = *vehicles_[ vehicleID ];
        if ( !vehicle_is_in_rectangle( vehicle, leftUp, rightDown ) || vehicle.getType() != vehicleType )
            selected_vehicles_ += vehicleID;
    }

    Move deselectAction;
    deselectAction.setAction( model::ActionType::DESELECT );
    deselectAction.setVehicleType( vehicleType );
    deselectAction.setLeft( leftUp.first );
    deselectAction.setTop( leftUp.second );
    deselectAction.setRight( rightDown.first );
    deselectAction.setBottom( rightDown.second );
    return deselectAction;
    return MovesQueue();
}

MovesQueue VehicleController::deselect( VehicleType vehicleType )
{
	if(is_deselected(vehicleType))
		return MovesQueue();
    return deselect( { 0,0 }, { worldWidth, worldHeight }, vehicleType );
}

MovesQueue VehicleController::deselect( int groupID )
{
	if(is_deselected(groupID))
		return MovesQueue();
    Vehicles oldVehicles;
    selected_vehicles_ -= groups_[ groupID ];

    Move deselectAction;
    deselectAction.setAction( model::ActionType::DESELECT );
    deselectAction.setGroup( groupID );
    return deselectAction;
}

bool VehicleController::is_selected_all()
{
    for ( auto vehicleIt : vehicles_ )
    {
        Vehicle& vehicle = *vehicleIt.second;
        if ( vehicle.getDurability() > 0 && vehicle.isSelected() == false )
            return false;
    }
    return true;
}

bool VehicleController::is_selected( VehicleID vehicleID , bool only )
{
	if( selected_vehicles_.find(vehicleID) == selected_vehicles_.end() )
	{
		return false;
	}
	else if(only)
	{
		if( selected_vehicles_.size() != 1 ) return false;
	}
    return  true ;
}

bool VehicleController::is_selected( Point leftUp, Point rightDown, bool only )
{
	return is_selected(get(leftUp, rightDown), only);
}

bool VehicleController::is_selected( Point leftUp, Point rightDown, VehicleType vehicleType, bool only )
{
	return is_selected(get(leftUp, rightDown, vehicleType), only);
}

bool VehicleController::is_selected( VehicleType vehicleType, bool only )
{
	return is_selected(get(vehicleType), only);
}

bool VehicleController::is_selected( int groupID, bool only )
{
	return is_selected(get(groupID), only);
}

bool VehicleController::is_selected(const Vehicles& vehicles, bool only)
{
	if(only)
	{
		return vehicles == selected_vehicles_;
	}
	else
	{
		int count = std::set_difference(selected_vehicles_.begin(), selected_vehicles_.end(),
										vehicles.begin(), vehicles.end(),
										CountingIterator());
		return ( vehicles.size() + count ) == selected_vehicles_.size();
	}
}

bool VehicleController::is_deselected_all()
{
	return selected_vehicles_.empty();
}

bool VehicleController::is_deselected(VehicleID vehicleID)
{
	if(selected_vehicles_.find(vehicleID) != selected_vehicles_.end())
	{
		return false;
	}
	return  true;
}

bool VehicleController::is_deselected(Point leftUp, Point rightDown)
{
	return is_deselected(get(leftUp,rightDown));
}

bool VehicleController::is_deselected(Point leftUp, Point rightDown, VehicleType vehicleType)
{
	return is_deselected(get(leftUp, rightDown,vehicleType));
}

bool VehicleController::is_deselected(VehicleType vehicleType)
{
	return is_deselected(get(vehicleType));
}

bool VehicleController::is_deselected(int groupID)
{
	return is_deselected(get(groupID));
}

bool VehicleController::is_deselected(const Vehicles& vehicles)
{
	int count = std::set_difference(selected_vehicles_.begin(), selected_vehicles_.end(),
									vehicles.begin(), vehicles.end(),
									CountingIterator());
	return ( vehicles.size() + count ) > selected_vehicles_.size();
}

VehicleController::Vehicles VehicleController::get_selected()
{
    return selected_vehicles_;
}

VehicleController::Vehicles VehicleController::get_moving()
{
    return moving_vehicles_;
}

VehicleController::Vehicles VehicleController::get_all()
{
    Vehicles vehicles;
    for ( auto vehicleIt : vehicles_ )
    {
        if ( vehicleIt.second->getDurability() > 0 )
        {
            vehicles += vehicleIt.first;
        }
    }
    return vehicles;
}

VehicleController::VehiclePtr VehicleController::get( VehicleID vehicleID )
{
    VehicleByID::iterator fit = vehicles_.find( vehicleID );
    assert( fit != vehicles_.end() );
    if ( fit->second->getDurability() > 0 )
        return fit->second;
    else
        return NULL;
}

VehicleController::Vehicles VehicleController::get( Point leftUp, Point rightDown )
{
    Vehicles vehicles;
    for ( auto vehicleIt : vehicles_ )
    {
        Vehicle& vehicle = *vehicleIt.second;
        if ( vehicle.getDurability() > 0 && vehicle_is_in_rectangle( vehicle, leftUp, rightDown ) )
            vehicles += vehicleIt.first;
    }
    return vehicles;
}

VehicleController::Vehicles VehicleController::get( Point leftUp, Point rightDown, VehicleType vehicleType )
{
    Vehicles vehicles;
    for ( auto vehicleIt : vehicles_ )
    {
        Vehicle& vehicle = *vehicleIt.second;
        if ( vehicle.getDurability() > 0 && vehicle_is_in_rectangle( vehicle, leftUp, rightDown ) && vehicle.getType() == vehicleType )
            vehicles += vehicleIt.first;
    }
    return vehicles;
}

VehicleController::Vehicles VehicleController::get( VehicleType vehicleType )
{
    Vehicles vehicles;
    for ( auto vehicleIt : vehicles_ )
    {
        if ( vehicleIt.second->getDurability() > 0 && vehicleIt.second->getType() == vehicleType )
            vehicles += vehicleIt.first;
    }
    return vehicles;
}

VehicleController::Vehicles VehicleController::get( int groupID )
{
    return groups_[ groupID ];
}

VehicleController::Point VehicleController::get_balanced_center_selected()
{
    return get_balanced_center( get_selected() );
}

VehicleController::Point VehicleController::get_balanced_center()
{
    return get_balanced_center( get_all() );
}

VehicleController::Point VehicleController::get_balanced_center( VehicleID vehicleID )
{
    VehiclePtr vehicle = vehicles_[ vehicleID ];
    assert( vehicle != nullptr );
    return{ vehicle->getX(), vehicle->getY() };
}

VehicleController::Point VehicleController::get_balanced_center( const Vehicles & vehicles )
{
    double Sx = 0, Sy = 0;
    int vehicles_count = 0;
    for ( auto vehicleID : vehicles )
    {
        VehiclePtr vehicle = vehicles_[ vehicleID ];
        assert( vehicle != nullptr );
        if ( vehicle->getDurability() > 0 )
        {
            ++vehicles_count;
            Sx += vehicle->getX();
            Sy += vehicle->getY();
        }
    }
    Sx /= vehicles_count;
    Sy /= vehicles_count;
    return{ Sx, Sy };
}

VehicleController::Point VehicleController::get_center_selected()
{
    return get_center( selected_vehicles_ );
}

VehicleController::Point VehicleController::get_center()
{
    return get_center( get_all() );
}

VehicleController::Point VehicleController::get_center( VehicleID vehicleID )
{
    return{ vehicles_[ vehicleID ]->getX(), vehicles_[ vehicleID ]->getY() };
}

VehicleController::Point VehicleController::get_center( const Vehicles & vehicles )
{
    double maxX = worldWidth * -3;
    double minX = worldWidth * 3;
    double maxY = worldWidth * -3;
    double minY = worldWidth * 3;
    for ( auto vehicleID : vehicles )
    {
        VehiclePtr vehicle = vehicles_[ vehicleID ];
        assert( vehicle != nullptr );
        if ( vehicle->getDurability() > 0 )
        {
            if ( vehicle->getX() > maxX )
            {
                maxX = vehicle->getX();
            }
            if ( vehicle->getX() < minX )
            {
                minX = vehicle->getX();
            }
            if ( vehicle->getY() > maxY )
            {
                maxY = vehicle->getY();
            }
            if ( vehicle->getY() < minY )
            {
                minY = vehicle->getY();
            }
        }
    }
    return{ ( minX + maxX ) / 2.0,( minY + maxY ) / 2.0 };
}

bool VehicleController::is_moving_selected()
{
    return is_moving( selected_vehicles_ );
}

bool VehicleController::is_moving()
{
    return moving_vehicles_.empty() == false;
}

bool VehicleController::is_moving( VehicleID vehicleID )
{
    return moving_vehicles_.find( vehicleID ) != moving_vehicles_.end();
}

bool VehicleController::is_moving( const Vehicles& vehicles )
{
    int count = std::set_difference( moving_vehicles_.begin(), moving_vehicles_.end(),
                                     vehicles.begin(), vehicles.end(),
                                     CountingIterator() );
    return count < moving_vehicles_.size();
}

void VehicleController::update( const model::Player & me, const model::World & world, const model::Game & game )
{
    const VehicleUpdates& vehicleUpdates = world.getVehicleUpdates();
    moving_vehicles_.clear();
    enemy_moving_vehicles_.clear();
    for ( auto vehicleUpdate : vehicleUpdates )
    {
        if ( vehicles_.find( vehicleUpdate.getId() ) != vehicles_.end() )
        {
            VehiclePtr& vehicle = vehicles_[ vehicleUpdate.getId() ];
            if ( vehicleUpdate.getDurability() > 0 ) 
            {
                if ( vehicle->getX() != vehicleUpdate.getX() || vehicle->getY() != vehicleUpdate.getY() )
                {
                    moving_vehicles_.insert( vehicle->getId() );
                }
                if ( vehicle->getGroups() != vehicleUpdate.getGroups() )
                {
                    auto& old_groups = vehicle->getGroups();
                    auto& new_groups = vehicleUpdate.getGroups();
                    for ( auto groupID : old_groups ) groups_[ groupID ] -= vehicle->getId();
                    for ( auto groupID : new_groups ) groups_[ groupID ] += vehicle->getId();
                }
            }
            vehicle.reset( new Vehicle( *vehicles_[ vehicleUpdate.getId() ], vehicleUpdate ) );
        }
        else if ( enemy_vehicles_.find( vehicleUpdate.getId() ) != enemy_vehicles_.end() )
        {
            VehiclePtr& vehicle = enemy_vehicles_[ vehicleUpdate.getId() ];
            if ( vehicleUpdate.getDurability() > 0 )
            {
                if ( vehicle->getX() != vehicleUpdate.getX() || vehicle->getY() != vehicleUpdate.getY() )
                {
                    enemy_moving_vehicles_.insert( vehicle->getId() );
                }
            }
            vehicle.reset( new Vehicle( *enemy_vehicles_[ vehicleUpdate.getId() ], vehicleUpdate ) );
        }
    }
	auto& vehicles = world.getNewVehicles();
	for(auto& vehicle : vehicles)
	{
		if(vehicle.getPlayerId() == me.getId())
		{
			vehicles_[ vehicle.getId() ] = VehiclePtr(new Vehicle(vehicle));
		}
        else
        {
            enemy_vehicles_[ vehicle.getId() ] = VehiclePtr( new Vehicle( vehicle ) );
        }
	}
}

void VehicleController::reset_selected()
{
    selected_vehicles_.clear();
    for ( auto vehicleIt : vehicles_ )
    {
        if ( vehicleIt.second->getDurability() > 0 && vehicleIt.second->isSelected() )
        {
            selected_vehicles_ += vehicleIt.first;
        }
    }

}

bool VehicleController::vehicle_is_in_rectangle( const Vehicle & vehicle, Point leftUp, Point rightDown )
{
    return leftUp.first <= vehicle.getX() && vehicle.getX() <= rightDown.first ||
        leftUp.second <= vehicle.getY() && vehicle.getY() <= rightDown.second;
}

VehicleController::Vehicles VehicleController::enemy_get_moving()
{
    return enemy_moving_vehicles_;
}

VehicleController::Vehicles VehicleController::enemy_get_all()
{
    Vehicles vehicles;
    for ( auto vehicleIt : enemy_vehicles_ )
    {
        vehicles += vehicleIt.first;
    }
    return vehicles;
}

VehicleController::VehiclePtr VehicleController::enemy_get( VehicleID vehicleID )
{
    VehicleByID::iterator fit = enemy_vehicles_.find( vehicleID );
    assert( fit != enemy_vehicles_.end() );
    if ( fit->second->getDurability() > 0 )
        return fit->second;
    else
        return NULL;
}

VehicleController::Vehicles VehicleController::enemy_get( Point leftUp, Point rightDown )
{
    Vehicles vehicles;
    for ( auto vehicleIt : enemy_vehicles_ )
    {
        Vehicle& vehicle = *vehicleIt.second;
        if ( vehicle.getDurability() > 0 && vehicle_is_in_rectangle( vehicle, leftUp, rightDown ) )
            vehicles += vehicleIt.first;
    }
    return vehicles;
}

VehicleController::Vehicles VehicleController::enemy_get( Point leftUp, Point rightDown, VehicleType vehicleType )
{
    Vehicles vehicles;
    for ( auto vehicleIt : enemy_vehicles_ )
    {
        Vehicle& vehicle = *vehicleIt.second;
        if ( vehicle.getDurability() > 0 && vehicle_is_in_rectangle( vehicle, leftUp, rightDown ) && vehicle.getType() == vehicleType )
            vehicles += vehicleIt.first;
    }
    return vehicles;
}

VehicleController::Vehicles VehicleController::enemy_get( VehicleType vehicleType )
{
    Vehicles vehicles;
    for ( auto vehicleIt : enemy_vehicles_ )
    {
        if ( vehicleIt.second->getDurability() > 0 && vehicleIt.second->getType() == vehicleType )
            vehicles += vehicleIt.first;
    }
    return vehicles;
}

VehicleController::Vehicles VehicleController::enemy_get( int groupID )
{
    Vehicles vehicles;
    for ( auto vehicleIt : enemy_vehicles_ )
    {
        const VehicleGroups& groups = vehicleIt.second->getGroups();
        if ( vehicleIt.second->getDurability() > 0 && std::find( groups.begin(), groups.end(), groupID ) != groups.end() )
            vehicles += vehicleIt.first;
    }
    return vehicles;
}

VehicleController::Point VehicleController::enemy_get_balanced_center()
{
    return enemy_get_balanced_center( enemy_get_all() );
}

VehicleController::Point VehicleController::enemy_get_balanced_center( VehicleID vehicleID )
{
    VehiclePtr vehicle = enemy_vehicles_[ vehicleID ];
    assert( vehicle != nullptr );
    return{ vehicle->getX(), vehicle->getY() };
}

VehicleController::Point VehicleController::enemy_get_balanced_center( const Vehicles & vehicles )
{
    double Sx = 0, Sy = 0;
    int vehicles_count = 0;
    for ( auto vehicleID : vehicles )
    {
        VehiclePtr vehicle = enemy_vehicles_[ vehicleID ];
        assert( vehicle != nullptr );
        if ( vehicle->getDurability() > 0 )
        {
            ++vehicles_count;
            Sx += vehicle->getX();
            Sy += vehicle->getY();
        }
    }
    Sx /= vehicles_count;
    Sy /= vehicles_count;
    return{ Sx, Sy };
}

VehicleController::Point VehicleController::enemy_get_center()
{
    return enemy_get_center(enemy_get_all());
}

VehicleController::Point VehicleController::enemy_get_center( VehicleID vehicleID )
{
    VehiclePtr vehicle = enemy_vehicles_[ vehicleID ];
    assert( vehicle != nullptr );
    return{ vehicle->getX(), vehicle->getY() };
}

VehicleController::Point VehicleController::enemy_get_center( const Vehicles & vehicles )
{
    double maxX = worldWidth * -3;
    double minX = worldWidth * 3;
    double maxY = worldWidth * -3;
    double minY = worldWidth * 3;
    for ( auto vehicleID : vehicles )
    {
        VehiclePtr vehicle = enemy_vehicles_[ vehicleID ];
        assert( vehicle != nullptr );
        if ( vehicle->getDurability() > 0 )
        {
            if ( vehicle->getX() > maxX )
            {
                maxX = vehicle->getX();
            }
            if ( vehicle->getX() < minX )
            {
                minX = vehicle->getX();
            }
            if ( vehicle->getY() > maxY )
            {
                maxY = vehicle->getY();
            }
            if ( vehicle->getY() < minY )
            {
                minY = vehicle->getY();
            }
        }
    }
    return{ ( minX + maxX ) / 2.0,( minY + maxY ) / 2.0 };
}

bool VehicleController::enemy_is_moving()
{
    return enemy_moving_vehicles_.empty() == false;
}

bool VehicleController::enemy_is_moving( VehicleID vehicleID )
{
    return enemy_moving_vehicles_.find( vehicleID ) != enemy_moving_vehicles_.end();
}

bool VehicleController::enemy_is_moving( const Vehicles & vehicles )
{
    int count = std::set_difference( enemy_moving_vehicles_.begin(), enemy_moving_vehicles_.end(),
                                     vehicles.begin(), vehicles.end(),
                                     CountingIterator() );
    return count < moving_vehicles_.size();
}
