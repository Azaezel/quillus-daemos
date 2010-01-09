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
#ifndef ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_CELL_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_CELL_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/Database/I_Persistable.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_SpreadSheetController;

class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetCell
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:

    class WORKBENCHCOMMON_DLL_LINK UserData
    :   public Database::I_Persistable
    {
    public:
        /// The framework will call setCell() with user data is associated
        /// with an I_SpreadSheetCell.
        virtual void setCell(I_SpreadSheetCell* _pCell) { m_pCell = _pCell; }
        virtual I_SpreadSheetCell* getCell() const { return m_pCell; }
        /// Return true if this user data can be modified.
        virtual bool getMutable() const = 0;

        /// Set the text value of this cell.
        virtual void setTextValue(const std::string& _value) { m_textValue = _value; }

        /// Get the text value of this cell.
        /// @todo TR - Should this be an I_DataElement?
        virtual const std::string& getTextValue() const { return m_textValue; }

    public:
                 UserData() {}
        virtual ~UserData() {}
    private:
        I_SpreadSheetCell*      m_pCell;
        std::string             m_textValue;
    }; // class UserData

    typedef Memory::managed_ptr<UserData>               pUserData_type;
    /// @}

    /// @name I_SpreadSheetCell interface
    /// @{
public:
    virtual pUserData_type getUserData() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SpreadSheetCell();
    virtual ~I_SpreadSheetCell();
    /// @}

};  // interface I_SpreadSheetCell

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_CELL_HPP_INCLUDED
