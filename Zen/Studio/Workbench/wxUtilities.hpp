//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef WORKBENCH_WXUTILITIES_HPP_INCLUDED
#define WORKBENCH_WXUTILITIES_HPP_INCLUDED

#include "Configuration.hpp"

// Remove some MicroCrap stupidity
#ifdef Yield
#undef Yield
#endif

#include <wx/window.h>

// TR: Not in a namespace so as to make it easier to use.... a mistake?

/// Translate a wxString to std::string.
WORKBENCH_DLL_LINK std::string wx2std(const wxString& _inputString, wxMBConv* const _pConv = 0);

/// Translate std::string to wxString.
WORKBENCH_DLL_LINK wxString std2wx(const std::string& _inputString, wxMBConv* const _pConv = 0);


#endif // WORKBENCH_WXUTILITIES_HPP_INCLUDED
