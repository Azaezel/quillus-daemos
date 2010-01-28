//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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
#ifndef ZEN_DATABASE_I_DATABASE_COMMAND_HPP_INCLUDED
#define ZEN_DATABASE_I_DATABASE_COMMAND_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DatabaseConnection;
class I_DatabaseResult;

class DATABASE_DLL_LINK I_DatabaseCommand
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_DatabaseCommand>         pDatabaseCommand_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseCommand>    wpDatabaseCommand_type;
    typedef Zen::Event::Event<wpDatabaseCommand_type>           databaseCommandEvent_type;

    typedef Zen::Memory::managed_ptr<I_DatabaseConnection>      pConnection_type;
    typedef Zen::Memory::managed_ptr<I_DatabaseResult>          pDatabaseResult_type;
    typedef Zen::Event::Event<pDatabaseResult_type>             databaseResultEvent_type;
    /// @}

    /// @name I_DatabaseCommand interface
    /// @{
public:
    virtual void execute(pConnection_type _pConnection) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    databaseCommandEvent_type   onDestroyEvent;
    databaseResultEvent_type    onNotifyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DatabaseCommand();
    virtual ~I_DatabaseCommand();
    /// @}

};  // interface I_DatabaseCommand

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
namespace Memory {
    // I_DatabaseCommand is managed by I_DatabaseTransaction
    template<>
    struct is_managed_by_factory<Database::I_DatabaseCommand> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_DATABASE_I_DATABASE_COMMAND_HPP_INCLUDED
