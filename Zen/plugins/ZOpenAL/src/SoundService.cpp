//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
#include "SoundService.hpp"

#include <stdio.h>
#include <al.h>
#include <algorithm>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOpenAL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::SoundService(config_type& _config)
{
    alutInit(NULL,NULL); //initialize the ALUT library and create a default current context (from: The OpenAL Utility Toolkit)
    // Check for EAX 2.0 support
    m_bEAX = alIsExtensionPresent("EAX2.0");
    alGetError(); // clear error code
    setListenMatrix(Math::Matrix4(Math::Matrix4::INIT_ZERO));
    m_maxSources = 128;                     // verry verry unlikely to see a card that can handle this many sources
    m_listenRadius = 20.0f;
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
    pSource->setPosition(_pos);
    pSource->play();
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
    pSource->setPosition(pos);
    pSource->play();
    m_SoundSources.push_back(pSource);
    return pSource;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/*
SoundService::pSource_type attatchSource(config_type& _config, I_GameObject)
{
}
*/
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
            loopsListing.push_back(*iter);
        else 
            singlePlayListing.push_back(*iter);
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
        Math::Real dist = (listenerPos-(*iter)->getPosition()).magnitude();
        if (dist<((*iter)->getEmissionRadius() + m_listenRadius))
            inRangeListing.push_back(*iter);
        else 
            outofRangeListing.push_back(*iter);
    }
    sortVectorbyLooping(&inRangeListing);

    //next, filter by listed distances*volumes
    Math::Real mindist = FLT_MAX;
    Math::Real maxdist = 0;
    pSource_type pTempSource;
    int start = 0, min = 0;
    int end = inRangeListing.size()-1, max = inRangeListing.size()-1;
    bool sorting = false;

    //store off our current distances from the listener * the volume of the sounds themselves so were not constantly recalculating them
    for(int i = start; i < end; i++)
    {
        inRangeListing[i]->setVolDist((listenerPos - inRangeListing[i]->getPosition()).magnitude() * inRangeListing[i]->getVolume());
        sorting = true;
    }

    while (sorting == true)
    {
        sorting = false;
	    for(int i = start; i < end; i++)
        {
            Math::Real volDist = inRangeListing[i]->getVolDist();
            //grab our highs and lows
            if (volDist<=mindist)
            {
                mindist = volDist;
                min = i;
                sorting = true;
            }
            if (volDist>maxdist)
            {
                maxdist = volDist;
                max = i;
                sorting = true;
            }
        }

        //toss the next lowest and highest to thier respective places, and clip the search area by 2
        pTempSource = inRangeListing[min];
        inRangeListing[min] = inRangeListing[start];
        inRangeListing[start] = pTempSource;
        start++;
        pTempSource = inRangeListing[max];
        inRangeListing[max] = inRangeListing[end];
        inRangeListing[end] = pTempSource;
        end--;
    }


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
    //3d sounds you can hear
	for(std::vector<pSource_type>::iterator iter = inRangeListing.begin(); iter != inRangeListing.end(); iter++)
    {
        sourceCount++;
        m_SoundSources.push_back(*iter);
        if (sourceCount<m_maxSources) (*iter)->queue();
        else (*iter)->dequeue();
    }
    //3d sounds you can't hear
	for(std::vector<pSource_type>::iterator iter = outofRangeListing.begin(); iter != outofRangeListing.end(); iter++)
    {
        m_SoundSources.push_back(*iter);
        (*iter)->dequeue();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::onFrame()
{
    alGetError(); // clear error code

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

    Math::Real activeSourceCount = 0;
    Math::Real sourceAddFails = 0;
	for(std::vector<pSource_type>::iterator iter = m_SoundSources.begin(); iter != m_SoundSources.end(); iter++)
    {
        //first order of buisiness: if we're exceeding the max sourcecount, skip it entirely.
        if (activeSourceCount>m_maxSources)
        {
            for(std::vector<pSource_type>::iterator iter2 = iter; iter2 != m_SoundSources.end(); iter2++)
                (*iter2)->dequeue();
            continue;
        }

        Source::PLAYSTATE playState = (*iter)->getPlayState();
        //second, if we're marked as de-queued, free us up a voice
        if (playState == Source::DEQUEUED)
        {
            (*iter)->dequeue();
            continue;
        }

        activeSourceCount++;
        //third, if we're marked as playing, keep playing
        if (playState == Source::PLAYING) continue;
        //fourth, try just adding a new one
        if (playState == Source::QUEUED) 
        {
            if ((*iter)->play() == false)
            {
                sourceAddFails++;
            }
        }
    }
    if (sourceAddFails>0)
    {
        //we must have run out of possible voices. Update the maxSources to reflect this, and dequeue the rest
        //this way we avoid artificial hardware limitations
        m_maxSources = activeSourceCount-sourceAddFails;
        std::cout << "alGenSources(1, &sourceID); failed! setting new voices cap to:" << m_maxSources <<"\n";
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::muteAll(bool bMute)
{
    //std::cout << "muteAll() called" << std::endl;
	m_bMainMuted = bMute;
	//m_channelGroupMain->setMute(m_bMainMuted);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::setListenMatrix(Zen::Math::Matrix4 _listenMatrix)
{
    alGetError();
    m_ListenMatrix = _listenMatrix;

    // http://www.gamedev.net/reference/articles/article1958.asp

    //set current listener position
    Math::Point3 pos;
    m_ListenMatrix.getPosition(pos);
    alListener3f(AL_POSITION,pos.m_x,pos.m_y,pos.m_z);

    //set current listener orientation
    Math::Real vec[6];
    Math::Vector3 fvec = m_ListenMatrix.getForwardVector();
    vec[0] = fvec.m_x; //forward vector x value
    vec[1] = fvec.m_y; //forward vector y value
    vec[2] = fvec.m_z; //forward vector z value
    vec[3] = m_ListenMatrix.m_array[1]; //up vector x value
    vec[4] = m_ListenMatrix.m_array[5]; //up vector y value
    vec[5] = m_ListenMatrix.m_array[9]; //up vector z value
    alListenerfv(AL_ORIENTATION, vec);

    //alListenerfv(AL_VELOCITY,    ListenerVel);
    if(alGetError() != AL_NO_ERROR) std::cout << "SoundService::setListenMatrix(Zen::Math::Matrix4 _listenMatrix) failed!";
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
}   // namespace ZOpenAL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
