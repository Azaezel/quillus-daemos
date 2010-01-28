//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
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
#ifndef ZEN_COMMUNITY_SESSIONPROTOCOL_I_DATA_REQUEST_HPP_INCLUDED
#define ZEN_COMMUNITY_SESSIONPROTOCOL_I_DATA_REQUEST_HPP_INCLUDED

#include "Configuration.hpp"

#include "I_Request.hpp"

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Protocol {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;
class SESSIONPROTOCOL_DLL_LINK I_DataRequest
:   public I_Request
{

    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_DataRequest>         pDataRequest_type;
    typedef Zen::Memory::managed_weak_ptr<I_DataRequest>    wpDataRequest_type;
    /// @}

    /// @name I_DataRequest interface
    /// @{
public:
    /// Get the session-id for this data request
    virtual boost::int32_t getSessionId() const = 0;

    /// Get the key for this data request
    virtual const std::string& getKey() const = 0;
    /// @}

	/// @name Static methods
	/// @{
public:
	static pRequest_type create(pEndpoint_type _pSourceEndpoint,
                                pEndpoint_type _pDestinationEndpoint,
                                boost::int32_t _sessionId, 
                                const std::string& _key);
	/// @}

    /// @name 'Structors
    /// @{
protected:
             I_DataRequest();
    virtual ~I_DataRequest();
    /// @}

};  // interface I_DataRequest

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Protocol
}   // namespace Community
namespace Memory {
    // I_DataRequest is managed by factory
    template<>
    struct is_managed_by_factory<Community::Protocol::I_DataRequest> 
        :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SESSIONPROTOCOL_I_DATA_REQUEST_HPP_INCLUDED
