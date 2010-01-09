//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
#ifndef ZEN_ZOGRE_ANIMATION_STATE_HPP_INCLUDED
#define ZEN_ZOGRE_ANIMATION_STATE_HPP_INCLUDED

#include <Zen/Engine/Rendering/I_AnimationState.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <OgreRoot.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Engine {
        namespace Rendering {
            class I_RenderableResource;
        }   // namespace Rendering
    }   // namespace Engine
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class AnimationState
:   public Zen::Engine::Rendering::I_AnimationState
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_AnimationState implementation
    /// @{
public:
    virtual const std::string& getAnimationName() const;
    virtual Zen::Math::Real getTimePosition() const;
    virtual void setTimePosition(Zen::Math::Real _timePosition);
    virtual Zen::Math::Real getLength() const;
    virtual void setLength(Zen::Math::Real _length);
    virtual Zen::Math::Real getWeight() const;
    virtual void setWeight(Zen::Math::Real _weight);
    virtual void addTime(Zen::Math::Real _offset);
    virtual bool hasEnded() const;
    virtual bool isEnabled() const;
    virtual void setEnabled(bool _isEnabled);
    virtual void setLooping(bool _isLooping);
    virtual bool isLooping() const;
    virtual void copyStateFrom(const I_AnimationState& _state);
    /// @}

    /// @name AnimationState implementation
    /// @{
public:
    const Ogre::AnimationState& getOgreAnimationState() const;
    void setState(Ogre::AnimationState& _state);
    /// @}

    /// @name 'Structors
    /// @{
public:
             AnimationState();
             AnimationState(Ogre::AnimationState& _animationState);
    virtual ~AnimationState();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Ogre::AnimationState*       m_pAnimationState;
    /// @}

};  // class AnimationState

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_ANIMATION_STATE_HPP_INCLUDED
