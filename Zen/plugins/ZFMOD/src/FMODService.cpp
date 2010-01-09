//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen FMOD Plugin
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//  portions Copyright (c), Firelight Technologies Pty, Ltd 2004-2009.
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "FMODService.hpp"
#include <stddef.h>
#include <stdio.h>
#include <fmod_errors.h>
#include <string.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZFMOD {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<> FMODService* Singleton<FMODService>::ms_Singleton = 0;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
FMODService* FMODService::getSingletonPtr(void)
{
    return ms_Singleton;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
FMODService& FMODService::getSingleton(void)
{  
    assert (ms_Singleton);
	return (*ms_Singleton);  
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
FMOD::System*
FMODService::getFMODSystemPtr(void)
{
    return m_pFMODSystem;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
FMODService::FMODService()
{
    int numdrivers;
    unsigned int version;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS        caps;
    char name[256];
    FMOD_RESULT result = FMOD_OK;

    /*
        Create a System object and initialize.
    */
    result = FMOD::System_Create(&m_pFMODSystem);
	if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));
    
    result = m_pFMODSystem->getVersion(&version);
	if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
    }
    
    result = m_pFMODSystem->getNumDrivers(&numdrivers);
	if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));

    if (numdrivers == 0)
    {
        result = m_pFMODSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	    if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));
    }
    else
    {
        result = m_pFMODSystem->getDriverCaps(0, &caps, 0, 0, &speakermode);
	    if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));

        result = m_pFMODSystem->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
	    if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));

        if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
        {                                                   /* You might want to warn the user about this. */
            result = m_pFMODSystem->setDSPBufferSize(1024, 10);
	        if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));
        }

        result = m_pFMODSystem->getDriverInfo(0, name, 256, 0);
	    if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));

        if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
        {
            result = m_pFMODSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
	        if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));
        }
    }

    result = m_pFMODSystem->init(128, FMOD_INIT_NORMAL, 0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
    {
        result = m_pFMODSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
	    if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));
            
        result = m_pFMODSystem->init(128, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
	    if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));
    }

    
    /*
        Set the distance units. (meters/feet etc).
    */
    result = m_pFMODSystem->set3DSettings(1.0, 1.0f, 1.0f);
	if (result != FMOD_OK) printf("FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
FMODService::~FMODService()
{
    FMOD_RESULT result = FMOD_OK;
    result = m_pFMODSystem->close();
	if (result != FMOD_OK) printf("~FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));
	result = m_pFMODSystem->release();
	if (result != FMOD_OK) printf("~FMODService() error! (%d) %s\n", result, FMOD_ErrorString(result));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZFMOD
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
