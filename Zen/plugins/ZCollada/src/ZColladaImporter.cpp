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

#include "ZColladaImporter.hpp"

#include <stdlib.h>
#include <dae.h>
#include <dom.h>

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

#include <IndieZen/framework/Threading/ThreadFactory.hpp>
#include <IndieZen/framework/Threading/I_Runnable.hpp>
#include <IndieZen/framework/Threading/I_Thread.hpp>

#include <IndieZen/framework/ResourceManager/I_CompoundResource.hpp>
#include <IndieZen/framework/ResourceManager/I_ResourceManager.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace ZCollada {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

ZColladaImporter::ZColladaImporter()
{
    m_type = "Collada .DAE Model";
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZColladaImporter::~ZColladaImporter()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string& 
ZColladaImporter::getType() const
{
    return m_type;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class RunnableImporter
:   public Threading::I_Runnable
{
public:
    RunnableImporter(ZColladaImporter* _pParent, const boost::filesystem::path& _path)
    :   m_pParent(_pParent)
    ,   m_path(_path.string())
    ,   m_pThread(NULL)
    {
    }

    virtual void run()
    {
        try
        {
            m_pParent->synchronousImport(m_path);
        }
        catch(...)
        {
            std::cout << "RunnableImporter::run(): Unhandled exception parsing " << m_path << std::endl;
        }

        // Since we're not ever going to join on this thread, delete it.
        Threading::ThreadFactory::destroy(m_pThread);
    }

    virtual void stop()
    {
    }

    void setThread(Threading::I_Thread* _pThread)
    {
        m_pThread = _pThread;
    }

private:
    ZColladaImporter*		m_pParent;
    std::string				m_path;
    Threading::I_Thread*	m_pThread;
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZColladaImporter::synchronousImport(const std::string& _path)
{
    // TODO: Lock
    // This method is not thread safe

    ResourceManager::I_CompoundResource* pResource = ResourceManager::I_ResourceManager::getSingleton().createCompoundResource();

    // TODO Implement Collada import

    onImportComplete(pResource);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZColladaImporter::import(const boost::filesystem::path& _path, bool _async)
{
    if (_async)
    {
        RunnableImporter* const pImporter = new RunnableImporter(this, _path);

        Threading::I_Thread* pThread = Threading::ThreadFactory::create(pImporter);

        pImporter->setThread(pThread);

        pThread->start();
    }
    else
    {
        synchronousImport(_path.string());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCollada
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
