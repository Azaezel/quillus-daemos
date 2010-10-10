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
#ifndef ZENSTUDIO_WORKBENCH_I_SCENE_PROPERTIES_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SCENE_PROPERTIES_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Studio/WorkbenchCommon/I_PropertiesPublisher.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Property;

/// Scene Properties interface
/// The lifetime of this object is the same as the model from whence
/// it came. Any calls to I_PropertiesPublisher::subscribe() will need
/// to retain a strong reference to the model for as long as the 
/// I_PropertiesPublisher::pSubscription_type remains in scope.
class WORKBENCHCOMMON_DLL_LINK I_SceneProperties
:   public virtual I_PropertiesPublisher
,   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_SceneProperties interface
    /// @{
public:
    /// Get a property by the fully qualified name of the property.
    /// @return The property, or NULL if the property wasn't found.
    virtual const I_Property* getPropertyByFullName(const std::string& _fullName) = 0;
    /// @}

    /// @name 'Structors
    /// @{
public:
             I_SceneProperties();
    virtual ~I_SceneProperties();
    /// @}

};  // interface I_SceneProperties

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
    /// I_SceneProperties is NOT managed by a factory (should it be?)
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_SceneProperties>
    :   public boost::false_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SCENE_PROPERTIES_HPP_INCLUDED
