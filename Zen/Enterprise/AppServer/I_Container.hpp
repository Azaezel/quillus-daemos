//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_ENTERPRISE_APPSERVER_I_CONTAINER_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_I_CONTAINER_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ApplicationServer;

/// Helper container class that sets up an application server.
/// Use this implementation if you wish, but it's not required.
class APPSERVER_DLL_LINK I_Container
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Container interface
    /// @{
public:
    /// Initialize the container.
    /// @return true if the container was able to load the
    ///     configuration file and install all of the application
    ///     services and protocol services.
    /// Parameters:
    ///     --config -c : configuration xml file.
    ///     --lang -l   : scripting plugin to load.
    ///     --script -s : boot script.
    /// 
    virtual bool init(int _argc, const char* _argv[]) = 0;

    /// Run the container.
    virtual int run() = 0;

    /// Stop the container.
    /// @note The container will need to be reinitialized prior to use after this is called.
    virtual void stop() = 0;

    virtual I_ApplicationServer& getApplicationServer() = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    /// Get the singleton container.
    static I_Container& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Container();
    virtual ~I_Container();
    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_I_CONTAINER_HPP_INCLUDED
