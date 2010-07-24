//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
// Copyright Tony Richards 2001 - 2009
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// @author  John S. Givler, Ph.D.(Computer Science)

#ifndef ZEN_OBJECTMODEL_CONFIGURAGION_HPP_INCLUDED
#define ZEN_OBJECTMODEL_CONFIGURAGION_HPP_INCLUDED

#ifdef  _MSC_VER
#    pragma once
#    pragma inline_depth(255)
#    pragma inline_recursion(off)
#    pragma warning(disable:4251)   // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#    pragma warning(disable:4275)   // non – DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#endif  // _MSC_VER


#ifdef  _MSC_VER
#    ifndef HOST_WIN32              // this probably should be done elsewhere ...
#        ifdef _WIN32
#            define HOST_WIN32
#        endif
#    endif
#    ifndef HOST_WIN64
#        ifdef _WIN64
#            define HOST_WIN64
#        endif
#    endif
#endif  // _MSC_VER


// Microsoft C++ compiler
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#    ifdef  OBJECTMODEL_EXPORTS
#        define OBJECTMODEL_DLL_LINK __declspec(dllexport)
#    else
#        define OBJECTMODEL_DLL_LINK __declspec(dllimport)
#    endif
#endif  // _MSC_VER


// GNU C++ compiler
#if defined(__GNUG__) && !defined(__INTEL_COMPILER)
#    if 0 // (__GNUC__ >= 4) && !defined(HOST_CYGWIN) && !defined(HOST_SOLARIS)
#        ifdef  OBJECTMODEL_EXPORTS
#            define OBJECTMODEL_DLL_LINK __attribute__ ((visibility ("default")))
#        else
#            define OBJECTMODEL_DLL_LINK __attribute__ ((visibility ("hidden")))
#        endif
#    else
#        ifdef  OBJECTMODEL_EXPORTS
#            define OBJECTMODEL_DLL_LINK // __declspec(dllexport)
#        else
#            define OBJECTMODEL_DLL_LINK // __declspec(dllimport)
#        endif
#    endif
#endif  // __GNUG__


// Sun C++ compiler
#if defined(__SUNPRO_CC)
#    ifdef  OBJECTMODEL_EXPORTS
#        define OBJECTMODEL_DLL_LINK   // ?
#    else
#        define OBJECTMODEL_DLL_LINK   // ?
#    endif
#endif  // __SUNPRO_CC


// Intel C++ compiler
#if defined(__INTEL_COMPILER)
#    ifdef  OBJECTMODEL_EXPORTS
#        define OBJECTMODEL_DLL_LINK __declspec(dllexport)
#    else
#        define OBJECTMODEL_DLL_LINK __declspec(dllimport)
#    endif
#endif  // __INTEL_COMPILER


#ifndef OBJECTMODEL_DLL_LINK
#    define OBJECTMODEL_DLL_LINK
#endif  // ! OBJECTMODEL_DLL_LINK


#endif  // ZEN_COMMUNITY_OBJECTMODEL_CONFIGURAGION_HPP_INCLUDED

