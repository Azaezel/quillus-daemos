//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_ZOGRE_ATTACHABLE_OBJECT_HPP_INCLUDED
#define ZEN_ZOGRE_ATTACHABLE_OBJECT_HPP_INCLUDED

#include <Zen/Engine/Rendering/I_AttachableObject.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>
#include <Zen/Core/Scripting.hpp>

#include <OgreRoot.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AttachableObject
:   public virtual Zen::Engine::Rendering::I_AttachableObject
,   public Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef AttachableObject*                               pScriptObject_type;
    typedef Scripting::ObjectReference<AttachableObject>    ScriptObjectReference_type;
    typedef ScriptObjectReference_type                      ScriptWrapper_type;
    /// @}

    /// @name I_AttachableObject implementation
    /// @{
public:
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name Static methods
    /// @{
    static void registerScriptModule(Zen::Scripting::script_module& _module);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             AttachableObject();
    virtual ~AttachableObject();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    ScriptWrapper_type*     m_pScriptObject;
    /// @}

};  // class SceneNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_SCENE_NODE_HPP_INCLUDED
