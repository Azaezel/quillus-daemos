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
#ifndef ZEN_ENTERPRISE_APPSERVER_MESSAGE_FACTORY_TEMPLATE_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_MESSAGE_FACTORY_TEMPLATE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include "I_MessageFactory.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace Deprecated { // TR - This message registry is being deprecated
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

template<class message_type>
class message_factory
:   public I_MessageFactory
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<I_Message>    wpMessage_type;
    /// @}

    /// @name message_factory implementation
    /// @{
public:
    virtual pMessage_type create(pEndpoint_type _pSourceEndpoint, pEndpoint_type _pDestinationEndpoint)
    {
        message_type* pRawMessage = new message_type(_pSourceEndpoint, _pDestinationEndpoint);

        pMessage_type pMessage(pRawMessage,
            boost::bind(&message_factory<message_type>::destroy, this, _1));

        return pMessage;
    }

    void destroy(wpMessage_type _pMessage)
    {
        // Delete the message object
        message_type* pMessage = 
            dynamic_cast<message_type*>(_pMessage.get());

        if( pMessage != NULL )
        {
            delete pMessage;
        }
        else
        {
            throw Zen::Utility::runtime_exception("message_factory::destroy() : Invalid type.");
        }
    }
    /// @}

    /// @name Static methods
    /// @{
public:
    static message_factory<message_type>& getSingleton()
    {
        static message_factory<message_type> sm_factory;

        return sm_factory;
    }
    /// @}

    /// @name 'Structors
    /// @{
protected:
             message_factory() {}
    virtual ~message_factory() {}
    /// @}

};  // message_factory<message_type> class

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Deprecated
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_MESSAGE_FACTORY_TEMPLATE_HPP_INCLUDED
