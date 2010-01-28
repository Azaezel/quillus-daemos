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
//	Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_COMMUNITY_SESSIONPROTOCOL_I_DATA_RESPONSE_HPP_INCLUDED
#define ZEN_COMMUNITY_SESSIONPROTOCOL_I_DATA_RESPONSE_HPP_INCLUDED

#include "Configuration.hpp"

#include "I_Response.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
    namespace Common {
        class I_Attribute;
    }   // namespace Common
namespace Protocol {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class SESSIONPROTOCOL_DLL_LINK I_DataResponse
:   public I_Response
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_DataResponse>        pDataResponse_type;
    typedef Zen::Memory::managed_weak_ptr<I_DataResponse>   wpDataResponse_type;

    enum NumericResponse_type
    {
        FAIL = 0,
        SUCCESS = 1
    };
    /// @}

    /// @name I_DataResponse interface
    /// @{
public:
    /// Get the numeric response to the login request
    virtual NumericResponse_type getResponse() const = 0;

    /// Get the key of this data message.
    virtual const std::string& getKey() const = 0;

    /// Get the value of this data message
    virtual const std::string& getValue() const = 0;
    /// @}

	/// @name Static methods
	/// @{
public:
	static pResponse_type create(pEndpoint_type _pSourceEndpoint,
                                 pEndpoint_type _pDestinationEndpoint,
                                 unsigned int _messageId, 
                                 NumericResponse_type _response, 
                                 const Common::I_Attribute& _attribute);
	/// @}

    /// @name 'Structors
    /// @{
protected:
             I_DataResponse();
    virtual ~I_DataResponse();
    /// @}

};  // interface I_DataResponse

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Protocol
}   // namespace Community
namespace Memory {
    // I_DataResponse is managed by factory
    template<>
	struct is_managed_by_factory<Zen::Community::Protocol::I_DataResponse> 
        :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SESSIONPROTOCOL_I_DATA_RESPONSE_HPP_INCLUDED
