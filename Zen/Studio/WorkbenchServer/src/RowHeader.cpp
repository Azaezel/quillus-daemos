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
#include "RowHeader.hpp"

#include <Zen/Studio/WorkbenchModel/I_SpreadSheetRowDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_SpreadSheetRowDomainObject.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RowHeader::RowHeader(boost::uint64_t _spreadSheetDocumentId, const std::string& _label)
:   m_spreadSheetDocumentId(_spreadSheetDocumentId)
,   m_label(_label)
,   m_spreadSheetRowId(0)
,   m_columnNumber(-1)
,   m_isDirty(true)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RowHeader::insert(pDatabaseConnection_type _pDBConn)
{
    I_SpreadSheetRowDataMap::pSpreadSheetRowDataMap_type
        pRowDM = I_SpreadSheetRowDataMap::create(_pDBConn);

    I_SpreadSheetRowDataMap::pSpreadSheetRowDomainObject_type
        pRowDO = pRowDM->createNew();

    pRowDO->getRowNumber() = m_columnNumber;
    pRowDO->getSpreadSheetDocumentId() = m_spreadSheetDocumentId;
    pRowDO->getRowText() = m_label;

    m_spreadSheetRowId = pRowDM->update(pRowDO)->getValue();
    m_isDirty = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RowHeader::load(pDatabaseConnection_type _pDBConn)
{
    I_SpreadSheetRowDataMap::pSpreadSheetRowDataMap_type
        pRowDM = I_SpreadSheetRowDataMap::create(_pDBConn);

    I_SpreadSheetRowDataMap::pSpreadSheetRowDomainObject_type
        pRowDO = pRowDM->getByKey(m_spreadSheetRowId)->getValue();

    m_spreadSheetDocumentId = pRowDO->getSpreadSheetDocumentId();
    m_label = pRowDO->getRowText().getStringValue();
    m_spreadSheetRowId = pRowDO->getSpreadSheetRowId();
    m_columnNumber = pRowDO->getRowNumber();
    m_isDirty = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RowHeader::save(pDatabaseConnection_type _pDBConn)
{
    // Don't do anything if the data isn't dirty
    if (!m_isDirty)
        return;

    // If the columnId is 0, the record hasn't been inserted yet,
    // so insert it.
    if (m_spreadSheetRowId == 0)
    {
        insert(_pDBConn);
    }
    else
    {
        I_SpreadSheetRowDataMap::pSpreadSheetRowDataMap_type
            pRowDM = I_SpreadSheetRowDataMap::create(_pDBConn);

        // TODO This should be a read for update, not just a read.
        I_SpreadSheetRowDataMap::pSpreadSheetRowDomainObject_type
            pRowDO = pRowDM->getByKey(m_spreadSheetRowId)->getValue();

        // For now just update the column number and label.
        pRowDO->getRowNumber() = m_columnNumber;
        pRowDO->getRowText() = m_label;

        pRowDM->update(pRowDO);

    }
    m_isDirty = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RowHeader::remove(pDatabaseConnection_type _pDBConn)
{
    throw Utility::runtime_exception("RowHeader::remove(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
RowHeader::getLabel() const
{
    return m_label;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RowHeader::setLabel(const std::string& _label)
{
    m_isDirty = true;
    m_label = _label;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::uint64_t
RowHeader::getSpreadSheetRowId() const
{
    return m_spreadSheetRowId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RowHeader::setSpreadSheetRowId(const boost::uint64_t _spreadSheetRowId)
{
    m_spreadSheetRowId = _spreadSheetRowId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const int
RowHeader::getRowNumber()
{
    return m_columnNumber;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RowHeader::setRowNumber(int _columnNumber)
{
    if (m_columnNumber != _columnNumber)
    {
        m_columnNumber = _columnNumber;
        m_isDirty = true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RowHeader::setDirty(bool _dirty)
{
    m_isDirty = _dirty;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
