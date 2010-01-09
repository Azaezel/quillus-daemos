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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_DECORATION_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_DECORATION_HPP_INCLUDED

#include "Configuration.hpp"

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ExplorerNode;

/// Explorer Node Decoration.
/// An explorer node decoration is a set of decorators that can be applied
/// to an explorer node.  Decorators modify the appearance of the explorer node,
/// such as providing a name suffix or prefix.  Eventually this will be extended
/// to also include special icons and modifications to the root icon.
class WORKBENCH_DLL_LINK I_ExplorerNodeDecoration
{
    /// @name I_ExplorerNodeDecoration interface
    /// @{
public:
    /// Get the name prefix for an explorer node.
    virtual const std::string& getNamePrefix(I_ExplorerNode& _explorerNode) const = 0;

    /// Get the name suffix for an explorer node.
    virtual const std::string& getNameSuffix(I_ExplorerNode& _explorerNode) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ExplorerNodeDecoration();
    virtual ~I_ExplorerNodeDecoration();
    /// @}

};  // interface I_ExplorerNodeDecoration

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_DECORATION_HPP_INCLUDED
