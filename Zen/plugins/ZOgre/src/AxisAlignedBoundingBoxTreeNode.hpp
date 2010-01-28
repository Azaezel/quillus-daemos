//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_TREE_NODE_HPP_INCLUDED
#define ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_TREE_NODE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Engine/Rendering/I_BoundingVolumeTree.hpp>
#include <Zen/Engine/Rendering/I_AxisAlignedBoundingBox.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class AxisAlignedBoundingBoxTreeNode
:   public Zen::Engine::Rendering::I_BoundingVolumeTree::I_Node
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Engine::Rendering::I_AxisAlignedBoundingBox>  pAxisAlignedBoundingBox_type;
    /// @}

    /// @name Zen::Engine::Rendering::I_BoundingVolumeTree::I_Node implementation
    /// @{
public:
    virtual unsigned int getNumChildren() const;
    virtual const pBoundingVolume_type getBoundingVolume(unsigned int _index) const;
    virtual const pBoundingVolume_type getNodeBoundingVolume() const;
    virtual const pNode_type getLeftChild() const;
    virtual const pNode_type getRightChild() const;
    virtual bool isLeaf() const;
    /// @}

    /// @name Zen::ZOgre::AxisAlignedBoundingBoxTreeNode implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             AxisAlignedBoundingBoxTreeNode(const std::vector<pNode_type>& _nodes);
    virtual ~AxisAlignedBoundingBoxTreeNode();
    /// @}

    /// @name Member variables
    /// @{
private:
    pAxisAlignedBoundingBox_type                m_pBoundingVolume;
    std::vector<pNode_type>                     m_children;
    /// @}

};  // class AxisAlignedBoundingBoxTreeNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_TREE_NODE_HPP_INCLUDED
