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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_GENERIC_EXPLORER_NODE_TYPE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_GENERIC_EXPLORER_NODE_TYPE_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <boost/bind.hpp>

#include <string>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Generic I_ExplorerNodeType template.
/// Use this template as a helper for creating explorer node types.
/// TR: Why a template?  Dunno....
template<class BaseType_type = void>
class generic_explorer_node_type
:   public Zen::Studio::Workbench::I_ExplorerNodeType
{
    /// @name Types
    /// @{
public:
    typedef std::set<I_ExplorerNodeContributor*>        ExplorerNodeContributors_type;
    /// @}

    /// @name I_ExplorerNodeType partial implementation
    /// @{
public:
    virtual void getContributors(I_ExplorerNode& _explorerNode, I_ExplorerNodeContributorVisitor& _visitor);
    virtual void addContributor(I_ExplorerNodeContributor& _contributor);
    virtual void removeContributor(I_ExplorerNodeContributor& _contributor);
    virtual const std::string& getIconName() const;
    virtual const std::string& getName() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             generic_explorer_node_type(const std::string& _name, const std::string& _iconName);
    virtual ~generic_explorer_node_type();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    std::string                     m_name;
    std::string                     m_iconName;
    ExplorerNodeContributors_type   m_contributors;
    /// @}

};  // template generic_explorer_node_type

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
generic_explorer_node_type<BaseType_type>::generic_explorer_node_type(const std::string& _name, const std::string& _iconName)
:   m_name(_name)
,   m_iconName(_iconName)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
generic_explorer_node_type<BaseType_type>::~generic_explorer_node_type()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
void
generic_explorer_node_type<BaseType_type>::getContributors(I_ExplorerNode& _explorerNode, I_ExplorerNodeContributorVisitor& _visitor)
{
    _visitor.begin();

    for(ExplorerNodeContributors_type::iterator iter = m_contributors.begin(); iter != m_contributors.end(); iter++)
    {
        _visitor.visit(_explorerNode, **iter);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
void
generic_explorer_node_type<BaseType_type>::addContributor(I_ExplorerNodeContributor& _contributor)
{
    m_contributors.insert(&_contributor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
void
generic_explorer_node_type<BaseType_type>::removeContributor(I_ExplorerNodeContributor& _contributor)
{
    m_contributors.erase(&_contributor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
const std::string& 
generic_explorer_node_type<BaseType_type>::getIconName() const
{
    return m_iconName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class BaseType_type>
const std::string& 
generic_explorer_node_type<BaseType_type>::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_GENERIC_EXPLORER_NODE_TYPE_HPP_INCLUDED
