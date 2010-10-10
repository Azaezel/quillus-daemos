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
#include "Folder.hpp"
#include "Project.hpp"
#include "TerrainGroupNode.hpp"

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeAction.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainFolderDomainObject.hpp>
#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainFolderDataCollection.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Folder::Folder(Project& _parent, const std::string& _name)
:   I_Folder(_name)
,   m_parent(_parent)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Folder::~Folder()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
Folder::getType() const
{
    return I_Folder::getNodeType();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::registerProperties(Zen::Studio::Workbench::I_PropertiesPublisher& _properties)
{
    /// TODO Implement
    throw Zen::Utility::runtime_exception("Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_PropertiesPublisher&
Folder::getPropertiesPublisher()
{
    /// TODO Implement
    throw Zen::Utility::runtime_exception("Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::insert(pDatabaseConnection_type _pDBConn)
{
    pTerrainFolderDataMap_type pFolderDM =
        I_TerrainFolderDataMap::create(_pDBConn);

    m_pFolderDO = pFolderDM->createNew();

    m_pFolderDO->getExplorerNodeId() = getNode()->getNodeId();
    m_pFolderDO->getName() = getNode()->getDisplayName();

    pFolderDM->update(m_pFolderDO)->getValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::load(pDatabaseConnection_type _pDBConn)
{
    pTerrainFolderDataMap_type pFolderDM =
        I_TerrainFolderDataMap::create(_pDBConn);

    struct FolderVisitor
    :   public I_TerrainFolderDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pTerrainFolderDomainObject_type _pDomainObject)
        {
            // Only use the first DO. There can be only one (hehe).
            if (m_firstTime)
            {
                m_parent.setFolderDO(_pDomainObject);
                m_firstTime = false;
            }

            // TODO Throw an error if more than one exists?
        }

        virtual void end()
        {
        }

        FolderVisitor(Folder& _parent)
        :   m_parent(_parent)
        ,   m_firstTime(true)
        {
        }

    private:
        Folder&             m_parent;
        bool                m_firstTime;
    };  // struct FolderVisitor

    FolderVisitor visitor(*this);

    pFolderDM->getByExplorerNodeId(getNode()->getNodeId())->getValue()
        ->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::save(pDatabaseConnection_type _pDBConn)
{
    pTerrainFolderDataMap_type pFolderDM =
        I_TerrainFolderDataMap::create(_pDBConn);

    pFolderDM->update(m_pFolderDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::remove(pDatabaseConnection_type _pDBConn)
{
    pTerrainFolderDataMap_type pFolderDM =
        I_TerrainFolderDataMap::create(_pDBConn);

    pFolderDM->deleteByKey(m_pFolderDO->getTerrainFolderId());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::newTerrainGroup(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    Zen::Studio::Workbench::I_ExplorerNode::pUserData_type pData(
        new TerrainGroupNode(m_parent, "NewTerrainGroup")
    );

    m_parent.getController().createChildNode(*getNode(), pData);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Project&
Folder::getProject() const
{
    return m_parent;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::setFolderDO(pTerrainFolderDomainObject_type _pFolderDO)
{
    m_pFolderDO = _pFolderDO;

    getNode()->setDisplayName(m_pFolderDO->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
