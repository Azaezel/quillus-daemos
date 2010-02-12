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
#include "ArtAssetExplorerNode.hpp"
#include "ArtAssetDocument.hpp"

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

#include <Zen/StudioPlugins/ArtLibraryModel/I_ArtAssetExplorerNodeDomainObject.hpp>
#include <Zen/StudioPlugins/ArtLibraryModel/I_ArtAssetExplorerNodeDataCollection.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetExplorerNode::ArtAssetExplorerNode(Zen::Studio::Workbench::I_Project& _parent, const std::string& _name)
:   I_ArtAssetExplorerNode(_name)
,   m_parent(_parent)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetExplorerNode::~ArtAssetExplorerNode()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetExplorerNode::setName(const std::string& _name)
{
    // Only do this if the name changes
    if (getName() != _name)
    {
        m_pArtAssetExplorerNodeDO->getName() = _name;

        // Call the base class set name, which will fire the "modified" event.
        UserData::setName(_name);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
ArtAssetExplorerNode::getType() const
{
    return I_ArtAssetExplorerNode::getNodeType();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetExplorerNode::insert(pDatabaseConnection_type _pDBConn)
{
    pArtAssetExplorerNodeDataMap_type pArtAssetExplorerNodeDM =
        I_ArtAssetExplorerNodeDataMap::create(_pDBConn);

    m_pArtAssetExplorerNodeDO = pArtAssetExplorerNodeDM->createNew();

    m_pArtAssetExplorerNodeDO->getExplorerNodeId() = getNode()->getNodeId();
    m_pArtAssetExplorerNodeDO->getName() = getNode()->getDisplayName();

    pArtAssetExplorerNodeDM->update(m_pArtAssetExplorerNodeDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetExplorerNode::load(pDatabaseConnection_type _pDBConn)
{
    pArtAssetExplorerNodeDataMap_type pArtAssetExplorerNodeDM =
        I_ArtAssetExplorerNodeDataMap::create(_pDBConn);

    struct ArtAssetExplorerNodeVisitor 
    : public I_ArtAssetExplorerNodeDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pArtAssetExplorerNodeDomainObject_type _pDomainObject)
        {
            // Only use the first DO.  There should only be one.
            // Throw an error if more than one?
            if (m_firstTime)
            {
                m_parent.setArtAssetExplorerNodeDO(_pDomainObject);
                m_firstTime = false;
            }
        }

        virtual void end()
        {
        }

        ArtAssetExplorerNodeVisitor(ArtAssetExplorerNode& _parent)
        :   m_parent(_parent)
        ,   m_firstTime(true)
        {
        }

    private:
        ArtAssetExplorerNode& m_parent;
        bool            m_firstTime;
    };

    ArtAssetExplorerNodeVisitor visitor(*this);

    pArtAssetExplorerNodeDM->getByExplorerNodeId(getNode()->getNodeId())->getValue()
            ->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetExplorerNode::save(pDatabaseConnection_type _pDBConn)
{
    pArtAssetExplorerNodeDataMap_type pArtAssetExplorerNodeDM =
        I_ArtAssetExplorerNodeDataMap::create(_pDBConn);

    pArtAssetExplorerNodeDM->update(m_pArtAssetExplorerNodeDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetExplorerNode::remove(pDatabaseConnection_type _pDBConn)
{
    pArtAssetExplorerNodeDataMap_type pArtAssetExplorerNodeDM =
        I_ArtAssetExplorerNodeDataMap::create(_pDBConn);

    pArtAssetExplorerNodeDM->deleteByKey(m_pArtAssetExplorerNodeDO->getArtAssetExplorerNodeId());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetExplorerNode::setArtAssetExplorerNodeDO(pArtAssetExplorerNodeDomainObject_type _pArtAssetExplorerNodeDO)
{
    m_pArtAssetExplorerNodeDO = _pArtAssetExplorerNodeDO;

    getNode()->setDisplayName(m_pArtAssetExplorerNodeDO->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetExplorerNode::pDocument_type
ArtAssetExplorerNode::getDocument()
{
    if (m_pDocument.isValid())
    {
        return m_pDocument;
    }

    // Create a document and return it.

    // Get a reference to the workbench
    Zen::Studio::Workbench::I_WorkbenchService& 
        workbench(m_parent.getController().getWorkbenchService());

    // Create a spread sheet document user data 
    // (a ArtAssetExplorerNodeModel in this case)
    // Zen::Studio::Workbench::I_SpreadSheetDocument::pUserData_type
    //    pUserData(new ArtAssetDocument());

    // Get the parent document.
    Zen::Studio::Workbench::I_ExplorerNode* const pParentNode = getNode()->getParent();

    pDocument_type* ppParentDocument = NULL;

    // If the parent node isn't NULL, see if the user data is a ArtAssetExplorerNode
    if( pParentNode != NULL )
    {
        ArtAssetExplorerNode* pParent = dynamic_cast<ArtAssetExplorerNode*>(pParentNode->getUserData().get());

        if (pParent)
        {
            ppParentDocument = new pDocument_type(pParent->getDocument());
        }
    }

    // Create the document.
    ArtAssetDocument* pRawDocument = new ArtAssetDocument(m_parent, ppParentDocument);
    Zen::Studio::Workbench::I_Document::pDocument_type 
        pArtAssetDocument(pRawDocument, &ArtAssetExplorerNode::destroyDocument);

    // TODO Create a future and pass it to this method.  Then have
    // this method fire the future event when the document is fully loaded.
    pRawDocument->load(*this);

    m_pDocument = pArtAssetDocument;

    //onOpened(pRawDocument);

    // TODO Return a future.
    return m_pDocument;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
ArtAssetExplorerNode::getArtAssetId() const
{
    //return m_pArtAssetExplorerNodeDO->getArtAssetId();
    throw Zen::Utility::runtime_exception("ArtAssetExplorerNode::getArtAssetId() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetExplorerNode::destroyDocument(Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_Document> _wpDoc)
{
    ArtAssetDocument* pDoc = dynamic_cast<ArtAssetDocument*>(_wpDoc.get());

    if (pDoc)
    {
        //onClosed(pDoc);
        delete pDoc;
    }
    else 
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
