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
#include "PythonType.hpp"
#include "PythonModule.hpp"

#include <sstream>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPython {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonType::PythonType(PythonModule* _pModule, const std::string& _name, const std::string& _docString, unsigned long _rawSize)
:   m_pModule(_pModule)
,   m_name(_name)
,   m_docString(_docString)
{
#if 1
    memset(&m_type, 0, sizeof(PyTypeObject));

    std::ostringstream fullName;
    fullName << m_pModule->getName() << "." << m_name;
    m_fullName = fullName.str();

    m_type._ob_next     = 0;
    m_type._ob_prev     = 0;
    m_type.ob_refcnt    = 1;
    m_type.ob_type      = NULL;
    m_type.ob_size      = 0;
    m_type.tp_name      = m_fullName.c_str();
    m_type.tp_basicsize = _rawSize;
    m_type.tp_flags     = Py_TPFLAGS_DEFAULT;
    m_type.tp_doc       = m_docString.c_str();

    // Is this correct?
    m_type.tp_new       = PyType_GenericNew;

    PyType_Ready(&m_type);

    Py_INCREF(&m_type);
#else

    /// New style?
    PyObject* const pParent = NULL;
    m_pClassDict = PyDict_New();
    PyObject* const pClassName = PyString_FromString(m_name.c_str());
    m_pClass = PyClass_New(pParent, m_pClassDict, pClassName);

    Py_DECREF(pClassName);
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PythonType::~PythonType()
{
    Py_DECREF(m_pClassDict);
    Py_DECREF(m_pClass);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PythonType::activate()
{
    //PyType_Ready(&m_type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPython
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
