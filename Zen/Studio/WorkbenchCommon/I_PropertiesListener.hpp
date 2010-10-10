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
#ifndef ZENSTUDIO_WORKBENCH_I_PROPERTIES_LISTENER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROPERTIES_LISTENER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Property.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_PropertiesPublisher;

/// Properties Listener interface.
class WORKBENCHCOMMON_DLL_LINK I_PropertiesListener
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_PropertiesListener interface
    /// @{
public:
    /// Called when a new property is added to a property set.
    /// The implementation may retain a reference to _property
    /// since the listener must outlive the subscription to
    /// the publisher.
    virtual void onAddProperty(I_PropertiesPublisher& _publisher, I_Property& _property) = 0;

    /// Called when a property value has been modified.
    virtual void onValueModified(I_PropertiesPublisher& _publisher, I_Property& _property) = 0;

    /// Called when a property is removed from the property set.
    virtual void onRemoveProperty(I_PropertiesPublisher& _publisher, I_Property& _property) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PropertiesListener();
    virtual ~I_PropertiesListener();
    /// @}

};  // interface I_PropertiesListener

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory 
{
    /// I_PropertiesListener is NOT managed by a factory (should it be?)
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_PropertiesListener> 
    :   public boost::false_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROPERTIES_LISTENER_HPP_INCLUDED
