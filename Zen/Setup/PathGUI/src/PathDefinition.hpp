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
#ifndef ZEN_PATH_GUI_PATH_DEFINITION_HPP_INCLUDED
#define ZEN_PATH_GUI_PATH_DEFINITION_HPP_INCLUDED

#include <boost/serialization/serialization.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace PathGUI {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PathDefinition
{
    /// @name Forward declarations
    /// @{
public:
    class ResidentFileVisitor;
    class ResidentFolderVisitor;
    /// @}

    /// @name Types
    /// @{
public:
    /// @}

    /// @name PathDefinition implementation
    /// @{
public:
    void setName(const std::string& _name);
    const std::string& getName() const;

    void setDescription(const std::string& _description);
    const std::string& getDescription() const;

    void setDefaultPath(const std::string& _defaultPath);
    const std::string& getDefaultPath() const;

    void addResidentFile(const std::string& _file);
    void dropResidentFile(const std::string& _file);
    void processResidentFiles(const ResidentFileVisitor& _visitor);

    void addResidentFolder(const std::string& _folder);
    void dropResidentFolder(const std::string& _folder);
    void processResidentFolders(const ResidentFolderVisitor& _visitor);
    /// @}

    /// @name ResidentFile declaration
    /// @{
public:
    class ResidentFile
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name ResidentFile implementation
        /// @{
    public:
        void setName(const std::string& _name);
        const std::string& getName() const;
        /// @}

        /// @name ResidentFile serialization
        /// @{
    private:
        friend boost::serialization::access;
        template<class Archive>
        void serialize(Archive& _ar, const unsigned int _version)
        {
            _ar & boost::serialization::make_nvp("name", m_name);
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 ResidentFile();
        virtual ~ResidentFile();
        /// @}

        /// @name Member variables
        /// @{
    private:
        std::string         m_name;
        /// @}

    };  // class ResidentFile
    /// @}

    /// @name ResidentFileVisitor declaration
    /// @{
public:
    class ResidentFileVisitor
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name ResidentFileVisitor interface
        /// @{
    public:
        virtual void begin() = 0;

        virtual void visit(const ResidentFile& _residentFile) = 0;

        virtual void end() = 0;
        /// @}

        /// @name 'Structors
        /// @{
                 ResidentFileVisitor();
        virtual ~ResidentFileVisitor();
        /// @}

    };  // class ResidentFileVisitor
    /// @}

    /// @name ResidentFolder declaration
    /// @{
public:
    class ResidentFolder
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name ResidentFolder implementation
        /// @{
    public:
        void setName(const std::string& _name);
        const std::string& getName() const;

        void addResidentFile(const std::string& _file);
        void dropResidentFile(const std::string& _file);
        void processResidentFiles(const ResidentFileVisitor& _visitor);

        void addResidentFolder(const std::string& _folder);
        void dropResidentFolder(const std::string& _folder);
        void processResidentFolders(const ResidentFolderVisitor& _visitor);
        /// @}

        /// @name ResidentFolder Serialization
        /// @{
    private:
        friend boost::serialization::access;
        template<class Archive>
        void serialize(Archive& _ar, const unsigned int _version)
        {
            _ar & boost::serialization::make_nvp("name", m_name);
            _ar & boost::serialization::make_nvp("residentFiles", m_pResidentFiles);
            _ar & boost::serialization::make_nvp("residentFolders", m_pResidentFolders);
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 ResidentFolder();
        virtual ~ResidentFolder();
        /// @}

        /// @name Member variables
        /// @{
    private:
        std::string                     m_name;
        std::vector<ResidentFile*>      m_pResidentFiles;
        std::vector<ResidentFolder*>    m_pResidentFolders;
        /// @}

    };  // class ResidentFolder
    /// @}

    /// @name ResidentFolderVisitor declaration
    /// @{
public:
    class ResidentFolderVisitor
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name ResidentFolderVisitor interface
        /// @{
    public:
        virtual void begin() = 0;

        virtual void visit(const ResidentFolder& _residentFolder) = 0;

        virtual void end() = 0;
        /// @}

        /// @name 'Structors
        /// @{
    protected:
                 ResidentFolderVisitor();
        virtual ~ResidentFolderVisitor();
        /// @}

    };  // class ResidentFolderVisitor
    /// @}

    /// @name PathDefinition Serialization
    /// @{
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& _ar, const unsigned int _version)
    {
        _ar & boost::serialization::make_nvp("name", m_name);
        _ar & boost::serialization::make_nvp("description", m_description);
        _ar & boost::serialization::make_nvp("defaultPath", m_defaultPath);
        _ar & boost::serialization::make_nvp("residentFiles", m_pResidentFiles);
        _ar & boost::serialization::make_nvp("residentFolders", m_pResidentFolders);        
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
             PathDefinition();
    virtual ~PathDefinition();
    /// @}

    /// @name Member variables
    /// @{
private:
    std::string                     m_name;
    std::string                     m_description;
    std::string                     m_defaultPath;
    std::vector<ResidentFile*>      m_pResidentFiles;
    std::vector<ResidentFolder*>    m_pResidentFolders;
    /// @}

};  // class PathDefinition

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace PathGUI
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PATH_GUI_PATH_DEFINITION_HPP_INCLUDED
