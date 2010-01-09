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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_PROJECT_EXPLORER_NODE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_PROJECT_EXPLORER_NODE_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/II_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <string>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ProjectExplorerModel;

/// Explorer Node.
/// @todo Should this be renamed to ExplorerNode?
class ProjectExplorerNode
:   public Internal::II_ExplorerNode
{
    /// @name Types
    /// @{
public:
    typedef std::set<ProjectExplorerNode*>          Children_type;
    /// @}

    /// @name I_ExplorerNode implementation
    /// @{
public:
    virtual const std::string& getDisplayName() const;
    virtual void setDisplayName(const std::string& _name);
    virtual pUserData_type getUserData() const;
    virtual I_ExplorerNode* getParent();
    virtual void getChildren(I_ExplorerNodeVisitor& _visitor);
    virtual boost::uint64_t getNodeId() const;
    /// @}

    /// @name Internal::I_ExplorerNode implementation
    /// @{
public:
    virtual pInternalData_type getInternalData() const;
    virtual void setInternalData(pInternalData_type _pInternalData);
    /// @}

    /// @name ProjectExplorerNode implementation
    /// @{
public:
    /// Set the database node Id.
    void setNodeId(boost::uint64_t _nodeId);

    /// Add a child to this node.
    void addChild(ProjectExplorerNode* _pChild);

    /// Remove a child from this node.
    void removeChild(ProjectExplorerNode* _pChild);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ProjectExplorerModel;
    /// Construct a ProjectExplorerNode.
    /// @param _model Kept as a reference to the model that contains this node.  It's also
    ///     used to attach I_ExplorerNode::onModified event to ProjectExplorerModel::onNodeModified().
    /// @param _pUserData is not an optional parameter.  All explorer nodes must be 
    ///     associated with a user data.
    /// @param _pParent parent node or NULL if this node does not have a parent.
    ProjectExplorerNode(ProjectExplorerModel& _model, pUserData_type _pUserData, ProjectExplorerNode* _pParent = NULL);
    virtual ~ProjectExplorerNode();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Model that contains this node
    ProjectExplorerModel&   m_model;
    /// User data that is attached to this node
    pUserData_type          m_pUserData;
    /// Parent node (possibly NULL)
    ProjectExplorerNode*    m_pParent;
    /// Display name of this node.  This value is only used if
    /// m_pUserData is invalid (which it never is, so why do this?)
    std::string             m_displayName;
    /// Internal user data attached to this node.  Generally this
    /// is used by the GUI to associate this node with a tree-view node.
    pInternalData_type      m_pInternalData;
    /// Database node Id.
    boost::uint64_t         m_nodeId;
    /// Children of this node (possibly empty if this is a leaf node)
    Children_type           m_children;

    /// Retain a connection to the model's event subscription.
    ExplorerNodeEvent_type::pConnection_type    m_pEventConnection;
    /// @}

};  // class ProjectExplorerNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_PROJECT_I_EXPLORER_NODE_HPP_INCLUDED
