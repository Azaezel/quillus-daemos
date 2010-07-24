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
#include "ColumnHeader.hpp"

#include <Zen/Studio/WorkbenchModel/I_SpreadSheetColumnDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_SpreadSheetColumnDomainObject.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ColumnHeader::ColumnHeader(boost::uint64_t _spreadSheetDocumentId, const std::string& _label)
:   m_spreadSheetDocumentId(_spreadSheetDocumentId)
,   m_label(_label)
,   m_spreadSheetColumnId(0)
,   m_columnNumber(-1)
,   m_isDirty(true)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ColumnHeader::insert(pDatabaseConnection_type _pDBConn)
{
    I_SpreadSheetColumnDataMap::pSpreadSheetColumnDataMap_type
        pColumnDM = I_SpreadSheetColumnDataMap::create(_pDBConn);

    I_SpreadSheetColumnDataMap::pSpreadSheetColumnDomainObject_type
        pColumnDO = pColumnDM->createNew();

    pColumnDO->getColumnNumber() = m_columnNumber;
    pColumnDO->getSpreadSheetDocumentId() = m_spreadSheetDocumentId;
    pColumnDO->getColumnText() = m_label;

    m_spreadSheetColumnId = pColumnDM->update(pColumnDO)->getValue();
    m_isDirty = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ColumnHeader::load(pDatabaseConnection_type _pDBConn)
{
    I_SpreadSheetColumnDataMap::pSpreadSheetColumnDataMap_type
        pColumnDM = I_SpreadSheetColumnDataMap::create(_pDBConn);

    I_SpreadSheetColumnDataMap::pSpreadSheetColumnDomainObject_type
        pColumnDO = pColumnDM->getByKey(m_spreadSheetColumnId)->getValue();

    m_spreadSheetDocumentId = pColumnDO->getSpreadSheetDocumentId();
    m_label = pColumnDO->getColumnText().getStringValue();
    m_spreadSheetColumnId = pColumnDO->getSpreadSheetColumnId();
    m_columnNumber = pColumnDO->getColumnNumber();
    m_isDirty = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ColumnHeader::save(pDatabaseConnection_type _pDBConn)
{
    // Don't do anything if the data isn't dirty
    if (!m_isDirty)
        return;

    // If the columnId is 0, the record hasn't been inserted yet,
    // so insert it.
    if (m_spreadSheetColumnId == 0)
    {
        insert(_pDBConn);
    }
    else
    {
        I_SpreadSheetColumnDataMap::pSpreadSheetColumnDataMap_type
            pColumnDM = I_SpreadSheetColumnDataMap::create(_pDBConn);

        // TODO This should be a read for update, not just a read.
        I_SpreadSheetColumnDataMap::pSpreadSheetColumnDomainObject_type
            pColumnDO = pColumnDM->getByKey(m_spreadSheetColumnId)->getValue();

        // For now just update the column number and label.
        pColumnDO->getColumnNumber() = m_columnNumber;
        pColumnDO->getColumnText() = m_label;

        pColumnDM->update(pColumnDO);

    }
    m_isDirty = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ColumnHeader::remove(pDatabaseConnection_type _pDBConn)
{
    throw Utility::runtime_exception("ColumnHeader::remove(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ColumnHeader::getLabel() const
{
    return m_label;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ColumnHeader::setLabel(const std::string& _label)
{
    m_isDirty = true;
    m_label = _label;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::uint64_t
ColumnHeader::getSpreadSheetColumnId() const
{
    return m_spreadSheetColumnId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ColumnHeader::setSpreadSheetColumnId(const boost::uint64_t _spreadSheetColumnId)
{
    m_spreadSheetColumnId = _spreadSheetColumnId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const int
ColumnHeader::getColumnNumber()
{
    return m_columnNumber;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ColumnHeader::setColumnNumber(int _columnNumber)
{
    if (m_columnNumber != _columnNumber)
    {
        m_columnNumber = _columnNumber;
        m_isDirty = true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ColumnHeader::setDirty(bool _dirty)
{
    m_isDirty = _dirty;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
