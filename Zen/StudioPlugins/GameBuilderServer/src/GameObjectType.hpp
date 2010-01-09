//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAMEBUILDER_GAME_OBJECT_TYPE_HPP_INCLUDED
#define GAMEBUILDER_GAME_OBJECT_TYPE_HPP_INCLUDED

#include "DocumentProperties.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocument.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectType.hpp>
#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectTypeDataMap.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ExplorerNodeActions;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Project;
class GameObjectTypeDocument;

/// Game Object Type explorer node.
/// @note Possibly should rename this to GameObjectTypeNode or
///     GameObjectTypeExplorerNode but for now I'll leave it as-is.
class GameObjectType
:   public I_GameObjectType
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_ExplorerNodeActions> pExplorerNodeActions_type;
    typedef I_GameObjectTypeDataMap::pGameObjectTypeDataMap_type                    pGameObjectTypeDataMap_type;
    typedef I_GameObjectTypeDataMap::pGameObjectTypeDomainObject_type               pGameObjectTypeDomainObject_type;
    typedef Zen::Event::Event<GameObjectTypeDocument*>                              DocumentEvent_type;
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

    /// @name I_GameObjectType implementation
    /// @{
public:
    virtual pDocument_type getDocument();
    virtual void addChild(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);
    virtual boost::uint64_t getGameObjectTypeId() const;
    /// @}

    /// @name GameObjectType implementation
    /// @{
public:
    /// Called by load() to set the m_pGameObjectTypeDO.
    /// This should be treated as private, but it needs to be public since
    /// it's called by a visitor struct.
    void setGameObjectTypeDO(pGameObjectTypeDomainObject_type _pGameObjectTypeDO);

    /// Get the project which this Game Object Type belongs.
    Project& getProject();

    DocumentProperties& getProperties();

private:
    /// Static factory destroy method.
    static void destroyDocument(Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_Document> _wpDoc);
    /// @}

    /// @name Static events for notifying when documents are opened / closed.
    ///     The payload is GameObjectTypeDocument*.  It's a raw pointer
    ///     instead of a managed pointer because it would be incorrect for
    ///     the event handlers to maintain
    /// @{
public:
    static DocumentEvent_type  onOpened;
    static DocumentEvent_type  onClosed;
    /// @}

    /// @name 'Structors
    /// @{
public:
    /// @param _parent Project which contains this document.
    /// @todo Should _parent be Folder?  What about other game object types?
    explicit GameObjectType(Project& _project, const std::string& _name);
    virtual ~GameObjectType();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Project&                            m_project;
    static class GameObjectTypeType     sm_type;
    pGameObjectTypeDomainObject_type    m_pGameObjectTypeDO;

    DocumentProperties                  m_properties;

private:
    friend class GameObjectTypeType;
    static pExplorerNodeActions_type    sm_pActions;
    pDocument_type                      m_pDocument;
    /// @}

};  // class GameObjectType

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // GAMEBUILDER_GAME_OBJECT_TYPE_HPP_INCLUDED
