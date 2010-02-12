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
#ifndef ZEN_ENGINE_RENDERING_I_RENDERING_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_RENDERING_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <Zen/Core/Scripting/I_ScriptableService.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifdef WIN32
#include <windows.h>
#endif

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Context;
class I_View;
class I_RenderingServiceFactory;

/// Basic renderer interface
/// This class
class RENDERING_DLL_LINK I_RenderingService
:   public virtual Zen::Scripting::I_ScriptableService
{
    /// @name Types
    /// @{
public:
    typedef std::string                             index_type;
    typedef I_RenderingServiceFactory               factory_type;

    typedef Zen::Memory::managed_ptr<I_RenderingService>        pScriptObject_type;
    typedef Scripting::ObjectReference<I_RenderingService>      ScriptObjectReference_type;

    typedef Zen::Memory::managed_ptr<I_RenderingService>        pRenderingService_type;
    typedef Zen::Memory::managed_weak_ptr<I_RenderingService>   wpRenderingService_type;

    typedef Zen::Event::Event<wpRenderingService_type>          ServiceEvent_type;

#ifdef _WIN32
    typedef HWND                                    window_handle_type;
#else
    // On X11, this is a string in the format of display:screen:window
    typedef const char*                             window_handle_type;
#endif
    /// @}

    /// @name I_RenderingService interface
    /// @{
public:
    /// Create a rendering context
    /// @param _pParent Parent window for this context, or NULL
    ///         if no parent window exists.
    virtual I_Context* createContext(window_handle_type _pParent = NULL) = 0;

    /// Destroy a rendering context
    virtual void destroyContext(I_Context* _pContext) = 0;

    /// Create a render view
    /// @param _context context
    /// @param _windowName name in the render window title bar
    /// @param _height height of the render window
    /// @param _width width of the render window
    virtual I_View* createView(I_Context& _context, const std::string& _windowName = std::string("Zen Engine"), unsigned _width = 800, unsigned _height = 600) = 0;

    /// Destroy a render view
    virtual void destroyView(I_View* _pView) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "RenderingService"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    /// Fired immediately before this object is destroyed.
    /// The payload is about to be destroyed, so do not keep a reference of it around.
    ServiceEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_RenderingService();
    virtual ~I_RenderingService();
    /// @}

};	// interface I_RenderingService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory
{
    /// I_RenderingService is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_RenderingService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_RENDERING_SERVICE_HPP_INCLUDED
