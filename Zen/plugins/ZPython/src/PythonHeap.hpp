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
#ifndef INDIEZEN_ZPYTHON_PYTHON_HEAP_HPP_INCLUDED
#define INDIEZEN_ZPYTHON_PYTHON_HEAP_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Scripting/I_ObjectHeap.hpp>
#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ObjectReference.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPython {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class PythonEngine;

class PythonHeap
:   public Scripting::I_ObjectHeap
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptEngine>     pScriptEngine_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ObjectReference>  pObjectReference_type;
    typedef std::map<unsigned, pObjectReference_type>               heap_index_type;
    /// @}

    /// @name I_ObjectHeap implementation
    /// @{
public:
    virtual void createObject(pObjectReference_type _pObject);
    virtual pObjectReference_type getGlobalReference(Scripting::I_ObjectReference& _object);
    /// @}

    /// @name 'Structors
    /// @{
public:
             PythonHeap(PythonEngine* _pEngine);
    virtual ~PythonHeap();
    /// @}

    /// @name Member Variables
    /// @{
private:
    PythonEngine*  m_pEngine;
    heap_index_type     m_heap;
    /// @}

};  // class PythonEngine

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPython
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_ZPYTHON_PYTHON_HEAP_HPP_INCLUDED
