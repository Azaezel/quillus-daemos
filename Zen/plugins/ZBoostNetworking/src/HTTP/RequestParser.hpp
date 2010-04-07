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
#if 0 // deprecated
#ifndef ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_PARSER_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_PARSER_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/spirit/include/classic_core.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
    class I_Request;
namespace HTTP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//class Request;
class RequestHandler;

class RequestParser
{
    /// @name Types
    /// @{
public:
    //typedef Memory::managed_ptr<I_Request>          pRequest_type;

    typedef char                                    char_t;
    typedef boost::array<char, 8192>::iterator      iterator_t;
    typedef boost::spirit::scanner<iterator_t>      scanner_t;
    typedef boost::spirit::rule<scanner_t>          rule_t;
    /// @}

    /// @name RequestParser implementation
    /// @{
public:
    void parse(RequestHandler* _pHandler, iterator_t _begin, iterator_t _end);
    /// @}

    /// @name 'Structors
    /// @{
public:
             RequestParser();
    virtual ~RequestParser();
    /// @}

};  // class RequestParser

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace HTTP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_PARSER_HPP_INCLUDED
#endif  // deprecated
