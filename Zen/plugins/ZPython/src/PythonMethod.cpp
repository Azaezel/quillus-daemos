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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "PythonMethod.hpp"
#include "PythonType.hpp"
#include "PythonModule.hpp"
#include "PythonObject.hpp"
#include "PythonTypeMap.hpp"
#include "PythonEngine.hpp"

#include <Zen/Core/Scripting/I_ScriptObject.hpp>
#include <Zen/Core/Scripting/I_ObjectReference.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPython {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, 
                           Scripting::I_ScriptType::void_function_no_args_type _function, PyCFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(VOID_FUNCTION_NO_ARGS)
,   m_function0(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, 
                           Scripting::I_ScriptType::void_function_args_type _function, PyCFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(VOID_FUNCTION_ARGS)
,   m_function1(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, 
                           Scripting::I_ScriptType::object_function_args_type _function, PyCFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(OBJECT_FUNCTION_ARGS)
,   m_function2(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, 
                           Scripting::I_ScriptType::object_function_no_args_type _function, PyCFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(OBJECT_FUNCTION_NO_ARGS)
,   m_function3(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, 
                           Scripting::I_ScriptType::string_function_args_type _function, PyCFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(STRING_FUNCTION_ARGS)
,   m_function4(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, 
                           Scripting::I_ScriptType::string_function_no_args_type _function, PyCFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(STRING_FUNCTION_NO_ARGS)
,   m_function5(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, 
                           Scripting::I_ScriptType::bool_function_no_args_type _function, PyCFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(BOOL_FUNCTION_NO_ARGS)
,   m_function6(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, 
                           Scripting::I_ScriptType::bool_function_args_type _function, PyCFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(BOOL_FUNCTION_ARGS)
,   m_function7(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, 
                           Scripting::I_ScriptType::int_function_no_args_type _function, PyCFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(INT_FUNCTION_NO_ARGS)
,   m_function8(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::PythonMethod(PythonType* _pType, const std::string& _name, const std::string& _docString, 
                           Scripting::I_ScriptType::int_function_args_type _function, PyCFunction _pCFunction)
:   m_pType(_pType)
,   m_name(_name)
,   m_docString(_docString)
,   m_pCFunction(_pCFunction)
,   m_functionType(INT_FUNCTION_ARGS)
,   m_function9(_function)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonMethod::~PythonMethod()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PythonMethod::init()
{
    m_pDef = new PyMethodDef;
    memset(m_pDef, 0, sizeof(PyMethodDef));
    m_pDef->ml_name = m_name.c_str();
    m_pDef->ml_meth = m_pCFunction;

    //m_pFunction = PyCFunction_New(m_pDef, NULL);

    PyObject* pMethod = PyDescr_NewMethod(m_pType->getRawType(), m_pDef);

    PyDict_SetItemString(m_pType->getRawType()->tp_dict, m_pDef->ml_name, pMethod);
    Py_DECREF(pMethod);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PyMethodDef*
PythonMethod::getDef()
{
    return m_pDef;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PyObject*
PythonMethod::operator ()(PyObject* _pObj, PyObject* _pArgs)
{
    pObjectReference_type const pObj = m_pType->getModule().getEngine().getObject(_pObj);

    if (pObj)
    {
        // TODO Throw a Python exception
        std::cout << "Error mapping Python object to Zen object" << std::endl;
    }
    else
    {
        switch(m_functionType)
        {
        case VOID_FUNCTION_NO_ARGS:
            m_function0(pObj);
            return Py_None;
        case OBJECT_FUNCTION_NO_ARGS:
            {
                pObjectReference_type const pObjReference = m_function3(pObj);
                pScriptObject_type const pReturnObj = pObjReference->getScriptObject();
                return dynamic_cast<PythonObject*>(pReturnObj.get())->get();
            }
        case STRING_FUNCTION_NO_ARGS:
            {
                const std::string& returnValue = m_function5(pObj);
                return PyString_FromString(returnValue.c_str());
            }
        case BOOL_FUNCTION_NO_ARGS:
            {
                const bool returnValue = m_function6(pObj);
                return returnValue ? Py_True : Py_False;
            }
        case INT_FUNCTION_NO_ARGS:
            {
                const int returnValue = m_function8(pObj);
                return PyLong_FromLong(returnValue);
            }
        case VOID_FUNCTION_ARGS:
        case OBJECT_FUNCTION_ARGS:
        case STRING_FUNCTION_ARGS:
        case BOOL_FUNCTION_ARGS:
        case INT_FUNCTION_ARGS:
            {
                std::vector<boost::any> parms;

                PythonTypeMap& typeMap = m_pType->getModule().getEngine().getTypeMap();

                if (PyTuple_Check(_pArgs))
                {
                    Py_ssize_t size = PyTuple_Size(_pArgs);

                    for(int x = 0; x < size; x++)
                    {
                        PyObject* const pArg = PyTuple_GetItem(_pArgs, x);

                        if (typeMap.push_back(parms, pArg))
                        {
                            // It's converted
                        }
                        else
                        {
                            pObjectReference_type const pObjRef = m_pType->getModule().getEngine().getObject(pArg);

                            if (pObj)
                            {
                                parms.push_back(pObj);
                            }
                            else
                            {
                                // Error, type not supported
                                std::string unknown("Unknown Argument Type");
                                parms.push_back(unknown);
                            }
                        }

                        _typeobject* pType = pArg->ob_type;
                    }
                }
                else
                {
                    // TODO Don't copy / paste this logic from above
                    if (typeMap.push_back(parms, _pArgs))
                    {
                        // It's converted
                    }
                    else
                    {
                        pObjectReference_type const pObjRef = m_pType->getModule().getEngine().getObject(_pArgs);

                        if (pObjRef)
                        {
                            parms.push_back(pObjRef);
                        }
                        else
                        {
                            // Error, type not supported
                            std::string unknown("Unknown Argument Type");
                            parms.push_back(unknown);
                        }
                    }
                }

                if (m_functionType == VOID_FUNCTION_ARGS)
                {
                    m_function1(pObj, parms);
                    return Py_None;
                }
                else if (m_functionType == OBJECT_FUNCTION_ARGS)
                {
                    pObjectReference_type const pObjReference = m_function2(pObj, parms);
                    pScriptObject_type const pReturnObj = pObjReference->getScriptObject();
                    return dynamic_cast<PythonObject*>(pReturnObj.get())->get();
                }
                else if (m_functionType == STRING_FUNCTION_ARGS)
                {
                    const std::string& returnValue = m_function4(pObj, parms);

                    return PyString_FromString(returnValue.c_str());
                }
                else if (m_functionType == BOOL_FUNCTION_ARGS)
                {
                    const bool returnValue = m_function7(pObj, parms);
                    return returnValue ? Py_True : Py_False;
                }
                else if (m_functionType == INT_FUNCTION_ARGS)
                {
                    const int returnValue = m_function9(pObj, parms);
                    return PyLong_FromLong(returnValue);
                }

            } // case All functions that take args
        } // switch(m_functionType)
    }

    // TODO Throw an exception because we should never get here.
    return Py_None;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPython
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
