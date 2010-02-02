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
#ifndef ZENSTUDIO_WORKBENCH_CONTRIBUTOR_VISITOR_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_CONTRIBUTOR_VISITOR_HPP_INCLUDED

// This template is intended to replace multiple copy / paste 
// implementations of ContributorVisitor

#if 0 // not fully implemented, possibly not needed

#include "

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;
/// Contributor Visitor helper template.
/// This template implements a generic contributor visitor that can be used for 
/// most contributor (all?) types.
template <class Contributee_type>
class contributor_visitor
:   public Contributee_type::Contributor_type::Visitor_type
{
    /// @name Types
    /// @{
public:
    typedef Contributee_type::Contributor_type                      Contributor_type;
    typedef Contributee_type::Contributor_type::Visitor_type        Visitor_type;
    typedef Contributee_type::Contributor_type::Action_type         Action_type;
    typedef std::list<Action_type>                                  Actions_type;
    /// @}

    /// @name Visitor_type implementation
    /// @{
    void begin()
    {
    }

    void visit(Contributee_type& _contributee, Contributor_type& _contributor)
    {
        ActionVisitor_type visitor(m_actionList);

        _contributor.getActions(_contributee, visitor);
    }

    void end()
    {
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
    contributor_visitor()
    {
    }

    virtual ~contributor_visitor()
    {
    }
    /// @}

};  // template contributor_visitor
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // not finished

#endif // ZENSTUDIO_WORKBENCH_CONTRIBUTOR_VISITOR_HPP_INCLUDED
