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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_INDIRECT_SPREAD_SHEET_ACTION_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_INDIRECT_SPREAD_SHEET_ACTION_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetAction.hpp>

#include <boost/function.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Generic I_SpreadSheetAction template.
/// Use this template as a helper for creating spread sheet actions that
/// calls a method in a class that's not derived from I_SpreadSheetDocument::UserData type.
template<class DelegateClass_type>
class indirect_spread_sheet_action
:   public Zen::Studio::Workbench::I_SpreadSheetAction
{
    /// @name Types
    /// @{
public:
    typedef boost::function<void(Zen::Studio::Workbench::I_SpreadSheetView&, int, int)>  HandlerFunction_type;
    /// @}

    /// @name I_SpreadSheetAction implementation
    /// @{
public:
    virtual const std::string& getName() const
    {
        return m_name;
    }

    virtual const std::string& getType() const
    {
        return m_type;
    }

    virtual void invoke(I_SpreadSheetView& _view, const int _column, const int _row)
    {
        m_handler(_view, _column, _row);
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
    indirect_spread_sheet_action(const std::string& _name, const std::string& _type, HandlerFunction_type _handler)
    :   m_name(_name)
    ,   m_type(_type)
    ,   m_handler(_handler)
    {
    }

    virtual ~indirect_spread_sheet_action()
    {
    }
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string             m_name;
    std::string             m_type;
    HandlerFunction_type    m_handler;
    /// @}
};  // template indirect_spread_sheet_action

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_INDIRECT_SPREAD_SHEET_ACTION_HPP_INCLUDED
