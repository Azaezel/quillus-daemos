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
#ifndef ZEN_ZOGRE_NULL_LIGHT_HPP_INCLUDED
#define ZEN_ZOGRE_NULL_LIGHT_HPP_INCLUDED

#include <Zen/Engine/Rendering/I_Light.hpp>

#include <OgreRoot.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Default NULL Light for manually manipulating an Ogre Light
class NullLight
:   public Engine::Rendering::I_Light
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Scripting::I_ScriptModule>    pScriptModule_type;
    typedef Zen::Memory::managed_ptr<Zen::Scripting::I_ScriptEngine>    pScriptEngine_type;
    /// @}

    /// @name I_Light implementation
    /// @{
public:
    virtual void setPosition(float _x, float _y, float _z);
    virtual Math::Point3 getPosition() const;
    virtual void setDirection(Math::Real _x, Math::Real _y, Math::Real _z);
    virtual Math::Vector3 getDirection() const;
    virtual void setDiffuse(Math::Real _r, Math::Real _g, Math::Real _b, Math::Real _a);
    virtual Engine::Rendering::Color getDiffuse() const;
    virtual void setSpecular(Math::Real _r, Math::Real _g, Math::Real _b, Math::Real _a);
    virtual Engine::Rendering::Color getSpecular() const;
    virtual void setAttenuation(Math::Real _range, Math::Real _constant, Math::Real _linear, Math::Real _quadratic);
    virtual Math::Vector4 getAttenuation() const;
    virtual void setRange(Math::Real _x0, Math::Real _x1, Math::Real _x2);
    virtual Math::Vector3 getRange() const;
    //virtual void setType(LightType_enum _type);
    virtual void setType(int _type);
    virtual LightType_enum getType() const;
    virtual void enableShadowCasting(bool _isEnabled);
    virtual bool shadowCastingEnabled() const;
    virtual void setPower(Math::Real _power);
    virtual Math::Real getPower() const;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name 'Structors
    /// @{
public:
             NullLight(Zen::Scripting::script_module& _module, Ogre::Light* _pCamera);
    virtual ~NullLight();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ScriptObjectReference_type*     m_pScriptObject;

    Ogre::Light*                    m_pLight;

    Zen::Scripting::script_module&  m_module;
    /// @}

};  // class NullLight

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_NULL_LIGHT_HPP_INCLUDED
