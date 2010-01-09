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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_DIRECT_EXPLORER_NODE_ACTION_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_DIRECT_EXPLORER_NODE_ACTION_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeAction.hpp>

#include <boost/bind.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Generic I_ExplorerNodeAction template.
/// Use this template as a helper for creating explorer node actions that
/// directly call the target object that's based on I_ExplorerNode::UserData.
template<class DelegateClass_type>
class direct_explorer_node_action
:   public Zen::Studio::Workbench::I_ExplorerNodeAction
{
    /// @name Types
    /// @{
public:
    typedef void(DelegateClass_type::*HandlerFunction_type)(Zen::Studio::Workbench::I_ExplorerNode&);
    /// @}

    /// @name I_ExplorerNodeAction implementation
    /// @{
public:
    virtual const std::string& getName()
    {
        return m_name;
    }

    virtual void invoke(Zen::Studio::Workbench::I_ExplorerNode& _node)
    {
        boost::bind(boost::mem_fn(m_handler),
            dynamic_cast<DelegateClass_type*>(_node.getUserData().get()))
            (_node);
    }
    virtual const std::string& getType()
    {
        return m_type;
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
    direct_explorer_node_action(const std::string& _name, const std::string& _type, HandlerFunction_type _handler)
    :   m_name(_name)
    ,   m_type(_type)
    ,   m_handler(_handler)
    {
    }

    virtual ~direct_explorer_node_action()
    {
    }
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string             m_name;
    std::string             m_type;
    HandlerFunction_type    m_handler;
    /// @}
};  // template direct_explorer_node_action

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_DIRECT_EXPLORER_NODE_ACTION_HPP_INCLUDED
