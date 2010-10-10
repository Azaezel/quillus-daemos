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
#include "Folder.hpp"
#include "Project.hpp"
#include "GameObjectType.hpp"

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeAction.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectFolderDomainObject.hpp>
#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectFolderDataCollection.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
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
    pGameObjectFolderDataMap_type pFolderDM =
        I_GameObjectFolderDataMap::create(_pDBConn);

    m_pFolderDO = pFolderDM->createNew();

    m_pFolderDO->getExplorerNodeId() = getNode()->getNodeId();
    m_pFolderDO->getName() = getNode()->getDisplayName();

    pFolderDM->update(m_pFolderDO)->getValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::load(pDatabaseConnection_type _pDBConn)
{
    pGameObjectFolderDataMap_type pFolderDM =
        I_GameObjectFolderDataMap::create(_pDBConn);

    struct FolderVisitor 
    : public I_GameObjectFolderDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pGameObjectFolderDomainObject_type _pDomainObject)
        {
            // Only use the first DO.  There should only be one.
            // Throw an error if more than one?
            if (m_firstTime)
            {
                m_parent.setFolderDO(_pDomainObject);
                m_firstTime = false;
            }
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
        Folder&         m_parent;
        bool            m_firstTime;
    };

    FolderVisitor visitor(*this);

    pFolderDM->getByExplorerNodeId(getNode()->getNodeId())->getValue()
            ->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::save(pDatabaseConnection_type _pDBConn)
{
    pGameObjectFolderDataMap_type pFolderDM =
        I_GameObjectFolderDataMap::create(_pDBConn);

    pFolderDM->update(m_pFolderDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::remove(pDatabaseConnection_type _pDBConn)
{
    pGameObjectFolderDataMap_type pFolderDM =
        I_GameObjectFolderDataMap::create(_pDBConn);

    pFolderDM->deleteByKey(m_pFolderDO->getGameObjectFolderId());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Folder::newObjectType(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    Zen::Studio::Workbench::I_ExplorerNode::pUserData_type pData(new GameObjectType(m_parent, "NewObjectType"));

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
Folder::setFolderDO(pGameObjectFolderDomainObject_type _pFolderDO)
{
    m_pFolderDO = _pFolderDO;

    getNode()->setDisplayName(m_pFolderDO->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
