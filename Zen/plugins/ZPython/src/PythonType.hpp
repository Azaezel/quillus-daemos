//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
//  Jason Smith   jsmith@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef INDIEZEN_ZPYTHON_PYTHON_TYPE_HPP_INCLUDED
#define INDIEZEN_ZPYTHON_PYTHON_TYPE_HPP_INCLUDED

#include <Python.h>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <string>
#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPython {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class PythonModule;
class PythonMethod;

class PythonType
:   public Scripting::I_ScriptType
{
    /// @name Types
    /// @{
public:
    typedef std::list<PythonMethod*>    methods_type;
    /// @}

    /// @name I_ScriptType implementation
    /// @{
public:
    virtual void addMethod(const std::string& _name, const std::string& _docString, void_function_no_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, void_function_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, object_function_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, object_function_no_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, string_function_no_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, string_function_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, bool_function_no_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, bool_function_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, int_function_no_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, int_function_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, Scripting::I_ScriptMethod* _function);
    virtual void activate();
    /// @}

    /// @name Additional Implementation
    /// @{
public:
    PyTypeObject*   getRawType();
    //PyObject*       getRawClass();
    const std::string& getName() const;
    PythonModule&   getModule();
    /// @}

    /// @name 'Structors
    /// @{
public:
             PythonType(PythonModule* _pModule, const std::string& _name, const std::string& _docString, unsigned long _rawSize);
    virtual ~PythonType();
    /// @}

    /// @name Member Variables
    /// @{
private:
    PythonModule*           m_pModule;      ///< Module
    std::string             m_name;         ///< Class Name
    std::string             m_docString;    ///< Document String
    std::string             m_fullName;     ///< Full Class Name including module

    PyTypeObject            m_type;
    PyObject*               m_pClass;
    PyObject*               m_pClassDict;

    methods_type            m_methods;
    /// @}

};  // class PythonModule

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PyTypeObject*
PythonType::getRawType()
{ 
    return &m_type; 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
inline
PyObject*
PythonType::getRawClass()
{ 
    return m_pClass; 
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
const std::string&
PythonType::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PythonModule&
PythonType::getModule()
{
    return *m_pModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPython
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_ZPYTHON_PYTHON_TYPE_HPP_INCLUDED
