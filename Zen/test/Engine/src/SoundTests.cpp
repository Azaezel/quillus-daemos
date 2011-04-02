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

#include "Suite.hpp"
#include "SoundTests.hpp"

#include <Zen/Core/Math/Vector3.hpp>

#include <Zen/Core/Plugins/I_PluginManager.hpp>

#include <Zen/Engine/Sound/I_SoundManager.hpp>
#include <Zen/Engine/Sound/I_SoundService.hpp>
#include <Zen/Engine/Sound/I_SoundResource.hpp>
#include <Zen/Engine/Sound/I_SoundSource.hpp>

#include <Zen/Engine/Resource/I_ResourceManager.hpp>
#include <Zen/Engine/Resource/I_ResourceService.hpp>

#include <boost/filesystem.hpp>

#include <conio.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace UnitTests {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Register the suite
CPPUNIT_TEST_SUITE_REGISTRATION(SoundTests);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
SoundTests::setUp()
{
    boost::filesystem::path configPath = boost::filesystem::system_complete
        (
            boost::filesystem::path("SoundTests.xml", boost::filesystem::native)
        ).normalize();

    m_pApp = Zen::Plugins::I_PluginManager::getSingleton()
        .installApplication(configPath);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
SoundTests::tearDown()
{
    m_pApp.reset();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testOpenALServiceLoad()
{
    Zen::Engine::Sound::I_SoundManager::config_type soundConfig;

    Zen::Engine::Sound::I_SoundManager::pService_type pSoundService =
        Zen::Engine::Sound::I_SoundManager::getSingleton()
            .create("ZOpenAL", soundConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound service was not loaded.",
        CPPUNIT_ASSERT(pSoundService.isValid())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testOpenALResourceServiceLoad()
{
    Zen::Engine::Resource::I_ResourceManager::config_type resourceConfig;

    Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton()
            .create("ZOpenAL", resourceConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound resource service was not loaded.",
        CPPUNIT_ASSERT(pResourceService.isValid())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testOpenALResourceLoad()
{
    Zen::Engine::Resource::I_ResourceManager::config_type resourceConfig;

    Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton()
            .create("ZOpenAL", resourceConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound resource service was not loaded.",
        CPPUNIT_ASSERT(pResourceService.isValid())
    );

    pResourceService->addResourceLocation("Engine/resources/sound", "", "", true);

    resourceConfig["fileName"] = "sirabhorn.ogg";

    typedef Zen::Memory::managed_ptr<Zen::Engine::Sound::I_SoundResource>   pSoundResource_type;
    pSoundResource_type pSoundResource = 
        pResourceService
            ->loadResource(resourceConfig)
                .as<pSoundResource_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound resource was not loaded.",
        CPPUNIT_ASSERT(pSoundResource.isValid())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testOpenALSourceCreation()
{
    Zen::Engine::Sound::I_SoundManager::config_type soundConfig;
    Zen::Engine::Resource::I_ResourceManager::config_type resourceConfig;

    Zen::Engine::Sound::I_SoundManager::pService_type pSoundService =
        Zen::Engine::Sound::I_SoundManager::getSingleton()
            .create("ZOpenAL", soundConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound service was not loaded.",
        CPPUNIT_ASSERT(pSoundService.isValid())
    );

    Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton()
            .create("ZOpenAL", resourceConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound resource service was not loaded.",
        CPPUNIT_ASSERT(pResourceService.isValid())
    );

    pResourceService->addResourceLocation("Engine/resources/sound", "", "", true);

    resourceConfig["fileName"] = "sirabhorn.ogg";

    typedef Zen::Memory::managed_ptr<Zen::Engine::Sound::I_SoundResource>   pSoundResource_type;
    pSoundResource_type pSoundResource = 
        pResourceService
            ->loadResource(resourceConfig)
                .as<pSoundResource_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound resource was not loaded.",
        CPPUNIT_ASSERT(pSoundResource.isValid())
    );

    Zen::Engine::Sound::I_SoundSource::pSoundSource_type pSound =
        pSoundService->createSource(pSoundResource);

    char input(' ');
    while(input != 'Y' && input != 'N' && input != 'y' && input != 'n')
    {
        std::cout << "Unit Test - SoundTests::testOpenALSourceCreation() : Is music playing through the sound card successfully? (Y/N) : ";
        std::cin.get(input);
    }
    pSound->stop();
    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound source is not playing through the sound card.",
        CPPUNIT_ASSERT(input == 'Y' || input == 'y')
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testOpenALSourceMotion()
{
    Zen::Engine::Sound::I_SoundManager::config_type soundConfig;
    Zen::Engine::Resource::I_ResourceManager::config_type resourceConfig;

    Zen::Engine::Sound::I_SoundManager::pService_type pSoundService =
        Zen::Engine::Sound::I_SoundManager::getSingleton()
            .create("ZOpenAL", soundConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound service was not loaded.",
        CPPUNIT_ASSERT(pSoundService.isValid())
    );

    Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton()
            .create("ZOpenAL", resourceConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound resource service was not loaded.",
        CPPUNIT_ASSERT(pResourceService.isValid())
    );

    pResourceService->addResourceLocation("Engine/resources/sound", "", "", true);

    resourceConfig["fileName"] = "thunder.wav";

    typedef Zen::Memory::managed_ptr<Zen::Engine::Sound::I_SoundResource>   pSoundResource_type;
    pSoundResource_type pSoundResource = 
        pResourceService
            ->loadResource(resourceConfig)
                .as<pSoundResource_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound resource was not loaded.",
        CPPUNIT_ASSERT(pSoundResource.isValid())
    );

    Zen::Engine::Sound::I_SoundSource::pSoundSource_type pSound =
        pSoundService->createSource(pSoundResource,Zen::Math::Point3(100.0f,0.0f,0.0f));

    char input(' ');
    while(input != 'Y' && input != 'N' && input != 'y' && input != 'n')
    {
        std::cout << "\n Unit Test - SoundTests::testOpenALSourceMotion() : Is thunder playing to your right? (Y/N) : ";
        std::cin.get(input);
    }
    pSound->setPosition(Zen::Math::Point3(-100.0f,0.0f,0.0f));
    pSound->stop();
    pSound->play();

    input = ' ';
    while(input != 'Y' && input != 'N' && input != 'y' && input != 'n')
    {
        std::cout << "\n Unit Test - SoundTests::testOpenALSourceMotion() : Is thunder playing to your left? (Y/N) : ";
        std::cin.get(input);
    }
    pSound->stop();
    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound source is not moving.",
        CPPUNIT_ASSERT(input == 'Y' || input == 'y')
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testOpenALListenerMotion()
{
    Zen::Engine::Sound::I_SoundManager::config_type soundConfig;
    Zen::Engine::Resource::I_ResourceManager::config_type resourceConfig;

    Zen::Engine::Sound::I_SoundManager::pService_type pSoundService =
        Zen::Engine::Sound::I_SoundManager::getSingleton()
            .create("ZOpenAL", soundConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound service was not loaded.",
        CPPUNIT_ASSERT(pSoundService.isValid())
    );

    Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton()
            .create("ZOpenAL", resourceConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound resource service was not loaded.",
        CPPUNIT_ASSERT(pResourceService.isValid())
    );

    pResourceService->addResourceLocation("Engine/resources/sound", "", "", true);

    resourceConfig["fileName"] = "thunder.wav";

    typedef Zen::Memory::managed_ptr<Zen::Engine::Sound::I_SoundResource>   pSoundResource_type;
    pSoundResource_type pSoundResource = 
        pResourceService
            ->loadResource(resourceConfig)
                .as<pSoundResource_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound resource was not loaded.",
        CPPUNIT_ASSERT(pSoundResource.isValid())
    );

    Zen::Engine::Sound::I_SoundSource::pSoundSource_type pSound =
        pSoundService->createSource(pSoundResource,Zen::Math::Point3(0.0f,0.0f,0.0f));

    Math::Matrix4 yourLocation;
    yourLocation = pSoundService->getListenMatrix();
    Math::Point3 pos;
    yourLocation.getPosition(pos);
    std::cout << "\n Position was:" << boost::int32_t(pos.m_x) << "," << boost::int32_t(pos.m_y) << "," << boost::int32_t(pos.m_z);
    pos = Math::Point3(100.0f,0.0f,0.0f);
    yourLocation.setPosition(pos);
    pSoundService->setListenMatrix(yourLocation);
    yourLocation.getPosition(pos);
    std::cout << "and is now" << boost::int32_t(pos.m_x) << "," << boost::int32_t(pos.m_y) << "," << boost::int32_t(pos.m_z);
    pSound->play();
    pSound->setLooping(true);

    char input(' ');
    while(input != 'Y' && input != 'N' && input != 'y' && input != 'n')
    {
        std::cout << "\n Unit Test - SoundTests::testOpenALListenerMotion() : Is thunder playing to your right? (Y/N) : ";
        std::cin.get(input);
    }

    pos = Math::Point3(-100.0f,0.0f,0.0f);
    yourLocation.setPosition(pos);
    pSoundService->setListenMatrix(yourLocation);
    pSoundService->processEvents(0);

    input = ' ';
    while(input != 'Y' && input != 'N' && input != 'y' && input != 'n')
    {
        std::cout << "/n Unit Test - SoundTests::testOpenALListenerMotion() : Is thunder playing to your left? (Y/N) : ";
        std::cin.get(input);
    }
    pSound->stop();
    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZOpenAL Sound listener is not moving.",
        CPPUNIT_ASSERT(input == 'Y' || input == 'y')
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testFMODServiceLoad()
{
    Zen::Engine::Sound::I_SoundManager::config_type soundConfig;

    Zen::Engine::Sound::I_SoundManager::pService_type pSoundService =
        Zen::Engine::Sound::I_SoundManager::getSingleton()
            .create("ZFMOD", soundConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound service was not loaded.",
        CPPUNIT_ASSERT(pSoundService.isValid())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testFMODResourceServiceLoad()
{
    Zen::Engine::Resource::I_ResourceManager::config_type resourceConfig;

    Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton()
            .create("ZFMOD", resourceConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound resource service was not loaded.",
        CPPUNIT_ASSERT(pResourceService.isValid())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testFMODResourceLoad()
{
    Zen::Engine::Resource::I_ResourceManager::config_type resourceConfig;

    Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton()
            .create("ZFMOD", resourceConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound resource service was not loaded.",
        CPPUNIT_ASSERT(pResourceService.isValid())
    );

    pResourceService->addResourceLocation("Engine/resources/sound", "", "", true);

    resourceConfig["fileName"] = "sirabhorn.ogg";

    typedef Zen::Memory::managed_ptr<Zen::Engine::Sound::I_SoundResource>   pSoundResource_type;
    pSoundResource_type pSoundResource = 
        pResourceService
            ->loadResource(resourceConfig)
                .as<pSoundResource_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound resource was not loaded.",
        CPPUNIT_ASSERT(pSoundResource.isValid())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testFMODSourceCreation()
{
    Zen::Engine::Sound::I_SoundManager::config_type soundConfig;
    Zen::Engine::Resource::I_ResourceManager::config_type resourceConfig;

    Zen::Engine::Sound::I_SoundManager::pService_type pSoundService =
        Zen::Engine::Sound::I_SoundManager::getSingleton()
            .create("ZFMOD", soundConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound service was not loaded.",
        CPPUNIT_ASSERT(pSoundService.isValid())
    );

    Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton()
            .create("ZFMOD", resourceConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound resource service was not loaded.",
        CPPUNIT_ASSERT(pResourceService.isValid())
    );

    pResourceService->addResourceLocation("Engine/resources/sound", "", "", true);

    resourceConfig["fileName"] = "sirabhorn.ogg";

    typedef Zen::Memory::managed_ptr<Zen::Engine::Sound::I_SoundResource>   pSoundResource_type;
    pSoundResource_type pSoundResource = 
        pResourceService
            ->loadResource(resourceConfig)
                .as<pSoundResource_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound resource was not loaded.",
        CPPUNIT_ASSERT(pSoundResource.isValid())
    );

    Zen::Engine::Sound::I_SoundSource::pSoundSource_type pSound =
        pSoundService->createSource(pSoundResource);
    pSoundService->processEvents(0);

    char input(' ');
    while(input != 'Y' && input != 'N' && input != 'y' && input != 'n')
    {
        std::cout << "Unit Test - SoundTests::testFMODSourceCreation() : Is music playing through the sound card successfully? (Y/N) : ";
        std::cin.get(input);
    }
    pSound->stop();
    pSoundService->processEvents(0);
    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound source is not playing through the sound card.",
        CPPUNIT_ASSERT(input == 'Y' || input == 'y')
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testFMODSourceMotion()
{
    Zen::Engine::Sound::I_SoundManager::config_type soundConfig;
    Zen::Engine::Resource::I_ResourceManager::config_type resourceConfig;

    Zen::Engine::Sound::I_SoundManager::pService_type pSoundService =
        Zen::Engine::Sound::I_SoundManager::getSingleton()
            .create("ZFMOD", soundConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound service was not loaded.",
        CPPUNIT_ASSERT(pSoundService.isValid())
    );

    Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton()
            .create("ZFMOD", resourceConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound resource service was not loaded.",
        CPPUNIT_ASSERT(pResourceService.isValid())
    );

    pResourceService->addResourceLocation("Engine/resources/sound", "", "", true);

    resourceConfig["fileName"] = "thunder.wav";

    typedef Zen::Memory::managed_ptr<Zen::Engine::Sound::I_SoundResource>   pSoundResource_type;
    pSoundResource_type pSoundResource = 
        pResourceService
            ->loadResource(resourceConfig)
                .as<pSoundResource_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound resource was not loaded.",
        CPPUNIT_ASSERT(pSoundResource.isValid())
    );

    Zen::Engine::Sound::I_SoundSource::pSoundSource_type pSound =
        pSoundService->createSource(pSoundResource,Zen::Math::Point3(100.0f,0.0f,0.0f));

    char input(' ');
    while(input != 'Y' && input != 'N' && input != 'y' && input != 'n')
    {
        std::cout << "\n Unit Test - SoundTests::testFMODSourceMotion() : Is thunder playing to your right? (Y/N) : ";
        std::cin.get(input);
    }
    pSound->setPosition(Zen::Math::Point3(-100.0f,0.0f,0.0f));
    pSound->stop();
    pSound->play();
    pSoundService->processEvents(0);

    input = ' ';
    while(input != 'Y' && input != 'N' && input != 'y' && input != 'n')
    {
        std::cout << "\n Unit Test - SoundTests::testFMODSourceMotion() : Is thunder playing to your left? (Y/N) : ";
        std::cin.get(input);
    }
    pSound->stop();
    pSoundService->processEvents(0);
    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound source is not moving.",
        CPPUNIT_ASSERT(input == 'Y' || input == 'y')
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundTests::testFMODListenerMotion()
{
    Zen::Engine::Sound::I_SoundManager::config_type soundConfig;
    Zen::Engine::Resource::I_ResourceManager::config_type resourceConfig;

    Zen::Engine::Sound::I_SoundManager::pService_type pSoundService =
        Zen::Engine::Sound::I_SoundManager::getSingleton()
            .create("ZFMOD", soundConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound service was not loaded.",
        CPPUNIT_ASSERT(pSoundService.isValid())
    );

    Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton()
            .create("ZFMOD", resourceConfig);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound resource service was not loaded.",
        CPPUNIT_ASSERT(pResourceService.isValid())
    );

    pResourceService->addResourceLocation("Engine/resources/sound", "", "", true);

    resourceConfig["fileName"] = "thunder.wav";

    typedef Zen::Memory::managed_ptr<Zen::Engine::Sound::I_SoundResource>   pSoundResource_type;
    pSoundResource_type pSoundResource = 
        pResourceService
            ->loadResource(resourceConfig)
                .as<pSoundResource_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound resource was not loaded.",
        CPPUNIT_ASSERT(pSoundResource.isValid())
    );

    Zen::Engine::Sound::I_SoundSource::pSoundSource_type pSound =
        pSoundService->createSource(pSoundResource,Zen::Math::Point3(0.0f,0.0f,0.0f));

    Math::Matrix4 yourLocation;
    yourLocation = pSoundService->getListenMatrix();
    Math::Point3 pos;
    yourLocation.getPosition(pos);

    pos = Math::Point3(100.0f,0.0f,0.0f);
    yourLocation.setPosition(pos);
    pSoundService->setListenMatrix(yourLocation);
    yourLocation.getPosition(pos);
    pSound->play();
    pSound->setLooping(true);
    pSoundService->processEvents(0);

    char input(' ');
    while(input != 'Y' && input != 'N' && input != 'y' && input != 'n')
    {
        std::cout << "\n Unit Test - SoundTests::testFMODListenerMotion() : Is thunder playing to your right? (Y/N) : ";
        std::cin.get(input);
    }

    pos = Math::Point3(-100.0f,0.0f,0.0f);
    yourLocation.setPosition(pos);
    pSoundService->setListenMatrix(yourLocation);
    pSoundService->processEvents(0);

    input = ' ';
    while(input != 'Y' && input != 'N' && input != 'y' && input != 'n')
    {
        std::cout << "/n Unit Test - SoundTests::testFMODListenerMotion() : Is thunder playing to your left? (Y/N) : ";
        std::cin.get(input);
    }
    pSound->stop();
    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "ZFMOD Sound listener is not moving.",
        CPPUNIT_ASSERT(input == 'Y' || input == 'y')
    );
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UnitTests
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
