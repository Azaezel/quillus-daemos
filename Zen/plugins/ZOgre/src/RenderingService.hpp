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

#include <Zen/Engine/Rendering/I_RenderingService.hpp>

#include <OgreRoot.h>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class RenderingView;

class RenderingService
:   public Zen::Engine::Rendering::I_RenderingService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Scripting::I_ScriptModule>    pScriptModule_type;
    typedef Zen::Memory::managed_ptr<Zen::Scripting::I_ScriptEngine>    pScriptEngine_type;

    typedef std::map<RenderingView*, std::pair<std::string,int> >       ViewIdx_type;
    typedef std::map<std::pair<std::string,int>, RenderingView*>        ViewMap_type;
    /// @}


    /// @name I_RenderingService implementation
    /// @{
public:
    virtual Zen::Engine::Rendering::I_Context* createContext(window_handle_type _pParent);
    virtual void destroyContext(Zen::Engine::Rendering::I_Context* _pContext);
    virtual Zen::Engine::Rendering::I_View* createView(Zen::Engine::Rendering::I_Context& _context, const std::string& _windowName, unsigned int _width, unsigned int _height);
    virtual void destroyView(Zen::Engine::Rendering::I_View* _pView);
    virtual void registerScriptModule(Zen::Scripting::script_module& _module);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
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

    ViewIdx_type                    m_viewIdx;
    ViewMap_type                    m_viewMap;
    /// @}

};  // class RenderingService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_RENDERING_SERVICE_HPP_INCLUDED
