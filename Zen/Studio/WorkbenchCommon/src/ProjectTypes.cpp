//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
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

#include "ProjectTypes.hpp"
#include "ProjectType.hpp"

#include <Zen/Core/Plugins/I_Configuration.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>
#include <Zen/Core/Plugins/I_ExtensionQuery.hpp>
#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>
#include <Zen/Core/Plugins/I_Extension.hpp>

#include <Zen/Core/Threading/ThreadFactory.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/I_Runnable.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectTypes::ProjectTypes()
//:   m_pParent(_pParent)
{
    m_pMutex = Zen::Threading::MutexFactory::create();
    m_pThread = NULL;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectTypes::~ProjectTypes()
{
    {
        Threading::CriticalSection guard(m_pMutex);
        m_projectTypes.clear();
    }

    m_pThread->stop();
    m_pThread->join();

    Zen::Threading::ThreadFactory::destroy(m_pThread);
    Zen::Threading::MutexFactory::destroy(m_pMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectTypes::init()
{
    // Just in case init is ever called more than once, be sure to
    // join and destroy the previous thread first.
    if (m_pThread != NULL)
    {
        m_pThread->stop();
        m_pThread->join();
        Zen::Threading::ThreadFactory::destroy(m_pThread);
        m_pThread = NULL;
    }

    // Dual purpose class that is the extension visitor as well as a
    // runnable.
    class ThreadedExtensionPointVisitor
    :   public Plugins::I_ExtensionPoint::I_ExtensionVisitor
    ,   public Threading::I_Runnable
    {
    public:
        virtual void run() throw()
        {
            Plugins::I_ExtensionRegistry::pExtensionPoint_type pExtensionPoint =
            Plugins::I_ExtensionRegistry::getSingleton()
                .getExtensionPoint("Zen::Studio::Workbench", "ProjectService");

            Threading::CriticalSection guard(m_pParent->m_pMutex);
            pExtensionPoint->getExtensions(*this);
        }

        virtual void begin()
        {
            m_pParent->m_projectTypes.clear();
        }

        virtual void visit(pExtension_type _pExtension)
        {
            const Plugins::I_ConfigurationElement& config = _pExtension->getConfigurationElement();

            std::string name = config.getName();

            Plugins::I_ConfigurationElement const * const pProjectConfig = config.getChild("project-type");
            ProjectType* pProjectType = new ProjectType(pProjectConfig, _pExtension);
            m_pParent->onNewProjectType(*pProjectType);
            m_pParent->m_projectTypes.push_back(pProjectType);
        }

        virtual void end()
        {
        }

        ThreadedExtensionPointVisitor(ProjectTypes* _pParent) : m_pParent(_pParent) {}
        ProjectTypes*   m_pParent;
    };

    // Create the runnable visitor
    // Don't worry about leaking this.  Thread runnables are automatically
    // destroyed upon completion of the thread.
    ThreadedExtensionPointVisitor* pVisitor = new ThreadedExtensionPointVisitor(this);

    // Create the thread that will do the visiting
    m_pThread = Zen::Threading::ThreadFactory::create(pVisitor);

    // Start the thread.
    m_pThread->start();
}    

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectTypes::getProjectTypes(I_ProjectTypeVisitor& _visitor)
{
    _visitor.begin();

    {
        // Guard the collection
        Threading::CriticalSection guard(m_pMutex);

        for(collection_type::iterator iter = m_projectTypes.begin(); iter != m_projectTypes.end(); iter++)
        {
            _visitor.visit(**iter);
        }
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
