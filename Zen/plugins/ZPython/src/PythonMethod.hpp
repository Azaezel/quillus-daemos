//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Matthew Alan Gray
// Copyright (C)        2008 Walt Collins
// Copyright (C)        2010 Jason Smith
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//  Jason Smith jsmith@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef INDIEZEN_ZPYTHON_PYTHON_METHOD_HPP_INCLUDED
#define INDIEZEN_ZPYTHON_PYTHON_METHOD_HPP_INCLUDED

#include <Python.h>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Scripting.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPython {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class PythonType;

class PythonMethod
{
    /// @name Types
    /// @{
public:
    typedef Scripting::I_ObjectReference*                           pObjectReference_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptObject>     pScriptObject_type;
    /// @}

    /// @name PythonMethod implementation
    /// @{
public:
    PyMethodDef* getDef();
    PyObject* operator()(PyObject* _pObj, PyObject* _pArgs);
    /// @}

    /// @name Helper Methods
    /// @{
private:
    void init();
    /// @}

    /// @name 'Structors
    /// @{
public:
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::void_function_no_args_type _function, PyCFunction _pCFunction);
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::void_function_args_type _function, PyCFunction _pCFunction);
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::object_function_args_type _function, PyCFunction _pCFunction);
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::object_function_no_args_type _function, PyCFunction _pCFunction);
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::string_function_args_type _function, PyCFunction _pCFunction);
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::string_function_no_args_type _function, PyCFunction _pCFunction);
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::bool_function_no_args_type _function, PyCFunction _pCFunction);
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::bool_function_args_type _function, PyCFunction _pCFunction);
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::int_function_no_args_type _function, PyCFunction _pCFunction);
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptType::int_function_args_type _function, PyCFunction _pCFunction);
             PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, Scripting::I_ScriptMethod* _function, PyCFunction _pCFunction);
    virtual ~PythonMethod();
    /// @}

    /// @name Member Variables
    /// @{
public:
    PythonType*     m_pType;
    std::string     m_name;
    std::string     m_docString;
    PyCFunction     m_pCFunction;

    enum FunctionType
    {
        VOID_FUNCTION_NO_ARGS,      // 0
        VOID_FUNCTION_ARGS,         // 1
        OBJ_FUNCTION_ARGS,          // 2
        OBJ_FUNCTION_NO_ARGS,       // 3
        STRING_FUNCTION_ARGS,       // 4
        STRING_FUNCTION_NO_ARGS,    // 5
        BOOL_FUNCTION_NO_ARGS,      // 6
        BOOL_FUNCTION_ARGS,         // 7
        INT_FUNCTION_NO_ARGS,       // 8
        INT_FUNCTION_ARGS           // 9
        GENERIC_FUCNTION_ARGS,      // 10
    };

    FunctionType                                            m_functionType;
    Scripting::I_ScriptType::void_function_no_args_type     m_function0;
    Scripting::I_ScriptType::void_function_args_type        m_function1;
    Scripting::I_ScriptType::object_function_args_type      m_function2;
    Scripting::I_ScriptType::object_function_no_args_type   m_function3;
    Scripting::I_ScriptType::string_function_args_type      m_function4;
    Scripting::I_ScriptType::string_function_no_args_type   m_function5;
    Scripting::I_ScriptType::bool_function_no_args_type     m_function6;
    Scripting::I_ScriptType::bool_function_args_type        m_function7;
    Scripting::I_ScriptType::int_function_no_args_type      m_function8;
    Scripting::I_ScriptType::int_function_args_type         m_function9;

    Scripting::I_ScriptMethod*                              m_function10;
    /// }

};  // class PythonMethod

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPython
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_ZPYTHON_PYTHON_METHOD_HPP_INCLUDED
