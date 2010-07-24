//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#ifndef ZEN_ZOGRE_NULL_CONTEXT_HPP_INCLUDED
#define ZEN_ZOGRE_NULL_CONTEXT_HPP_INCLUDED

#include <Zen/Engine/Rendering/I_Context.hpp>

#include <OgreRoot.h>

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class NullContext
:   public Zen::Engine::Rendering::I_Context
{
    /// @name I_Context interface
    /// @{
public:
    virtual void setFullScreen(bool _fullScreen)
    {
        m_bIsFullScreen = _fullScreen;
    }
    virtual bool isFullScreen() const
    {
        return m_bIsFullScreen;
    }

    virtual parent_window_handle_type getParentWindow() const
    {
#ifdef WIN32
        return m_pParent;
#else
        return m_parent.c_str();
#endif
    }

    virtual window_handle_type getWindow() const
    {
        return m_pHandle;
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
    NullContext(Zen::Scripting::script_module& _module, parent_window_handle_type _pParent)
    :   m_bIsFullScreen(false)
#ifdef WIN32
    ,   m_pParent(_pParent)
#else
    ,   m_parent(_pParent)
#endif
    ,   m_pHandle(NULL)
    ,   m_module(_module)
    ,   m_pScriptObject(NULL)
    {
    }

    virtual ~NullContext()
    {
    }
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject()
    {
        // TODO Make thread safe?
        if (m_pScriptObject == NULL)
        {
            m_pScriptObject = new ScriptObjectReference_type(
                m_module.getScriptModule(),
                m_module.getScriptModule()->getScriptType(getScriptTypeName()),
                this
            );
        }
        return m_pScriptObject;
    }
    /// @}

    /// @name Member Variables
    /// @{
public:
    bool    m_bIsFullScreen;

#ifdef WIN32
    parent_window_handle_type  m_pParent;
#else
    // On non-Win32 platforms, this provides a place to keep a copy of the
    // string.
    std::string m_parent;
#endif

    window_handle_type  m_pHandle;
private:
    ScriptObjectReference_type*     m_pScriptObject;
    Zen::Scripting::script_module&  m_module;
    /// @}

};  // class NulLContext


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_NULL_CONTEXT_HPP_INCLUDED
