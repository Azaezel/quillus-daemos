cmake_minimum_required (VERSION 2.6) 

project ( ZenThreading )
#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Newer CMake Find system for locating Boost includes and libraries

SET( Boost_USE_MULTITHREAD On )
SET( Boost_USE_STATIC_LIBS Off )

Find_Package( Boost REQUIRED )

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# We want this to be a shared lib (.so, .dylib, or .dll)
SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

set ( Threading_SRC

    #in Threading
    ConditionFactory.hpp
    Configuration.hpp
    CriticalSection.hpp
    I_Callable.hpp
    I_Condition.hpp
    I_Mutex.hpp
    I_Runnable.hpp
    I_Thread.hpp
    MutexFactory.hpp
    SpinLock.hpp
    SynchronizedQueue.hpp
    ThreadFactory.hpp
    ThreadPool.hpp
    ThreadSafeQueue.hpp

    #in Threading/src
    src/ConditionFactory.cpp
    src/CriticalSection.cpp
    src/I_Condition.cpp
    src/I_Mutex.cpp
    src/I_Thread.cpp
    src/MutexFactory.cpp
    src/SynchronizedQueue.cpp
    src/ThreadFactory.cpp
    src/ThreadPool.cpp
    )


find_file ( SYNCH_H synch.h ENV )
if ( SYNCH_H )
    add_definitions( -DTHREADMODEL_SOLARIS )
    set ( Threading_SRC
        ${Threading_SRC}
        src/Condition_solaris.cpp
        src/Condition_solaris.hpp
        src/Mutex_solaris.cpp
        src/Mutex_solaris.hpp
        src/Thread_solaris.cpp
        src/Thread_solaris.hpp
        )
endif( SYNCH_H )

find_file ( PTHREAD_H pthread.h ENV )
if ( PTHREAD_H )
    add_definitions( -DTHREADMODEL_POSIX )
    set ( Threading_SRC
        ${Threading_SRC}
        src/Thread_posix.cpp
        src/Thread_posix.hpp
        src/Mutex_posix.cpp
        src/Mutex_posix.hpp
        src/Condition_posix.cpp
        src/Condition_posix.hpp
        )
    set ( CPPFLAGS
        ${CPPFLAGS}
        -pthread
        )

    set ( LDFLAGS
        -pthread
        )
endif ( PTHREAD_H )

find_file ( WINDOWS_H windows.h ENV )
if ( WINDOWS_H )
    add_definitions( -DTHREADMODEL_WIN32 )
    set ( Threading_SRC
        ${Threading_SRC}
        src/Condition_win32.cpp
        src/Condition_win32.hpp
        src/Mutex_win32.cpp
        src/Mutex_win32.hpp
        src/Thread_win32.cpp
        src/Thread_win32.hpp
        )
endif ( WINDOWS_H )

find_file ( THREAD_H thread.h ENV )
if ( SYNCH_H AND THREAD_H )
    add_definitions( -DHOST_SOLARIS )
endif ( SYNCH_H AND THREAD_H )

find_file ( UNISTD_H unistd.h ENV )
if ( UNISTD_H )
    add_definitions( -DHOST_POSIX )
endif ( UNISTD_H )

find_file ( WINDOWS_H windows.h ENV )
if ( WINDOWS_H )
    add_definitions( -DHOST_WIN32 )
endif ( WINDOWS_H )

find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ../../.. )

add_library ( Threading ${LIB_TYPE} ${Threading_SRC} )

