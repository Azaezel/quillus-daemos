//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// World Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef WORLDBUILDER_ZONE_HPP_INCLUDED
#define WORLDBUILDER_ZONE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/StudioPlugins/WorldBuilderCommon/I_ZoneModel.hpp>

#include <Zen/StudioPlugins/WorldBuilderModel/I_ZoneDataMap.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ExplorerNodeActions;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Project;

/// @see GameObjectTypeDocument
class ZoneModel
:   public I_ZoneModel
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_SceneModelType    I_SceneModelType;
    /// @}

    /// @name I_SceneModel::UserData implementation
    /// @{
public:
    virtual I_SceneModelType& getType() const;
    /// @}

    /// @name I_ZoneModel implementation
    /// @{
public:
    /// @}

    /// @name ZoneModel implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
    friend class Zone;
             ZoneModel();
    virtual ~ZoneModel();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

};  // class Zone

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // WORLDBUILDER_ZONE_HPP_INCLUDED
