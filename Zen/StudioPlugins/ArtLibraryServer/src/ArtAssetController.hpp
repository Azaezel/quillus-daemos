//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Art Library
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ARTLIBRARY_ART_ASSET_CONTROLLER_HPP_INCLUDED
#define ARTLIBRARY_ART_ASSET_CONTROLLER_HPP_INCLUDED

#include <Zen/StudioPlugins/ArtLibraryCommon/I_ArtAssetController.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ArtAssetDocument;

/// Art Asset Document Controller.
class ArtAssetController
:   public I_ArtAssetController
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ArtAssetController implementation
    /// @{
public:
    /// @}

    /// @name I_SpreadSheetController implementation
    /// @{
public:
    virtual void insertColumn(int _position, const std::string& _columnLabel);
    virtual void insertRow(int _position, const std::string& _rowLabel);
    virtual void setColumnLabelText(int _columnNumber, const std::string& _text);
    virtual void setRowLabelText(int _rowNumber, const std::string& _text);
    virtual void setCellTextValue(int _column, int _row, const std::string& _value);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ArtAssetDocument;
             ArtAssetController(ArtAssetDocument& _document);
    virtual ~ArtAssetController();
    /// @}

    /// @name Member variables
    /// @{
private:
    ArtAssetDocument&       m_document;
    /// @}

};  // class ArtAssetController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ARTLIBRARY_ART_ASSET_CONTROLLER_HPP_INCLUDED
