//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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
#ifndef ZEN_ENTERPRISE_APPSERVER_MESSAGE_SERIALIZATION_ADAPTER_TEMPLATE_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_MESSAGE_SERIALIZATION_ADAPTER_TEMPLATE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include "message_factory.hpp"
#include "I_MessageSerializationAdapter.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace Deprecated {  // MG - This message serialization adapter is being deprecated
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<class message_type>
class message_serialization_adapter
:   public I_MessageSerializationAdapter
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name message_serialization_adapter implementation
    /// @{
public:
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive,
                           pMessage_type _pMessage)
    {
        message_type* pDerivedMessage =
            dynamic_cast<message_type*>(_pMessage.get());

        if( pDerivedMessage != NULL )
        {
            _archive & *pDerivedMessage;
        }
        else
        {
            // TODO Error
        }
    }

    virtual pMessage_type deserialize(boost::archive::polymorphic_iarchive& _archive,
                                      pEndpoint_type _pSourceEndpoint,
                                      pEndpoint_type _pDestinationEndpoint)
    {
        pMessage_type pMessage = message_factory<message_type>::getSingleton().create(_pSourceEndpoint, _pDestinationEndpoint);

        message_type* pDerivedMessage = 
            dynamic_cast<message_type*>(pMessage.get());

        if( pDerivedMessage != NULL )
        {
            _archive & *pDerivedMessage;

            return pMessage;
        }
        else
        {
            // TODO Error
            return pMessage_type();
        }
    }
    /// @}

    /// @name Static methods
    /// @{
public:
    static message_serialization_adapter<message_type>& getSingleton()
    {
        static message_serialization_adapter<message_type> sm_adapter;

        return sm_adapter;
    }
    /// @}

    /// @name 'Structors
    /// @{
protected:
             message_serialization_adapter() {}
    virtual ~message_serialization_adapter() {}
    /// @}

};  // class message_serialization_adapter<message_type>


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Deprecated
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_MESSAGE_SERIALIZATION_ADAPTER_TEMPLATE_HPP_INCLUDED
