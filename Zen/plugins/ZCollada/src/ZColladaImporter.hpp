//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
#ifndef INDIEZEN_ZCOLLADA_ZCOLLADA_IMPORTER_HPP_INCLUDED
#define INDIEZEN_ZCOLLADA_ZCOLLADA_IMPORTER_HPP_INCLUDED

#include <IndieZen/framework/ResourceManager/I_Importer.hpp>

//#include "ZColladaParser.hpp"
//#include "ZColladaGrammar.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace ZCollada {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Alias|Wavefront .OBJ importer
class ZColladaImporter
:   public ResourceManager::I_Importer
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Importer implementation
    /// @{
public:
    /// Get the type of resource this importer imports.
    virtual const std::string& getType() const;

    virtual void import(const boost::filesystem::path& _path, bool _async);
    // @}

	/// @name Additional implementation
	/// @{
public:
	void synchronousImport(const std::string& _path);
    //ZColladaParser& getParser() { return m_parser; }
	/// @}

    /// @name 'Structors
    /// @{
public:
             ZColladaImporter();
    virtual ~ZColladaImporter();
    /// @}

    /// @name Member variables
    /// @{
private:
    std::string m_type;

    //ZColladaParser   m_parser;
    //ZColladaGrammar  m_grammar;

    /// @}

};  // class ZColladaImporter

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCollada
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_ZCOLLADA_ZCOLLADA_IMPORTER_HPP_INCLUDED
