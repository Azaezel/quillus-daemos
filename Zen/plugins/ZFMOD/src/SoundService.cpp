//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "SoundService.hpp"

#include <iostream>
#include <fmod_errors.h>

namespace Zen {
namespace ZFMOD {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::SoundService(config_type& _config)
{
    m_pFMODSystem = FMODService::getSingletonPtr()->getFMODSystemPtr();
    setListenMatrix(Math::Matrix4(Math::Matrix4::INIT_ZERO));
    m_listenRadius = 20.0f;
    m_maxSources = 128;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::~SoundService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::pSource_type
SoundService::createSource(Engine::Sound::I_SoundResource::pSoundResource_type _pResource)
{
    Source* pRawPointer = new Source();
    pSource_type pSource(pRawPointer);
    pSource->setResource(_pResource);
    _pResource->setIs3d(false);
    pSource->play(); 
    m_SoundSources.push_back(pSource);

    return pSource;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::pSource_type
SoundService::createSource(Engine::Sound::I_SoundResource::pSoundResource_type _pResource, Math::Point3 _pos)
{
    Source* pRawPointer = new Source();
    pSource_type pSource(pRawPointer);
    pSource->setResource(_pResource);
    pSource->play();
    pSource->setPosition(_pos);
    m_SoundSources.push_back(pSource);
    return pSource;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::pSource_type
SoundService::createSource(Engine::Sound::I_SoundResource::pSoundResource_type _pResource, Math::Real _x, Math::Real _y)
{
    Source* pRawPointer = new Source();
    pSource_type pSource(pRawPointer);
    pSource->setResource(_pResource);
    Math::Point3 pos = Math::Point3(_x,_y,0);
    pSource->play();
    pSource->setPosition(pos);
    m_SoundSources.push_back(pSource);
    return pSource;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::sortVectorbyLooping(std::vector<pSource_type> * _vector)
{

    std::vector<pSource_type> loopsListing;
    std::vector<pSource_type> singlePlayListing;

    //split it between looping, and unique, prioritizing looping so we avoid sound restarts
	for(std::vector<pSource_type>::iterator iter = _vector->begin(); iter != _vector->end(); iter++)
    {
        if ((*iter)->getLooping())
        {
            loopsListing.push_back(*iter);
        }
        else
        {
            (*iter)->setVolDist((*iter)->getVolDist()-1.0f); // slightly lower priority if it's a unique sound
            singlePlayListing.push_back(*iter);
        }
    }

    _vector->clear();
	for(std::vector<pSource_type>::iterator iter = loopsListing.begin(); iter != loopsListing.end(); iter++)
    {
        _vector->push_back(*iter);
    }
	for(std::vector<pSource_type>::iterator iter = singlePlayListing.begin(); iter != singlePlayListing.end(); iter++)
    {
        _vector->push_back(*iter);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::sortSounds()
{
    //fallback functionality - override by ap is preffered

    //first, filter by 2d vs 3d
    std::vector<pSource_type> is2DListing;
    std::vector<pSource_type> is3DListing;
	for(std::vector<pSource_type>::iterator iter = m_SoundSources.begin(); iter != m_SoundSources.end(); iter++)
    {
        Engine::Sound::I_SoundResource::pSoundResource_type pResource = (*iter)->getResource();
        if (pResource->getIs3d())
            is3DListing.push_back(*iter);
        else 
            is2DListing.push_back(*iter);
            (*iter)->setVolDist(FLT_MAX);   //set 2d sounds to the highest priority
    }
    sortVectorbyLooping(&is2DListing);
    sortVectorbyLooping(&is3DListing);

    //next, filter by distance from listener to cull out out-of-range sources
    std::vector<pSource_type>    inRangeListing;
    std::vector<pSource_type>    outofRangeListing;

    Math::Point3 listenerPos;
    m_ListenMatrix.getPosition(listenerPos);

	for(std::vector<pSource_type>::iterator iter = is3DListing.begin(); iter != is3DListing.end(); iter++)
    {
        Math::Real volDist = (listenerPos-(*iter)->getPosition()).magnitude() * (*iter)->getVolume();
        (*iter)->setVolDist(volDist);
    }
    sortVectorbyLooping(&is3DListing);


    //finally, we recreate our sources vector from the various lists, and queue/deque them
    //based on how many voices are available
    m_SoundSources.clear();
    Math::Real sourceCount = 0;
    //music and menus
	for(std::vector<pSource_type>::iterator iter = is2DListing.begin(); iter != is2DListing.end(); iter++)
    {
        sourceCount++;
        m_SoundSources.push_back(*iter);
        if (sourceCount<m_maxSources) (*iter)->queue();
        else (*iter)->dequeue();
    }
    //3d sounds 
	for(std::vector<pSource_type>::iterator iter = is3DListing.begin(); iter != is3DListing.end(); iter++)
    {
        sourceCount++;
        m_SoundSources.push_back(*iter);
        if (sourceCount<m_maxSources) (*iter)->queue();
        else (*iter)->dequeue();
    }
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::onFrame()
{

    std::vector<pSource_type>::iterator iter = m_SoundSources.begin();
    while( iter != m_SoundSources.end() )
    {
        if ((*iter)->getPlayState() == Source::STOPPED)
        {
            (*iter)->stop();
            //TODO: kill source class?
            iter = m_SoundSources.erase( iter );
        }
        else
            ++iter;
    }
    sortSounds();

	for(std::vector<pSource_type>::iterator iter = m_SoundSources.begin(); iter != m_SoundSources.end(); iter++)
    {
        Source::PLAYSTATE playState = (*iter)->getPlayState();
        //second, if we're marked as de-queued, free us up a voice (check for redundency later)
        if (playState == Source::DEQUEUED)
        {
            (*iter)->dequeue();
            continue;
        }

        //third, if we're marked as playing, keep playing
        if (playState == Source::PLAYING) continue;
        //fourth, try just adding a new one
        if (playState == Source::QUEUED) (*iter)->play();
    }
	// this should fire every frame...
	static FMOD_RESULT result = FMOD_OK;
	result = m_pFMODSystem->update();
	if (result != FMOD_OK)
	{
		// TODO - throw an error here: couldn't update FMOD system during processEvents()
        //printf("onFrame() error! (%d) %s\n", result, FMOD_ErrorString(result));
        return;
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::muteAll(bool bMute)
{
    //std::cout << "muteAll() called" << std::endl;
	m_bMainMuted = bMute;
	m_channelGroupMain->setMute(m_bMainMuted);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::setListenMatrix(Zen::Math::Matrix4 _listenMatrix)
{
    FMOD_RESULT result = FMOD_OK;
    m_ListenMatrix = _listenMatrix;
    // http://jerome.jouvie.free.fr/Fmod/Tutorials/Tutorial03.php

    //set current listener position
    Math::Point3 pos;
    m_ListenMatrix.getPosition(pos);

    //set current listener orientation
    Math::Vector3 forward = m_ListenMatrix.getForwardVector();
    Math::Vector3 up;
    up.m_x = m_ListenMatrix.m_array[1]; //up vector x value
    up.m_y = m_ListenMatrix.m_array[5]; //up vector y value
    up.m_z = m_ListenMatrix.m_array[9]; //up vector z value
    Math::Vector3 vel(0,0,0); //We'll assume (for now) that this'll primarily be unnecessary since dopler-shifting while youre moving towards something
    //static is far less noticeable than when somethings moving towards you, wich is more the job of sources than listeners 

    result = m_pFMODSystem->set3DListenerAttributes(0, (const FMOD_VECTOR *)pos.m_array, (const FMOD_VECTOR *)vel.m_array, (const FMOD_VECTOR *)forward.m_array, (const FMOD_VECTOR *)up.m_array);
    if (result != FMOD_OK) printf("setListenMatrix(Zen::Math::Matrix4 _listenMatrix) error! (%d) %s\n", result, FMOD_ErrorString(result));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Matrix4
SoundService::getListenMatrix()
{
    return m_ListenMatrix;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::setListenRadius(Math::Real _radius)
{
    m_listenRadius = _radius;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Real
SoundService::getListenRadius()
{
    return m_listenRadius;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZFMOD
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
