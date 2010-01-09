//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
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
#ifndef INDIEZEN_THREESPACE_I_ANIMATED_MODEL_HPP_INCLUDED
#define INDIEZEN_THREESPACE_I_ANIMATED_MODEL_HPP_INCLUDED

#include <IndieZen/framework/I_Model.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace ThreeSpace {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Animated Model
/// @author Tony Richards
/// @since 1.0
class THREESPACE_DLL_LINK I_AnimatedModel
:   public I_Model
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_AnimatedModel interface
    /// @{ 
public:
    /// Select the sequence for this model and set the starting offset.
    ///
    /// @param _name Name of the sequence to select for the entire model including all
    ///             sub-meshes.
    /// @param _start Starting point (0 is beginning, 1.0 is end)
    virtual void selectSequence(const std::string& _name, const TimeDelta& _start) = 0;

    /// Increment the currently selected sequence by the specified amount of time.
    virtual void incrementSequence(const TimeDelta& _timeDelta) = 0;
    /// @}

    /// @{ 'Structors
protected:
             I_AnimatedModel();
    virtual ~I_AnimatedModel();
    /// @}

};  // interface I_AnimatedModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ThreeSpace
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_THREESPACE_I_ANIMATED_MODEL_HPP_INCLUDED

