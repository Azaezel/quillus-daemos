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
#ifndef ZEN_ENGINE_UNIT_TESTS_SOUND_TESTS_HPP_INCLUDED
#define ZEN_ENGINE_UNIT_TESTS_SOUND_TESTS_HPP_INCLUDED

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <Zen/Core/Plugins/I_PluginManager.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace UnitTests {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SoundTests
:   public CppUnit::TestFixture
{
    // CppUnit macros for setting up the test suite
    CPPUNIT_TEST_SUITE(SoundTests);

    CPPUNIT_TEST(testOpenALServiceLoad);
    CPPUNIT_TEST(testOpenALResourceServiceLoad);
    CPPUNIT_TEST(testOpenALResourceLoad);
    CPPUNIT_TEST(testOpenALSourceCreation);
    CPPUNIT_TEST(testOpenALSourceMotion);
    CPPUNIT_TEST(testOpenALListenerMotion);

    CPPUNIT_TEST(testFMODServiceLoad);
    CPPUNIT_TEST(testFMODResourceServiceLoad);
    CPPUNIT_TEST(testFMODResourceLoad);
    CPPUNIT_TEST(testFMODSourceCreation);
    CPPUNIT_TEST(testFMODSourceMotion);
    CPPUNIT_TEST(testFMODListenerMotion);

    CPPUNIT_TEST_SUITE_END();
    
public:
    void setUp();
    void tearDown();
    
    void testOpenALServiceLoad();
    void testOpenALResourceServiceLoad();
    void testOpenALResourceLoad();
    void testOpenALSourceCreation();
    void testOpenALSourceMotion();
    void testOpenALListenerMotion();
    
    void testFMODServiceLoad();
    void testFMODResourceServiceLoad();
    void testFMODResourceLoad();
    void testFMODSourceCreation();
    void testFMODSourceMotion();
    void testFMODListenerMotion();
private:
    Zen::Plugins::I_PluginManager::app_ptr_type m_pApp;

};  // class SoundTests

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UnitTests
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZEN_ENGINE_UNIT_TESTS_SOUND_TESTS_HPP_INCLUDED
