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
#ifndef ZENSTUDIO_WORKBENCH_I_PROPERTIES_PUBLISHER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROPERTIES_PUBLISHER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>
#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_PropertiesListener;
class I_PropertiesController;
class I_Property;

/// Properties Publisher interface.
class WORKBENCHCOMMON_DLL_LINK I_PropertiesPublisher
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef I_PropertiesListener*                                       pPropertiesListener_type;

    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Filter>          pFilter_type;
    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Subscription>    pSubscription_type;
    /// @}

    /// @name I_PropertiesPublisher interface
    /// @{
public:
    /// Get the name of this property set.
    virtual const std::string& getPropertiesName() const = 0;

    /// @return the subscription object.  You are responsible for making sure this
    ///     object goes out of scope before this publisher goes out of scope.
    virtual pSubscription_type subscribe(pPropertiesListener_type _pListener) = 0;

    /// Get the controller that can be used to modify a property.
    virtual I_PropertiesController& getController() = 0;

    virtual const I_Property* getPropertyByName(const std::string& _name) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PropertiesPublisher();
    virtual ~I_PropertiesPublisher();
    /// @}

};  // interface I_PropertiesPublisher

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory
{
    /// I_PropertiesPublisher is NOT managed by a factory (should it be?)
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_PropertiesPublisher> : public boost::false_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROPERTIES_PUBLISHER_HPP_INCLUDED
