//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Terrain Builder
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef TERRAINBUILDER_TERRAIN_CHUNK_NODE_HPP_INCLUDED
#define TERRAINBUILDER_TERRAIN_CHUNK_NODE_HPP_INCLUDED

#include "Properties.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainChunkNode.hpp>
#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainChunkDataMap.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ExplorerNodeActions;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Project;
class TerrainChunkDocument;

/// Terrain Group explorer node.
class TerrainChunkNode
:   public I_TerrainChunkNode
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_ExplorerNodeActions> pExplorerNodeActions_type;
    typedef I_TerrainChunkDataMap::pTerrainChunkDataMap_type                        pTerrainChunkDataMap_type;
    typedef I_TerrainChunkDataMap::pTerrainChunkDomainObject_type                   pTerrainChunkDomainObject_type;
    typedef Zen::Event::Event<TerrainChunkDocument*>                                DocumentEvent_type;
    /// @}

    /// @name UserData implementation
    /// @{
public:
    virtual void setName(const std::string& _name);
    virtual Zen::Studio::Workbench::I_ExplorerNodeType& getType() const;
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    virtual void registerProperties(Zen::Studio::Workbench::I_PropertiesPublisher& _properties);
    virtual Zen::Studio::Workbench::I_PropertiesPublisher& getPropertiesPublisher();
    /// @}

    /// @name I_TerrainChunkNode implementation
    /// @{
public:
    virtual pDocument_type getDocument();
    virtual boost::uint64_t getTerrainChunkId() const;
    /// @}

    /// @name TerrainChunk implementation
    /// @{
public:
    /// Called by load() to set the m_pTerrainChunkDO.
    /// This should be treated as private, but it needs to be public since
    /// it is called by a visitor struct.
    void setTerrainChunkDO(pTerrainChunkDomainObject_type _pTerrainChunkDO);

    /// Get the project to which this Terrain Group belongs.
    Project& getProject();

    Properties& getProperties();
    /// @}

    /// @name Static Methods & Events
    /// @{
public:
    static DocumentEvent_type onOpened;
    static DocumentEvent_type onClosed;
private:
    /// Static factory destroy method
    static void destroyDocument(Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_Document> _wpDocument);
    /// @}

    /// @name 'Structors
    /// @{
public:
    /// @param _parent Project which contains this document.
    explicit TerrainChunkNode(Project& _project, const std::string& _name);
    virtual ~TerrainChunkNode();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Project&                                    m_project;
    static class TerrainChunkNodeType           sm_type;
    pTerrainChunkDomainObject_type              m_pTerrainChunkDO;

    Properties                                  m_properties;

    friend class TerrainChunkNodeType;
    static pExplorerNodeActions_type            sm_pActions;
    pDocument_type                              m_pDocument;
    /// @}

};  // class TerrainChunkNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAINBUILDER_TERRAIN_CHUNK_NODE_HPP_INCLUDED
