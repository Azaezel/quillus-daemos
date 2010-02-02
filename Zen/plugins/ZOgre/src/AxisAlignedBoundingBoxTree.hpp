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
#ifndef ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_TREE_HPP_INCLUDED
#define ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_TREE_HPP_INCLUDED

#include <Zen/Engine/Rendering/I_BoundingVolumeTree.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class AxisAlignedBoundingBoxTree
:   public Zen::Engine::Rendering::I_BoundingVolumeTree
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Zen::Engine::Rendering::I_BoundingVolumeTree implementation
    /// @{
public:
    virtual const pNode_type getRoot() const;
    virtual void addBoundingVolume(const pBoundingVolume_type& _pBoundingVolume);
    virtual void removeBoundingVolume(const pBoundingVolume_type& _pBoundingVolume);
    /// @}

    /// @name Zen::ZOgre::AxisAlignedBoundingBoxTree implementation
    /// @{
protected:
    
    /// @}

    /// @name 'Structors
    /// @{
public:
             AxisAlignedBoundingBoxTree();
    virtual ~AxisAlignedBoundingBoxTree();
    /// @}

    /// @name Member variables
    /// @{
private:
    pNode_type                                      m_root;
    std::set<pBoundingVolume_type>                  m_boundingVolumes;
    /// @}

};  // class AxisAlignedBoundingBoxTree

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_TREE_HPP_INCLUDED
