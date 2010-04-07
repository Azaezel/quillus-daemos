//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_COMMUNITY_ACCOUNTCOMMON_I_PERMISSIBLE_RESOURCE_HPP_INCLUDED
#define ZEN_COMMUNITY_ACCOUNTCOMMON_I_PERMISSIBLE_RESOURCE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Account;
class I_Group;

/// I_PermissibleResource
class ACCOUNTCOMMON_DLL_LINK I_PermissibleResource
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_PermissibleResource interface
    /// @{
public:
    virtual boost::uint32_t getAccessFlags() const = 0;             // TODO Abstract access flags to an I_Permissions interface?
    virtual void setAccessFlags(boost::uint32_t _accessFlags) = 0;  // TODO Abstract access flags to an I_Permissions interface?
    virtual const I_Account& getOwner() const = 0;
    virtual void setOwner(const I_Account& _owner) = 0;
    virtual const I_Group& getGroup() const = 0;
    virtual void setGroup(const I_Group& _group) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PermissibleResource();
    virtual ~I_PermissibleResource();
    /// @}

};  // interface I_PermissibleResource

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_ACCOUNTCOMMON_I_PERMISSIBLE_RESOURCE_HPP_INCLUDED
