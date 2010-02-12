//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#include "PythonModule.hpp"
#include "PythonType.hpp"
#include "PythonObject.hpp"
#include "PythonEngine.hpp"

#include <Zen/Core/Scripting/I_ObjectReference.hpp>

#include <stddef.h>

#include <boost/function.hpp>
#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPython {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static PyMethodDef module_methods[] = 
{
    {NULL}  /* Sentinel */
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonModule::PythonModule(PythonEngine* _pEngine, const std::string& _name, const std::string& _docString)
:   m_pEngine(_pEngine)
,   m_name(_name)
,   m_docString(_docString)
{
    m_pModule = Py_InitModule3(m_name.c_str(), module_methods, m_docString.c_str());
    m_pModuleDict = PyModule_GetDict(m_pModule);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonModule::~PythonModule()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonModule::pScriptType_type
PythonModule::createScriptType(const std::string& _typeName, const std::string& _docString, unsigned long _rawSize)
{
    PythonType* const pPythonType = new PythonType(this, _typeName, _docString, _rawSize);

    PyModule_AddObject(m_pModule, pPythonType->getName().c_str(), (PyObject *)pPythonType->getRawType());

    pScriptType_type const pType(pPythonType,
        boost::bind(&PythonModule::onDestroyScriptType,this,_1));

    m_types[_typeName] = pType;

    // TODO Retain a reference to this object?
    return pType;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonModule::pScriptType_type
PythonModule::getScriptType(const std::string& _typeName)
{
    return m_types[_typeName];
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PythonModule::activate()
{
#if 0
    for(type_collection_type::iterator iter = m_types.begin(); iter != m_types.end(); iter++)
    {
        PythonType* const pType = *iter;

        pType->activate();

        PyDict_SetItemString(m_pModuleDict, pType->getName().c_str(), pType->getRawClass());
    }
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PythonModule::createObject(pScriptType_type _pType, pObjectReference_type _pObject)
{
    newObject(_pType, _pObject);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PythonModule::createGlobalObject(const std::string& _name, pScriptType_type _pType, pObjectReference_type _pObject)
{
    PyObject* const pObject = newObject(_pType, _pObject);
    PyDict_SetItemString(m_pModuleDict, _name.c_str(), pObject);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonModule::pScriptEngine_type
PythonModule::getScriptEngine()
{
    return getEngine().getSelfReference().lock();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PyObject*
PythonModule::newObject(pScriptType_type _pType, pObjectReference_type _pObject)
{
    // Convert the type
    PythonType* const pType = dynamic_cast<PythonType*>(_pType.get());

    if(pType == NULL)
    {
        // TODO Error, wrong I_ScriptType
        return NULL;
    }

    PyObject* const pObject = pType->getRawType()->tp_new(pType->getRawType(), NULL, NULL);

    // Associate the I_ObjectReference with the PyObject
    m_pEngine->setObject(pObject, _pObject);

    // Create a new PythonObject from the new PyObject
    pScriptObject_type pScriptObject(new PythonObject(pObject),
        boost::bind(&PythonModule::onDestroyScriptObject,this,_1));

    // Associate the new PyObject with the I_ObjectReference
    _pObject->setScriptObject(pScriptObject);

    return pObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonEngine&
PythonModule::getEngine()
{
    return *m_pEngine;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
PythonModule::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PythonModule::onDestroyScriptType(wpScriptType_type _pScriptType)
{
    /// Fire the ScriptType onDestroyEvent
    _pScriptType->onDestroyEvent(_pScriptType);

    /// Delete the ScriptType
    PythonType* pScriptType = dynamic_cast<PythonType*>(_pScriptType.get());

    if( pScriptType )
    {
        delete pScriptType;
    }
    else
    {
        throw std::exception("Zen::ZPython::PythonModule::onDestroyScriptType() : _pScriptType is an invalid PythonType.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PythonModule::onDestroyScriptObject(wpScriptObject_type _pScriptObject)
{
    /// Fire the ScriptObject onDestroyEvent
    _pScriptObject->onDestroyEvent(_pScriptObject);

    /// Delete the ScriptObject
    PythonObject* pScriptObject = dynamic_cast<PythonObject*>(_pScriptObject.get());

    if( pScriptObject )
    {
        delete pScriptObject;
    }
    else
    {
        throw std::exception("Zen::ZPython::PythonModule::onDestroyScriptObject() : _pScriptObject is an invalid PythonObject.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPython
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
