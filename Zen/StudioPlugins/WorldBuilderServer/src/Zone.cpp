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
#include "Zone.hpp"
#include "ZoneModel.hpp"

#include "Project.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeAction.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_WorkbenchService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/generic_explorer_node_type.hpp>

#include <Zen/StudioPlugins/WorldBuilderModel/I_ZoneDomainObject.hpp>
#include <Zen/StudioPlugins/WorldBuilderModel/I_ZoneDataCollection.hpp>


#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zone::Zone(Zen::Studio::Workbench::I_Project& _parent, const std::string& _name)
:   I_Zone(_name)
,   m_parent(_parent)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zone::~Zone()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Zone::setName(const std::string& _name)
{
    // Only do this if the name changes
    if (getName() != _name)
    {
        m_pZoneDO->getName() = _name;

        // Call the base class set name, which will fire the "modified" event.
        UserData::setName(_name);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
Zone::getType() const
{
    return I_Zone::getNodeType();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Zone::insert(pDatabaseConnection_type _pDBConn)
{
    pZoneDataMap_type pZoneDM =
        I_ZoneDataMap::create(_pDBConn);

    m_pZoneDO = pZoneDM->createNew();

    m_pZoneDO->getExplorerNodeId() = getNode()->getNodeId();
    m_pZoneDO->getName() = getNode()->getDisplayName();

    pZoneDM->update(m_pZoneDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Zone::load(pDatabaseConnection_type _pDBConn)
{
    pZoneDataMap_type pZoneDM =
        I_ZoneDataMap::create(_pDBConn);

    struct ZoneVisitor 
    : public I_ZoneDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pZoneDomainObject_type _pDomainObject)
        {
            // Only use the first DO.  There should only be one.
            // Throw an error if more than one?
            if (m_firstTime)
            {
                m_parent.setZoneDO(_pDomainObject);
                m_firstTime = false;
            }
        }

        virtual void end()
        {
        }

        ZoneVisitor(Zone& _parent)
        :   m_parent(_parent)
        ,   m_firstTime(true)
        {
        }

    private:
        Zone& m_parent;
        bool            m_firstTime;
    };

    ZoneVisitor visitor(*this);

    pZoneDM->getByExplorerNodeId(getNode()->getNodeId())->getValue()
            ->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Zone::save(pDatabaseConnection_type _pDBConn)
{
    pZoneDataMap_type pZoneDM =
        I_ZoneDataMap::create(_pDBConn);

    pZoneDM->update(m_pZoneDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Zone::remove(pDatabaseConnection_type _pDBConn)
{
    pZoneDataMap_type pZoneDM =
        I_ZoneDataMap::create(_pDBConn);

    pZoneDM->deleteByKey(m_pZoneDO->getZoneId());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Zone::setZoneDO(pZoneDomainObject_type _pZoneDO)
{
    m_pZoneDO = _pZoneDO;

    getNode()->setDisplayName(m_pZoneDO->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zone::pDocument_type
Zone::getDocument()
{
    if (m_pDocument.isValid())
    {
        return m_pDocument;
    }

    // Create a Scene model user data
    // (a ZoneModel in this case)
    Zen::Studio::Workbench::I_SceneModel::pUserData_type
        pUserData(new ZoneModel());

    // Create a document and return it.
    Zen::Studio::Workbench::I_WorkbenchService& workbench(m_parent.getController().getWorkbenchService());
    Zen::Studio::Workbench::I_WorkbenchService::pSceneModel_type 
        pSceneModel = workbench.createSceneModel(getNode(), &m_parent, pUserData);

    m_pDocument = pSceneModel;

    // TODO Populate this document in a separate thread.
    Zen::Studio::Workbench::I_SceneController& controller = pSceneModel->getController();

    // TODO Return a future.
    return m_pDocument;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
