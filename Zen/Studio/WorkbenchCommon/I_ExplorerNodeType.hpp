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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_TYPE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_TYPE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ExplorerNode;
class I_ExplorerNodeContributor;

/// Explorer Node Type.
/// Implement this to create a custom explorer node type.
class WORKBENCHCOMMON_DLL_LINK I_ExplorerNodeType
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    // Defined below.
    struct I_ExplorerNodeContributorVisitor;
    /// @}

    /// @name I_ExplorerNodeType interface
    /// @{
public:
    /// Gets the mutability of the I_ExplorerNode display name.
    /// @note Don't confuse this with the type name.  The type name of a
    ///         I_ExplorerNodeType cannot ever change.  The I_ExplorerNode
    ///         that is of the type represented by this I_ExplorerNodeType
    ///         has a I_ExplorerNode::setDisplayName() method.  That method
    ///         will only be called if getNameMutability() returns true.
    virtual bool getNameMutability(I_ExplorerNode& _explorerNode) const = 0;

    /// Gets the dragability of the I_ExplorerNode.
    virtual bool isDraggable(I_ExplorerNode& _explorerNode) const = 0;

    /// Gets the dropability of a node onto another node.
    /// @param _sourceNode Node that's being dragged / dropped.
    /// @param _destNode Node that is being dropped onto.
    /// @return true if the _sourceNode can be dropped onto _destNode.
    virtual bool canDrop(Zen::Studio::Workbench::I_ExplorerNode& _sourceNode, Zen::Studio::Workbench::I_ExplorerNode& _destNode) const = 0;

    /// Get the contributors associated with this explorer node.
    /// @todo Should this be const?
    virtual void getContributors(I_ExplorerNode& _explorerNode, I_ExplorerNodeContributorVisitor& _visitor) = 0;

    /// Add a contributor to this explorer node type.
    /// Be sure to call removeContributor() before _contributor goes
    /// out of scope because the implementation of this method may retain
    /// a reference to _contributor.
    virtual void addContributor(I_ExplorerNodeContributor& _contributor) = 0;

    /// Remove a contributor from this explorer node type.
    virtual void removeContributor(I_ExplorerNodeContributor& _contributor) = 0;

    /// Get the icon name of this type.
    virtual const std::string& getIconName() const = 0;

    /// Get the name of this type.
    virtual const std::string& getName() const = 0;
    /// @}

    /// @name Inner Classes
    /// @{
public:
    struct I_ExplorerNodeContributorVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_ExplorerNode& _node, I_ExplorerNodeContributor& _contributor) = 0;
        virtual void end() = 0;
    };  // struct I_ExplorerNodeContributorVisitor;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ExplorerNodeType();
    virtual ~I_ExplorerNodeType();
    /// @}

};  // interface I_ExplorerNodeType

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
	/// I_ExplorerNodeType is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_ExplorerNodeType> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_I_EXPLORER_NODE_TYPE_HPP_INCLUDED
