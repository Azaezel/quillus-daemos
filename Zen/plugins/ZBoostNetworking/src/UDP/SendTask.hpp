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

#include "../UserDatagramProtocolService.hpp"

#include <Zen/Core/Threading/ThreadPool.hpp>

#include <Zen/Enterprise/AppServer/I_Message.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace UDP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SendTask
:   public Zen::Threading::ThreadPool::PooledTask
{
    /// @name Types
    /// @{
public:
    typedef Zen::Enterprise::AppServer::I_ApplicationServer::pMessage_type  pMessage_type;
    typedef Zen::Enterprise::AppServer::I_ProtocolService::pEndpoint_type   pEndpoint_type;
    /// @}

    /// @name PooledTask implementation
    /// @{
public:
    virtual void call()
    {
        try
        {
            m_pParent->handleSendTo(m_pMessage, m_pEndpoint);
        }
        catch(...)
        {
            /// @todo Log an error!
        }
    }
    /// @}

    /// @name SendTask implementation
    /// @{
public:
    void initialize(UserDatagramProtocolService* _pParent,
                    pMessage_type _pMessage,
                    pEndpoint_type _pEndpoint)
    {
        m_pParent = _pParent;
        m_pMessage = _pMessage;
        m_pEndpoint = _pEndpoint;
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
    SendTask(Zen::Threading::ThreadPool::TaskPool& _parent) : Zen::Threading::ThreadPool::PooledTask(_parent) {}
    /// @}

    /// @name Member variables
    /// @{
private:
    UserDatagramProtocolService*    m_pParent;
    pMessage_type                   m_pMessage;
    pEndpoint_type                  m_pEndpoint;
    /// @}

};  // SendTask class

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UDP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
