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
/// @author Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZENSTUDIO_WORKBENCHCOMMON_II_EXPLORER_NODE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_II_EXPLORER_NODE_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
namespace Internal {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Internal Explorer Node.
/// @note FOR INTERNAL USE ONLY
class WORKBENCHCOMMON_DLL_LINK II_ExplorerNode
:   public I_ExplorerNode
{
    /// @name Types
    /// @{
public:
    /// @todo TR - Code review.  This class heirarchy does not
    ///         look correct since so much of I_ExplorerNode is not
    ///         implemented in II_ExplorerNode and children of II_ExplorerNode.
    ///         This just does not look correct.
    class WORKBENCHCOMMON_DLL_LINK InternalData
    : public I_ExplorerNode::UserData
    {
    public:
        InternalData(I_ExplorerNode* _pNode)
        :   I_ExplorerNode::UserData(_pNode)
        {
        }
        InternalData(const std::string& _name) 
        : I_ExplorerNode::UserData(_name) {}
        virtual ~InternalData() {}

        virtual I_ExplorerNodeType& getType() const
        {
            throw Utility::runtime_exception("II_ExplorerNode::getType(): Error, this method should not be called.");
        }

        virtual void insert(pDatabaseConnection_type _pDBConn) {}
        virtual void load(pDatabaseConnection_type _pDBConn) {}
        virtual void save(pDatabaseConnection_type _pDBConn) {}
        virtual void remove(pDatabaseConnection_type _pDBConn) {}

    };

    typedef InternalData*                                       pInternalData_type;
    /// @}

    /// @name II_ExplorerNode interface
    /// @{
public:
    /// Get the internal data associated with this node.
    virtual pInternalData_type getInternalData() const = 0;

    /// Set the user data associated with this 
    virtual void setInternalData(pInternalData_type _pInternalData) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             II_ExplorerNode();
    virtual ~II_ExplorerNode();
    /// @}

};  // interface II_ExplorerNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Internal
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_II_EXPLORER_NODE_HPP_INCLUDED
