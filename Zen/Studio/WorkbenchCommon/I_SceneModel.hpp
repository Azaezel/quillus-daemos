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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_I_SCENE_MODEL_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_I_SCENE_MODEL_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/Database/I_Persistable.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Spaces {
        namespace ObjectModel {
            class I_Filter;
            class I_Subscription;
        }   // namespace ObjectModel
    }   // namespace Spaces
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_SceneController;
class I_SceneView;
class I_SceneModelType;
class I_SceneContributor;
class I_SceneAction;

class WORKBENCHCOMMON_DLL_LINK I_SceneModel
:   public virtual I_Document
{
    /// @name Types
    /// @{
public:
    typedef I_SceneContributor                                  Contributor_type;
    typedef I_SceneAction                                       Action_type;

    /// User Data for a I_SceneModel.
    /// This UserData class is data that is associated with an I_SceneModel
    /// instance.  This is what the application developer uses in order to associate
    /// scene models with application data.
    class WORKBENCHCOMMON_DLL_LINK UserData
    {
    public:
        /// The framework will call setDocument() when user data is associated with
        /// a I_SpreadSheetDocument.
        virtual void setModel(I_SceneModel* _pModel) { m_pModel = _pModel; }
        virtual I_SceneModel* getModel() const { return m_pModel; }

        /// Implement this method.
        /// Your extension to UserData should return the type of scene model
        /// this data belongs.
        virtual I_SceneModelType& getType() const = 0;

    public:
        UserData()
        : m_pModel(NULL)
        {
        }
        virtual ~UserData() {}
    private:
        I_SceneModel*       m_pModel;
    };  // class UserData

    typedef Memory::managed_ptr<UserData>                               pUserData_type;

    typedef I_SceneView*                                                pSceneView_type;

    typedef Memory::managed_ptr<I_Document>                             pDocument_type;
    typedef Memory::managed_ptr<I_SceneModel>                           pSceneModel_type;

    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Filter>          pFilter_type;
    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Subscription>    pSubscription_type;

    /// @}

    /// @name I_SceneModel interface
    /// @{
public:
    /// Subscribe a view to this model.
    /// The subscription should be stored in the view so that when
    /// the view goes out of scope the subscription also goes out
    /// of scope.  The view should also retain a strong reference
    /// to the model.
    /// @param _pView - Unmanaged pointer to the view.  The view MUST remain
    ///     alive for at least as long as the model (which it will if it keeps
    ///     a srong reference to the document).  The view MUST dereference the
    ///     subscription before the view is destroyed.
    virtual pSubscription_type subscribe(pSceneView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    /// Get the controller.
    /// I_SceneController is the write access to the model.
    /// Use it to modify the contents of the model.
    /// @return A controller that can be used to modify this model.
    ///     Do not retain a reference to this controller.
    virtual I_SceneController& getController() = 0;

    /// Get the user data associated with this scene model.
    /// If you need to get the type of the this scene model, use
    /// getUserData()->getType().
    virtual pUserData_type getUserData() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
    explicit I_SceneModel();
    virtual ~I_SceneModel();
    /// @}

};  // interface I_SceneModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
	/// I_SceneModel is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_SceneModel> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_I_SCENE_MODEL_HPP_INCLUDED
