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
#include "ExplorerNodeFactory.hpp"

#include "ProjectService.hpp"

#include "Project.hpp"
#include "Folder.hpp"
#include "TerrainGroupNode.hpp"
#include "TerrainChunkNode.hpp"

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <Zen/Studio/WorkbenchModel/I_ProjectDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDomainObject.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeFactory::ExplorerNodeFactory()
{
    /// TODO Create a static const version of this map using boost::assign::map_list_of
    m_nodeTypes[I_Folder::getNodeType().getName()] = boost::bind(createFolder,_1);
    m_nodeTypes[I_TerrainGroupNode::getNodeType().getName()] = boost::bind(createTerrainGroup,_1);
    m_nodeTypes[I_TerrainChunkNode::getNodeType().getName()] = boost::bind(createTerrainChunk,_1);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeFactory::~ExplorerNodeFactory()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeFactory&
ExplorerNodeFactory::getSingleton()
{
    static ExplorerNodeFactory sm_factory;

    return sm_factory;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeFactory::pProject_type
ExplorerNodeFactory::createProject(Zen::Studio::Workbench::I_ProjectExplorerController& _controller,
                                   pProjectDomainObject_type _pProjectDO,
                                   pExplorerNodeDomainObject_type _pExplorerNodeDO)
{
    pProject_type pProject =
        ProjectService::getSingleton().createProject(
            _controller,
            _pProjectDO->getName()
        );

    return pProject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeFactory::pUserData_type
ExplorerNodeFactory::createNodeUserData(boost::uint64_t _explorerNodeId,
                                        const std::string& _nodeType,
                                        Zen::Studio::Workbench::I_ExplorerNode& _parent)
{
    NodeTypeUserDataFunctionMap_type::iterator iter = m_nodeTypes.find(_nodeType);
    if (iter != m_nodeTypes.end())
    {
        return iter->second(_parent);
    }
    
    return pUserData_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeFactory::pUserData_type
ExplorerNodeFactory::createFolder(Zen::Studio::Workbench::I_ExplorerNode& _node)
{
    Zen::Studio::Workbench::I_ExplorerNode::UserData* pParentUserData =
        _node.getUserData().get();

    // This is a folder, so quite likely (always?) the parent's user data is 
    // a project.
    Project* pParentProject = dynamic_cast<Project*>(pParentUserData);

    if (pParentProject != NULL)
    {
        // The Folder will get the name from the database, so just
        // pass an empty string for now.
        pUserData_type pData(new Folder(*pParentProject, ""));
        return pData;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Error, invalid parent type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeFactory::pUserData_type
ExplorerNodeFactory::createTerrainGroup(Zen::Studio::Workbench::I_ExplorerNode& _node)
{
    Zen::Studio::Workbench::I_ExplorerNode::UserData* pParentUserData =
        _node.getUserData().get();

    Folder* pParentFolder = dynamic_cast<Folder*>(pParentUserData);

    if (pParentFolder != NULL)
    {
        // The TerrainGroupNode will get the name from the database later, so
        // just pass an empty string for now.
        pUserData_type pData(new TerrainGroupNode(pParentFolder->getProject(), ""));
        return pData;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Error, invalid parent type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeFactory::pUserData_type
ExplorerNodeFactory::createTerrainChunk(Zen::Studio::Workbench::I_ExplorerNode& _node)
{
    Zen::Studio::Workbench::I_ExplorerNode::UserData* pParentUserData =
        _node.getUserData().get();

    TerrainGroupNode* pParentNode = dynamic_cast<TerrainGroupNode*>(pParentUserData);

    if (pParentNode != NULL)
    {
        // The TerrainGroupNode will get the name from the database later, so
        // just pass an empty string for now.
        pUserData_type pData(new TerrainChunkNode(pParentNode->getProject(), ""));
        return pData;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Error, invalid parent type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
