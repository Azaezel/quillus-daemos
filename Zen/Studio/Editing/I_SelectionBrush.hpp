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
#ifndef INDIEZEN_EDITING_I_SELECTION_BRUSH_HPP_INCLUDED
#define INDIEZEN_EDITING_I_SELECTION_BRUSH_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
    namespace ThreeSpace {
        class I_Vertex;
        class I_Triangle;
        class I_Quad;
        class I_BoundingBox;
    }
namespace Editing {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// ThreeSpace Selection Brush Interface
/// Brush that selects a region
class EDITING_DLL_LINK I_SelectionBrush
:   boost::noncopyable
{
    /// @name I_SelectionBrush interface
    /// @{
public:
    /// Checks to see if this brush selects the specified vertex.
    virtual float selects(const ThreeSpace::I_Vertex& _vertex) = 0;

    /// Checks to see if the left vertex is closer than the right vertex.
    ///
    /// This is used when only one vertex is being selected and the selection system
    /// needs to choose between two selected vertices.
    ///
    /// @return true if _left is closer to being selected that _right.
    virtual bool closer(const ThreeSpace::I_Vertex& _left, const ThreeSpace::I_Vertex& _right) = 0;

    /// Checks to see if this brush selects the specified triangle.
    virtual float selects(const ThreeSpace::I_Triangle& _triangle) = 0;

    /// Checks to see if this brush selects the specified quad.
    virtual float selects(const ThreeSpace::I_Quad& _quad) = 0;

    /// Checks to see if this brush selects anything inside of the specified bounding box.
    virtual bool selects(const ThreeSpace::I_BoundingBox& _boundingBox) = 0;

    /// Checks to see if this brush selects anything inside of the specified bounding sphere.
    virtual bool selects(const ThreeSpace::I_BoundingSphere& _boundingSphere) = 0;

    /// Destroy this object.
    ///
    /// Use the class factory to create the object, then call this method when
    /// the object is no longer required.
    virtual void destroy() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SelectionBrush();
    virtual ~I_SelectionBrush();
    /// @}

};	// interface I_SelectionBrush

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Editing
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_EDITING_I_SELECTION_BRUSH_HPP_INCLUDED
