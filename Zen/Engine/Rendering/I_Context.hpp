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
#ifndef ZEN_ENGINE_RENDERING_I_CONTEXT_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_CONTEXT_HPP_INCLUDED

#include "Configuration.hpp"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
	namespace Camera {
		class I_Camera;
	}
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Rendering context
class RENDERING_DLL_LINK I_Context
{
    /// @name Types
    /// @{
public:
#ifdef _WIN32
    typedef HWND                                    parent_window_handle_type;
    typedef HWND                                    window_handle_type;
#else
    // On X11, this is a string in the format of display:screen:window
    typedef void*                                   window_handle_type;
    typedef const char*                             parent_window_handle_type;
#endif
    /// @}

    /// @name I_Context interface
    /// @{
public:
    virtual void setFullScreen(bool _fullScreen) = 0;
    virtual bool isFullScreen() const = 0;

    /// Get the parent window.
    /// This value is used when creating a new view.
    virtual parent_window_handle_type getParentWindow() const = 0;

    /// Get the window.
    /// This value is not valid until this context has been used
    /// to create a view.
    virtual window_handle_type getWindow() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Context();
    virtual ~I_Context();
    /// @}

};	// interface I_Context

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_ENGINE_RENDERING_I_CONTEXT_HPP_INCLUDED
