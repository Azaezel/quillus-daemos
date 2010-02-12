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
#include "ArtFolder.hpp"
#include "ArtAssetExplorerNode.hpp"

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeAction.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <Zen/StudioPlugins/ArtLibraryModel/I_ArtFolderDomainObject.hpp>
#include <Zen/StudioPlugins/ArtLibraryModel/I_ArtFolderDataCollection.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtFolder::ArtFolder(Zen::Studio::Workbench::I_Project& _parent, const std::string& _name)
:   I_Folder(_name)
,   m_parent(_parent)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtFolder::ArtFolder(ArtFolder& _parentFolder)
:   I_Folder("")
,   m_parent(_parentFolder.getProject())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtFolder::~ArtFolder()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtFolder::setName(const std::string& _name)
{
    // Only do this if the name changes
    if (getName() != _name)
    {
        m_pArtFolderDO->getName() = _name;

        // Call the base class set name, which will fire the "modified" event.
        UserData::setName(_name);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
ArtFolder::getType() const
{
    return I_Folder::getNodeType();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtFolder::insert(pDatabaseConnection_type _pDBConn)
{
    pArtFolderDataMap_type pArtFolderDM =
        I_ArtFolderDataMap::create(_pDBConn);

    m_pArtFolderDO = pArtFolderDM->createNew();

    m_pArtFolderDO->getExplorerNodeId() = getNode()->getNodeId();
    m_pArtFolderDO->getName() = getNode()->getDisplayName();

    pArtFolderDM->update(m_pArtFolderDO)->getValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtFolder::load(pDatabaseConnection_type _pDBConn)
{
    pArtFolderDataMap_type pArtFolderDM =
        I_ArtFolderDataMap::create(_pDBConn);

    struct ArtFolderVisitor 
    : public I_ArtFolderDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pArtFolderDomainObject_type _pDomainObject)
        {
            // Only use the first DO.  There should only be one.
            // Throw an error if more than one?
            if (m_firstTime)
            {
                m_parent.setArtFolderDO(_pDomainObject);
                m_firstTime = false;
            }
        }

        virtual void end()
        {
        }

        ArtFolderVisitor(ArtFolder& _parent)
        :   m_parent(_parent)
        ,   m_firstTime(true)
        {
        }

    private:
        ArtFolder&     m_parent;
        bool            m_firstTime;
    };

    ArtFolderVisitor visitor(*this);

    pArtFolderDM->getByExplorerNodeId(getNode()->getNodeId())->getValue()
            ->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtFolder::save(pDatabaseConnection_type _pDBConn)
{
    pArtFolderDataMap_type pArtFolderDM =
        I_ArtFolderDataMap::create(_pDBConn);

    pArtFolderDM->update(m_pArtFolderDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtFolder::remove(pDatabaseConnection_type _pDBConn)
{
    pArtFolderDataMap_type pArtFolderDM =
        I_ArtFolderDataMap::create(_pDBConn);

    pArtFolderDM->deleteByKey(m_pArtFolderDO->getArtFolderId());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtFolder::newArtAsset(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    Zen::Studio::Workbench::I_ExplorerNode::pUserData_type pData(new ArtAssetExplorerNode(m_parent, "New Art Asset"));

    m_parent.getController().createChildNode(*getNode(), pData);
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtFolder::newFolder(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    Zen::Studio::Workbench::I_ExplorerNode::pUserData_type pData(new ArtFolder(m_parent, "New Folder"));

    m_parent.getController().createChildNode(*getNode(), pData);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Project&
ArtFolder::getProject() const
{
    return m_parent;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtFolder::setArtFolderDO(pArtFolderDomainObject_type _pArtFolderDO)
{
    m_pArtFolderDO = _pArtFolderDO;

    getNode()->setDisplayName(m_pArtFolderDO->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
