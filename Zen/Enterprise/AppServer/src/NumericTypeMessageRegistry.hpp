//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_APPSERVER_NUMERIC_TYPE_MESSAGE_REGISTRY_HPP_INCLUDED
#define ZEN_APPSERVER_NUMERIC_TYPE_MESSAGE_REGISTRY_HPP_INCLUDED

#include "../I_NumericTypeMessageRegistry.hpp"

#include <Zen/Enterprise/AppServer/I_MessageType.hpp>
#include <Zen/Enterprise/AppServer/I_MessageHeader.hpp>

#include <boost/archive/polymorphic_oarchive.hpp>

#include <boost/cstdint.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class NumericTypeMessageRegistry
:   public I_NumericTypeMessageRegistry
{
    /// @name Types
    /// @{
public:
    typedef std::map<boost::uint32_t, pMessageType_type>    MessageTypes_type;
    /// @}

    /// @name I_MessageRegistry implementation
    /// @{
public:
    virtual void registerMessageType(pMessageType_type _pMessageType);
    virtual void unregisterMessageType(pMessageType_type _pMessageType);
    virtual pMessageHeader_type getMessageHeader(boost::archive::polymorphic_iarchive& _archive) const;
    virtual pMessageHeader_type createMessageHeader(pMessageType_type _pMessageType) const;
    /// @}

    /// @name I_NumericTypeMessageRegistry implementation
    /// @{
public:
    virtual pMessageType_type getMessageType(boost::uint32_t _type);
    /// @}

    /// @name Static methods
    /// @{
private:
    static void destroyMessageType(wpMessageType_type _wpMessageType);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ApplicationServer;
             NumericTypeMessageRegistry();
public:
    virtual ~NumericTypeMessageRegistry();
    /// @}

    /// @name Inner types 
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class NumericType
    :   public I_NumericType
    {
    public:
        NumericType(boost::uint32_t _type)
        :   m_type(_type)
        {
        }

        virtual ~NumericType()
        {
        }

        virtual pMessageFactory_type getMessageFactory() const
        {
            return m_pMessageFactory;
        }

        virtual void setMessageFactory(pMessageFactory_type _pMessageFactory)
        {
            m_pMessageFactory = _pMessageFactory;
        }

        virtual boost::uint32_t getType() const
        {
            return m_type;
        }
    private:
        boost::uint32_t         m_type;
        pMessageFactory_type    m_pMessageFactory;
    };  // class NumericType

    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class MessageHeader
    :   public I_MessageHeader
    {
    public:
        MessageHeader(pMessageType_type _pMessageType)
        :   m_pMessageType(_pMessageType)
        {
        }

        virtual ~MessageHeader()
        {
        }

        virtual pMessageType_type getMessageType() const
        {
            return m_pMessageType;
        }

        virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
        {
            boost::uint32_t messageType = m_pMessageType.as<Memory::managed_ptr<NumericType> >()->getType();

            _archive & messageType;
        }
    private:
        pMessageType_type       m_pMessageType;
    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    ///@}

    /// @name Member Variables
    /// @{
private:
    MessageTypes_type           m_messageTypes;
    /// @}

};  // class NumericTypeMessageRegistry

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_NUMERIC_TYPE_MESSAGE_REGISTRY_HPP_INCLUDED
