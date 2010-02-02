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
#ifndef ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_GRAMMAR_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_GRAMMAR_HPP_INCLUDED

#include <boost/spirit/core.hpp>
#include <boost/spirit/dynamic.hpp>
#include <boost/spirit/utility/chset.hpp>

#include <boost/array.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace HTTP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class RequestParserState;

struct RequestGrammar
:   public boost::spirit::grammar<RequestGrammar>
{
    /// @name Types
    /// @{
public:
    typedef RequestGrammar                          self_t;
    typedef char                                    char_t;
    typedef boost::array<char, 8192>::iterator      iterator_t;
    typedef boost::spirit::scanner<iterator_t>      scanner_t;
    typedef boost::spirit::rule<scanner_t>          rule_t;

    typedef boost::spirit::chset<unsigned char>     chset_t;
    typedef boost::spirit::chlit<unsigned char>     chlit_t;
    /// @}

    /// @name Scanner
    /// @{
public:
    template <typename ScannerT>
    struct definition
    {
        /// @name definition implementation
        /// @{
    public:
        boost::spirit::rule<ScannerT> const&
        start() const { return request_parser; }
        /// @}

        /// @name 'Structors
        /// @{
    public:
        definition(RequestGrammar const& self);
        /// @}

        /// @name Rules 
        /// @{
    public:
        boost::spirit::rule<ScannerT>
            method_name,
            initial_line, host_line, host_name, port_number,
            generic_header_line, header_line, header_lines,
            anything_but_colon, anything_but_crlf,
            spaces, 
            url, http_version,
            request_parser;
        /// @}

    };  // struct definition
    /// @}

    /// @name RequestParserState implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
    RequestGrammar(RequestParserState& _parserState);
    /// @}

    /// @name Member Variables
    /// @{
public: // public on purpose so that the grammar implementation can use it
    RequestParserState&     m_parserState;
    /// @}

};  // class RequestParser

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace HTTP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_GRAMMAR_HPP_INCLUDED
