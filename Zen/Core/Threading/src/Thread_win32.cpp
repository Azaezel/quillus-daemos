//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2006 - 2008 John Givler
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
//  John S. Givler, Ph.D.(Computer Science) - Dr.John.S.Givler@Givler.com
//  Tony Richards                           - trichards@indiezen.com
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifdef THREADMODEL_WIN32

#include "Thread_win32.hpp"

#include "../I_Runnable.hpp"

//Define this in your main_env_vars.vsprops file if you need to use CPP threading for some reason
//If you're not sure, keep it this way.
//#define USE_CPP_THREADING

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

Thread_win32::Thread_win32(I_Runnable *const _pRunnable)
:	m_pRunnable(_pRunnable)
,	m_isStarted(false)
,	m_isJoined(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Thread_win32::~Thread_win32()
{
	if (m_isStarted)
	{
		stop();

		if (::GetCurrentThreadId() != m_threadId)
		{
			join();
		}

#ifdef USE_CPP_THREADING
		::CloseHandle(m_threadHandle);
#endif
	}

	delete m_pRunnable;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef USE_CPP_THREADING
extern "C" 
{
void threadFunctionC(void* _pThis)
{
    Thread_win32::threadFunction((::LPVOID)_pThis);
}
}
#endif

void
Thread_win32::start()
{
	if (!m_isStarted)
	{
#ifdef USE_CPP_THREADING
		m_threadHandle = ::CreateThread(NULL, 0, threadFunction,
			this, 0, &m_threadId);
		if (m_threadHandle == NULL)
		{
			// TODO log and throw a real exception
			throw 1;
		}
#else
        m_threadHandle = (HANDLE)::_beginthread(threadFunctionC, 0, this);
        if (m_threadHandle == (HANDLE)-1)
        {
			// TODO log and throw a real exception
			throw 1;
        }
#endif

		m_isStarted = true;
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Thread_win32::stop()
{
	if (m_isStarted && m_pRunnable != NULL)
	{
		m_pRunnable->stop();
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Thread_win32::join()
{
	if (m_isStarted && !m_isJoined)
	{
		::WaitForSingleObject(m_threadHandle, INFINITE);
		m_isJoined = true;
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DWORD WINAPI
Thread_win32::threadFunction (::LPVOID _pThis)
{
    Thread_win32* const pThis = static_cast <Thread_win32*> (_pThis);
#ifndef USE_CPP_THREADING
    pThis->m_threadId = ::GetCurrentThreadId();
#endif
	try
	{
		pThis->m_pRunnable->run();
	}
	catch(...)
	{
	}
#ifndef USE_CPP_THREADING
    _endthread();
#endif
    return 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // THREADMODEL_WIN32
