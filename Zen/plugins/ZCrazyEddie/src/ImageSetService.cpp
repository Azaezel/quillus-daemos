//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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

#include "ImageSetService.hpp"
#include "ImageSet.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <CEGUI/CEGUI.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCrazyEddie {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ImageSetService::ImageSetService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ImageSetService::~ImageSetService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ImageSet&
ImageSetService::createImageSet(const std::string& _imageSet)
{
    /// TODO Guard m_imageSetCache

    imageSetCache_type::iterator iter = m_imageSetCache.find(_imageSet);
    if( iter != m_imageSetCache.end() )
    {
        return *iter->second;
    }

    CEGUI::Imageset& wrappedImageSet = CEGUI::ImagesetManager::getSingletonPtr()->create(
        (CEGUI::utf8*)_imageSet.c_str()
    );

    I_ImageSet* pImageSet = new ImageSet(*this, wrappedImageSet);

    m_imageSetCache[_imageSet] = pImageSet;

    return *pImageSet;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ImageSet&
ImageSetService::createImageSetFromFile(const std::string& _imageSet,
                                        const std::string& _filename)
{
    /// TODO Guard m_imageSetCache

    imageSetCache_type::iterator iter = m_imageSetCache.find(_imageSet);
    if( iter != m_imageSetCache.end() )
    {
        return *iter->second;
    }

    CEGUI::Imageset& wrappedImageSet = CEGUI::ImagesetManager::getSingletonPtr()->createFromImageFile(
        (CEGUI::utf8*)_imageSet.c_str(),
        (CEGUI::utf8*)_filename.c_str()
    );

    I_ImageSet* pImageSet = new ImageSet(*this, wrappedImageSet);

    m_imageSetCache[_imageSet] = pImageSet;

    return *pImageSet;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCrazyEddie
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
