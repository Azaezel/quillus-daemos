//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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
#ifndef ZEN_ENTERPRISE_SESSION_I_SESSION_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_I_SESSION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
	namespace Networking {
		class I_Endpoint;
	}	// namespace Networking
namespace Enterprise {
namespace Session {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ZENSESSION_DLL_LINK I_Session
:   public Zen::Memory::managed_self_ref<I_Session>
,	public Zen::Scripting::I_ScriptableType
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

    struct SessionId;

	typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>		                pEndpoint_type;
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_ResourceLocation>    pResourceLocation_type;

	typedef Zen::Memory::managed_ptr<I_Session>				            pScriptObject_type;
	typedef Zen::Scripting::ObjectReference<I_Session>		            ScriptObjectReference_type;
    /// @}

	/// @name I_ScriptableType implementation
	/// @{
public:
	virtual const std::string& getScriptTypeName();
	/// @}

	/// @name I_Session interface
	/// @{
public:
    /// Get the session state.
    virtual SessionState_type getSessionState() const = 0;

    /// Get the session endpoint.
    virtual const pEndpoint_type getEndpoint() const = 0;

    /// Get the session resource location.
    virtual const pResourceLocation_type getLocation() const = 0;

    /// Get the session Id.
    virtual const SessionId& getSessionId() const = 0;
	/// @}

    /// @name Inner Classes
    /// @{
public:
    struct SessionId
    {
        /// @name Inner Interfaces
        /// @{
    public:
        class I_NativeSessionId
        {
            /// @name Types
            /// @{
        public:
            /// @}

            /// @name I_NativeSessionId interface
            /// @{
        public:
            virtual bool operator==(const I_NativeSessionId& _otherId) const = 0;
            virtual bool operator!=(const I_NativeSessionId& _otherId) const = 0;
            virtual bool operator< (const I_NativeSessionId& _otherId) const = 0;
            virtual I_NativeSessionId* clone() const = 0;
            virtual std::string toString() const = 0;
            virtual void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version) = 0;
            virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version) = 0;
            /// @}

            /// @name 'Structors
            /// @{
        public:
                     I_NativeSessionId() {}
            virtual ~I_NativeSessionId() {}
            /// @}

        };  // interface I_NativeSessionId
        /// @}

        /// @name SessionId implementation
        /// @{
    public:
        SessionId& operator= (const SessionId& _otherId)       {delete m_pNativeSessionId; m_pNativeSessionId = _otherId.m_pNativeSessionId->clone(); return *this;}
        bool       operator==(const SessionId& _otherId) const {return (m_pNativeSessionId == _otherId.m_pNativeSessionId) || ( m_pNativeSessionId &&  _otherId.m_pNativeSessionId && (*m_pNativeSessionId == *_otherId.m_pNativeSessionId));}
        bool       operator!=(const SessionId& _otherId) const {return (m_pNativeSessionId != _otherId.m_pNativeSessionId) && (!m_pNativeSessionId || !_otherId.m_pNativeSessionId || (*m_pNativeSessionId != *_otherId.m_pNativeSessionId));}
        bool       operator< (const SessionId& _otherId) const {return (m_pNativeSessionId <  _otherId.m_pNativeSessionId);}
        operator std::string ()                          const {return m_pNativeSessionId ? m_pNativeSessionId->toString() : "";}
        std::string toString ()                          const {return m_pNativeSessionId ? m_pNativeSessionId->toString() : "";}
        
        void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version) 
        {
            _archive & *m_pNativeSessionId; 
        }

        void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
        {
            _archive & *m_pNativeSessionId;
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
         SessionId(I_NativeSessionId* const _sessionId = NULL) : m_pNativeSessionId(_sessionId)                           {}
         SessionId(const SessionId& _otherId)                  : m_pNativeSessionId(_otherId.m_pNativeSessionId->clone()) {}
        ~SessionId()                                                                                                      {delete m_pNativeSessionId;}
        /// @}

        /// @name Member variables
        /// @{
    public:
        I_NativeSessionId* m_pNativeSessionId;
        /// @}

    };  // struct SessionId
    /// @}

	/// @name 'Structors
	/// @{
protected:
			 I_Session();
             I_Session(const I_Session& _session);
	virtual ~I_Session();
	/// @}

};	// interface I_Session

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Session
}	// namespace Enterprise
namespace Memory {
	// I_Session is managed by factory
	template<>
	struct is_managed_by_factory<Zen::Enterprise::Session::I_Session>
	:	public boost::true_type{};
}	// namespace Memory
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_I_SESSION_HPP_INCLUDED
