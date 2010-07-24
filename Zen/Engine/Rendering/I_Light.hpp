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
#ifndef ZEN_ENGINE_RENDERING_I_LIGHT_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_LIGHT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Engine/Rendering/Color.hpp>
#include <Zen/Engine/Rendering/I_AttachableObject.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Basic light interface
class RENDERING_DLL_LINK I_Light
:   public I_AttachableObject
,   public Memory::managed_self_ref<Zen::Engine::Rendering::I_Light>
{
    /// @name Types
    /// @{
public:
	enum LightType_enum
	{
		POINT = 0,
		DIRECTIONAL = 1,
		SPOTLIGHT = 2
	};

    typedef Zen::Memory::managed_ptr<I_Light>           pScriptObject_type;
    typedef Scripting::ObjectReference<I_Light>         ScriptObjectReference_type;
    typedef ScriptObjectReference_type                  ScriptWrapper_type;
    typedef ScriptWrapper_type*                         pScriptWrapper_type;

    typedef Zen::Memory::managed_ptr<I_Light>           pLight_type;
    typedef Zen::Memory::managed_weak_ptr<I_Light>      wpLight_type;
    typedef Zen::Event::Event<wpLight_type>             lightEvent_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name I_Light interface
    /// @{
public:
    virtual void setPosition(Math::Real _x, Math::Real _y, Math::Real _z) = 0;
    virtual Math::Point3 getPosition() const = 0;
    virtual void setDirection(Math::Real _x, Math::Real _y, Math::Real _z) = 0;
    virtual Math::Vector3 getDirection() const = 0;
    virtual void setDiffuse(Math::Real _r, Math::Real _g, Math::Real _b, Math::Real _a) = 0;
    virtual Rendering::Color getDiffuse() const = 0;
    virtual void setSpecular(Math::Real _r, Math::Real _g, Math::Real _b, Math::Real _a) = 0;
    virtual Rendering::Color getSpecular() const = 0;
    virtual void setAttenuation(Math::Real _range, Math::Real _constant, Math::Real _linear, Math::Real _quadratic) = 0;
    virtual Math::Vector4 getAttenuation() const = 0;
    virtual void setRange(Math::Real _x0, Math::Real _x1, Math::Real _x2) = 0;
    virtual Math::Vector3 getRange() const = 0;
    //virtual void setType(LightType_enum _type) = 0;
    virtual void setType(int _type) = 0;				/// This is a hack until we can handle enums in script.
    virtual LightType_enum getType() const = 0;
    virtual void enableShadowCasting(bool _isEnabled) = 0;
    virtual bool shadowCastingEnabled() const = 0;
    virtual void setPower(Math::Real _power) = 0;
    virtual Math::Real getPower() const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    lightEvent_type     onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Light();
    virtual ~I_Light();
    /// @}

};	// interface I_Light

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory {
    // I_Light is managed by I_SceneService
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_Light> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_LIGHT_HPP_INCLUDED
