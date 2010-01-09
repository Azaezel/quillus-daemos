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
#ifndef ZEN_ENGINE_MOVEMANAGER_I_MOVE_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_MOVEMANAGER_I_MOVE_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
    namespace Utility {
        class TimeDelta;
    }   // namespace Utility
namespace MoveManager {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Move;
class I_Controllable;
class I_MoveRequest;
class I_MoveRequestFactory;

class MOVEMANAGER_DLL_LINK I_MoveService
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_Move>                pMove_type;
    typedef Zen::Memory::managed_ptr<I_Controllable>        pControllable_type;
    typedef Zen::Memory::managed_ptr<I_MoveRequest>         pMoveRequest_type;
    typedef Zen::Memory::managed_ptr<I_MoveRequestFactory>  pMoveRequestFactory_type;
    /// @}

    /// @name I_MoveService interface
    /// @{
public:
    /// @todo Should this be moved to I_Controllable?
    virtual void move(pMoveRequest_type& _pMoveRequest, pControllable_type& _pControllable) = 0;

    /// @note TR - I'm not sure this will be used.
    virtual void processTick(const Utility::TimeDelta& _delta) = 0;

    /// Register a move request factory.
    virtual void registerMoveRequesetFactory(const unsigned _type, pMoveRequestFactory_type _pFactory) = 0;

    /// Create a move request
    virtual pMoveRequest_type& createMoveRequest(const unsigned _type) = 0;

    /// Create a controllable object
    virtual pControllable_type& createControllable() = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "MoveService"
    /// Override this method if you create a derived type
    const std::string& getScriptTypeName();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_MoveService();
    virtual ~I_MoveService();
    /// @}

};	// interface I_MoveService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace MoveManager
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_MOVEMANAGER_I_MOVE_SERVICE_HPP_INCLUDED
