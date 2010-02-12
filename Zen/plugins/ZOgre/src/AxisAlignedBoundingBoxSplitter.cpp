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

#include "AxisAlignedBoundingBoxSplitter.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AxisAlignedBoundingBoxSplitter::AxisAlignedBoundingBoxSplitter(const std::vector<pNode_type>& _nodes)
{
    this->m_longObjectPercentageTolerance = 0.75f;
    std::vector<pNode_type>::const_iterator iter = _nodes.begin();
    while( iter != _nodes.end() )
    {
        pNode_type pNode = *iter;
        const Zen::Engine::Rendering::I_AxisAlignedBoundingBox* pBox = 
            dynamic_cast<const Zen::Engine::Rendering::I_AxisAlignedBoundingBox*>
            (pNode->getNodeBoundingVolume().get());
        this->m_nodeBoundingVolume->merge(*pBox);
        iter++;
    }
    this->initialize(_nodes);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AxisAlignedBoundingBoxSplitter::~AxisAlignedBoundingBoxSplitter()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBoxSplitter::splitLeft(const pNode_type& _pNode) const
{
    if(this->m_isolateLongVolumes)
    {
        return this->getSplitAxisLength(_pNode->getNodeBoundingVolume()) < this->m_maxVolumeLength;
    }
    else
    {
        return this->getSplitAxisCenter(_pNode->getNodeBoundingVolume()) < this->m_splitValue;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBoxSplitter::lessThan(const pNode_type& _lhs, const pNode_type& _rhs) const
{
    return this->getSplitAxisCenter(_lhs->getNodeBoundingVolume()) < 
        this->getSplitAxisCenter(_rhs->getNodeBoundingVolume());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const AxisAlignedBoundingBoxSplitter::pBoundingVolume_type 
AxisAlignedBoundingBoxSplitter::getNodeBoundingVolume() const
{
    return this->m_nodeBoundingVolume;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBoxSplitter::operator()(const pNode_type& _pNode) const
{
    return this->splitLeft(_pNode);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBoxSplitter::operator()(const pNode_type& _lhs, const pNode_type& _rhs) const
{
    return this->lessThan(_lhs,_rhs);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AxisAlignedBoundingBoxSplitter::initialize(const std::vector<pNode_type>& _nodes)
{
    this->m_splitAxis = this->findSplitAxis(this->m_nodeBoundingVolume);
    this->m_splitValue = this->findSplitValue(this->m_nodeBoundingVolume, this->m_splitAxis);
    this->m_maxVolumeLength = this->getSplitAxisLength(this->m_nodeBoundingVolume)*
        this->m_longObjectPercentageTolerance;

    this->m_isolateLongVolumes = false;
    std::vector<pNode_type>::const_iterator iter = _nodes.begin();
    while( iter != _nodes.end() )
    {
        pNode_type pNode = *iter;
        if( this->getSplitAxisLength(pNode->getNodeBoundingVolume()) > this->m_maxVolumeLength)
        {
            this->m_isolateLongVolumes = true;
            break;
        }
        iter++;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
AxisAlignedBoundingBoxSplitter::findSplitAxis(const pAxisAlignedBoundingBox_type& _pBoundingVolume)
{
    int axis = 0, i;
    Zen::Math::Vector3 vector = _pBoundingVolume->getMaximum() - _pBoundingVolume->getMinimum();
    for( i = 1 ; i < 3 ; ++i )
    {
        if( vector.m_array[i] > vector.m_array[axis] )
        {
            axis = i;
        }
    }
    return axis;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
AxisAlignedBoundingBoxSplitter::findSplitValue(const pAxisAlignedBoundingBox_type& _pBoundingVolume, int _splitAxis)
{
    return (_pBoundingVolume->getMinimum().m_array[_splitAxis] +
        _pBoundingVolume->getMaximum().m_array[_splitAxis])*0.5f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
AxisAlignedBoundingBoxSplitter::getSplitAxisLength(const pAxisAlignedBoundingBox_type& _pBoundingVolume) const
{
    return (_pBoundingVolume->getMaximum().m_array[this->m_splitAxis] -
        _pBoundingVolume->getMinimum().m_array[this->m_splitAxis]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
AxisAlignedBoundingBoxSplitter::getSplitAxisCenter(const pAxisAlignedBoundingBox_type& _pBoundingVolume) const
{
    return (_pBoundingVolume->getMinimum().m_array[this->m_splitAxis] +
        _pBoundingVolume->getMaximum().m_array[this->m_splitAxis])*0.5f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
