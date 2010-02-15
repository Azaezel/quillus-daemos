//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_ZOGRE_RENDERING_SERVICE_HPP_INCLUDED
#define ZEN_ZOGRE_RENDERING_SERVICE_HPP_INCLUDED

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Engine/Rendering/I_RenderingService.hpp>

#include <OgreRoot.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class RenderingService
:   public Zen::Engine::Rendering::I_RenderingService
,   public Memory::managed_self_ref<Zen::Engine::Rendering::I_RenderingService>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<RenderingService>              pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<RenderingService>       ScriptObjectReference_type;
    typedef ScriptObjectReference_type                              ScriptWrapper_type;
    typedef ScriptWrapper_type*                                     pScriptWrapper_type;

    typedef Zen::Memory::managed_ptr<Zen::Scripting::I_ScriptModule>    pScriptModule_type;
    typedef Zen::Memory::managed_ptr<Zen::Scripting::I_ScriptEngine>    pScriptEngine_type;
    /// @}


    /// @name I_RenderingService implementation
    /// @{
public:
    virtual Zen::Engine::Rendering::I_Context* createContext(window_handle_type _pParent);
    virtual void destroyContext(Zen::Engine::Rendering::I_Context* _pContext);
    virtual Zen::Engine::Rendering::I_View* createView(Zen::Engine::Rendering::I_Context& _context, const std::string& _windowName, unsigned int _width, unsigned int _height);
    virtual void destroyView(Zen::Engine::Rendering::I_View* _pView);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_ScriptableService implementation.
    /// @{
public:
    virtual void registerScriptEngine(pScriptEngine_type _pScriptEngine);
    /// @}

    /// @name RenderingService implementation
    /// @{
    bool showConfigDialog();
    /// @}

    /// @name 'Structors
    /// @{
public:
             RenderingService();
    virtual ~RenderingService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Ogre::Root&                     m_root;
    ScriptObjectReference_type*     m_pScriptObject;
    Zen::Scripting::script_module*  m_pModule;
    /// @}

};  // class RenderingService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_RENDERING_SERVICE_HPP_INCLUDED
