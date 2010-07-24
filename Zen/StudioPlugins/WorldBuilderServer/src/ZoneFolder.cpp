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
#include "ZoneFolder.hpp"
#include "Project.hpp"
#include "Zone.hpp"

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeAction.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <Zen/StudioPlugins/WorldBuilderModel/I_ZoneFolderDomainObject.hpp>
#include <Zen/StudioPlugins/WorldBuilderModel/I_ZoneFolderDataCollection.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZoneFolder::ZoneFolder(Zen::Studio::Workbench::I_Project& _parent, const std::string& _name)
:   I_ZoneFolder(_name)
,   m_parent(_parent)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZoneFolder::~ZoneFolder()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
ZoneFolder::getType() const
{
    return I_ZoneFolder::getNodeType();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneFolder::insert(pDatabaseConnection_type _pDBConn)
{
    pZoneFolderDataMap_type pZoneFolderDM =
        I_ZoneFolderDataMap::create(_pDBConn);

    m_pZoneFolderDO = pZoneFolderDM->createNew();

    m_pZoneFolderDO->getExplorerNodeId() = getNode()->getNodeId();
    m_pZoneFolderDO->getName() = getNode()->getDisplayName();

    pZoneFolderDM->update(m_pZoneFolderDO)->getValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneFolder::load(pDatabaseConnection_type _pDBConn)
{
    pZoneFolderDataMap_type pZoneFolderDM =
        I_ZoneFolderDataMap::create(_pDBConn);

    struct ZoneFolderVisitor 
    : public I_ZoneFolderDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pZoneFolderDomainObject_type _pDomainObject)
        {
            // Only use the first DO.  There should only be one.
            // Throw an error if more than one?
            if (m_firstTime)
            {
                m_parent.setZoneFolderDO(_pDomainObject);
                m_firstTime = false;
            }
        }

        virtual void end()
        {
        }

        ZoneFolderVisitor(ZoneFolder& _parent)
        :   m_parent(_parent)
        ,   m_firstTime(true)
        {
        }

    private:
        ZoneFolder&     m_parent;
        bool            m_firstTime;
    };

    ZoneFolderVisitor visitor(*this);

    pZoneFolderDM->getByExplorerNodeId(getNode()->getNodeId())->getValue()
            ->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneFolder::save(pDatabaseConnection_type _pDBConn)
{
    pZoneFolderDataMap_type pZoneFolderDM =
        I_ZoneFolderDataMap::create(_pDBConn);

    pZoneFolderDM->update(m_pZoneFolderDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneFolder::remove(pDatabaseConnection_type _pDBConn)
{
    pZoneFolderDataMap_type pZoneFolderDM =
        I_ZoneFolderDataMap::create(_pDBConn);

    pZoneFolderDM->deleteByKey(m_pZoneFolderDO->getZoneFolderId());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneFolder::newZone(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    Zen::Studio::Workbench::I_ExplorerNode::pUserData_type pData(new Zone(m_parent, "New Zone"));

    m_parent.getController().createChildNode(*getNode(), pData);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Project&
ZoneFolder::getProject() const
{
    return m_parent;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneFolder::setZoneFolderDO(pZoneFolderDomainObject_type _pZoneFolderDO)
{
    m_pZoneFolderDO = _pZoneFolderDO;

    getNode()->setDisplayName(m_pZoneFolderDO->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
