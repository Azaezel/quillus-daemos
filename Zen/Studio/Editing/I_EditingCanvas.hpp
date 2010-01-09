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
#ifndef INDIEZEN_EDITING_I_EDITING_CANVAS_HPP_INCLUDED
#define INDIEZEN_EDITING_I_EDITING_CANVAS_HPP_INCLUDED

#include "Configuration.hpp"

#include <IndieZen/framework/Rendering/I_RenderingCanvas.hpp>

#include <boost/filesystem/path.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
    namespace ThreeSpace {
        class I_Model;
        class I_Mesh;
        class I_Face;
        class I_Vertex;
    }
namespace Editing {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_MeshDocument;

/// ThreeSpace Editing Canvas Interface
class EDITING_DLL_LINK I_EditingCanvas
:   public Rendering::I_RenderingCanvas
{
    /// @name I_EditingCanvas interface
    /// @{
public:
    /// Set the raw binary input.
    virtual void setEditorInput(I_MeshDocument* _pDocument) = 0;

    /// A mouse button is pressed at x, y coordinates
    /// @todo Eventually these events need to go through an
    /// input service and some sort of action mapper.
    virtual void onMouseDown(int _button, int _x, int _y) = 0;

    /// A mouse button is released at x, y coordinates
    /// @todo Eventually these events need to go through an
    /// input service and some sort of action mapper.
    virtual void onMouseUp(int _button, int _x, int _y) = 0;

    /// The mouse was moved to x, y
    /// @todo Eventually these events need to go through an
    /// input service and some sort of action mapper.
    virtual void onMouseMove(int _button, int _x, int _y) = 0;

    /// Switch to Pick mode
    virtual void enablePickMode() = 0;

    /// Switch to Orbit mode
    virtual void enableOrbitMode() = 0;

    /// Switch to Pan mode
    virtual void enablePanMode() = 0;

	/// TODO Use a selection query and result.

    class NotImplemented
    {
    /// Select the first model at the specified 2d coordinates assuming
    /// the viewpoint of the current camera using the current scene graph.
    ///
    /// @return NULL if no model exists at the specified coordinates.
    virtual const ThreeSpace::I_Model* selectModel(int _x, int _y) = 0;

    /// Select the first mesh of the specified model at the specified 2d
    /// coordinates assuming the viewpoint of the current camera using
    /// the current scene graph.
    ///
    /// @return NULL if no mesh exists at the specified coordinates.
    virtual const ThreeSpace::I_Mesh* selectMesh(const ThreeSpace::I_Model& _model, int _x, int _y) = 0;

    /// Select the closest face of the specified mesh at the specified 2d
    /// cordinates assuming the viewpoint of the current camera using the current
    /// scene graph.
    ///
    /// @param _backFacing false to ignore back faces, true to pick back faces.
    /// @return NULL if no face exists near the specified coordinates.
    virtual const ThreeSpace::I_Face* selectFace(const ThreeSpace::I_Mesh& _mesh, int _x, int _y, bool _backFacing = false) = 0;

    /// Select the closest vertex of the specified face at the specified 2d
    /// cordinates assuming the viewpoint of the current camera using the current
    /// scene graph.
    ///
    /// @return NULL if no vertex exists near the specified coordinates.
    virtual const ThreeSpace::I_Vertex* selectVertex(const ThreeSpace::I_Face& _mesh, int _x, int _y) = 0;
    };
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_EditingCanvas();
    virtual ~I_EditingCanvas();
    /// @}

};	// interface I_EditingCanvas

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Editing
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_EDITING_I_EDITING_CANVAS_HPP_INCLUDED
