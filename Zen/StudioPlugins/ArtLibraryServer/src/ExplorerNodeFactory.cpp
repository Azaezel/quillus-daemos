//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Art Library
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

#include "ArtFolder.hpp"
#include "ArtAssetExplorerNode.hpp"

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <Zen/Studio/WorkbenchModel/I_ProjectDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDomainObject.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
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
    throw Zen::Utility::runtime_exception("ExplorerNodeFactory::createProject(): Error, not implemented.");
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
        Zen::Studio::Workbench::I_Project* pParentProject = dynamic_cast<Zen::Studio::Workbench::I_Project*>(pParentUserData);

        if (pParentProject)
        {
            // If _explorerNodeId is not 0 then the ArtFolder will get the name from the 
            // database so just pass an empty string.  If _explorerNodeId is 0 then default 
            // the value name to "Art Assets" although eventually we might want to customize this
            // based on the game type.
            pUserData_type pData(new ArtFolder(*pParentProject, _explorerNodeId == 0 ? "Art Assets" : ""));
            return pData;
        }
        else
        {
            // Check to see if 
            ArtFolder* pParentFolder = dynamic_cast<ArtFolder*>(pParentUserData);

            if (pParentFolder)
            {
                // Create a child folder
                pUserData_type pData(new ArtFolder(*pParentFolder));
                return pData;
            }
            else
            {
                // Huh?  Not a folder, so what do we do with it?
                throw Zen::Utility::runtime_exception("ExplorerNodeFactory::createNodeUserData(): Error, Art Folder with an invalid parent type.");
            }
        }
    }
    else if (strcmp(_nodeType.c_str(), I_ArtAssetExplorerNode::getNodeType().getName().c_str()) == 0)
    {
        ArtFolder* pParentFolder = dynamic_cast<ArtFolder*>(pParentUserData);

        if (pParentFolder)
        {
            // The Art Asset Explorer Node will get the name from the database later, so
            // just pass an empty string for now.
            pUserData_type pData(new ArtAssetExplorerNode(pParentFolder->getProject(), ""));
            return pData;
        }
        else
        {
            throw Zen::Utility::runtime_exception("Error, nested art assets are not supported.");
        }
    }

    // TODO Error?
    return pUserData_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
