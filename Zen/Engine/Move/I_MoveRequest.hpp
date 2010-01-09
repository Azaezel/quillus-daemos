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
#ifndef ZEN_ENGINE_MOVE_MANAGER_I_MOVE_REQUEST_HPP_INCLUDED
#define ZEN_ENGINE_MOVE_MANAGER_I_MOVE_REQUEST_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Engine/Core/I_Action.hpp>

namespace boost {
namespace archive {
    class polymorphic_iarchive;
    class polymorphic_oarchive;
} // namespace archive
} // namespace boost

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace MoveManager {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Client driven request to move a controlled object
class I_MoveRequest
// TR - Why inherit I_Action?  I don't remember my thoughts on this matter.
//:   public Core::I_Action 
{
    /// @name I_MoveRequest interface
    /// @{
public:
    /// Deserialize this move request
    virtual void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version) = 0;

    /// Serialize this move request
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "MoveRequest"
    /// Override this method if you create a derived type
    const std::string& getScriptTypeName();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_MoveRequest();
    virtual ~I_MoveRequest();
    /// @}

};	// interface I_MoveRequest

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace MoveManager
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_MOVE_MANAGER_I_MOVE_REQUEST_HPP_INCLUDED
