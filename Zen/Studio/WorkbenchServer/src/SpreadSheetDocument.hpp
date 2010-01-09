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
#ifndef ZENSTUDIO_WORKBENCH_SPREAD_SHEET_DOCUMENT_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_SPREAD_SHEET_DOCUMENT_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocument.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCell.hpp>

#include <Zen/Studio/WorkbenchModel/I_SpreadSheetColumnDataCollection.hpp>

#include "SpreadSheetController.hpp"

#include <string>
#include <vector>
#include <set>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class WorkbenchService;
class ColumnHeader;
class RowHeader;
class I_SpreadSheetCellDomainObject;

/// Spread Sheet Document.
/// @todo TR - Do I need to have an implemeentation for a Document class?
class SpreadSheetDocument
:   public I_SpreadSheetDocument
{
    /// @name Types
    /// @{
public:
    typedef std::vector<I_SpreadSheetCell*>                     Row_type;
    typedef std::vector<Row_type>                               Model_type;

    typedef ColumnHeader*                                       pColumnHeader_type;
    typedef std::vector<pColumnHeader_type>                     ColumnHeaders_type;
    typedef std::map<boost::uint64_t,pColumnHeader_type>        IndexedColumnHeaders_type;

    typedef RowHeader*                                          pRowHeader_type;
    typedef std::vector<pRowHeader_type>                        RowHeaders_type;
    typedef std::map<boost::uint64_t,pRowHeader_type>           IndexedRowHeaders_type;

    /// Set of weak pointers to views.
    typedef std::set<pSpreadSheetView_type>                     ExplorerViews_type;

    typedef I_ExplorerNode*                                             pExplorerNode_type;
    typedef I_ExplorerNode::ExplorerNodeEvent_type::pConnection_type    pNodeEventConnection_type;
    /// @}

    /// @name I_Document implementation
    /// @todo Should this be moved to a Document class?
    /// @{
public:
    virtual const std::string& getShortName() const;
    virtual const std::string& getLongName() const;
    virtual I_Project* getProject() const;
    /// @}

    /// @name I_SpreadSheetViewable implementation
    /// @{
public:
    virtual I_SpreadSheetController& getSpreadSheetController();
    virtual I_SpreadSheetProperties& getProperties();
    virtual I_SpreadSheetCellProperties& getCellProperties(const int _column, const int _row);
    /// @}

    /// @name I_SpreadSheetDocument implementation
    /// @{
public:
    virtual pSubscription_type subscribe(pSpreadSheetView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual int getColumnCount();
    virtual int getRowCount();
    virtual I_SpreadSheetCell& getCell(const int _column, const int _row);
    virtual void getAllColumnMajor(I_CellVisitor& _visitor);
    virtual void getAllRowMajor(I_CellVisitor& _visitor);
    virtual const std::string& getColumnLabel(const int _columnNumber) const;
    virtual pUserData_type getUserData() const;
    /// @}

    /// @name Used by SpreadSheetController.
    /// @{
protected:
    friend class SpreadSheetController;
    void insertColumn(int _position, const std::string& _columnLabel);
    void insertRow(int _position, const std::string& _rowLabel);
    void setColumnLabelText(int _columnNumber, const std::string& _text);
    void setRowLabelText(int _rowNumber, const std::string& _text);
    /// @}

    /// @name Used by WorkbenchService.
    /// @{
protected:
    friend class WorkbenchService;
    /// Load (or create) a SpreadhSheet document using the node's
    /// nodeId as a unique identifier for the document.
    void load(pExplorerNode_type _pNode);

    /// Save this document.
    /// @note If we add a mutex to lock the document then assume that
    ///     it is locked before save() is called.
    void save();
    /// @}

    /// @name Internal implementation.
    /// Don't call these methods directly.  They're called by helper
    /// classes / functions in SpreadSheetDocument.cpp
    /// @{
public:
    /// Set the internal document id.
    void setSpreadSheetDocumentId(boost::uint64_t _documentId);

    /// Get the internal document Id.
    boost::uint64_t getSpreadSheetDocumentId() const;

    /// Push a column that was loaded from the database to the end 
    /// of the collection of columns.
    /// This doesn't fire any notification events; that will be done later.
    /// These must be loaded in order of the column number.
    void pushBackColumn(ColumnHeader* _pHeader);

    /// Push a row that was loaded from the database to the end 
    /// of the collection of rows.
    /// Don't fire any notification events; that will be done later.
    /// These must be loaded in order of the row number.
    void pushBackRow(RowHeader* _pHeader);

    /// Create a cell that was loaded from the database.
    void createCell(I_SpreadSheetCellDomainObject* _pCellDO);

    /// Unsubscribe a view from this document.
    void unSubscribe(pSpreadSheetView_type _pView);
    /// @}

    /// @name Helper Methods called from load().
    /// These methods were split out from the load() method due
    /// to the length, but they're only called from load().
    /// @{
private:
    /// Load the columns from the database.
    void loadColumns();
    /// Load the rows from the database.
    void loadRows();
    /// Load the cells from the database.
    void loadCells();
    /// @}

    /// @name Event Handlers
    /// @{
public:
    void handleExplorerNodeModified(I_ExplorerNode* _pNode);
    /// @}

    /// @name 'Structors
    /// @{
public:
    /// @param _pProject Project which contains this document.
    explicit SpreadSheetDocument(I_Project* _pProject, pUserData_type _pUserData);
    virtual ~SpreadSheetDocument();
    /// @}

    /// @name Inner Types
    /// @{
public:
    class Cell
    :   public I_SpreadSheetCell
    {
    public:
        virtual void setTextValue(const std::string& _value);
        virtual const std::string& getTextValue() const;
        virtual bool getMutable() const;
        virtual pUserData_type getUserData();
    public:
                 Cell(pUserData_type _pUserData);
        virtual ~Cell();
    private:
        pUserData_type  m_pUserData;
        std::string     m_textValue;
        bool            m_isMutable;
    };  // class Cell
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// The containing project, or NULL if this document is not contained
    /// by a project.
    I_Project*                  m_pProject;
    /// The I_SpreadSheetDocument::UserData associated with this document.
    pUserData_type              m_pUserData;
    /// The SpreadSheetController that controls this model.
    /// i.e. m_controller is the public write-access to this document.
    SpreadSheetController       m_controller;
    pExplorerNode_type          m_pNode;
    pNodeEventConnection_type   m_pNodeConnection;

    boost::uint64_t             m_spreadSheetDocumentId;
    /// true if this document has been loaded from the database.
    bool                        m_loaded;

    std::string                 m_shortName;
    std::string                 m_longName;

    /// The model for the document.
    /// This is a vector of vectors of I_Cell pointers.
    Model_type                  m_model;

    /// Column Headers.
    ColumnHeaders_type          m_columnHeader;
    IndexedColumnHeaders_type   m_indexedColumnHeaders;

    /// Row Headers.
    RowHeaders_type             m_rowHeader;
    IndexedRowHeaders_type      m_indexedRowHeaders;

    /// Views subscribed to this model.
    ExplorerViews_type          m_views;

    /// Guard for m_views.
    Threading::I_Mutex*         m_pViewsGuard;
    /// @}

};  // class SpreadSheetDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_SPREAD_SHEET_DOCUMENT_HPP_INCLUDED
