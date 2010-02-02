//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
// Copyright (C)        2009 Johan Verbelen
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
/// @brief Basic color class (RGBA)
///
/// Basic color class using RGBA.
///
///
/// @author Tony Richards trichards@indiezen.com
/// @author Matthew Alan Gray mgray@indiezen.org
/// @author Johan Verbelen johan.verbelen@telenet.be
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_RENDERING_COLOR_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_COLOR_HPP_INCLUDED

#include <Zen/Core/Math/Math.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Color
{
    /// @name Color implementation
    /// @{
public:
    /// Get the Red color component
    Math::Real getRed();

    /// Get the Blue color component
    Math::Real getBlue();

    /// Get the Green color component
    Math::Real getGreen();

    /// Get the Alpha component
    Math::Real getAlpha();

    /// Set the color
    void setColor(Math::Real _red, Math::Real _green, Math::Real _blue, Math::Real _alpha);
    /// @}

    /// @name Operators
    /// @{
public:
    Color& operator=(const Color& _right);

    /// @name 'Structors
    /// @{
public:
    /// Default constructor.
    /// Constructs a LightSteelBlue color
    Color();

    /// Construct a color by specifiying the RGB and optionally A component.
    Color(Math::Real _red, Math::Real _green, Math::Real _blue, Math::Real _alpha = 1.0f);

    Color(const Color& _right);
    /// @}

    /// @name Member variables
    /// @{
public:
    /// Intentionally declaring members as public for direct access.
    union
    {
        Math::Real m_array[4];
        struct
        {
            Math::Real m_red;
            Math::Real m_green;
            Math::Real m_blue;
            Math::Real m_alpha;
        };
    };
    /// @}

};  // class Color

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Color::Color()
:   m_red   (176.0f/255.0f)
,   m_green (196.0f/255.0f)
,   m_blue  (222.0f/255.0f)
,   m_alpha (1.0f)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Color::Color(Math::Real _red, Math::Real _green, Math::Real _blue, Math::Real _alpha)
:   m_red   (_red)
,   m_green (_green)
,   m_blue  (_blue)
,   m_alpha (_alpha)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Color::Color(const Color& _right)
:   m_red(_right.m_red)
,   m_green(_right.m_green)
,   m_blue(_right.m_blue)
,   m_alpha(_right.m_alpha)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Math::Real
Color::getRed()
{ 
    return m_red; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Math::Real
Color::getBlue()
{ 
    return m_blue; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Math::Real
Color::getGreen() 
{ 
    return m_green; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Math::Real
Color::getAlpha()
{ 
    return m_alpha; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
Color::setColor(Math::Real _red, Math::Real _green, Math::Real _blue, Math::Real _alpha)
{
    m_red   = _red;
    m_green = _green;
    m_blue  = _blue;
    m_alpha = _alpha;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Color&
Color::operator=(const Color& _right)
{
    m_red   = _right.m_red;
    m_green = _right.m_green;
    m_blue  = _right.m_blue;
    m_alpha = _right.m_alpha;

    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_COLOR_HPP_INCLUDED
