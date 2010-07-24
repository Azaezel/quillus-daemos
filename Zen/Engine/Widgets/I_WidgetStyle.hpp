//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)		2009 Denis Hilliard
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
#ifndef ZEN_ENGINE_WIDGETS_I_WIDGET_STYLE_HPP_INCLUDED
#define ZEN_ENGINE_WIDGETS_I_WIDGET_STYLE_HPP_INCLUDED
namespace Zen {
namespace Engine {
namespace Widgets {

class I_Widget;
enum HorizontalAlignment { Default, Left, Right, Centre };
enum VerticalAlignment { Default, Top, Bottom, Middle };
	
class I_WidgetStyle
{
	/// @name Types
	/// @{
	typedef int				pixel_type;
	typedef float			percent_type;
	/// @}
	
	/// @name I_WidgetStyle interface
	/// @{
public:
	/// Apply the style to the child relative to the parent
	virtual void apply(I_Widget & _parent, I_Widget & _child) = 0;
	
	virtual void setHeight(const pixel_type _height) = 0;
	virtual void setHeight(const percent_type _height) = 0;
	virtual void setWidth(const pixel_type _width) = 0;
	virtual void setWidth(const percent_type _width) = 0;
	virtual void setPadding(const pixel_type _horz, const pixel_type _vert) = 0;
	virtual void setAlignment(const HorizontalAlignment _align) = 0;
	virtual void setAlignment(const VerticalAlignment _align) = 0;
	
	/// @}
};
}
}
}
#endif //ZEN_ENGINE_WIDGETS_I_WIDGET_STYLE_HPP_INCLUDED
