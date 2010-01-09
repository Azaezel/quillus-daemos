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
#ifndef WORLD_BUILDER_I_ZONE_MODEL_HPP_INCLUDED
#define WORLD_BUILDER_I_ZONE_MODEL_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Studio/WorkbenchCommon/I_SceneModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SceneModelType.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ExplorerNodeType;
            class I_ExplorerNodeActions;
            class I_Document;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// @see I_GameObjectTypeDocument
class WORLDBUILDERCOMMON_DLL_LINK I_ZoneModel
:   public Zen::Studio::Workbench::I_SceneModel::UserData
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_Document>            pDocument_type;
    /// @}

    /// @name I_ZoneModel interface
    /// @{
public:
    /// @}

    /// @name Static methods
    /// @{
public:
    static Zen::Studio::Workbench::I_SceneModelType& getModelType();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ZoneModel();
    virtual ~I_ZoneModel();
    /// @}

};  // interface I_ZoneModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLD_BUILDER_I_ZONE_MODEL_HPP_INCLUDED
