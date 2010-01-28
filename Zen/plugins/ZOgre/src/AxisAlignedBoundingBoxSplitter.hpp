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
#ifndef ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_SPLITTER_HPP_INCLUDED
#define ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_SPLITTER_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Engine/Rendering/I_BoundingVolumeTree.hpp>
#include <Zen/Engine/Rendering/I_AxisAlignedBoundingBox.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class AxisAlignedBoundingBoxSplitter
:   public Zen::Engine::Rendering::I_BoundingVolumeTree::I_BoundingVolumeSplitter
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Engine::Rendering::I_AxisAlignedBoundingBox>  pAxisAlignedBoundingBox_type;
    /// @}

    /// @name Zen::Engine::Rendering::I_BoundingVolumeTree::I_BoundingVolumeSplitter implementation
    /// @{
public:
    virtual bool splitLeft(const pNode_type& _pNode) const;
    virtual bool lessThan(const pNode_type& _lhs, const pNode_type& _rhs) const;
    virtual const pBoundingVolume_type getNodeBoundingVolume() const;
    virtual bool operator()(const pNode_type& _pNode) const;
    virtual bool operator()(const pNode_type& _lhs, const pNode_type& _rhs) const;
    /// @}

    /// @name Zen::ZOgre::AxisAlignedBoundingBoxSplitter implementation
    /// @{
protected:
    void initialize(const std::vector<pNode_type>& _nodes);
    int findSplitAxis(const pAxisAlignedBoundingBox_type& _boundingVolume);
    Zen::Math::Real findSplitValue(const pAxisAlignedBoundingBox_type& _boundingVolume, int _splitAxis);
    Zen::Math::Real getSplitAxisLength(const pAxisAlignedBoundingBox_type& _boundingVolume) const;
    Zen::Math::Real getSplitAxisCenter(const pAxisAlignedBoundingBox_type& _boundingVolume) const;
    /// @}
    
    /// @name 'Structors
    /// @{
public:
             AxisAlignedBoundingBoxSplitter(const std::vector<pNode_type>& _nodes);
    virtual ~AxisAlignedBoundingBoxSplitter();
    /// @}

    /// @name Member variables
    /// @{
private:
    int                                 m_splitAxis;
    Zen::Math::Real                     m_splitValue;
    bool                                m_isolateLongVolumes;
    Zen::Math::Real                     m_maxVolumeLength;
    Zen::Math::Real                     m_longObjectPercentageTolerance;

    pAxisAlignedBoundingBox_type        m_nodeBoundingVolume;
    /// @}

};  // class AxisAlignedBoundingBoxSplitter

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_SPLITTER_HPP_INCLUDED
