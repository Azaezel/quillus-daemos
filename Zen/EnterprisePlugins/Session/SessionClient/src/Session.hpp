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
#ifndef ZEN_ENTERPRISE_SESSION_CLIENT_SESSION_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_CLIENT_SESSION_HPP_INCLUDED

#include "../I_Session.hpp"

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Session.
/// This represents a stateful session.
class Session
:	public Zen::Enterprise::Session::Client::I_Session
{
	/// @name Types
	/// @{
public:
	typedef ScriptObjectReference_type						ScriptWrapper_type;
	typedef ScriptWrapper_type*								pScriptWrapper_type;
	/// @}

	/// @name I_ScriptableType implementation
	/// @{
public:
	virtual Zen::Scripting::I_ObjectReference* getScriptObject();
	/// @}

	/// @name I_Session implemenation
	/// @{
public:
	virtual SessionState_type getSessionState() const;
	virtual const pEndpoint_type getEndpoint() const;
    virtual const pResourceLocation_type getLocation() const;
    virtual const SessionId& getSessionId() const;
	/// @}

	/// @name Session implementation
	/// @{
public:
    int scriptGetSessionState();
    void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);
    void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
	/// @}

    /// @name Inner classes
    /// @{
public:
    class NativeSessionId
    :   public SessionId::I_NativeSessionId
    {
        /// @name Friend declarations
        /// @{
    private:
        friend class Session;
        /// @}

        /// @name I_NativeSessionId implementation
        /// @{
    public:
        virtual bool operator==(const SessionId::I_NativeSessionId& _otherId) const;
        virtual bool operator!=(const SessionId::I_NativeSessionId& _otherId) const;
        virtual bool operator< (const SessionId::I_NativeSessionId& _otherId) const;
        virtual I_NativeSessionId* clone() const;
        virtual std::string toString() const;
        virtual void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);
        virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
        /// @}

        /// @name NativeSessionId implementation
        /// @{
    public:
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 NativeSessionId() : m_nativeSessionId(0) {}
                 NativeSessionId(boost::uint64_t const _id) : m_nativeSessionId(_id) {}
        virtual ~NativeSessionId() {}
        /// @}

        /// @name Member Variables
        /// @{
    private:
        boost::uint64_t m_nativeSessionId;
        /// @}

    };  // class NativeSessionId
    /// @}

	/// @name Static Methods
	/// @{
public:
	static void registerScriptModule(Zen::Scripting::script_module& _module);
	/// @}

	/// @name 'Structors
	/// @{
public:
             Session();
             Session(const Session& _session);
             Session(pEndpoint_type _pDestination);
			 Session(pEndpoint_type _pDestination, pResourceLocation_type _pLocation);
	virtual ~Session();
	/// @}

	/// @name Member Variables
	/// @{
private:
	pEndpoint_type				m_pEndpoint;
    pResourceLocation_type      m_pLocation;

	SessionId				    m_sessionId;
	SessionState_type			m_sessionState;

	pScriptWrapper_type			m_pScriptObject;
	/// @}

};	// class Session

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Client
}	// namespace Session
}	// namespace Enterprise
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_CLIENT_SESSION_HPP_INCLUDED
