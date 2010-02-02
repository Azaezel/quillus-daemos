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
#include "GameObjectTypeDocument.hpp"
#include "GameObjectElement.hpp"
#include "GameObjectType.hpp"
#include "ElementProperties.hpp"

#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/WorkbenchCommon/subscription.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetView.hpp>

#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectElementDataMap.hpp>

#include <boost/bind.hpp>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectTypeDocument::GameObjectTypeDocument(Zen::Studio::Workbench::I_Project& _project, Zen::Studio::Workbench::I_Document::pDocument_type* _ppParent)
:   m_project(_project)
,   m_ppParent(_ppParent)
,   m_controller(*this)
,   m_pSubscriptionsGuard(Zen::Threading::MutexFactory::create())
,   m_parentElementCount(0)
,   m_loaded(false)
{
    // TODO Create default columns
    // Element Name
    // Data Type
    // Default Value

    // Subscribe to the parent document.
    if (m_ppParent != NULL)
    {
        m_pParentSubscription = dynamic_cast<GameObjectTypeDocument*>(m_ppParent->get())->subscribe(this);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectTypeDocument::~GameObjectTypeDocument()
{
    Zen::Threading::MutexFactory::destroy(m_pSubscriptionsGuard);

    if (m_ppParent == NULL)
    {
        delete m_ppParent;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameObjectTypeDocument::getShortName() const
{
    return m_pNode->getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameObjectTypeDocument::getLongName() const
{
    // TODO Create a fully qualified name, including project, parents, etc?
    return m_pNode->getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Project*
GameObjectTypeDocument::getProject() const
{
    return &m_project;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Zen::Studio::Workbench::subscription<GameObjectTypeDocument*, GameObjectTypeDocument::pSpreadSheetView_type>    SubscriptionSS;
typedef Zen::Studio::Workbench::subscription<GameObjectTypeDocument*, GameObjectTypeDocument::pGameObjectTypeView_type> SubscriptionGOT;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Zen::Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    // Try the GOT subscription
    {
        SubscriptionGOT* pSubscription = dynamic_cast<SubscriptionGOT*>(_wpSubscription.get());

        if (pSubscription)
        {
            delete pSubscription;
            return;
        }
    }

    // Try the SS subscription
    {
        SubscriptionSS* pSubscription = dynamic_cast<SubscriptionSS*>(_wpSubscription.get());

        if (pSubscription)
        {
            delete pSubscription;
            return;
        }
    }

    // Else error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectTypeDocument::pSubscription_type
GameObjectTypeDocument::subscribe(pGameObjectTypeView_type _pView, pFilter_type _pFilter)
{
    // Cannot subscribe unless the document has already been loaded.
    assert(m_loaded);

    // This lock needs to only guard the insert and not the notification,
    // otherwise a nested lock will occur, which will deadlock on some
    // platforms.
    {
        Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

        m_subscribersGOT.insert(_pView);
    }

    // Notify the view of the current state of the document.
    notifyView(_pView);

    return pSubscription_type(new SubscriptionGOT(this, _pView), &destroySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
GameObjectTypeDocument::notifyView(pGameObjectTypeView_type _pView)
{
    // Notify the view that this document changed.
    _pView->onDocumentModified(*this);

    // Have the parent document notify this view of all elements.
#if 0 // this won't notify of overridden values
    if (m_ppParent)
    {
        getParent()->notifyView(_pView);

        // TODO Is this correct?
        assert(m_parentElementCount == m_parentElements.size());
    }
#endif

    // Notify of the parent elements
    int row = 0;
    for(Elements_type::iterator iter = m_parentElements.begin(); iter != m_parentElements.end(); iter++)
    {
        // Notify of a new row inserted at the end.
        _pView->onNewElement(**iter, -1);

        // Let the element notify the view of it's current state.
        (*iter)->notifyView(_pView, row);

        // Skip to the row number.
        row++;
    }

    // Do this?
    // assert(row == m_parentElementCount);

    // Iterate through the elements and notify the view
    // of each element as if they were rows in a spread sheet.

    // Skip the parent element count because the parent has already notified
    // the view.
    for(Elements_type::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
    {
        _pView->onNewElement(**iter, -1);

        // Let the element notify the view of it's current state.
        (*iter)->notifyView(_pView, row);

        // Skip to the row number.
        row++;
    }

    return row;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectTypeDocument::pSubscription_type
GameObjectTypeDocument::subscribe(pSpreadSheetView_type _pView, pFilter_type _pFilter)
{
    // Cannot subscribe unless the document has already been loaded.
    assert(m_loaded);

    // Only the insertion of the view into the collection needs to guarded.
    {
        Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

        m_subscribersSS.insert(_pView);
    }

    // TODO Possibly don't hard-code these columns.
    _pView->onNewColumn(-1, "Element Name");
    _pView->onNewColumn(-1, "Data Type");
    _pView->onNewColumn(-1, "Default Value");

    notifyView(_pView);

    return pSubscription_type(new SubscriptionSS(this, _pView), &destroySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
GameObjectTypeDocument::notifyView(pSpreadSheetView_type _pView)
{
    // Notify of the parent elements

    int row = 0;
    for(Elements_type::iterator iter = m_parentElements.begin(); iter != m_parentElements.end(); iter++)
    {
        // Notify of a new row inserted at the end.
        _pView->onNewRow(-1, "");

        // Let the element notify the view of it's current state.
        (*iter)->notifyView(_pView, row);

        // Skip to the row number.
        row++;
    }

    // Iterate through the elements and notify the view
    // of each element as if they were rows in a spread sheet.

    for(Elements_type::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
    {
        // Notify of a new row inserted at the end.
        _pView->onNewRow(-1, "");

        // Let the element notify the view of it's current state.
        (*iter)->notifyView(_pView, row);

        // Skip to the row number.
        row++;
    }

    return row;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_GameObjectTypeController&
GameObjectTypeDocument::getGameObjectTypeController()
{
    return m_controller;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectTypeDocument::pExplorerNode_type
GameObjectTypeDocument::getNode()
{
    return m_pNode;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_SpreadSheetController&
GameObjectTypeDocument::getSpreadSheetController()
{
    return m_controller;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_SpreadSheetProperties&
GameObjectTypeDocument::getProperties()
{
    assert(m_loaded);

    GameObjectType* pNode = dynamic_cast<GameObjectType*>(m_pNode);
    return pNode->getProperties();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_SpreadSheetCellProperties&
GameObjectTypeDocument::getCellProperties(const int _column, const int _row)
{
    assert(m_loaded);

    if (_row < m_parentElementCount)
    {
        return m_parentElements[_row]->getProperties();
    }
    else
    {
        return m_elements[_row - m_parentElementCount]->getProperties();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::onDocumentModified(I_GameObjectTypeDocument& _gameObjectTypeDocument)
{
    // TODO How should this be handled?
    std::cout << "GameObjectTypeDocument::onDocumentModified(): What goes here?" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::onNewElement(I_GameObjectElement& _element, int _position)
{
    // This is called when the parent GOT changes.

    // TR - Why was this being locked here?  I moved it below.
    //Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

    GameObjectElement* pGameObjectElement = new GameObjectElement(*this, "", &_element);

    int newPosition = _position;

    // TODO Need to create a new GameObjectElement and place it at the correct location.
    if (_position == -1 || _position == m_parentElementCount)
    {
        m_parentElements.push_back(pGameObjectElement);

        // Correct the position given to the views.  When it's -1, we need to
        // pass the correct value (-1 means "the end" but when this GOT has elements,
        // the parent's end is not necessarily this GOT's end).
        newPosition = m_parentElementCount;

        ++m_parentElementCount;
    }
    else
    {
        // Iterate to find the _position
        int x = _position;
        for(Elements_type::iterator iter = m_parentElements.begin(); iter != m_parentElements.end(); iter++)
        {
            if (--x == 0)
            {
                // Insert the element
                m_parentElements.insert(iter, pGameObjectElement);
                break;
            }
        }
    }

    // Notify each of the elements that have shifted position of their new position
    int position = 0;
    for(Elements_type::iterator iter = m_parentElements.begin(); iter != m_parentElements.end(); iter++)
    {
        (*iter)->setElementPosition(position++);
    }

    for(Elements_type::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
    {
        (*iter)->setElementPosition(position++);
    }

    // Notify the child GOT's
    for(GameObjectTypeSubscribers_type::iterator iter = m_subscribersGOT.begin(); iter != m_subscribersGOT.end(); iter++)
    {
        (*iter)->onNewElement(*pGameObjectElement, newPosition);
    }

    {
        // TR Moved from above and made it local since it's guarding m_subscribersSS
        Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

        // Notify the spreadsheet views
        for(SpreadSheetSubscribers_type::iterator iter = m_subscribersSS.begin(); iter != m_subscribersSS.end(); iter++)
        {
            (*iter)->onNewRow(newPosition, "");
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::onElementRemoved(int _position)
{
    // This is called when the parent GOT changes.
    // TODO Implement
    throw Zen::Utility::runtime_exception("GameObjectTypeDocument::onElementRemoved(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::onElementModified(I_GameObjectElement& _element, int _row)
{
    // This is called when an element changes.  It's a parent GOT element
    // if _row < m_parentElementCount

    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

    GameObjectElement* pElement;

    // If this is a parent element, update the parent values of the GameObjectElement.
    if (_row < m_parentElementCount)
    {
        m_parentElements[_row]->updateParentValues(_element);

        pElement = m_parentElements[_row];
    }

    // If this document is being loaded, don't do anything else.
    if (!m_loaded)
    {
        return;
    }

    // If this is a child element, get the element.
    if (_row >= m_parentElementCount)
    {
        pElement = m_elements[_row - m_parentElementCount];
    }

    // Whichever element, use it to notify all of the views that is has
    // been modified.

    // Notify the child GOT's
    for(GameObjectTypeSubscribers_type::iterator iter = m_subscribersGOT.begin(); iter != m_subscribersGOT.end(); iter++)
    {
        pElement->notifyView(*iter, _row);
    }

    // Notify the spreadsheet views
    for(SpreadSheetSubscribers_type::iterator iter = m_subscribersSS.begin(); iter != m_subscribersSS.end(); iter++)
    {
        pElement->notifyView(*iter, _row);
    }

    // And save everything.
    save();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::onAddProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property)
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::onValueModified(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property)
{
    notifyViewsOfModification();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::onRemoveProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::unSubscribe(pSpreadSheetView_type _pView)
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);
    m_subscribersSS.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::unSubscribe(pGameObjectTypeView_type _pView)
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);
    m_subscribersGOT.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::save()
{
    pDatabaseConnection_type pDbConn = getProject()->getDatabaseConnection();

    // Save the parent elements.
    for(Elements_type::iterator iter = m_parentElements.begin(); iter != m_parentElements.end(); iter++)
    {
        (*iter)->save(pDbConn);
    }

    // Save this GOT's elements.
    for(Elements_type::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
    {
        (*iter)->save(pDbConn);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::load(GameObjectType& _node)
{
    m_pNode = &_node;

    // Subscribe to this node's onModified event so we can be notified in particular about
    // name changes.
    m_pNodeConnection = m_pNode->getNode()->onModified.connect(boost::bind(&GameObjectTypeDocument::handleExplorerNodeModified, this, _1));

    // Load this document from the database
    if (getProject() != NULL)
    {
        struct ElementCollectionVisitor
        :   public I_GameObjectElementDataCollection::I_CollectionVisitor
        {
            virtual void begin() {}
            virtual void visit(I_GameObjectElementDataCollection::pGameObjectElementDomainObject_type _pDomainObject)
            {
                m_document.addElement(m_pDbConn, _pDomainObject);
            }

            virtual void end() {}

            ElementCollectionVisitor(pDatabaseConnection_type _pDbConn, GameObjectTypeDocument& _document)
            :   m_pDbConn(_pDbConn)
            ,   m_document(_document)
            {
            }

        private:
            pDatabaseConnection_type    m_pDbConn;
            GameObjectTypeDocument&     m_document;
        };

        pDatabaseConnection_type pDbConn = getProject()->getDatabaseConnection();

        I_GameObjectElementDataMap::pGameObjectElementDataMap_type
            pElementDM = I_GameObjectElementDataMap::create(pDbConn);

        I_GameObjectElementDataMap::pFutureGameObjectElementDataCollection_type
            pElementDC = pElementDM->getByGameObjectTypeId(_node.getGameObjectTypeId());

        ElementCollectionVisitor elementVisitor(pDbConn, *this);
        pElementDC->getValue()->getAll(elementVisitor);

    }

    // Subscribe to the node's
    GameObjectType* const pNode = dynamic_cast<GameObjectType*>(m_pNode);
    m_pDocumentPropertySubscription = pNode->getProperties().subscribe(this);

    m_loaded = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::notifyViewsOfModification()
{
    // TODO Guard?  Right now it's not necessary

    std::cout << "GameObjectTypeDocument::notifyViewsOfModification()" << std::endl;

    // Notify the child GOT's
    for(GameObjectTypeSubscribers_type::iterator iter = m_subscribersGOT.begin(); iter != m_subscribersGOT.end(); iter++)
    {
        (*iter)->onDocumentModified(*this);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::addElement(pDatabaseConnection_type _pDbConn, I_GameObjectElementDataCollection::pGameObjectElementDomainObject_type _pDomainObject)
{
    // Find the GameObjectElement.
    GameObjectElement* pElement = NULL;
    bool isParent = false;
    int position = 0;

    // If overriding a parent value, find it and update the GameObjectElement.
    if (_pDomainObject->getOverrideParent() == 1)
    {
        for(Elements_type::iterator iter = m_parentElements.begin(); iter != m_parentElements.end(); iter++)
        {
            if ((*iter)->getElementName() == _pDomainObject->getName().getStringValue())
            {
                pElement = *iter;
                isParent = true;
                break;
            }
            position++;
        }
    }
    else
    {
        // Create the element and put it on the list.  It should already be in
        // the correct sequence since the DC ordered it correctly.
        pElement = new GameObjectElement(*this, _pDomainObject->getName());
        position = m_elements.size() + m_parentElementCount;
        m_elements.push_back(pElement);
    }

    // If the element was found (or created), load the rest of the data from
    // the domain object.
    if (pElement)
    {
        pElement->load(_pDbConn, _pDomainObject, isParent, position);
    }
    else
    {
        // TODO Handle this case.
        // Quite likely this is caused by a parent element that no longer exists,
        // but it could be an "out of memory" error (unlikely, but possible).
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::handleExplorerNodeModified(I_ExplorerNode* _pNode)
{
    // Notifiy the subscriptions of the change

    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

    for(SpreadSheetSubscribers_type::iterator view = m_subscribersSS.begin(); view != m_subscribersSS.end(); view++)
    {
        // TODO Long name and short name should probably be different.
        (*view)->onNameModified(_pNode->getDisplayName(), _pNode->getDisplayName());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeDocument::insertElement(int _position, const std::string& _elementName)
{
    // This is called by the controller to insert a new element in this GOT.

    GameObjectElement* pElement = new GameObjectElement(*this, _elementName);

    int newPosition = _position;

    // TODO Guard this document
    {
        if (_position < 0)
        {
            // Append to the end
            m_elements.push_back(pElement);

            newPosition = m_elements.size() - 1;
        }
        else
        {
            // Iterate to find the _position
            int x = _position;
            for(Elements_type::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
            {
                if (--x == 0)
                {
                    // Insert the element
                    m_elements.insert(iter, pElement);
                    break;
                }
            }
        }

        // Notify each of the elements that have shifted position of their new position
        for(int x = newPosition; x < m_elements.size(); x++)
        {
            m_elements[x]->setElementPosition(x + m_parentElementCount);
        }

        // TODO This is the appropriate place to save.
        //save();
    }



    // Notify the view of the new element.
    {
        Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

        // Notify the GOT listeners
        for(GameObjectTypeSubscribers_type::iterator iter = m_subscribersGOT.begin(); iter != m_subscribersGOT.end(); iter++)
        {
            (*iter)->onNewElement(*pElement, newPosition + m_parentElementCount);
        }

        // Notify the SS views
        for(SpreadSheetSubscribers_type::iterator iter = m_subscribersSS.begin(); iter != m_subscribersSS.end(); iter++)
        {
            (*iter)->onNewRow(newPosition + m_parentElementCount, "");
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectElement&
GameObjectTypeDocument::getElement(int _row)
{
    // TODO Validate _row is not out of bounds.
    if (_row < m_parentElementCount)
    {
        return *m_parentElements[_row];
    }
    else
    {
        return *m_elements[_row - m_parentElementCount];
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
GameObjectTypeDocument::getGameObjectTypeId() const
{
    return m_pNode->getGameObjectTypeId();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
