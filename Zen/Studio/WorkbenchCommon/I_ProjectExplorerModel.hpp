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
#ifndef ZENSTUDIO_WORKBENCH_I_PROJECT_EXPLORER_MODEL_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROJECT_EXPLORER_MODEL_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_ExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

//#include <Zen/Core/Event/Event.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

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
class I_Project;
class I_ExplorerNode;
class I_ProjectExplorerView;

/// Project Explorer Model.
/// This model represents the data in the project explorer view.
/// @see I_ProjectExplorerController If you need to make modifications to this model,
///     use I_ProjectExplorerController
class WORKBENCHCOMMON_DLL_LINK I_ProjectExplorerModel
:   public I_ExplorerModel
{
    /// @name Types
    /// @{
public:
    typedef I_ProjectExplorerView*                                      pExplorerView_type;

    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Filter>          pFilter_type;
    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Subscription>    pSubscription_type;

    typedef Memory::managed_ptr<I_Project>                              pProject_type;
    //typedef Event::Event<I_ExplorerNode*>                               ProjectEvent_type;
    /// @}

    /// @name I_ProjectExplorerModel interface
    /// @{
public:
    /// Subscribe a view to this model.
    /// The subscription should be stored in the view so that when
    /// the view goes out of scope the subscription also goes out
    /// of scope.  The view should also retain a strong reference
    /// to the model.
    /// @param _pView - Unmanaged pointer to the view.  The view MUST remain
    ///     alive for at least as long as the model (which it will if it keeps
    ///     a srong reference to the model).  The view MUST dereference the
    ///     subscription before the view is destroyed.
    virtual pSubscription_type subscribe(pExplorerView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectExplorerModel();
    virtual ~I_ProjectExplorerModel();
    /// @}

};  // interface I_ProjectExplorerModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
	/// I_ProjectExplorerModel is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_ProjectExplorerModel> : public boost::true_type{};

    /// I_ProjectExplorerView is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_ProjectExplorerView> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROJECT_EXPLORER_MODEL_HPP_INCLUDED
