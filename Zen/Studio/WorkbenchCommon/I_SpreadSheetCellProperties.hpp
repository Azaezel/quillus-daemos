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
#ifndef ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_CELL_PROPERTIES_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_CELL_PROPERTIES_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Studio/WorkbenchCommon/I_PropertiesPublisher.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Spread Sheet Cell Properties interface.
/// The lifetime of this object is the same as the model
/// from whence it came.  Any calls to I_PropertiesPublisher::subscribe()
/// will need to retain a strong reference to the model for as long
/// as the I_PropertiesPublisher::pSubscription_type remains in scope.
class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetCellProperties
:   public virtual I_PropertiesPublisher
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_SpreadSheetCellProperties interface
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SpreadSheetCellProperties();
    virtual ~I_SpreadSheetCellProperties();
    /// @}

};  // interface I_SpreadSheetCellProperties

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory
{
    /// I_SpreadSheetCellProperties is NOT managed by a factory (should it be?)
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_SpreadSheetCellProperties> : public boost::false_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_CELL_PROPERTIES_HPP_INCLUDED
