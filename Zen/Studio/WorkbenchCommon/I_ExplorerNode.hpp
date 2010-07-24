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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Enterprise/Database/I_Persistable.hpp>

#include <boost/noncopyable.hpp>

#include <boost/cstdint.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;
class I_ExplorerNodeType;

/// Explorer Node.
class WORKBENCHCOMMON_DLL_LINK I_ExplorerNode
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// User Data is data that is associated with an I_ExplorerNode instance.
    /// This is what the application developer uses in order to associate
    /// node objects with application objects.
    class WORKBENCHCOMMON_DLL_LINK UserData
    :   public Database::I_Persistable
    {
    public:

    public:
        /// The framework will call setNode() when user data is associated with
        /// a I_ExplorerNode.  Each explorer node can only have one user-data, but
        /// you can extend UserData to fit your needs.
        virtual void setNode(I_ExplorerNode* _pNode) { m_pNode = _pNode; }
        virtual I_ExplorerNode* getNode() const { return m_pNode; }

        virtual void setName(const std::string& _name);
        virtual const std::string& getName() const { return m_name; }

        /// Implement this method.
        /// Your extension to UserData should return the type of node this user
        /// data belongs
        virtual I_ExplorerNodeType& getType() const = 0;
    public:
        UserData(I_ExplorerNode* _pNode)
        :   m_pNode(_pNode)
        ,   m_name(m_pNode->getDisplayName())
        {
        }
        UserData(const std::string& _name)
        :   m_pNode(NULL)
        ,   m_name(_name) {}
        virtual ~UserData() {}
    private:
        I_ExplorerNode*     m_pNode;
        std::string         m_name;
    }; // class I_UserData

    typedef Memory::managed_ptr<UserData>               pUserData_type;
    typedef Event::Event<I_ExplorerNode*>               ExplorerNodeEvent_type;

    /// Defined below.
    struct I_ExplorerNodeVisitor;
    /// @}

    /// @name I_ExplorerNode interface
    /// @{
public:
    /// Get the display name of this node.
    /// This name is not necessarily unique among siblings.
    virtual const std::string& getDisplayName() const = 0;

    /// Sets the display name of the node.
    virtual void setDisplayName(const std::string& _name) = 0;

    /// Get the user data associated with this node.
    /// If you need to get the type of the this explorer node, use
    /// getUserData()->getType().
    virtual pUserData_type getUserData() const = 0;

    /// Get the parent of this node.
    /// @return NULL if this node has no parent, or the pointer to the parent
    ///         node.
    virtual I_ExplorerNode* getParent() = 0;

    /// Get the childrin of this explorer node.
    virtual void getChildren(I_ExplorerNodeVisitor& _visitor) = 0;

    /// Get the project to which this node belongs.
    /// @return NULL if this node does not belong to a project, or a pointer to
    ///     the project.
    virtual I_Project* getProject() = 0;

    /// Get the internal node Id.
    /// Every node can be identified via a unique 64 bit value.  This method
    /// returns that value.
    virtual boost::uint64_t getNodeId() const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    ExplorerNodeEvent_type  onModified;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ExplorerNode();
    virtual ~I_ExplorerNode();
    /// @}

    /// @name Inner Types
    /// @{
public:
    struct I_ExplorerNodeVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_ExplorerNode& _node) = 0;
        virtual void end() = 0;
    };  // struct I_ExplorerNodeVisitor
    /// @}

};  // interface I_ExplorerNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
	/// I_ExplorerNode is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_ExplorerNode> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_HPP_INCLUDED
