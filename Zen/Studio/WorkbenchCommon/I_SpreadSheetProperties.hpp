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
#ifndef ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_PROPERTIES_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_PROPERTIES_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_PropertiesPublisher.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Property;

/// Spread Sheet Properties interface.
/// This is a collection of properties associated with a spread sheet.
/// The lifetime of this object is the same as the model
/// from whence it came.  Any calls to I_PropertiesPublisher::subscribe()
/// will need to retain a strong reference to the model for as long
/// as the I_PropertiesPublisher::pSubscription_type remains in scope.
class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetProperties
:   public virtual I_PropertiesPublisher
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_SpreadSheetProperties interface
    /// @{
public:
    /// @return true if the cell properties change for each row.
    virtual bool cellPropertiesChangeEachRow() const = 0;

    /// @return true if the cell properties change for each column.
    virtual bool cellPropertiesChangeEachColumn() const = 0;

    /// Get a property by the fully qualified name of the property.
    /// @return The property, or NULL if the property wasn't found.
    virtual const I_Property* getPropertyByFullName(const std::string& _fullName) = 0;
    /// @}

    /// @name 'Structors
    /// @{
public:
             I_SpreadSheetProperties();
    virtual ~I_SpreadSheetProperties();
    /// @}

};  // interface I_PropertiesPublisher

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory
{
    /// I_SpreadSheetProperties is NOT managed by a factory (should it be?)
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_SpreadSheetProperties> : public boost::false_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_PROPERTIES_HPP_INCLUDED
