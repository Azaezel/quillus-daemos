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
#include "ExplorerNodeFactory.hpp"

#include "ProjectService.hpp"

#include "Project.hpp"
#include "Zone.hpp"
#include "ZoneFolder.hpp"

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <Zen/Studio/WorkbenchModel/I_ProjectDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDomainObject.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
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
    std::cout << "ExplorerNodeFactory::createNodeUserData(): Begin" << std::endl;
    Zen::Studio::Workbench::I_ExplorerNode::UserData* pParentUserData = _parent.getUserData().get();

    // TODO Use a map for _nodeType?
    if (strcmp(_nodeType.c_str(), I_ZoneFolder::getNodeType().getName().c_str()) == 0)
    {
        // This is a folder, so quite likely (always?) the parent's user data is
        // a project.
        Zen::Studio::Workbench::I_Project* pParentProject = dynamic_cast<Zen::Studio::Workbench::I_Project*>(pParentUserData);

        if (pParentProject)
        {
            // If _explorerNodeId is not 0 then the ZoneFolder will get the name from the
            // database so just pass an empty string.  If _explorerNodeId is 0 then default
            // the value name to "Zones" although eventually we might want to customize this
            // based on the game type.
            pUserData_type pData(new ZoneFolder(*pParentProject, _explorerNodeId == 0 ? "Zones" : ""));
            return pData;
        }
        else
        {
            // TODO Implement a nested folder
            throw Zen::Utility::runtime_exception("Nested folders not supported.");
        }
    }
    else if (strcmp(_nodeType.c_str(), I_Zone::getNodeType().getName().c_str()) == 0)
    {
        ZoneFolder* pParentFolder = dynamic_cast<ZoneFolder*>(pParentUserData);

        if (pParentFolder)
        {
            // The Zone will get the name from the database later, so
            // just pass an empty string for now.
            pUserData_type pData(new Zone(pParentFolder->getProject(), ""));
            return pData;
        }
        else
        {
            // TODO Implement a nested object
            throw Zen::Utility::runtime_exception("Error, nested objects are not supported.");
        }
    }

    // TODO Error?
    return pUserData_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
