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
#ifndef ZEN_COMMUNITY_CHATCOMMON_CHAT_EVENT_MODEL_HPP_INCLUDED
#define ZEN_COMMUNITY_CHATCOMMON_CHAT_EVENT_MODEL_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/I_Event.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>
#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ChatEvent;
class I_ChatEventView;
class I_Session;

class CHATCOMMON_DLL_LINK I_ChatEventModel
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ChatEventView>                          pChatEventView_type;
    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Filter>          pFilter_type;

    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Subscription>    pSubscription_type;
    typedef Event::future_return_value<pSubscription_type>              FutureSubscription_type;
    typedef Memory::managed_ptr<FutureSubscription_type>                pFutureSubscription_type;

    typedef Memory::managed_ptr<I_ChatEvent>                              pChatEvent_type;
    /// @}

    /// @name I_ChatEventModel interface
    /// @{
public:
    /// Subscribe a view to this model.
    /// The subscription should be stored in the view so that when
    /// the view goes out of scope the subscription also goes out
    /// of scope.  The view should also retain a strong reference
    /// to the model.
    /// @param _pView - Unmanaged pointer to the view.  The view MUST remain
    ///     alive for at least as long as the model (which it will if it keeps
    ///     a srong reference to the model).  The view MUST dereference the
    ///     subscription before the view is destroyed.
    virtual pFutureSubscription_type subscribe(Common::I_Session& _session, pChatEventView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    virtual Event::I_Event& getChatEventEvent() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ChatEventModel();
    virtual ~I_ChatEventModel();
    /// @}

};  // interface I_ChatEventModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
namespace Memory {
	/// I_ChatEventModel is managed by factory
	template<>
	struct is_managed_by_factory<Community::Common::I_ChatEventModel> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHATCOMMON_CHAT_EVENT_MODEL_HPP_INCLUDED
