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
#ifndef ZEN_PATH_GUI_APPLICATION_HPP_INCLUDED
#define ZEN_PATH_GUI_APPLICATION_HPP_INCLUDED

#include <boost/serialization/serialization.hpp>

#include <string>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace PathGUI {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Application
{
    /// @name Forward declarations
    /// @{
public:
    class PluginPathVisitor;
    class ModulePathVisitor;
    class RequireDirective;
    /// @}

    /// @name Types
    /// @{
public:
    /// @}

    /// @name Application implementation
    /// @{
public:
    void setID(const std::string& _id);
    const std::string& getID() const;

    void setName(const std::string& _name);
    const std::string& getName() const;

    void setVersion(const std::string& _version);
    const std::string& getVersion() const;

    void setProviderName(const std::string& _providerName);
    const std::string& getProviderName() const;

    void addPluginPath(const std::string& _pluginPath);
    void dropPluginPath(const std::string& _pluginPath);
    void processPluginPaths(PluginPathVisitor& _visitor);

    void addModulePath(const std::string& _modulePath);
    void dropModulePath(const std::string& _modulePath);
    void processModulePaths(ModulePathVisitor& _visitor);

    void setRequireDirective(const RequireDirective& _requireDirective);
    const RequireDirective& getRequireDirective() const;
    /// @}

    /// @name PluginPath declaration
    /// @{
public:
    class PluginPath
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name PluginPath implementation
        /// @{
    public:
        void setPath(const std::string& _path);
        const std::string& getPath() const;
        /// @}

        /// @name PluginPath Serialization
        /// @{
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& _ar, const unsigned int _version)
        {
            _ar & boost::serialization::make_nvp("path", m_path);
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 PluginPath();
        virtual ~PluginPath();
        /// @}

        /// @name Member variables
        /// @{
    private:
        std::string     m_path;
        /// @}

    };  // class PluginPath
    /// @}

    /// @name PluginPathVisitor declaration
    /// @{
public:
    class PluginPathVisitor
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name PluginPathVisitor interface
        /// @{
        virtual void begin() = 0;

        virtual void visit(const PluginPath& _pluginPath) = 0;

        virtual void end() = 0;
        /// @}

        /// @name 'Structors
        /// @{
    protected:
                 PluginPathVisitor();
        virtual ~PluginPathVisitor();
        /// @}

    };  // class PluginPathVisitor
    /// @}

    /// @name ModulePath declaration
    /// @{
public:
    class ModulePath
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name ModulePath implementation
        /// @{
    public:
        void setPath(const std::string& _path);
        const std::string& getPath() const;
        /// @}

        /// @name ModulePath Serialization
        /// @{
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& _ar, const unsigned int _version)
        {
            _ar & boost::serialization::make_nvp("path", m_path);
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 ModulePath();
        virtual ~ModulePath();
        /// @}

        /// @name Member variables
        /// @{
    private:
        std::string     m_path;
        /// @}

    };  // class ModulePath
    /// @}

    /// @name ModulePathVisitor
    /// @{
public:
    class ModulePathVisitor
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name ModulePathVisitor interface
        /// @{
    public:
        virtual void begin() = 0;

        virtual void visit(const ModulePath& _modulePath) = 0;

        virtual void end() = 0;
        /// @}

        /// @name 'Structors
        /// @{
    protected:
                 ModulePathVisitor();
        virtual ~ModulePathVisitor();
        /// @}

    };  // class ModulePathVisitor
    /// @}

    /// @name RequireDirective declaration
    /// @{
public:
    class RequireDirective
    {
        /// @name Forward declarations
        /// @{
    public:
        class ImportDirectiveVisitor;
        /// @}

        /// @name Types
        /// @{
    public:
        /// @}

        /// @name RequireDirective implementation
        /// @{
    public:
        void addImportDirective(const std::string& _plugin);
        void dropImportDirective(const std::string& _plugin);
        void processImportDirectives(const ImportDirectiveVisitor& _visitor);
        /// @}

        /// @name ImportDirective declaration
        /// @{
        class ImportDirective
        {
            /// @name Types
            /// @{
        public:
            /// @}

            /// @name ImportDirective implementation
            /// @{
        public:
            /// @}

            /// @name ImportDirective serialization
            /// @{
        private:
            friend class boost::serialization::access;
            template<class Archive>
            void serialize(Archive& _ar, const unsigned int _version)
            {
                _ar & boost::serialization::make_nvp("plugin", m_plugin);
            }
            /// @}

            /// @name 'Structors
            /// @{
        public:
                     ImportDirective();
            virtual ~ImportDirective();
            /// @}

            /// @name Member variables
            /// @{
        private:
            std::string     m_plugin;
            /// @}

        };  // class ImportDirective
        /// @}

        /// @name ImportDirectiveVisitor
        /// @{
        class ImportDirectiveVisitor
        {
            /// @name Types
            /// @{
        public:
            /// @}

            /// @name ImportDirectiveVisitor interface
            /// @{
        public:
            virtual void begin() = 0;

            virtual void visit(const ImportDirective& _importDirective) = 0;

            virtual void end() = 0;
            /// @}

            /// @name 'Structors
            /// @{
        protected:
                     ImportDirectiveVisitor();
            virtual ~ImportDirectiveVisitor();
            /// @}

        };  // class ImportDirectiveVisitor
        /// @}

        /// @name RequireDirective Serialization
        /// @{
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& _ar, const unsigned int _version)
        {
            if( _ar.is_loading<> == boost::mpl::bool_<true> )
            {
                // If using a input archive

                // Destroy the contents of m_pImportDirectives
                std::vector<ImportDirective*>::iterator iter = m_pImportDirectives.begin();
                while( iter != m_pImportDirectives.end() )
                {
                    delete *iter;
                    *iter = NULL;
                    iter++;
                }

                // Populate m_pImportDirectives from the archive
                bool done = false;
                m_pImportDirectives.clear();
                ImportDirective* pImportDirective = NULL;
                while( !done )
                {
                    _ar & boost::serialization::make_nvp("import", pImportDirective);

                    if( pImportDirective != NULL || m_pImportDirectives.empty() || pImportDirective != m_pImportDirectives.back() )
                    {
                        m_pImportDirectives.push_back(pImportDirective);
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
                std::vector<ImportDirective*>::iterator iter = m_pImportDirectives.begin();
                while( iter != m_pImportDirectives.end() )
                {
                    _ar & boost::serialization::make_nvp("import", *iter);

                    iter++;
                }
            }
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 RequireDirective();
        virtual ~RequireDirective();
        /// @}

        /// @name Member variables
        /// @{
    private:
        std::vector<ImportDirective*>   m_pImportDirectives;
        /// @}

    };  // class RequireDirective
    /// @}

    /// @name Application Serialization
    /// @{
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& _ar, const unsigned int _version)
    {
        _ar & boost::serialization::make_nvp("id", m_id);
        _ar & boost::serialization::make_nvp("name", m_name);
        _ar & boost::serialization::make_nvp("version", m_version);
        _ar & boost::serialization::make_nvp("provider-name", m_providerName);

        if( _ar.is_loading<> == boost::mpl::bool_<true> )
        {
            // If using a input archive

            // Destroy the contents of m_pPluginPaths
            std::vector<PluginPath*>::iterator iter = m_pPluginPaths.begin();
            while( iter != m_pPluginPaths.end() )
            {
                delete *iter;
                *iter = NULL;
                iter++;
            }

            // Populate m_pPluginPaths from the archive
            bool done = false;
            m_pPluginPaths.clear();
            PluginPath* pPluginPath = NULL;
            while( !done )
            {
                _ar & boost::serialization::make_nvp("plugin-path", pPluginPath);

                if( pPluginPath != NULL || m_pPluginPaths.empty() || pPluginPath != m_pPluginPaths.back() )
                {
                    m_pPluginPaths.push_back(pPluginPath);
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
            std::vector<PluginPath*>::iterator iter = m_pPluginPaths.begin();
            while( iter != m_pPluginPaths.end() )
            {
                _ar & boost::serialization::make_nvp("plugin-path", *iter);

                iter++;
            }
        }

        if( _ar.is_loading<> == boost::mpl::bool_<true> )
        {
            // If using an input archive
        
            // Destroy the contents of m_pModulePaths
            std::vector<ModulePath*>::iterator iter = m_pModulePaths.begin();
            while( iter != m_pModulePaths.end() )
            {
                delete *iter;
                *iter = NULL;
                iter++;
            }

            // Populate m_pModulePaths from the archive
            bool done = false;
            m_pModulePaths.clear();
            ModulePath* pModulePath = NULL;
            while( !done )
            {
                _ar & boost::serialization::make_nvp("module-path", pModulePath);

                if( pModulePath != NULL || m_pModulePaths.empty() || pModulePath != m_pModulePaths.back() )
                {
                    m_pModulePaths.push_back(pModulePath);
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
            std::vector<ModulePath*>::iterator iter = m_pModulePaths.begin();
            while( iter != m_pModulePaths.end() )
            {
                _ar & boost::serialization::make_nvp("plugin-path", *iter);

                iter++;
            }
        }

        _ar & boost::serialization::make_nvp("requires", m_requireDirective);
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
             Application();
    virtual ~Application();
    /// @}

    /// @name Member variables
    /// @{
private:
    std::string                 m_id;
    std::string                 m_name;
    std::string                 m_version;
    std::string                 m_providerName;

    std::vector<PluginPath*>    m_pPluginPaths;
    std::vector<ModulePath*>    m_pModulePaths;
    RequireDirective            m_requireDirective;
    /// @}

};  // class Application

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace PathGUI
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PATH_GUI_APPLICATION_HPP_INCLUDED
