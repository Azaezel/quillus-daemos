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
#ifndef ZEN_ENGINE_RENDERING_I_ANIMATION_STATE_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_ANIMATION_STATE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_RenderableResource;

class RENDERING_DLL_LINK I_AnimationState
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_AnimationState interface
    /// @{
public:
    /// Get the name of the animation to which this state applies
    virtual const std::string& getAnimationName() const = 0;

    /// Gets the time position for this animation
    virtual Zen::Math::Real getTimePosition() const = 0;

    /// Sets the time position for this animation
    virtual void setTimePosition(Zen::Math::Real _timePosition) = 0;

    /// Gets the total length of this animation (may be shorter than the whole animation).
    virtual Zen::Math::Real getLength() const = 0;

    /// Sets the total length of this animation (may be shorter than the whole animation).
    virtual void setLength(Zen::Math::Real _length) = 0;

    /// Gets the weight (influence) of this animation
    virtual Zen::Math::Real getWeight() const = 0;

    /// Sets the weight (influence) of this animation
    virtual void setWeight(Zen::Math::Real _weight) = 0;

    /// Modifies the time position, adjusting for animation length
    virtual void addTime(Zen::Math::Real _offset) = 0;

    /// Returns true if the animation has reached the end and is not looping
    virtual bool hasEnded() const = 0;

    /// Returns true if this animation is currently enabled
    virtual bool isEnabled() const = 0;

    /// Sets whether this animation is enabled
    virtual void setEnabled(bool _isEnabled) = 0;

    /// Sets whether or not an animation loops at the start and end of
    /// the animation if the time continues to be altered.
    virtual void setLooping(bool _isLooping) = 0;

    /// Gets whether or not this animation loops
    virtual bool isLooping() const = 0;

    /// Copies the states from another animation state, preserving
    /// the animatino name but copying everything else
    virtual void copyStateFrom(const I_AnimationState& _state) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_AnimationState();
public:
    virtual ~I_AnimationState();
    /// @}

};  // interface I_AnimationState

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_ANIMATION_STATE_HPP_INCLUDED
