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

#include "GroupViewProxy.hpp"
#include "AccountService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>

#include <Zen/Community/AccountProtocol/I_GroupModelUpdateMessage.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupViewProxy::GroupViewProxy(AccountService& _service, pEndpoint_type _pEndpoint)
:   m_service(_service)
,   m_pEndpoint(_pEndpoint)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupViewProxy::~GroupViewProxy()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupViewProxy::onAddGroup(Common::I_Group& _group)
{
    Protocol::I_GroupModelUpdateMessage::pMessage_type pMessage =
        Protocol::I_GroupModelUpdateMessage::create(
            pEndpoint_type(),
            m_pEndpoint
        );

    typedef Memory::managed_ptr<Protocol::I_GroupModelUpdateMessage>  pConcreteMessage_type;
    pConcreteMessage_type pConcreteMessage = pMessage.as<pConcreteMessage_type>();

    pConcreteMessage->setGroupName(_group.getGroupName());
    pConcreteMessage->setGroupDescription(_group.getGroupDescription());
    pConcreteMessage->setGroupId(_group.getGroupId());
    pConcreteMessage->setUpdateKind(Protocol::I_GroupModelUpdateMessage::ADD);

    m_service.getApplicationServer().handleMessage(pMessage);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupViewProxy::onRemoveGroup(Common::I_Group& _group)
{
    Protocol::I_GroupModelUpdateMessage::pMessage_type pMessage =
        Protocol::I_GroupModelUpdateMessage::create(
            pEndpoint_type(),
            m_pEndpoint
        );

    typedef Memory::managed_ptr<Protocol::I_GroupModelUpdateMessage>  pConcreteMessage_type;
    pConcreteMessage_type pConcreteMessage = pMessage.as<pConcreteMessage_type>();

    pConcreteMessage->setGroupName(_group.getGroupName());
    pConcreteMessage->setGroupDescription(_group.getGroupDescription());
    pConcreteMessage->setGroupId(_group.getGroupId());
    pConcreteMessage->setUpdateKind(Protocol::I_GroupModelUpdateMessage::REMOVE);

    m_service.getApplicationServer().handleMessage(pMessage);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupViewProxy::onUpdateGroup(Common::I_Group& _group)
{
    Protocol::I_GroupModelUpdateMessage::pMessage_type pMessage =
        Protocol::I_GroupModelUpdateMessage::create(
            pEndpoint_type(),
            m_pEndpoint
        );

    typedef Memory::managed_ptr<Protocol::I_GroupModelUpdateMessage>  pConcreteMessage_type;
    pConcreteMessage_type pConcreteMessage = pMessage.as<pConcreteMessage_type>();

    pConcreteMessage->setGroupName(_group.getGroupName());
    pConcreteMessage->setGroupDescription(_group.getGroupDescription());
    pConcreteMessage->setGroupId(_group.getGroupId());
    pConcreteMessage->setUpdateKind(Protocol::I_GroupModelUpdateMessage::UPDATE);

    m_service.getApplicationServer().handleMessage(pMessage);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
