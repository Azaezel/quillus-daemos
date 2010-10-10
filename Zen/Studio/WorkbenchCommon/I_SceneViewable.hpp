//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZENSTUDIO_WORKBENCH_I_SCENE_VIEWABLE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SCENE_VIEWABLE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Viewable.hpp>

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
class I_SceneView;
class I_SceneController;
class I_SceneAction;
class I_SceneEntity;
class I_PropertiesPublisher;

/// Scene Viewable interface.
/// Implement this interface if you have a document or model that is viewable
/// by a I_SceneView.
class WORKBENCHCOMMON_DLL_LINK I_SceneViewable
:   public virtual I_Viewable
{
    /// @name Forward Declarations
    /// @{
public:
    class I_SceneViewEvent;
    /// @}

    /// @name Types
    /// @{
public:
    typedef I_SceneView*                                                pSceneView_type;

    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Filter>          pFilter_type;
    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Subscription>    pSubscription_type;

    typedef Event::Event<I_SceneViewEvent&>                             SceneViewEvent_type;
    /// @}

    /// @name I_SceneViewable interface
    /// @{
public:
    /// Subscribe to a view of this viewable.
    /// The subscription should be stored in the view so that when the view
    /// goes out of scope, the subscription also goes out of scope. The view
    /// should also retain a strong reference to the document.
    /// @param _pView - Unmanaged pointer to the view. The view MUST remain
    ///     alive for at least as long as the viewable object (which it will if
    ///     it keeps a strong reference to the viewable object). The view MUST
    ///     dereference the subscription before the view is destroyed.
    virtual pSubscription_type subscribe(pSceneView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    /// Get the controller to control this viewable.
    /// I_SceneController is the write access to the document.
    /// Use it to modify the contents of the document.
    /// @return A controller that can be used to modify this document.
    ///     Do not retain a reference to this controller.
    virtual I_SceneController& getSceneController() = 0;

    /// Get the properties of this scene.
    virtual I_PropertiesPublisher& getProperties() = 0;

    /// Get the properties of the specified scene entity.
    virtual I_PropertiesPublisher& getEntityProperties(const I_SceneEntity& _entity) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    SceneViewEvent_type onLeftClickEntity;
    /// @}

    /// @name Inner Structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class WORKBENCHCOMMON_DLL_LINK I_SceneViewEvent
    {
    public:
        /// Get the view associated with this event.
        virtual I_SceneView& getView() = 0;

        /// Get the entity associated with this event.
        /// @return NULL if this event is not associated with an entity,
        ///     otherwise it returns the entity that was highlighted when
        ///     the event was fired.
        virtual I_SceneEntity* getEntity() = 0;

        /// Call this method to add an action handler for this event.
        virtual void addAction(I_SceneAction& _action) = 0;

        /// Call this method to reject the event.
        /// If the event is rejected, no actions are displayed or invoked.
        virtual void rejectEvent() = 0;
    };  // class I_SceneViewEvent
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SceneViewable();
    virtual ~I_SceneViewable();
    /// @}

};  // class I_SceneViewable

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
    /// I_SceneViewable is managed by factory
    template<>
    struct is_managed_by_factory<Studio::Workbench::I_SceneViewable>
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SCENE_VIEWABLE_HPP_INCLUDED
