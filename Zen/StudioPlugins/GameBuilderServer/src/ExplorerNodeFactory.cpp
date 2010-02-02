//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
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
#include "ExplorerNodeFactory.hpp"

#include "ProjectService.hpp"

#include "Project.hpp"
#include "Folder.hpp"
#include "GameObjectType.hpp"

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <Zen/Studio/WorkbenchModel/I_ProjectDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDomainObject.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeFactory::ExplorerNodeFactory()
{
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
ExplorerNodeFactory::createProject(Zen::Studio::Workbench::I_ProjectExplorerController& _controller, pProjectDomainObject_type _pProjectDO, pExplorerNodeDomainObject_type _pExplorerNodeDO)
{
    pProject_type pProject =
        ProjectService::getSingleton().createProject(_controller, _pProjectDO->getName());

    return pProject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeFactory::pUserData_type
ExplorerNodeFactory::createNodeUserData(boost::uint64_t _explorerNodeId, const std::string& _nodeType, Zen::Studio::Workbench::I_ExplorerNode& _parent)
{
    Zen::Studio::Workbench::I_ExplorerNode::UserData* pParentUserData = _parent.getUserData().get();

    // TODO Use a map for _nodeType?
    if (strcmp(_nodeType.c_str(), I_Folder::getNodeType().getName().c_str()) == 0)
    {
        // This is a folder, so quite likely (always?) the parent's user data is
        // a project.
        Project* pParentProject = dynamic_cast<Project*>(pParentUserData);

        if (pParentProject)
        {
            // The Folder will get the name from the database so just
            // pass an empty string for now.
            pUserData_type pData(new Folder(*pParentProject, ""));
            return pData;
        }
        else
        {
            // TODO Implement a nested folder
            throw Zen::Utility::runtime_exception("Nested folders not supported.");
        }        
    }
    else if (strcmp(_nodeType.c_str(), I_GameObjectType::getNodeType().getName().c_str()) == 0)
    {
        Folder* pParentFolder = dynamic_cast<Folder*>(pParentUserData);
        GameObjectType* pParentType = dynamic_cast<GameObjectType*>(pParentUserData);
        if (pParentFolder)
        {
            // The GameObjectType will get the name from the database later, so
            // just pass an empty string for now.
            pUserData_type pData(new GameObjectType(pParentFolder->getProject(), ""));
            return pData;
        }
        else if (pParentType)
        {
            pUserData_type pData(new GameObjectType(pParentType->getProject(), ""));
            return pData;
        }
        else
        {
            // TODO Implement a nested object
            throw Zen::Utility::runtime_exception("Error, invalid parent type.");
        }
    }

    // TODO Error?
    return pUserData_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
