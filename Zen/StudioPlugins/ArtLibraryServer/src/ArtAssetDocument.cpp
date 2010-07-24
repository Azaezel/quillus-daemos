//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Art Library
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
#include "ArtAssetDocument.hpp"
#include "ArtAssetExplorerNode.hpp"

#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/WorkbenchCommon/subscription.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetView.hpp>

#include <Zen/StudioPlugins/ArtLibraryModel/I_ArtAssetDocumentDataMap.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetDocument::ArtAssetDocument(Zen::Studio::Workbench::I_Project& _project, Zen::Studio::Workbench::I_Document::pDocument_type* _ppParent)
:   m_project(_project)
,   m_ppParent(_ppParent)
,   m_controller(*this)
,   m_pSubscriptionsGuard(Zen::Threading::MutexFactory::create())
,   m_loaded(false)
{
    // Subscribe to the parent document.
    if( m_ppParent != NULL )
    {
        /// @TODO The following line could result in bad juju bwana should the dynamic cast
        ///       fail and return a NULL.  Refactor this so that it is exception safe -- 
        ///       Better yet, think about doing this somewhere other than the constructor.
        m_pParentSubscription = dynamic_cast<ArtAssetDocument*>(m_ppParent->get())->subscribe(this);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetDocument::~ArtAssetDocument()
{
    Zen::Threading::MutexFactory::destroy(m_pSubscriptionsGuard);

    if( m_ppParent == NULL )
    {
        delete m_ppParent;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ArtAssetDocument::getShortName() const
{
    return m_pNode->getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ArtAssetDocument::getLongName() const
{
    /// @TODO Create a fully qualified name, including project, parents, etc...
    return m_pNode->getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Project*
ArtAssetDocument::getProject() const
{
    return &m_project;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Zen::Studio::Workbench::subscription<ArtAssetDocument*, ArtAssetDocument::pSpreadSheetView_type>    SubscriptionSS_type;
typedef Zen::Studio::Workbench::subscription<ArtAssetDocument*, ArtAssetDocument::pArtAssetView_type>       SubscriptionAA_type;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Zen::Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    // Try the AA subscription
    {
        SubscriptionAA_type* pSubscription = 
            dynamic_cast<SubscriptionAA_type*>(_wpSubscription.get());

        if(pSubscription != NULL)
        {
            delete pSubscription;
            return;
        }
    }

    // Try the SS subscription
    {
        SubscriptionSS_type* pSubscription =
            dynamic_cast<SubscriptionSS_type*>(_wpSubscription.get());

        if(pSubscription != NULL)
        {
            delete pSubscription;
            return;
        }
    }

    /// Else error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetDocument::pSubscription_type
ArtAssetDocument::subscribe(pArtAssetView_type _pView, pFilter_type _pFilter)
{
    // Cannot subscribe unless the document has already been loaded.
    assert(m_loaded);

    // Only the insertion of the view into the collection needs to be guarded
    {
        Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

        m_subscribersAA.insert(_pView);
    }

    return pSubscription_type(new SubscriptionAA_type(this, _pView), &destroySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetDocument::pSubscription_type
ArtAssetDocument::subscribe(pSpreadSheetView_type _pView, pFilter_type _pFilter)
{
    // Cannot subscribe unless the document has already been loaded.
    assert(m_loaded);

    // Only the insertion of the view into the collection needs to be guarded
    {
        Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

        m_subscribersSS.insert(_pView);
    }

    return pSubscription_type(new SubscriptionSS_type(this, _pView), &destroySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ArtAssetController&
ArtAssetDocument::getArtAssetController()
{
    return m_controller;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetDocument::pExplorerNode_type
ArtAssetDocument::getNode()
{
    return m_pNode;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_SpreadSheetController&
ArtAssetDocument::getSpreadSheetController()
{
    return m_controller;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_SpreadSheetProperties&
ArtAssetDocument::getProperties()
{
    throw Zen::Utility::runtime_exception("ArtAssetDocument::getProperties() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_SpreadSheetCellProperties&
ArtAssetDocument::getCellProperties(const int _column, const int _row)
{
    throw Zen::Utility::runtime_exception("ArtAssetDocument::getCellProperties() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetDocument::unSubscribe(pSpreadSheetView_type _pView)
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);
    m_subscribersSS.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetDocument::unSubscribe(pArtAssetView_type _pView)
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);
    m_subscribersAA.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetDocument::save()
{
    throw Zen::Utility::runtime_exception("ArtAssetDocument::save() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetDocument::load(ArtAssetExplorerNode& _node)
{
    throw Zen::Utility::runtime_exception("ArtAssetDocument::load() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetDocument::handleExplorerNodeModified(I_ExplorerNode* _pNode)
{
    // Notify the subscriptions of the change
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

    for(SpreadSheetSubscribers_type::iterator ssView = m_subscribersSS.begin(); ssView != m_subscribersSS.end(); ssView++)
    {
        /// @TODO Long name and short name should probably be different.
        (*ssView)->onNameModified(_pNode->getDisplayName(), _pNode->getDisplayName());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
ArtAssetDocument::getArtAssetId() const
{
    return m_pNode->getArtAssetId();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
