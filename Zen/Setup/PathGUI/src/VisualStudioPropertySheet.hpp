//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C) 2008        Matthew Alan Gray
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
#ifndef ZEN_PATH_GUI_VISUAL_STUDIO_PROPERTY_SHEET_HPP_INCLUDED
#define ZEN_PATH_GUI_VISUAL_STUDIO_PROPERTY_SHEET_HPP_INCLUDED

#include <boost/serialization/serialization.hpp>

#include <string>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace PathGUI {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class VisualStudioPropertySheet
{
    /// @name Forward declarations
    /// @{
public:
    class Tool;
    class UserMacro;
    class UserMacroVisitor;
    /// @}

    /// @name Types
    /// @{
public:
    /// @}

    /// @name VisualStudioPropertySheet implementation
    /// @{
public:
    void setTool(const Tool& _tool);
    const Tool& getTool() const;

    void addUserMacro(const UserMacro& _userMacro);
    void dropUserMacro(const UserMacro& _userMacro);
    void processUserMacros(const UserMacroVisitor& _visitor);
    /// @}

    /// @name Tool declaration
    /// @{
public:
    class Tool
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name Tool implementation
        /// @{
    public:
        void setName(const std::string& _name);
        const std::string& getName() const;

        void setPreprocessorDefinitions(const std::string& _preprocessorDefinitions);
        const std::string& getPreprocessorDefinitions() const;
        /// @}

        /// @name Tool Serialization
        /// @{
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& _ar, const unsigned int _version)
        {
            _ar & boost::serialization::make_nvp("Name", m_name);
            _ar & boost::serialization::make_nvp("PreprocessorDefinitions", m_preprocessorDefinitions);
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 Tool();
        virtual ~Tool();
        /// @}

        /// @name Member variables
        /// @{
    private:
        std::string     m_name;
        std::string     m_preprocessorDefinitions;
        /// @}

    };  // class Tool
    /// @}

    /// @name UserMacro declaration
    /// @{
public:
    class UserMacro
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name UserMacro implementation
        /// @{
    public:
        void setName(const std::string& _name);
        const std::string& getName() const;

        void setValue(const std::string& _value);
        const std::string& getValue() const;

        void setPerformEnvironmentSet(bool _isPerformEnvironmentSet);
        bool isPerformEnvironmentSet() const;
        /// @}

        /// @name UserMacro Serialization
        /// @{
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& _ar, const unsigned int _version)
        {
            _ar & boost::serialization::make_nvp("Name", m_name);
            _ar & boost::serialization::make_nvp("Value", m_value);
            _ar & boost::serialization::make_nvp("PerformEnvironmentSet", m_performEnvironmentSet);
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 UserMacro();
        virtual ~UserMacro();
        /// @}

        /// @name Member variables
        /// @{
    private:
        /// Deviating from standard member variable naming convention
        /// in order to ensure boost::serialization output matches 
        /// vsprops format.
        std::string     m_name;
        std::string     m_value;
        std::string     m_performEnvironmentSet;
        /// @}

    };  // class UserMacro
    /// @}

    /// @name UserMacroVisitor declaration
    /// @{
public:
    class UserMacroVisitor
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name UserMacroVisitor interface
        /// @{
    public:
        virtual void begin() = 0;

        virtual void visit(const UserMacro& _userMacro) = 0;

        virtual void end() = 0;
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 UserMacroVisitor();
        virtual ~UserMacroVisitor();
        /// @}

    };  // class UserMacroVisitor
    /// @}

    /// @name VisualStudioPropertySheet Serialization
    /// @{
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& _ar, const unsigned int _version)
    {
        _ar & boost::serialization::make_nvp("Tool", m_tool);

        if( _ar.is_loading<> == boost::mpl::bool_<true> )
        {
            // If using an input archive

            // Destroy contents of m_pUserMacros
            std::vector<UserMacro*>::iterator iter = m_pUserMacros.begin();
            while( iter != m_pUserMacros.end() )
            {
                delete *iter;
                *iter = NULL;
                iter++;
            }

            // Populate m_pUserMacros from the archive
            bool done = false;
            m_pUserMacros.clear();
            UserMacro* pUserMacro = NULL;
            while( !done )
            {
                _ar & boost::serialization::make_nvp("UserMacro", pUserMacro);
                
                if( pUserMacro != NULL || m_pUserMacros.empty() || pUserMacro != m_pUserMacros.back() )
                {
                    m_pUserMacros.push_back(pUserMacro);
                }
                else
                {
                    done = true;
                }
            }
        }
        else
        {
            // If using an output archive
            std::vector<UserMacro*>::iterator iter = m_pUserMacros.begin();
            while( iter != m_pUserMacros.end() )
            {
                _ar & boost::serialization::make_nvp("UserMacro", *iter);

                iter++;
            }
        }
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
             VisualStudioPropertySheet();
    virtual ~VisualStudioPropertySheet();
    /// @}

    /// @name Member variables
    /// @{
private:
    Tool                        m_tool;
    std::vector<UserMacro*>     m_pUserMacros;
    /// @}

};  // class VisualStudioPropertySheet

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace PathGUI
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PATH_GUI_VISUAL_STUDIO_PROPERTY_SHEET_HPP_INCLUDED
