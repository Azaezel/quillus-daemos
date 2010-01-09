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
#ifndef ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_DOCUMENT_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_DOCUMENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/Database/I_Persistable.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetViewable.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCell.hpp>

#include <string>

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
class I_SpreadSheetController;
class I_SpreadSheetView;
class I_SpreadSheetDocumentType;
class I_SpreadSheetContributor;
class I_SpreadSheetAction;

class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetDocument
:   public virtual I_Document
,   public I_SpreadSheetViewable
{
    /// @name Types
    /// @{
public:

    /// User Data for a I_SpreadSheetDocument.
    /// This UserData class is data that is associated with an I_SpreadSheetDocument
    /// instance.  This is what the application developer uses in order to associate
    /// spreadsheet documents with application data.
    class WORKBENCHCOMMON_DLL_LINK UserData
    :   public Database::I_Persistable
    {
    public:
        /// The framework will call setDocument() when user data is associated with
        /// a I_SpreadSheetDocument.
        virtual void setDocument(I_SpreadSheetDocument* _pDocument) { m_pDocument = _pDocument; }
        virtual I_SpreadSheetDocument* getDocument() const { return m_pDocument; }

        /// Implement this method.
        /// Your extension to UserData should return the type of spreadsheet document
        /// this data belongs.
        virtual I_SpreadSheetDocumentType& getType() const = 0;

        /// Create a Cell User Data.
        /// Create a Cell User Data for the specified column and row.
        /// This is called by the framework when a new empty cell is being created.
        /// Non empty cells loaded from the database are constructed using an extension point.
        /// @see I_WorkbenchService::
        virtual I_SpreadSheetCell::pUserData_type createEmptyCellData(const int _column, const int _row) = 0;
    public:
        UserData()
        : m_pDocument(NULL)
        {
        }
        virtual ~UserData() {}
    private:
        I_SpreadSheetDocument*     m_pDocument;
    };  // class UserData

    typedef Memory::managed_ptr<UserData>               pUserData_type;

    class I_Column;
    class I_Row;
    struct I_CellVisitor;

    typedef Memory::managed_ptr<I_Document>                             pDocument_type;
    typedef Memory::managed_ptr<I_SpreadSheetDocument>                  pSpreadSheetDocument_type;

    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Filter>          pFilter_type;
    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Subscription>    pSubscription_type;
    /// @}

    /// @name I_SpreadSheetDocument interface
    /// @{
public:
    virtual int getColumnCount() = 0;

    virtual int getRowCount() = 0;

    /// Get a cell at a particular location.
    /// Do not retain a reference to this cell.
    virtual I_SpreadSheetCell& getCell(const int _column, const int _row) = 0;

    /// Get all cells with the cells grouped in columns.
    /// I_CellVisitor::nextRowColumn() is called before each column
    /// and then I_CellVisitor::visit() is called for each row in the 
    /// column.
    virtual void getAllColumnMajor(I_CellVisitor& _visitor) = 0;

    /// Get all cells with the cells grouped in rows.
    /// I_CellVisitor::nextRowColumn() is called before each row
    /// and then I_CellVisitor::visit() is called for each column in the 
    /// row.
    virtual void getAllRowMajor(I_CellVisitor& _visitor) = 0;

    /// Get the column label for the specified column.
    /// @param _columnNumber 0 based column number.  Must be <= getColumnCount().
    virtual const std::string& getColumnLabel(const int _columnNumber) const = 0;

    /// Get the user data associated with this spread sheet document.
    /// If you need to get the type of the this spread sheet document, use
    /// getUserData()->getType().
    virtual pUserData_type getUserData() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SpreadSheetDocument();
    virtual ~I_SpreadSheetDocument();
    /// @}

    /// @name Inner Types
    /// @{
public:
    struct I_CellVisitor
    {
        virtual void begin() = 0;
        /// This is called before a new row or column is started.
        /// "row" vs "column" depends on which 
        virtual void nextRowColumn() = 0;
        /// Empty cells may be NULL.
        virtual void visit(I_SpreadSheetCell* _pCell) = 0;
        virtual void end() = 0;
    };  // struct I_CellVisitor
    /// @}

};  // interface I_SpreadSheetDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
	/// I_SpreadSheetDocument is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_SpreadSheetDocument> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_DOCUMENT_HPP_INCLUDED
