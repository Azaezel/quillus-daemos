//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ZCRAZYEDDIE_I_WIDGET_SERVICE_HPP_INCLUDED
#define ZEN_ZCRAZYEDDIE_I_WIDGET_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Engine/Widgets/I_WidgetService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCrazyEddie {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ImageSet;
class I_ImageSetService;
class I_SchemeService;

class ZCRAZYEDDIE_DLL_LINK I_WidgetService
:   public Zen::Engine::Widgets::I_WidgetService
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name ZCrazyEddie::I_WidgetService interface
    /// @{
public:
    /// Get an imageset service
    virtual I_ImageSetService& getImageSetService() = 0;

    /// Get a style service
    virtual I_SchemeService& getSchemeService() = 0;

    /// Load the window layout
    virtual void loadWindowLayout(const std::string& _layout) = 0;

    /// Set the default mouse cursor
    virtual void setDefaultMouseCursor(const I_ImageSet& _imageSet, const std::string& _imageName) = 0;

    /// Set the mouse cursor
    virtual void setMouseCursor(const I_ImageSet& _imageSet, const std::string& _imageName) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_WidgetService();
    virtual ~I_WidgetService();
    /// @}

};  // interface I_WidgetService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCrazyEddie
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZCRAZYEDDIE_I_WIDGET_SERVICE_HPP_INCLUDED
