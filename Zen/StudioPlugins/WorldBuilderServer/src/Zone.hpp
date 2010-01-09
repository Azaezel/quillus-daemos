//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// World Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Worlds by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef WORLDBUILDER_GAME_OBJECT_TYPE_HPP_INCLUDED
#define WORLDBUILDER_GAME_OBJECT_TYPE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>

#include <Zen/StudioPlugins/WorldBuilderCommon/I_Zone.hpp>
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

/// @see GameObjectType
class Zone
:   public I_Zone
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_ExplorerNodeActions> pExplorerNodeActions_type;
    typedef I_ZoneDataMap::pZoneDataMap_type                                        pZoneDataMap_type;
    typedef I_ZoneDataMap::pZoneDomainObject_type                                   pZoneDomainObject_type;
    /// @}

    /// @name UserData overrides
    /// @{
public:
    virtual void setName(const std::string& _name);
    virtual Zen::Studio::Workbench::I_ExplorerNodeType& getType() const;
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name I_Zone implementation
    /// @{
public:
    virtual pDocument_type getDocument();
    /// @}

    /// @name Zone implementation
    /// @{
public:
    /// Called by load() to set the m_pZoneDO.
    /// This should be treated as private, but it needs to be public since
    /// it's called by a visitor struct.
    void setZoneDO(pZoneDomainObject_type _pZoneDO);
    /// @}

    /// @name 'Structors
    /// @{
public:
    /// @todo Should _parent be Folder?  What about other game object types?
    explicit Zone(Zen::Studio::Workbench::I_Project& _parent, const std::string& _name);
    virtual ~Zone();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_Project&  m_parent;
    static class ZoneType               sm_type;
    pZoneDomainObject_type              m_pZoneDO;

private:
    friend class ZoneType;
    static pExplorerNodeActions_type    sm_pActions;
    pDocument_type                      m_pDocument;
    /// @}

};  // class Zone

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // WORLDBUILDER_GAME_OBJECT_TYPE_HPP_INCLUDED
