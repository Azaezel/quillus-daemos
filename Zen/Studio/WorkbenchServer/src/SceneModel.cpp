//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "SceneModel.hpp"
#include "ColumnHeader.hpp"

#include "WorkbenchService.hpp"

#include <Zen/Studio/WorkbenchCommon/subscription.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetView.hpp>

#include <Zen/Studio/WorkbenchModel/I_SceneModelDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_SceneModelDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_SceneModelDataCollection.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneModel::SceneModel(WorkbenchService& _workbenchService, I_Project* _pProject, pUserData_type _pUserData)
:   m_workbenchService(_workbenchService)
,   m_pProject(_pProject)
,   m_pUserData(_pUserData)
,   m_controller(*this)
,   m_pNode(NULL)
,   m_sceneModelId(0)
,   m_loaded(false)
,   m_pViewsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneModel::~SceneModel()
{
    Threading::MutexFactory::destroy(m_pViewsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef subscription<SceneModel*, SceneModel::pSceneView_type> Subscription;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    Subscription* pSubscription = dynamic_cast<Subscription*>(_wpSubscription.get());

    if (pSubscription)
    {
        delete pSubscription;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneModel::pSubscription_type
SceneModel::subscribe(pSceneView_type _pView, pFilter_type _pFilter)
{
    // Cannot subscribe unless the document has already been loaded.
    assert(m_loaded);

    Threading::CriticalSection lock(m_pViewsGuard);

    m_views.insert(_pView);

    // TODO Notify this view of the complete model.
    //throw Utility::runtime_exception("SceneModel::subscribe(): Error, not implemented.");

    return pSubscription_type(new Subscription(this, _pView), &destroySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneModel::unSubscribe(pSceneView_type _pView)
{
    Threading::CriticalSection lock(m_pViewsGuard);
    m_views.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
SceneModel::getShortName() const
{
    return m_pNode->getDisplayName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
SceneModel::getLongName() const
{
    // TODO Create a fully qualified name, including project, parents, etc?
    return m_pNode->getDisplayName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Project*
SceneModel::getProject() const
{
    return m_pProject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SceneController&
SceneModel::getController()
{
    return m_controller;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneModel::pUserData_type
SceneModel::getUserData() const
{
    return m_pUserData;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneModel::load(pExplorerNode_type _pNode)
{
    m_pNode = _pNode;

    // Subscribe to this node's onModified event so we can be notified in particular about
    // name changes.
    m_pNodeConnection = m_pNode->onModified.connect(boost::bind(&SceneModel::handleExplorerNodeModified, this, _1));

    // Get the data map.
    I_SceneModelDataMap::pSceneModelDataMap_type 
        pDocDM = I_SceneModelDataMap::create(m_workbenchService.getDatabaseConnection());

    // Get the documents by explorer node.  There should be one or zero.
    I_SceneModelDataMap::pFutureSceneModelDataCollection_type 
        pDocDC = pDocDM->getByExplorerNodeId(_pNode->getNodeId());

    // If the collection is empty, this is a new document
    if (pDocDC->getValue()->isEmpty())
    {
        I_SceneModelDataMap::pSceneModelDomainObject_type 
            pDocDO = pDocDM->createNew();

        pDocDO->setExplorerNodeId(_pNode->getNodeId());

        Zen::Database::I_DatabaseTransaction::pFutureKey_type 
            pKey = pDocDM->update(pDocDO);

        setSceneModelId(pKey->getValue());
    }
    else
    {
        struct SceneModelCollectionVisitor
        :   public I_SceneModelDataCollection::I_CollectionVisitor
        {
            virtual void begin()
            {
            }

            virtual void visit(I_SceneModelDataCollection::pSceneModelDomainObject_type _pDomainObject)
            {
                m_model.setSceneModelId(_pDomainObject->getSceneModelId());

                // TODO Throw an exception if more than one?
            }

            virtual void end()
            {
            }

            SceneModelCollectionVisitor(SceneModel& _model)
            :   m_model(_model)
            {
            }
        private:
            SceneModel&     m_model;
        };  // struct SceneModelCollectionVisitor

        SceneModelCollectionVisitor sceneModelVisitor(*this);
        pDocDC->getValue()->getAll(sceneModelVisitor);

        // TODO Load the reset of the model.
    }

    // Mark the document as loaded.
    m_loaded = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneModel::save()
{
    // Don't lock document.  It must be locked before calling this method.

    /// See SpreadSheetDocument::save
    throw Utility::runtime_exception("SceneModel::save(): Error, not implemented.");

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneModel::setSceneModelId(boost::uint64_t _modelId)
{
    m_sceneModelId = _modelId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
SceneModel::getSceneModelId() const
{
    return m_sceneModelId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneModel::handleExplorerNodeModified(I_ExplorerNode* _pNode)
{
    // Notifiy the subscriptions of the change

    throw Utility::runtime_exception("SceneModel::handleExplorerNodeModified(): Error, not implemented.");

    Threading::CriticalSection lock(m_pViewsGuard);

    for(SceneViews_type::iterator view = m_views.begin(); view != m_views.end(); view++)
    {
        // TODO Long name and short name should probably be different.
        //(*view)->onNameModified(_pNode->getDisplayName(), _pNode->getDisplayName());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

