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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_GENERIC_SPREADSHEET_DOCUMENT_TYPE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_GENERIC_SPREADSHEET_DOCUMENT_TYPE_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocumentType.hpp>

#include <boost/bind.hpp>

#include <string>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Generic I_SpreadSheetDocumentType template.
/// Use this template as a helper for creating spread sheet document types.
/// TR: Why a template?  Dunno....
template<class BaseType_type = void>
class generic_spreadsheet_document_type
:   public Zen::Studio::Workbench::I_SpreadSheetDocumentType
{
    /// @name Types
    /// @{
public:
    typedef std::set<I_SpreadSheetContributor*>         SpreadSheetContributors_type;
    /// @}

    /// @name I_ExplorerNodeType partial implementation
    /// @{
public:
    virtual void getContributors(I_SpreadSheetView& _view, I_SpreadSheetContributorVisitor& _visitor);
    virtual void addContributor(I_SpreadSheetContributor& _contributor);
    virtual void removeContributor(I_SpreadSheetContributor& _contributor);
    /// @}

    /// @name 'Structors
    /// @{
public:
             generic_spreadsheet_document_type();
    virtual ~generic_spreadsheet_document_type();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    SpreadSheetContributors_type    m_contributors;
    /// @}

};  // template generic_explorer_node_type

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
generic_spreadsheet_document_type<BaseType_type>::generic_spreadsheet_document_type()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
generic_spreadsheet_document_type<BaseType_type>::~generic_spreadsheet_document_type()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
void
generic_spreadsheet_document_type<BaseType_type>::getContributors(I_SpreadSheetView& _view, I_SpreadSheetContributorVisitor& _visitor)
{
    _visitor.begin();

    for(SpreadSheetContributors_type::iterator iter = m_contributors.begin(); iter != m_contributors.end(); iter++)
    {
        _visitor.visit(_view, **iter);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
void
generic_spreadsheet_document_type<BaseType_type>::addContributor(I_SpreadSheetContributor& _contributor)
{
    m_contributors.insert(&_contributor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
void
generic_spreadsheet_document_type<BaseType_type>::removeContributor(I_SpreadSheetContributor& _contributor)
{
    m_contributors.erase(&_contributor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_GENERIC_SPREADSHEET_DOCUMENT_TYPE_HPP_INCLUDED
