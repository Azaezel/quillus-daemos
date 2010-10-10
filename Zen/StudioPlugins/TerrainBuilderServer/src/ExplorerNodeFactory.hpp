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
#ifndef TERRAINBUILDER_EXPLORER_NODE_FACTORY_HPP_INCLUDED
#define TERRAINBUILDER_EXPLORER_NODE_FACTORY_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeFactory.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ProjectService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ExplorerNodeFactory
:   public Zen::Studio::Workbench::I_ExplorerNodeFactory
{
    /// @name Types
    /// @{
public:
    typedef boost::function<pUserData_type(Zen::Studio::Workbench::I_ExplorerNode&)>    UserDataFunction_type;
    typedef std::map<std::string,UserDataFunction_type>                                 NodeTypeUserDataFunctionMap_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_ProjectService>     wpProjectService_type;
    /// @}

    /// @name I_ExplorerNodeFactory implementation
    /// @{
public:
    virtual pProject_type createProject(Zen::Studio::Workbench::I_ProjectExplorerController& _controller,
                                        pProjectDomainObject_type _pProjectDO,
                                        pExplorerNodeDomainObject_type _pExplorerNodeDO);

    virtual pUserData_type createNodeUserData(boost::uint64_t _explorerNodeId,
                                              const std::string& _nodeType,
                                              Zen::Studio::Workbench::I_ExplorerNode& _parent);
    /// @}

    /// @name Static Methods
    /// @{
public:
    static ExplorerNodeFactory& getSingleton();
    static pUserData_type createFolder(Zen::Studio::Workbench::I_ExplorerNode& _node);
    static pUserData_type createTerrainGroup(Zen::Studio::Workbench::I_ExplorerNode& _node);
    static pUserData_type createTerrainChunk(Zen::Studio::Workbench::I_ExplorerNode& _node);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             ExplorerNodeFactory();
    virtual ~ExplorerNodeFactory();
    /// @}

    /// @name Member Variables
    /// @{
private:
    NodeTypeUserDataFunctionMap_type    m_nodeTypes;
    /// @}

};  // class ExplorerNodeFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAINBUILDER_EXPLORER_NODE_FACTORY_HPP_INCLUDED
