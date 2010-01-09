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
#ifndef ZENSTUDIO_WORKBENCH_ROW_HEADER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_ROW_HEADER_HPP_INCLUDED

#include <Zen/Enterprise/Database/I_Persistable.hpp>

#include <boost/cstdint.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Spread Sheet Row Header.
class RowHeader
:   public Database::I_Persistable
{
    /// @name I_Persistable implementation
    /// @{
public:
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name RowHeader implementation
    /// @{
public:
    const std::string& getLabel() const;
    void setLabel(const std::string& _label);

    /// Get the primary key.
    const boost::uint64_t getSpreadSheetRowId() const;

    /// Set the primary key.
    /// Call this before load() or remove().  insert() will automatically
    /// update this value.  save() can only be called after load() or insert()
    /// has been called.
    void setSpreadSheetRowId(const boost::uint64_t _spreadSheetRowId);

    /// Get the column number.
    /// @return -1 if the column number hasn't been assigned yet,
    ///     otherwise it's a 0 based column number.  This number
    ///     indicates the order of the column within the document.
    const int getRowNumber();

    void setRowNumber(int _columnNumber);

    void setDirty(bool _dirty);
    /// @}

    /// @name 'Structors
    /// @{
public:
    RowHeader(boost::uint64_t _spreadSheetDocumentId, const std::string& _label);
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Parent document's primary key.
    boost::uint64_t     m_spreadSheetDocumentId;
    std::string         m_label;
    boost::uint64_t     m_spreadSheetRowId;
    int                 m_columnNumber;
    bool                m_isDirty;
    /// @}

};  // class RowHeader

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_ROW_HEADER_HPP_INCLUDED
