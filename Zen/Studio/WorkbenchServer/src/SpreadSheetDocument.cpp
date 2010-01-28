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
#include "SpreadSheetDocument.hpp"
#include "SpreadSheetCell.hpp"
#include "ColumnHeader.hpp"
#include "RowHeader.hpp"

#include "WorkbenchService.hpp"

#include <Zen/Studio/WorkbenchCommon/subscription.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetView.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>

#include <Zen/Studio/WorkbenchModel/I_SpreadSheetDocumentDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_SpreadSheetDocumentDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_SpreadSheetDocumentDataCollection.hpp>

#include <Zen/Studio/WorkbenchModel/I_SpreadSheetColumnDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_SpreadSheetColumnDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_SpreadSheetColumnDataCollection.hpp>

#include <Zen/Studio/WorkbenchModel/I_SpreadSheetRowDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_SpreadSheetRowDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_SpreadSheetRowDataCollection.hpp>

#include <Zen/Studio/WorkbenchModel/I_SpreadSheetCellDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_SpreadSheetCellDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_SpreadSheetCellDataCollection.hpp>


#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SpreadSheetDocument::SpreadSheetDocument(I_Project* _pProject, pUserData_type _pUserData)
:   m_pProject(_pProject)
,   m_pUserData(_pUserData)
,   m_controller(*this)
,   m_pNode(NULL)
,   m_spreadSheetDocumentId(0)
,   m_pViewsGuard(Threading::MutexFactory::create())
,   m_loaded(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SpreadSheetDocument::~SpreadSheetDocument()
{
    Threading::MutexFactory::destroy(m_pViewsGuard);

    // TODO Clean up m_model and m_header; for now we're leaking memory here.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
SpreadSheetDocument::getShortName() const
{
    return m_pNode->getDisplayName();
    //return m_shortName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
SpreadSheetDocument::getLongName() const
{
    // TODO Create a fully qualified name, including project, parents, etc?
    return m_pNode->getDisplayName();
    //return m_longName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Project*
SpreadSheetDocument::getProject() const
{
    return m_pProject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef subscription<SpreadSheetDocument*, SpreadSheetDocument::pSpreadSheetView_type> Subscription;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    Subscription* pSubscription = dynamic_cast<Subscription*>(_wpSubscription.get());

    if (pSubscription)
    {
        delete pSubscription;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SpreadSheetDocument::pSubscription_type
SpreadSheetDocument::subscribe(pSpreadSheetView_type _pView, pFilter_type _pFilter)
{
    // Cannot subscribe unless the document has already been loaded.
    assert(m_loaded);

    Threading::CriticalSection lock(m_pViewsGuard);

    m_views.insert(_pView);

    // First, notify of the columns
    for(ColumnHeaders_type::iterator column = m_columnHeader.begin(); column != m_columnHeader.end(); column++)
    {
        _pView->onNewColumn(-1, (*column)->getLabel());
    }

    // Then notify of rows
    for (RowHeaders_type::iterator row = m_rowHeader.begin(); row != m_rowHeader.end(); row++)
    {
        // TODO Row label?
        _pView->onNewRow(-1, (*row)->getLabel());
    }

    // TODO Should the long name should be fully qualified with parents, etc?
    _pView->onNameModified(m_pNode->getDisplayName(), m_pNode->getDisplayName());

    // TODO Notify of all the cells.

    return pSubscription_type(new Subscription(this, _pView), &destroySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::unSubscribe(pSpreadSheetView_type _pView)
{
    Threading::CriticalSection lock(m_pViewsGuard);
    m_views.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
SpreadSheetDocument::getColumnCount()
{
    return (int)m_columnHeader.size();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
SpreadSheetDocument::getRowCount()
{
    return (int)m_rowHeader.size();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SpreadSheetCell&
SpreadSheetDocument::getCell(const int _column, const int _row)
{
    // TODO Verify nothing is out of range?

    // See if the cell exists.
    I_SpreadSheetCell* pCell = m_model[_row][_column];

    // If the cell hasn't been created yet, create it.
    if (pCell == NULL)
    {
        pCell = new Cell(m_pUserData->createEmptyCellData(_column, _row));
        pCell->getUserData()->setCell(pCell);

        m_model[_row][_column] = pCell;
    }

    return *pCell;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::getAllColumnMajor(I_CellVisitor& _visitor)
{
    _visitor.begin();

    // Outer loops is the column
    for(int column = 0; column < getColumnCount(); column++)
    {
        _visitor.nextRowColumn();

        // The inner loop is the row
        for(Model_type::iterator iter = m_model.begin(); iter != m_model.end(); iter++)
        {
            _visitor.visit((*iter)[column]);
        }
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::getAllRowMajor(I_CellVisitor& _visitor)
{
    _visitor.begin();

    // Outer loops is the row
    for(Model_type::iterator row = m_model.begin(); row != m_model.end(); row++)
    {
        _visitor.nextRowColumn();

        // The inner loop is the column
        for(Row_type::iterator column = row->begin(); column != row->end(); column++)
        {
            _visitor.visit(*column);
        }
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
SpreadSheetDocument::getColumnLabel(const int _columnNumber) const
{
    // TODO Validate that _columnNumber is not out of bounds.
    return m_columnHeader[_columnNumber]->getLabel();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SpreadSheetController&
SpreadSheetDocument::getSpreadSheetController()
{
    return m_controller;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SpreadSheetProperties&
SpreadSheetDocument::getProperties()
{
    throw Zen::Utility::runtime_exception("SpreadSheetDocument::getProperties(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SpreadSheetCellProperties&
SpreadSheetDocument::getCellProperties(const int _column, const int _row)
{
    throw Zen::Utility::runtime_exception("SpreadSheetDocument::getCellProperties(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SpreadSheetDocument::pUserData_type
SpreadSheetDocument::getUserData() const
{
    return m_pUserData;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::insertColumn(int _position, const std::string& _columnLabel)
{
    // Validate m_spreadSheetDocumentId
    assert(m_spreadSheetDocumentId != 0);

    // TODO Guard this document
    {
        // Create the column header and insert it in m_columnHeader.
        ColumnHeader* pHeader  = new ColumnHeader(m_spreadSheetDocumentId, _columnLabel);

        if (_position < 0)
        {
            // Push a new column on the end
            m_columnHeader.push_back(pHeader);

            // Iterate through all of the rows and insert a new column
            for(Model_type::iterator row = m_model.begin(); row != m_model.end(); row++)
            {
                row->push_back(NULL);
            }
        }
        else
        {
            // Insert a new column in the header
            int x = _position;
            for(ColumnHeaders_type::iterator iter = m_columnHeader.begin(); iter != m_columnHeader.end(); iter++)
            {
                if (--x < 0)
                {
                    m_columnHeader.insert(iter, pHeader);
                    break;
                }
            }

            // Iterate through all of the rows and insert a new column
            for(Model_type::iterator row = m_model.begin(); row != m_model.end(); row++)
            {
                x = _position;
                for(Row_type::iterator iter = row->begin(); iter != row->end(); iter++)
                {
                    if (--x < 0)
                    {
                        row->insert(iter, NULL);
                        break;
                    }
                }
            }
        }

        save();
    }

    // Notify subscriptions of the new column
    // TODO Should this be inside of the document critical section?
    // Be careful of deadlocks.  Always lock the document then the view?
    {
        Threading::CriticalSection lock(m_pViewsGuard);

        for(ExplorerViews_type::iterator view = m_views.begin(); view != m_views.end(); view++)
        {
            (*view)->onNewColumn(_position, _columnLabel);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::insertRow(int _position, const std::string& _rowLabel)
{
    // Validate m_spreadSheetDocumentId
    assert(m_spreadSheetDocumentId != 0);

    // TODO Guard this document
    {
        RowHeader* pHeader = new RowHeader(m_spreadSheetDocumentId, _rowLabel);

        // First, create an empty row
        Row_type row;

        for(int x = 0; x < getColumnCount(); x++)
        {
            row.push_back(NULL);
        }

        if (_position < 0)
        {
            // Append a row on the end
            m_model.push_back(row);

            // Push a new row header on the end
            m_rowHeader.push_back(pHeader);
        }
        else
        {
            // Insert a new row in the header
            int x = _position;
            for(RowHeaders_type::iterator iter = m_rowHeader.begin(); iter != m_rowHeader.end(); iter++)
            {
                if (--x < 0)
                {
                    m_rowHeader.insert(iter, pHeader);
                    break;
                }
            }

            // Iterate to find the _position
            x = _position;
            for(Model_type::iterator iter = m_model.begin(); iter != m_model.end(); iter++)
            {
                if (--x == 0)
                {
                    // Insert the row
                    m_model.insert(iter, row);
                    break;
                }
            }
        }

        save();
    }

    // TODO Should this be inside of the document critical section?
    // Be careful of deadlocks.  Always lock the document then the view?
    {
        Threading::CriticalSection lock(m_pViewsGuard);

        for(ExplorerViews_type::iterator view = m_views.begin(); view != m_views.end(); view++)
        {
            (*view)->onNewRow(_position, _rowLabel);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::setColumnLabelText(int _columnNumber, const std::string& _text)
{
    // TODO validate _columnNumber
    m_columnHeader[_columnNumber]->setLabel(_text);

    // TODO notify subscriptions
    throw Zen::Utility::runtime_exception("SpreadSheetDocument::insertColumn(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::setRowLabelText(int _rowNumber, const std::string& _text)
{
    // TODO validate _columnNumber
    m_rowHeader[_rowNumber]->setLabel(_text);

    // TODO notify subscriptions
    throw Utility::runtime_exception("SpreadSheetDocument::setRowLabelText(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::load(pExplorerNode_type _pNode)
{
    // If this document is not part of a project, it cannot be loaded from a database.
    if (m_pProject == NULL)
    {
        return;
    }

    m_pNode = _pNode;

    // Subscribe to this node's onModified event so we can be notified in particular about
    // name changes.
    m_pNodeConnection = m_pNode->onModified.connect(boost::bind(&SpreadSheetDocument::handleExplorerNodeModified, this, _1));

    // Get the data map.
    I_SpreadSheetDocumentDataMap::pSpreadSheetDocumentDataMap_type 
        pDocDM = I_SpreadSheetDocumentDataMap::create(m_pProject->getDatabaseConnection());

    // Get the documents by explorer node.  There should be one or zero.
    I_SpreadSheetDocumentDataMap::pFutureSpreadSheetDocumentDataCollection_type 
        pDocDC = pDocDM->getByExplorerNodeId(_pNode->getNodeId());

    // If the collection is empty, this is a new document
    if (pDocDC->getValue()->isEmpty())
    {
        I_SpreadSheetDocumentDataMap::pSpreadSheetDocumentDomainObject_type 
            pDocDO = pDocDM->createNew();

        pDocDO->setExplorerNodeId(_pNode->getNodeId());

        Zen::Database::I_DatabaseTransaction::pFutureKey_type 
            pKey = pDocDM->update(pDocDO);

        setSpreadSheetDocumentId(pKey->getValue());
    }
    else
    {
        // This visitor loads the documents by explorerNodeId.
        // Again, there should be either one or none.
        struct DocCollectionVisitor
        :   public I_SpreadSheetDocumentDataCollection::I_CollectionVisitor
        {
            virtual void begin()
            {
            }

            virtual void visit(I_SpreadSheetDocumentDataMap::pSpreadSheetDocumentDomainObject_type _pDomainObject)
            {
                m_document.setSpreadSheetDocumentId(_pDomainObject->getSpreadSheetDocumentId());

                // TODO Throw an exception if more than one?
            }

            virtual void end()
            {
            }

            DocCollectionVisitor(SpreadSheetDocument& _document)
            :   m_document(_document)
            {
            }

        private:
            SpreadSheetDocument&        m_document;
        };

        DocCollectionVisitor docVisitor(*this);
        pDocDC->getValue()->getAll(docVisitor);

        // Load the columns, rows and cells for this document.
        // These methods do not fire events.
        loadColumns();
        loadRows();
        loadCells();
    }

    // Mark the document as loaded.
    m_loaded = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::loadColumns()
{
    // If this document is not part of a project, it cannot be loaded from a database.
    if (m_pProject == NULL)
    {
        return;
    }

    // This visitor loads all of the columns for this documentId.
    struct ColumnCollectionVisitor
    :   public I_SpreadSheetColumnDataCollection::I_CollectionVisitor
    {
        typedef std::map<int, ColumnHeader*>    Columns_type;


        virtual void begin()
        {
        }

        virtual void visit(I_SpreadSheetColumnDataCollection::pSpreadSheetColumnDomainObject_type _pDomainObject)
        {
            // Create a column header
            ColumnHeader* pColumnHeader =
                new ColumnHeader(m_document.getSpreadSheetDocumentId(), _pDomainObject->getColumnText());

            // Populate the column header based on the DO.
            pColumnHeader->setSpreadSheetColumnId(_pDomainObject->getSpreadSheetColumnId());
            pColumnHeader->setColumnNumber(_pDomainObject->getColumnNumber());

            // Store the column using the column number as an index.
            // We do this so that we can guarantee the order of the columns.
            // This is possibly unnecessary since I believe the DM has an ORDER BY
            // clause, but lets do it anyway so that we're 100% clear on what's 
            // happening and how.
            m_columns[_pDomainObject->getColumnNumber()] = pColumnHeader;

            // Set the header not-dirty since we just loaded it from the database.
            pColumnHeader->setDirty(false);
        }

        virtual void end()
        {
            // After the columns have been loaded into the m_columns map, pass it
            // to m_document.  The map<> is used to make the columns order by columnNumber.
            for(Columns_type::iterator iter = m_columns.begin(); iter != m_columns.end(); iter++)
            {
                m_document.pushBackColumn(iter->second);
            }
        }

        ColumnCollectionVisitor(SpreadSheetDocument& _document)
        :   m_document(_document)
        {
        }

    private:
        Columns_type                m_columns;
        SpreadSheetDocument&        m_document;
    };

    // Get the columns
    I_SpreadSheetColumnDataMap::pSpreadSheetColumnDataMap_type
        pColumnDM = I_SpreadSheetColumnDataMap::create(m_pProject->getDatabaseConnection());

    I_SpreadSheetColumnDataMap::pSpreadSheetColumnDataCollection_type
        pColumnDC = pColumnDM->getByDocumentId(m_spreadSheetDocumentId)->getValue();

    ColumnCollectionVisitor colVisitor(*this);
    pColumnDC->getAll(colVisitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::loadRows()
{
    // If this document is not part of a project, it cannot be loaded from a database.
    if (m_pProject == NULL)
    {
        return;
    }

    // load the rows.
    // This visitor loads all of the rows for this documentId.
    struct RowCollectionVisitor
    :   public I_SpreadSheetRowDataCollection::I_CollectionVisitor
    {
        typedef std::map<int, RowHeader*>    Rows_type;


        virtual void begin()
        {
        }

        virtual void visit(I_SpreadSheetRowDataCollection::pSpreadSheetRowDomainObject_type _pDomainObject)
        {
            // Create a row header
            RowHeader* pRowHeader =
                new RowHeader(m_document.getSpreadSheetDocumentId(), _pDomainObject->getRowText());

            // Populate the row header based on the DO.
            pRowHeader->setSpreadSheetRowId(_pDomainObject->getSpreadSheetRowId());
            pRowHeader->setRowNumber(_pDomainObject->getRowNumber());

            // Store the row using the row number as an index.
            // We do this so that we can guarantee the order of the rows.
            // This is possibly unnecessary since I believe the DM has an ORDER BY
            // clause, but lets do it anyway so that we're 100% clear on what's 
            // happening and how.
            m_rows[_pDomainObject->getRowNumber()] = pRowHeader;

            // Set the header not-dirty since we just loaded it from the database.
            pRowHeader->setDirty(false);
        }

        virtual void end()
        {
            // After the rows have been loaded into the m_rows map, pass it
            // to m_document.  The map<> is used to make the rows order by rowNumber.
            for(Rows_type::iterator iter = m_rows.begin(); iter != m_rows.end(); iter++)
            {
                m_document.pushBackRow(iter->second);
            }
        }

        RowCollectionVisitor(SpreadSheetDocument& _document)
        :   m_document(_document)
        {
        }

    private:
        Rows_type                   m_rows;
        SpreadSheetDocument&        m_document;
    };

    // Get the rows
    I_SpreadSheetRowDataMap::pSpreadSheetRowDataMap_type
        pRowDM = I_SpreadSheetRowDataMap::create(m_pProject->getDatabaseConnection());

    I_SpreadSheetRowDataMap::pSpreadSheetRowDataCollection_type
        pRowDC = pRowDM->getByDocumentId(m_spreadSheetDocumentId)->getValue();

    RowCollectionVisitor rowVisitor(*this);
    pRowDC->getAll(rowVisitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::loadCells()
{
    // If this document is not part of a project, it cannot be loaded from a database.
    if (m_pProject == NULL)
    {
        return;
    }

    // Load the Cells.
    // This visitor loads all of the rows for this documentId.
    struct CellCollectionVisitor
    :   public I_SpreadSheetCellDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(I_SpreadSheetCellDataCollection::pSpreadSheetCellDomainObject_type _pDomainObject)
        {
            m_document.createCell(_pDomainObject.get());
        }

        virtual void end()
        {
        }

        CellCollectionVisitor(SpreadSheetDocument& _document)
        :   m_document(_document)
        {
        }

    private:
        SpreadSheetDocument&        m_document;
    };

    // Get the Cells
    I_SpreadSheetCellDataMap::pSpreadSheetCellDataMap_type
        pCellDM = I_SpreadSheetCellDataMap::create(m_pProject->getDatabaseConnection());

    I_SpreadSheetCellDataMap::pSpreadSheetCellDataCollection_type
        pCellDC = pCellDM->getByDocumentId(m_spreadSheetDocumentId)->getValue();

    CellCollectionVisitor cellVisitor(*this);
    pCellDC->getAll(cellVisitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::pushBackColumn(ColumnHeader* _pHeader)
{
    // Make sure the column id is valid.
    assert(_pHeader->getSpreadSheetColumnId() != 0);

    // Keep two references, one in m_columnHeader and another that's indexed by 
    // getSpreadSheetColumnId.
    m_indexedColumnHeaders[_pHeader->getSpreadSheetColumnId()] = _pHeader;
    m_columnHeader.push_back(_pHeader);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::pushBackRow(RowHeader* _pHeader)
{
    // Keep two references, one in m_rowHeader and another that's indexed by 
    // getSpreadSheetRowId.

    assert(_pHeader->getSpreadSheetRowId() != 0);

    m_indexedRowHeaders[_pHeader->getSpreadSheetRowId()] = _pHeader;
    m_rowHeader.push_back(_pHeader);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::createCell(I_SpreadSheetCellDomainObject* _pCellDO)
{
    // If this document is not part of a project, it cannot be loaded from a database.
    if (m_pProject == NULL)
    {
        return;
    }

    IndexedRowHeaders_type::iterator pRowHeader = m_indexedRowHeaders.find(_pCellDO->getSpreadSheetRowId());
    assert(pRowHeader != m_indexedRowHeaders.end());

    IndexedColumnHeaders_type::iterator pColumnHeader = m_indexedColumnHeaders.find(_pCellDO->getSpreadSheetColumnId());
    assert(pColumnHeader != m_indexedColumnHeaders.end());

    const int rowNumber = pRowHeader->second->getRowNumber();
    const int columnNumber = pColumnHeader->second->getColumnNumber();

    if (m_model[rowNumber][columnNumber] == NULL)
    {
        // TODO Why not just go directly to this document's userdata?  If it cannot
        // create the cell then the user data could call the workbench.
        // Create the appropriate SpreadSheetCell user data.
        I_SpreadSheetCell::pUserData_type 
            pUserData = m_pProject->getWorkbenchService().createSpreadSheetCellUserData(*this, _pCellDO->getSpreadSheetCellId(), _pCellDO->getValueType(), rowNumber, columnNumber);

        // Create a SpreadSheetCell
        m_model[rowNumber][columnNumber] = new SpreadSheetCell(*this, pUserData);
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::save()
{
    // If this document is not part of a project, it cannot be loaded from a database.
    if (m_pProject == NULL)
    {
        return;
    }

    // Don't lock document.  It must be locked before calling this method.

    // Renumber all of the columns and save them.
    int columnNumber = 0;
    for(ColumnHeaders_type::iterator ppColumn = m_columnHeader.begin(); ppColumn != m_columnHeader.end(); ppColumn++)
    {
        // Update the column number
        (*ppColumn)->setColumnNumber(columnNumber);

        // Save the column
        (*ppColumn)->save(m_pProject->getDatabaseConnection());

        // Increment to the next column number
        columnNumber++;
    }

    // Renumber all of the rows and save them.
    int rowNumber = 0;
    for(RowHeaders_type::iterator ppRow = m_rowHeader.begin(); ppRow != m_rowHeader.end(); ppRow++)
    {
        // Update the row number
        (*ppRow)->setRowNumber(rowNumber);

        // Save the row
        (*ppRow)->save(m_pProject->getDatabaseConnection());

        // Increment to the next row number
        rowNumber++;
    }

    // TODO Save the rest of the document.

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::setSpreadSheetDocumentId(boost::uint64_t _documentId)
{
    m_spreadSheetDocumentId = _documentId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
SpreadSheetDocument::getSpreadSheetDocumentId() const
{
    return m_spreadSheetDocumentId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::handleExplorerNodeModified(I_ExplorerNode* _pNode)
{
    // Notifiy the subscriptions of the change

    Threading::CriticalSection lock(m_pViewsGuard);

    for(ExplorerViews_type::iterator view = m_views.begin(); view != m_views.end(); view++)
    {
        // TODO Long name and short name should probably be different.
        (*view)->onNameModified(_pNode->getDisplayName(), _pNode->getDisplayName());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SpreadSheetDocument::Cell::Cell(pUserData_type _pUserData)
:   m_pUserData(_pUserData)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SpreadSheetDocument::Cell::~Cell()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetDocument::Cell::setTextValue(const std::string& _value)
{
    m_textValue = _value;

    // TODO notify subscriptions
    throw Zen::Utility::runtime_exception("SpreadSheetDocument::setTextValue(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
SpreadSheetDocument::Cell::getTextValue() const
{
    return m_textValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
SpreadSheetDocument::Cell::getMutable() const
{
    return m_isMutable;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SpreadSheetCell::pUserData_type
SpreadSheetDocument::Cell::getUserData()
{
    return m_pUserData;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

