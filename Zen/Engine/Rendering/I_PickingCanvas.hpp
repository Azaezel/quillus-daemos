//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_RENDERING_I_PICKING_CANVAS_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_PICKING_CANVAS_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/I_Canvas.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Editing Canvas Interface
/// @todo Rename to I_EditingCanvas?
class RENDERING_DLL_LINK I_PickingCanvas
:   public I_Canvas
{
    /// @name I_PickingCanvas interface
    /// @{
public:
    /// Select the first model at the specified 2d coordinates assuming
    /// the viewpoint of the current camera using the current scene graph.
    ///
    /// @return NULL if no model exists at the specified coordinates.
    virtual const I_Model* selectModel(int _x, int _y) = 0;

    /// Select the first mesh of the specified model at the specified 2d
    /// coordinates assuming the viewpoint of the current camera using
    /// the current scene graph.
    ///
    /// @return NULL if no mesh exists at the specified coordinates.
    virtual const I_Mesh* selectMesh(const I_Model& _model, int _x, int _y) = 0;

    /// Select the closest face of the specified mesh at the specified 2d
    /// cordinates assuming the viewpoint of the current camera using the current
    /// scene graph.
    ///
    /// @param _backFacing false to ignore back faces, true to pick back faces.
    /// @return NULL if no face exists near the specified coordinates.
    virtual const I_Face* selectFace(const I_Mesh& _mesh, int _x, int _y, bool _backFacing = false) = 0;

    /// Select the closest vertex of the specified face at the specified 2d
    /// cordinates assuming the viewpoint of the current camera using the current
    /// scene graph.
    ///
    /// @return NULL if no vertex exists near the specified coordinates.
    virtual const I_Vertex* selectVertex(const I_Face& _mesh, int _x, int _y) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PickingCanvas();
    virtual ~I_PickingCanvas();
    /// @}

};	// interface I_PickingCanvas

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_PICKING_CANVAS_HPP_INCLUDED
