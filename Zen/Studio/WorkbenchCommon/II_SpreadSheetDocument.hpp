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
#ifndef ZENSTUDIO_WORKBENCH_II_SPREAD_SHEET_DOCUMENT_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_II_SPREAD_SHEET_DOCUMENT_HPP_INCLUDED

#include "I_SpreadSheetDocument.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Internal Spread Sheet Document.
/// This class is only used directly by either a client side or a server side
/// SpreadSheetController.  The "internal" version of this class and the
/// related inner classes are mutable.  The "external" version of this class
/// (I_SpreadSheetDocument) does not expose any mutator methods.  This
/// allows us to effectively assume that only controllers will modify the
/// model.  The Controllers are responsible for handling the document data
/// replication, either directly or through the use of views.
/// @note For Internal Use Only!
class WORKBENCHCOMMON_DLL_LINK II_SpreadSheetDocument
:   public I_SpreadSheetDocument
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name II_SpreadSheetDocument interface
    /// @{
public:
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             II_SpreadSheetDocument();
    virtual ~II_SpreadSheetDocument();
    /// @}

    /// @name Inner Types
    /// @{
public:
    class WORKBENCHCOMMON_DLL_LINK I_Column
    {
    protected:
                 I_Column();
        virtual ~I_Column();
    };  // interface I_Column

    /// A Row in a spreadsheet.
    class WORKBENCHCOMMON_DLL_LINK I_Row
    {
    protected:
                 II_Row();
        virtual ~II_Row();
    };  // interface II_Row

    /// A Cell in a spreadsheet.
    class WORKBENCHCOMMON_DLL_LINK II_Cell
    :   public I_Cell
    {
    protected:
        friend class SpreadSheetController;

        /// Get the text value of this cell.
        virtual void setTextValue(const std::string& _value) = 0;

        virtual void setMutable() = 0;
    protected:
                 II_Cell();
        virtual ~II_Cell();
    };  // interface II_Cell
    /// @}

};  // interface I_SpreadSheetDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_DOCUMENT_HPP_INCLUDED
