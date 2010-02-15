//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Matthew Alan Gray
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

#include "BasicConsoleService.hpp"
#include "StandardStream.hpp"

#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/I_Runnable.hpp>
#include <Zen/Core/Threading/ThreadFactory.hpp>
#include <Zen/Engine/Server/I_ConsoleInputEvent.hpp>
#include <Zen/Engine/Server/I_CommandHandler.hpp>

#include <boost/bind.hpp>

/// TODO : Using the SpiritTest as a reference, implement a 
/// I_CommandLineParserService and a basic command parsing 
/// implementation.

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBasicConsole {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_StreamRunnable
:   public Zen::Threading::I_Runnable
{
    /// @name Member variables
    /// @{
private:
    Zen::Engine::Server::I_ConsoleStream& m_stream;
    /// @}

    /// @name I_Runnable implementation
    /// @{
    virtual void run()
    {
        m_stream.run();
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
    I_StreamRunnable(Zen::Engine::Server::I_ConsoleStream& _stream)
        :   m_stream(_stream)
    {}
    virtual ~I_StreamRunnable() {}
    /// @}

};
    
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BasicConsoleService::BasicConsoleService()
:   m_isActive(true)
{
    // Create a mutex instance
    this->m_pConsoleStreamsGuard =
        Zen::Threading::MutexFactory::create();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BasicConsoleService::~BasicConsoleService()
{
    // Destroy the mutex instance
    Zen::Threading::MutexFactory::destroy(this->m_pConsoleStreamsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
BasicConsoleService::registerCommandHandler(const std::string& _command, handler_type _handler, const std::string& _usage)
{
    this->m_pCommandLineParser->registerHandler(_command, _handler, _usage);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
BasicConsoleService::broadcastOutput(const std::string& _output)
{
    Zen::Threading::CriticalSection guard(this->m_pConsoleStreamsGuard);

    for( consoleStreams_type::iterator iter = this->m_consoleStreams.begin() ;
        iter != this->m_consoleStreams.end() ; iter++ )
    {
        *(iter->first) << _output;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
BasicConsoleService::registerStream(pConsoleStream_type& _pStream)
{
    Zen::Threading::CriticalSection guard(this->m_pConsoleStreamsGuard);

    consoleStreams_type::iterator iter = this->m_consoleStreams.find(_pStream);
    if( iter != this->m_consoleStreams.end() )
    {
        // Stream is already registered
        throw std::exception("Zen::ZBasicConsole::BasicConsoleService::registerStream() : Stream is already registered with the ConsoleService.");
    }
    else
    {
        // Connect onInputEvent to _pStream->onConsoleInputEvent
        pConsoleInputEventConnection_type pConsoleInputEventConnection =
        _pStream->onConsoleInputEvent.connect(boost::bind(&BasicConsoleService::onInputEvent,this,_1));

        // Register the stream
        this->m_consoleStreams[_pStream] = 
            Threading::ThreadFactory::create(new I_StreamRunnable(*_pStream));

        this->m_consoleStreamInputEventConnections[_pStream] = 
            pConsoleInputEventConnection;

        this->m_consoleStreams[_pStream]->start();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
BasicConsoleService::unregisterStream(pConsoleStream_type& _pStream)
{
    Zen::Threading::CriticalSection guard(this->m_pConsoleStreamsGuard);

    consoleStreams_type::iterator iter = this->m_consoleStreams.find(_pStream);
    if( iter != this->m_consoleStreams.end() )
    {
        // TODO Code review for deadlocks
        iter->second->stop();
        iter->second->join();
        Threading::ThreadFactory::destroy(iter->second);

        // Unregister the stream
        this->m_consoleStreams.erase(iter);

        // Disconnect onInputEvent from _pStream->onConsoleInputEvent
        consoleStreamInputEventConnections_type::iterator eventIter =
            this->m_consoleStreamInputEventConnections.find(_pStream);
        if( eventIter != this->m_consoleStreamInputEventConnections.end() )
        {
            eventIter->second->disconnect();
            this->m_consoleStreamInputEventConnections.erase(eventIter);
        }

    }
    else
    {
        // The stream is not already registered.
        throw std::exception("Zen::ZBasicConsole::BasicConsoleService::unregisterStream() : Stream was not registered with the ConsoleService.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BasicConsoleService::pConsoleStream_type
BasicConsoleService::createStream()
{
    StandardStream* pRawConsoleStream = new StandardStream(this->getSelfReference().lock());

    pConsoleStream_type pConsoleStream(pRawConsoleStream,
        boost::bind(&BasicConsoleService::onConsoleStreamDestroy, this, _1));

    wpConsoleStream_type pWeakPtr(pConsoleStream);

    pRawConsoleStream->setSelfReference(pWeakPtr);

    return pConsoleStream;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BasicConsoleService::isActive()
{
    return this->m_isActive;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicConsoleService::setActive(bool _isActive)
{
    this->m_isActive = _isActive;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicConsoleService::onInputEvent(Zen::Engine::Server::I_ConsoleInputEvent* _pConsoleInputEvent)
{
    if( this->m_pCommandLineParser.isValid() )
    {
        this->m_pCommandLineParser->parse(_pConsoleInputEvent);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicConsoleService::attachCommandLineParser(pCommandLineParserService_type _pCommandLineParserService)
{
    this->m_pCommandLineParser = _pCommandLineParserService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicConsoleService::onConsoleStreamDestroy(wpConsoleStream_type _wpConsoleStream)
{
    /// Fire the ConsoleStream's onDestroyEvent
    _wpConsoleStream->onDestroyEvent(_wpConsoleStream);

    /// Delete the ConsoleStream
    StandardStream* pConsoleStream = dynamic_cast<StandardStream*>(_wpConsoleStream.get());

    if( pConsoleStream )
    {
        delete pConsoleStream;
    }
    else
    {
        throw std::exception("Zen::ZBasicConsole::BasicConsoleStream::onConsoleStreamDestroy()");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBasicConsole
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
