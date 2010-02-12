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
#ifndef ZEN_ENGINE_CINEMATICS_I_CINEMATIC_STEP_HPP_INCLUDED
#define ZEN_ENGINE_CINEMATICS_I_CINEMATIC_STEP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
    namespace Rules {
        class I_Event;
    }
namespace Cinematics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_CinematicStepData;

/// Basic cinematic step interface
class CINEMATICS_DLL_LINK I_CinematicStep
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Rules::I_Event>     pEvent_type;

    /// @name I_CinematicStep interface
    /// @{
public:
    /// Add an event that will cause this step to automatically transition
    /// to the next step in the sequence.  This is only suitable for DFSM style
    /// sequences.
    virtual void addTransitionEvent(pEvent_type _pEvent) = 0;

    /// Add an event that will cause this step to automatically transition
    /// to the specified step.
    virtual void addTransitionEvent(pEvent_type _pEvent, const I_CinematicStep& _nextStep) = 0;

    /// Get the user data associated with this step.
    /// @param _pData The user data to be associated with this step.
    ///     The I_CinematicStep object does not take ownership of
    ///     _pData and will not destroy it.  NULL is a valid value
    ///     if you wish to reset the data to nothing.
    virtual void setData(I_CinematicStepData* _pData) = 0;

    /// Set the user data associated with this step.
    /// @return The user data associated with this step or NULL
    virtual I_CinematicStepData* getData() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_CinematicStep();
    virtual ~I_CinematicStep();
    /// @}

};	// interface I_CinematicStep

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Cinematics
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_CINEMATICS_I_CINEMATIC_STEP_HPP_INCLUDED
