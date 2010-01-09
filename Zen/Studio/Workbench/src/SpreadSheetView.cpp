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
#include "SpreadSheetView.hpp"
#include "GridCellEditor.hpp"
#include "Workbench.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_SpreadSheetContributor.hpp>
#include <Zen/Studio/Workbench/I_PropertiesHandle.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetViewable.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocument.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocumentType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetAction.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCell.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetProperties.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCellProperties.hpp>

#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

#include <Zen/Studio/Workbench/wxUtilities.hpp>

BEGIN_EVENT_TABLE(Zen::Studio::Workbench::SpreadSheetView, wxPanel)
    EVT_GRID_CMD_LABEL_RIGHT_CLICK(-1, Zen::Studio::Workbench::SpreadSheetView::handleRightClickLabel)
    EVT_GRID_CMD_CELL_RIGHT_CLICK(-1, Zen::Studio::Workbench::SpreadSheetView::handleRightClickCell)
    EVT_GRID_SELECT_CELL(Zen::Studio::Workbench::SpreadSheetView::handleSelectCell)
END_EVENT_TABLE()

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SpreadSheetView::SpreadSheetView(wxWindow* _pParent, Workbench* _pWorkbench, pViewable_type _pViewable, const int _instance)
:   View(_pParent, _pWorkbench, _pViewable, _instance)
,   m_pGrid(NULL)
,   m_pEditor(NULL)
,   m_lastCellPropertyColumn(0) // HACK, this normally should be -1, -1 but thanks to a bug in wxWidgets we have to ignore the first event.
,   m_lastCellPropertyRow(0)
{
    // TODO Eventually _pDocument will be a future.  Set up an event to handle 
    // when it becomes available.

    // Create an empty grid.
    createGUIControls();

    // Subcribing will populate a lot of rows and columns.
    // Put this update into a batch.
    I_SpreadSheetViewable* pViewable = m_pViewable.as<Memory::managed_ptr<I_SpreadSheetViewable> >().get();
    m_pGrid->BeginBatch();
    m_pSubscription = pViewable->subscribe(this);
    m_pGrid->EndBatch();

    m_pGrid->Fit();

    // Register the properties for this document.
    m_changePropertiesEachRow = pViewable->getProperties().cellPropertiesChangeEachRow();
    m_changePropertiesEachColumn = pViewable->getProperties().cellPropertiesChangeEachColumn();
    m_pPropertiesHandle = m_pWorkbench->registerProperties(pViewable->getProperties().getPropertiesName(), pViewable->getProperties());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SpreadSheetView::~SpreadSheetView()
{
    if (m_pEditor)
    {
        // Is this needed?  I don't think it is.
        //m_pEditor->DecRef();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Project*
SpreadSheetView::getProject() const
{
    return getDocument()->getProject();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetView::activate()
{
    m_pPropertiesHandle->show();

    if (m_pCellPropertiesHandle.isValid())
    {
        m_pCellPropertiesHandle->show();
    }

    View::activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetView::deactivate()
{
    m_pPropertiesHandle->hide();

    if (m_pCellPropertiesHandle.isValid())
    {
        m_pCellPropertiesHandle->hide();
    }

    View::deactivate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetView::onNewColumn(const int _columnPosition, const std::string& _columnLabelText)
{
    int columnPosition;

    // -1 means append
    if (_columnPosition == -1)
    {
        m_pGrid->AppendCols();

        // Adjust the column position.
        columnPosition = m_pGrid->GetCols() - 1;
    }
    else
    {
        // Not -1, insert at the correct position
        m_pGrid->InsertCols(_columnPosition);
        columnPosition = _columnPosition;
    }

    if (!_columnLabelText.empty())
    {
        m_pGrid->SetColLabelValue(columnPosition, std2wx(_columnLabelText));
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetView::onNewRow(const int _rowPosition, const std::string& _rowLabeltext)
{
    int rowPosition;

    // -1 means append
    if (_rowPosition == -1)
    {
        m_pGrid->AppendRows();

        // Adjust the row position.
        rowPosition = m_pGrid->GetRows() - 1;
    }
    else
    {
        // Not -1, insert at the correct position
        m_pGrid->InsertRows(_rowPosition);

        rowPosition = _rowPosition;
    }

    if (!_rowLabeltext.empty())
    {
        m_pGrid->SetRowLabelValue(rowPosition, std2wx(_rowLabeltext));
    }

    m_pGrid->Fit();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetView::onCellModified(I_SpreadSheetCell& _cell, const int _column, const int _row)
{
    m_pGrid->SetCellValue(_row, _column, std2wx(_cell.getUserData()->getTextValue()));

    const bool isMutable = _cell.getUserData()->getMutable();

    // HACK If the cell is not mutable, make it italics
    // TODO Get the cell decorations from the cell or from a contributor.
    if (!isMutable)
    {
        wxFont cellFont = m_pGrid->GetCellFont(_row, _column);
        cellFont.SetStyle(wxFONTSTYLE_ITALIC);
        m_pGrid->SetCellFont(_row, _column, cellFont);
        m_pGrid->SetReadOnly(_row, _column, true);
    }

    m_pGrid->Fit();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetView::onNameModified(const std::string& _shortName, const std::string& _longName)
{
    // TODO Check preference to see which name should be used for the tab.

    setPageText(_shortName);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetView::createGUIControls()
{
    // Create the grid
    m_pGrid = new wxGrid(this, -1);

    m_pEditor = new GridCellEditor(*this);
    m_pGrid->SetDefaultEditor(m_pEditor);

    //I_SpreadSheetDocument* pDoc = m_pDocument.as<Memory::managed_ptr<I_SpreadSheetDocument> >().get();

    // Create an empty grid.  pDoc->subscribe(this) will populate the grid.
    m_pGrid->CreateGrid(0, 0, wxGrid::wxGridSelectCells);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace {

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// Helper class to collect all of the actions for all of the contributors for a node and
/// then present it as a menu.
/// @todo use menu_contributor_visitor template
class MenuEvent
:   public wxEvtHandler
,   public Zen::Studio::Workbench::I_SpreadSheetViewable::I_SpreadSheetViewEvent
{
    /// @name Types
    /// @{
public:
    typedef I_SpreadSheetAction                                             Action_type;
    typedef std::list<Action_type*>                                         Actions_type;
    typedef std::map<int, Action_type*>                                     IndexedActions_type;
    /// @}

    /// @name I_SpreadSheetViewEvent implementation
    /// @{ 
public:
    virtual I_SpreadSheetView& getView()
    {
        throw Utility::runtime_exception("MenuEvent::getView(): Error, not implemented.");
    }

    virtual I_SpreadSheetCell* getCell()
    {
        throw Utility::runtime_exception("MenuEvent::getCell(): Error, not implemented.");
    }

    virtual int getColumn()
    {
        throw Utility::runtime_exception("MenuEvent::getColumn(): Error, not implemented.");
    }

    virtual int getRow()
    {
        throw Utility::runtime_exception("MenuEvent::getRow(): Error, not implemented.");
    }

    virtual void addAction(I_SpreadSheetAction& _action)
    {
        m_actionList.push_back(&_action);
    }

    /// Call this method to reject the event.
    /// If the event is rejected, no actions are displayed or invoked.
    virtual void rejectEvent()
    {
        throw Utility::runtime_exception("MenuEvent::rejectEvent(): Error, not implemented.");
    }

    /// @name
    void handleEvent(wxCommandEvent& _event)
    {
        doAction(_event.GetId());
    }

    virtual void end()
    {
        // Iterate through the list of actions and create the menu
        for(Actions_type::iterator iter = m_actionList.begin(); iter != m_actionList.end(); iter++)
        {
            // Create a new menu item.
            // TODO This should be done differently so that the menu can
            //  have sections and have a guaranteed consistent order.
            m_pMenu->Append(m_id, std2wx((*iter)->getName()));
            m_actions[m_id] = *iter;
            m_id++;
        }

        // ACK!  wxCommandEventHandler does static casts instead of dynamic casts... can't
        // get rid of the warning.  If you copy this code, make sure wxEvtHandler is the
        // first class in the inheritence chain, otherwise it'll get it wrong.
        Connect(m_startId, m_id, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MenuEvent::handleEvent));

    }

    void doAction(int _id)
    {
        IndexedActions_type::iterator iter = m_actions.find(_id);
        if (iter != m_actions.end())
        {
            // TODO instead of -1, -1, pass in the coordinates of the currently selected cell.
            iter->second->invoke(m_contributee, -1, -1);
        }
    }

    MenuEvent(I_SpreadSheetView& _view, wxMenu* _pMenu, int _id)
    :   m_contributee(_view)
    ,   m_pMenu(_pMenu)
    ,   m_startId(_id)
    ,   m_id(_id)
    {
    }

private:
    I_SpreadSheetView&                          m_contributee;
    wxMenu*                                     m_pMenu;
    int                                         m_startId;
    int                                         m_id;
    Actions_type                                m_actionList;
    IndexedActions_type                         m_actions;
};  // class MenuEvent

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0 // deprecated?
/// Simple Contributor Visitor.
/// @todo use contributor_visitor template
struct ContributorVisitor
:   public Zen::Studio::Workbench::I_SpreadSheetDocumentType::I_SpreadSheetContributorVisitor
{
    typedef Zen::Studio::Workbench::I_SpreadSheetView                       Contributee_type;
    typedef Contributee_type::Contributor_type                              Contributor_type;
    typedef Contributee_type::Action_type                                   Action_type;

    typedef std::list<Action_type*>                                         Actions_type;

    virtual void begin()
    {
    }

    virtual void visit(Contributee_type& _contributee, Contributor_type& _contributor)
    {
        // Create an action visitor
        action_visitor<Contributee_type>
            visitor(m_actionList);

        // Get the actions.
        // TODO Instead of NULL, pass in the selected cell or coordinates or something.
        _contributor.getActions(_contributee, NULL, visitor);
    }

    virtual void end()
    {

    }

    ContributorVisitor()
    {
    }

public:
    Actions_type                    m_actionList;
};  // class ContributorVisitor
#endif // deprecated?
}   // anonymous namespace
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetView::handleRightClickLabel(wxGridEvent& _event)
{
    I_SpreadSheetViewable* pViewable = dynamic_cast<I_SpreadSheetViewable*>(getViewable());

    // TODO Distinguish between row and column events.

    // Create a popup menu
    std::auto_ptr<wxMenu> pMenu(new wxMenu);
    
    const int startId = wxID_HIGHEST + 1000;

    MenuEvent visitor(*this, pMenu.get(), startId);

    // TODO getType() needs to be moved to I_SpreadSheetViewable?
    // Or possibly getContributors() should be moved.
    // Or maybe have a priority event queue where the listeners
    // get to contribute to the I_SpreadSheetActionVisitor.
    pViewable->onRightClickLabel(visitor);
    visitor.end();

    if (pMenu->GetMenuItemCount() > 0)
    {
        pMenu->SetEventHandler(&visitor);
        PopupMenu(pMenu.get());
        pMenu->SetEventHandler(NULL);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetView::handleRightClickCell(wxGridEvent& _event)
{
    // TODO show the contributors or something.
    //throw Utility::runtime_exception("SpreadSheetView::handleRightClickCell(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SpreadSheetView::handleSelectCell(wxGridEvent& _event)
{
    // Continue processing this event since we don't actually handle it.
    _event.Skip();

    // Check to see if the properties need to be changed.
    bool changeProperties = false;
    if (m_changePropertiesEachRow && _event.GetRow() != m_lastCellPropertyRow)
    {
        changeProperties = true;
    }
    else if (m_changePropertiesEachColumn && _event.GetCol() != m_lastCellPropertyColumn)
    {
        changeProperties = true;
    }

    // If the properties need to be changed, do it.
    if (changeProperties)
    {
        I_SpreadSheetViewable* pViewable= dynamic_cast<I_SpreadSheetViewable*>(getDocument());


        I_SpreadSheetCellProperties& properties = pViewable->getCellProperties(_event.GetCol(), _event.GetRow());
        m_pCellPropertiesHandle = m_pWorkbench->registerProperties(properties.getPropertiesName(), properties);

        m_pCellPropertiesHandle->show();
    }

    m_lastCellPropertyColumn    = _event.GetCol();
    m_lastCellPropertyRow       = _event.GetRow();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SpreadSheetCell&
SpreadSheetView::getCell(const int _column, const int _row)
{
    I_SpreadSheetDocument* pDoc = dynamic_cast<I_SpreadSheetDocument*>(getDocument());

    return pDoc->getCell(_column, _row);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
