//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef INDIEZEN_THREESPACE_VERTEX_INDEX_HPP_INCLUDED
#define INDIEZEN_THREESPACE_VERTEX_INDEX_HPP_INCLUDED

#include <limits.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace ThreeSpace {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class VertexIndex
{
    /// @name VertexIndex implementation
    /// @{
public:
	inline unsigned int getVertexIndex() const { return m_index; }
	inline void setVertexIndex(unsigned int _index) { m_index = _index; }

	inline unsigned int getUVWIndex() const { return m_uvwIndex; }
	inline void setUVWIndex(unsigned int _index) { m_uvwIndex = _index; }

	inline unsigned int getNormalIndex() const { return m_normalIndex; }
	inline void setNormalIndex(unsigned int _index) { m_normalIndex = _index; }
    /// @}

    /// @name 'Structors
    /// @{
public:
             VertexIndex() { m_index = m_uvwIndex = m_normalIndex = UINT_MAX; }
             VertexIndex(unsigned _index) { m_index = _index; m_uvwIndex = m_normalIndex = UINT_MAX; }
    virtual ~VertexIndex() {}
    /// @}

private:
    unsigned int	m_index;
    unsigned int	m_uvwIndex;
    unsigned int	m_normalIndex;

};  // class VertexIndex

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace IndieZen
}   // namespace ThreeSpace
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_THREESPACE_VERTEX_INDEX_HPP_INCLUDED
