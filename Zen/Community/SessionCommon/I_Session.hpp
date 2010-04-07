//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_COMMUNITY_SESSIONCOMMON_I_SESSION_HPP_INCLUDED
#define ZEN_COMMUNITY_SESSIONCOMMON_I_SESSION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Event/future_return_value.hpp>

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Attribute;

/// Session Session
class SESSIONCOMMON_DLL_LINK I_Session
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    enum SessionState_type
    {
        INITIALIZED,
        CONNECTED,
        NOT_AUTHORIZED,
        DISCONNECTED
    };

    typedef Memory::managed_ptr<Networking::I_Endpoint>         pEndpoint_type;
    typedef Zen::Memory::managed_ptr<I_Attribute>               pAttribute_type;
    typedef Zen::Event::future_return_value<pAttribute_type>    FutureAttribute_type;
    typedef Zen::Memory::managed_ptr<FutureAttribute_type>      pFutureAttribute_type;
	/// @}

    /// @name I_Session interface
    /// @{
public:
    virtual SessionState_type getSessionState() const = 0;

    /// Get the session identifier.
    virtual boost::uint32_t getSessionId() const = 0;

    /// Get the session endpoint.
    virtual const pEndpoint_type getEndpoint() const = 0;

    /// Get session data
    /// Temporarily deprecated.  Should it use a listener instead of a future?
    //virtual pFutureAttribute_type getAttribute(const std::string& _key) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Session();
    virtual ~I_Session();
    /// @}

};  // interface I_Session

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
namespace Memory {
    // I_Session is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Community::Common::I_Session> : public boost::false_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SESSIONCOMMON_I_SESSION_HPP_INCLUDED
