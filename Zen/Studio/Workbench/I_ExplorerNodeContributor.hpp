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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_CONTRIBUTOR_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_CONTRIBUTOR_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Studio/Workbench/I_Contributor.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ExplorerNodeAction;
class I_ExplorerNodeDecoration;
class I_ExplorerNode;

/// Explorer Node Contributor.
/// This Contributor contributes actions and decorations to explorer nodes.
/// @see http://www.indiezen.org/wiki/irclogs/2009/05/22/#UTC2009-05-22T18:31:55
class WORKBENCH_DLL_LINK I_ExplorerNodeContributor
:   public virtual I_Contributor
{
    /// @name Types
    /// @{
public:
    // Defined below.
    struct I_ExplorerNodeActionVisitor;
    struct I_ExplorerNodeDecorationVisitor;
    /// @}

    /// @name I_ExplorerNodeContributor interface
    /// @{
public:
    /// Get the explorer node actions associated with this explorer node.
    virtual void getExplorerNodeActions(I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor) = 0;

    /// Get the explorer node decorations associated with this explorer node.
    virtual void getExplorerNodeDecorations(I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ExplorerNodeContributor();
    virtual ~I_ExplorerNodeContributor();
    /// @}


    /// @name Inner Classes
    /// @{
public:
    struct I_ExplorerNodeActionVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_ExplorerNodeAction& _action) = 0;
        virtual void end() = 0;
    };  // struct I_ExplorerNodeActionVisitor;


    struct I_ExplorerNodeDecorationVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_ExplorerNodeDecoration& _action) = 0;
        virtual void end() = 0;
    };  // struct I_ExplorerNodeDecorationVisitor;
    /// @}

};  // interface I_ExplorerNodeContributor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_CONTRIBUTOR_HPP_INCLUDED
