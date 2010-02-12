//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Matthew Alan Gray
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
#ifndef INDIEZEN_ZPYTHON_PYTHON_MODULE_HPP_INCLUDED
#define INDIEZEN_ZPYTHON_PYTHON_MODULE_HPP_INCLUDED

#include <Python.h>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptObject.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPython {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class PythonEngine;
class PythonType;

class PythonModule
:   public Scripting::I_ScriptModule
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptEngine>         pScriptEngine_type;

    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptType>           pScriptType_type;
    typedef Zen::Memory::managed_weak_ptr<Scripting::I_ScriptType>      wpScriptType_type;

    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptObject>         pScriptObject_type;
    typedef Zen::Memory::managed_weak_ptr<Scripting::I_ScriptObject>    wpScriptObject_type;

    typedef Scripting::I_ObjectReference*                               pObjectReference_type;

    typedef std::map<std::string, pScriptType_type>                     type_collection_type;
    /// @}

    /// @name I_ScriptModule implementation
    /// @{
public:
    virtual pScriptType_type createScriptType(const std::string& _typeName, const std::string& _docString, unsigned long _rawSize);
    virtual pScriptType_type getScriptType(const std::string& _typeName);
    virtual void activate();
    virtual void createObject(pScriptType_type _pType, pObjectReference_type _pObject);
    virtual void createGlobalObject(const std::string& _name, pScriptType_type _pType, pObjectReference_type _pObject);
    virtual pScriptEngine_type getScriptEngine();
    /// @}

    /// @name Additional Implementation
    /// @{
public:
    const std::string& getName() const;
    PyObject* newObject(pScriptType_type _pType, pObjectReference_type _pObject);
    PythonEngine& getEngine();
    /// @}

    /// @name Event handlers
    /// @{
public:
    void onDestroyScriptType(wpScriptType_type _pScriptType);
    void onDestroyScriptObject(wpScriptObject_type _pScriptObject);
    /// @}

    /// @name 'Structors
    /// @{
public:
             PythonModule(PythonEngine* _pEngine, const std::string& _name, const std::string& _docString);
    virtual ~PythonModule();
    /// @}

    /// @name Member Variables
    /// @{
private:
    PythonEngine*           m_pEngine;
    std::string             m_name;
    std::string             m_docString;

    type_collection_type    m_types;

    PyObject*               m_pModule;
    PyObject*               m_pModuleDict;

    /// @}

};  // class PythonModule

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPython
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_ZPYTHON_PYTHON_MODULE_HPP_INCLUDED
