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
#ifndef INDIEZEN_BRUSH_MODELING_I_GRID_LAYOUT_DATA_HPP_INCLUDED
#define INDIEZEN_BRUSH_MODELING_I_GRID_LAYOUT_DATA_HPP_INCLUDED

#include "Configuration.hpp"

#include <IndieZen/framework/BrushModeling/I_LayoutData.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace BrushModeling {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Grid Layout Interface
class BRUSHMODELING_DLL_LINK I_GridLayoutData
:   public I_LayoutData
{
    /// @name I_GridLayoutData interface
    /// @{
public:
    /// Set the number of columns to occupy.
    /// If no value is set, 1 is assumed.
    virtual void setColumnSpan(int _columns) = 0;

    /// Set the number of rows to occupy.
    /// If no value is set, 1 is assumed.
    virtual void setRowSpan(int _rows) = 0;

    /// Set the number of levels to occupy.
    /// If no value is set, 1 is assumed.
    virtual void setLevelSpan(int _levels) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GridLayoutData();
    virtual ~I_GridLayoutData();
    /// @}

};	// interface I_GridLayoutData

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace BrushModeling
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_BRUSH_MODELING_I_GRID_LAYOUT_DATA_HPP_INCLUDED
