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
#ifndef ZENSTUDIO_WORKBENCH_I_VIEW_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_VIEW_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;
class I_Viewable;

/// Base workbench view.
class WORKBENCHCOMMON_DLL_LINK I_View
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_View interface
    /// @{
public:
    /// Get the project associated with this view.
    /// If the view is associated with a document, that document generally
    /// will be associated with a project.  If so, return it, otherwise return
    /// NULL.
    /// @return NULL If the view is not related to a project, otherwise return
    ///     the project.
    virtual I_Project* getProject() const = 0;

    /// Get the viewable object associated with this view.
    /// @return NULL if the view is not associated with a viewable object, otherwise 
    ///     return the viewable object.
    virtual I_Viewable* getViewable() const = 0;

    /// This method is called by the framework when the view gets focus.
    /// deactivate() is called on the deactivating view before activate() 
    /// is called on the newly activating view.  If this view is contained by
    /// an I_Notebook then see I_Notebook::onActivate event, which is also
    /// fired by the framework.
    virtual void activate() = 0;

    /// Deactivate this view so that it loses the focus.
    /// @see activate()
    virtual void deactivate() = 0;
    /// @}

    /// @name 'Structors
    /// @{
public:
             I_View();
    virtual ~I_View();
    /// @}

};  // interface I_View

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory 
{
    /// I_View is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_View> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_VIEW_HPP_INCLUDED
