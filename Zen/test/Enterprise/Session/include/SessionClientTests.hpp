//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Unit Tests
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
#ifndef ZEN_ENTERPRISE_UNIT_TESTS_SESSION_CLIENT_TESTS_HPP_INCLUDED
#define ZEN_ENTERPRISE_UNIT_TESTS_SESSION_CLIENT_TESTS_HPP_INCLUDED

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace UnitTests {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SessionClientTests
:   public CppUnit::TestFixture
{
    // CppUnit macros for setting up the test suite
    CPPUNIT_TEST_SUITE(SessionClientTests);

    CPPUNIT_TEST(testClientLoad);
    CPPUNIT_TEST(testServiceLoad);
    CPPUNIT_TEST(testLoginSuccess);
    CPPUNIT_TEST(testLoginFail);
    CPPUNIT_TEST(testLoginTimeout);

    CPPUNIT_TEST_SUITE_END();
    
public:
    void setUp();
    void tearDown();

    void testClientLoad();
    void testServiceLoad();
    void testLoginSuccess();
    void testLoginFail();
    void testLoginTimeout();
    void testLogout();

private:
    static Zen::Enterprise::AppServer::I_ApplicationServer* sm_pAppServer;
    static bool                                             sm_initialized;
    
};  // class SessionClientTests

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UnitTests
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZEN_ENTERPRISE_UNIT_TESTS_SESSION_CLIENT_TESTS_HPP_INCLUDED
