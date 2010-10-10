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
#include "GameObjectType.hpp"
#include "Project.hpp"
#include "GameObjectTypeDocument.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeAction.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_WorkbenchService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/generic_explorer_node_type.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocument.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCell.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetController.hpp>

#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectTypeDomainObject.hpp>
#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectTypeDataCollection.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectType::DocumentEvent_type  GameObjectType::onOpened;
GameObjectType::DocumentEvent_type  GameObjectType::onClosed;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectType::GameObjectType(Project& _project, const std::string& _name)
:   I_GameObjectType(_name)
,   m_project(_project)
,   m_properties(*this)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectType::~GameObjectType()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectType::setName(const std::string& _name)
{
    // Only do this if the name changes
    if (getName() != _name)
    {
        m_pGameObjectTypeDO->getName() = _name;

        // Call the base class set name, which will fire the "modified" event.
        UserData::setName(_name);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
GameObjectType::getType() const
{
    return I_GameObjectType::getNodeType();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectType::registerProperties(Zen::Studio::Workbench::I_PropertiesPublisher& _properties)
{
    /// TODO Implement
    throw Zen::Utility::runtime_exception("Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_PropertiesPublisher&
GameObjectType::getPropertiesPublisher()
{
    /// TODO Implement
    throw Zen::Utility::runtime_exception("Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectType::insert(pDatabaseConnection_type _pDBConn)
{
    // First, insert the document properties.
    m_properties.insert(_pDBConn);

    pGameObjectTypeDataMap_type pGameObjectTypeDM =
        I_GameObjectTypeDataMap::create(_pDBConn);

    m_pGameObjectTypeDO = pGameObjectTypeDM->createNew();

    m_pGameObjectTypeDO->getExplorerNodeId() = getNode()->getNodeId();
    m_pGameObjectTypeDO->getName() = getNode()->getDisplayName();
    m_pGameObjectTypeDO->getPropertiesId() = m_properties.getPropertiesId();

    pGameObjectTypeDM->update(m_pGameObjectTypeDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectType::load(pDatabaseConnection_type _pDBConn)
{
    pGameObjectTypeDataMap_type pGameObjectTypeDM =
        I_GameObjectTypeDataMap::create(_pDBConn);

    struct GameObjectTypeVisitor 
    : public I_GameObjectTypeDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pGameObjectTypeDomainObject_type _pDomainObject)
        {
            // Only use the first DO.  There should only be one.
            // Throw an error if more than one?
            if (m_firstTime)
            {
                m_parent.setGameObjectTypeDO(_pDomainObject);
                m_firstTime = false;
            }
        }

        virtual void end()
        {
        }

        GameObjectTypeVisitor(GameObjectType& _parent)
        :   m_parent(_parent)
        ,   m_firstTime(true)
        {
        }

    private:
        GameObjectType& m_parent;
        bool            m_firstTime;
    };

    GameObjectTypeVisitor visitor(*this);

    pGameObjectTypeDM->getByExplorerNodeId(getNode()->getNodeId())->getValue()
            ->getAll(visitor);

    m_properties.setPropertiesId(m_pGameObjectTypeDO->getPropertiesId());
    m_properties.load(_pDBConn);
    
    getNode()->setDisplayName(m_pGameObjectTypeDO->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectType::save(pDatabaseConnection_type _pDBConn)
{
    //m_properties.save(_pDBConn);

    pGameObjectTypeDataMap_type pGameObjectTypeDM =
        I_GameObjectTypeDataMap::create(_pDBConn);

    m_pGameObjectTypeDO->setPropertiesId(m_properties.getPropertiesId());

    pGameObjectTypeDM->update(m_pGameObjectTypeDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectType::remove(pDatabaseConnection_type _pDBConn)
{
    pGameObjectTypeDataMap_type pGameObjectTypeDM =
        I_GameObjectTypeDataMap::create(_pDBConn);

    pGameObjectTypeDM->deleteByKey(m_pGameObjectTypeDO->getGameObjectTypeId());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectType::setGameObjectTypeDO(pGameObjectTypeDomainObject_type _pGameObjectTypeDO)
{
    m_pGameObjectTypeDO = _pGameObjectTypeDO;

    //getNode()->setDisplayName(m_pGameObjectTypeDO->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Project&
GameObjectType::getProject()
{
    return m_project;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DocumentProperties&
GameObjectType::getProperties()
{
    return m_properties;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
GameObjectType::getGameObjectTypeId() const
{
    return m_pGameObjectTypeDO->getGameObjectTypeId();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectType::destroyDocument(Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_Document> _wpDoc)
{
    GameObjectTypeDocument* pDoc = dynamic_cast<GameObjectTypeDocument*>(_wpDoc.get());

    if (pDoc)
    {
        onClosed(pDoc);
        delete pDoc;
    }
    else 
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectType::pDocument_type
GameObjectType::getDocument()
{
    if (m_pDocument.isValid())
    {
        return m_pDocument;
    }

    // Create a document and return it.

    // Get a reference to the workbench
    Zen::Studio::Workbench::I_WorkbenchService& 
        workbench(m_project.getController().getWorkbenchService());

    // TODO Create a spread sheet document user data.
    // Since GameObjectTypeDocument is a user-implemented document,
    // the user data isn't needed, but possibly eventually it will be.
    //Zen::Studio::Workbench::I_SpreadSheetDocument::pUserData_type
    //    pUserData();

    // Get the parent document.
    Zen::Studio::Workbench::I_ExplorerNode* const pParentNode = getNode()->getParent();

    pDocument_type* ppParentDocument = NULL;

    // If the parent node isn't NULL, see if the user data is a GameObjectType
    if (pParentNode != NULL)
    {
        GameObjectType* pParent = dynamic_cast<GameObjectType*>(pParentNode->getUserData().get());

        if (pParent)
        {
            ppParentDocument = new pDocument_type(pParent->getDocument());
        }
    }

    // Create the document.
    GameObjectTypeDocument* pRawDocument = new GameObjectTypeDocument(m_project, ppParentDocument);
    Zen::Studio::Workbench::I_Document::pDocument_type 
        pGameObjectTypeDocument(pRawDocument, &GameObjectType::destroyDocument);

    // TODO Create a future and pass it to this method.  Then have
    // this method fire the future event when the document is fully loaded.
    pRawDocument->load(*this);

    m_pDocument = pGameObjectTypeDocument;

    onOpened(pRawDocument);

    // TODO Return a future.
    return m_pDocument;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectType::addChild(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    Zen::Studio::Workbench::I_ExplorerNode::pUserData_type pData(new GameObjectType(m_project, "NewObjectType"));

    m_project.getController().createChildNode(*getNode(), pData);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
