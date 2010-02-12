//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_PARSER_STATE_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_PARSER_STATE_HPP_INCLUDED

#include <boost/array.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace HTTP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class RequestHandler;

class RequestParserState
{
    /// @name Types
    /// @{
public:
    typedef char                                    char_t;
    typedef boost::array<char, 8192>::iterator      iterator_t;
    /// @}

    /// @name RequestParserState implementation
    /// @{
public:
    /// Reset so that this object can be reused
    void reset(RequestHandler* _pHandler);

    /// Got a "GET" method
    void get(iterator_t _First, iterator_t const& _Last);

    /// Set the fully qualified URL for this request
    void url(iterator_t _First, iterator_t const& _Last);

    /// Finished parsing the request
    void done(iterator_t _First, iterator_t const& _Last);

    const std::string& getURL() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             RequestParserState();
    virtual ~RequestParserState();
    /// @}

    /// @name Member Variables
    /// @{
private:
    RequestHandler*         m_pHandler;
    std::string             m_url;
    /// @}

};  // class RequestParser

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace HTTP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_PARSER_STATE_HPP_INCLUDED
