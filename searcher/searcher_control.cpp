// seacher/searcher_control.cpp

#include "ttl_define.h"
#include "tt_string.h"
#include "tt_path.h"

#include "exception.h"
#include "string_table.h"

#include "searcher/searcher_common.h"

#include "searcher/searcher_control.h"

using namespace BMX2WAV;


// -- MainMenu -----------------------------------------------------------
Searcher::MainMenu::MainMenu( void ) :
TtMenuBar( false ),
file_menu_( TtSubMenu::Create() ),
export_menu_( TtSubMenu::Create() ),
tool_menu_( TtSubMenu::Create() ),
prototype_menu_( TtSubMenu::Create() )
{
  auto add_menu = [] ( TtMenu& menu, int command_id, Image::Index::Value image_index, const std::string& str ) {
    menu.AppendNewItem( command_id, str );
    if ( image_index != Image::Index::None ) {
      menu.GetLastItem().SetBmpImage( Image::BMPS[image_index] );
    }
  };
  using Index = Image::Index;

  // -- �t�@�C��
  this->AppendMenu( file_menu_, StrT::Searcher::Main::Menu::File.Get() );
  file_menu_.AppendMenu( export_menu_, StrT::Searcher::Main::Menu::FileExport.Get() );
  file_menu_.GetLastItem().SetBmpImage( Image::BMPS[Index::Export] );
  {
    add_menu( export_menu_, CommandID::ExportCSVToFile,      Index::None, StrT::Searcher::Main::Menu::FileExportCSVToFile.Get() );
    add_menu( export_menu_, CommandID::ExportTSVToClipboard, Index::None, StrT::Searcher::Main::Menu::FileExportTSVToClipboard.Get() );
  }
  file_menu_.AppendSeparator();
  add_menu( file_menu_, CommandID::Close, Index::None, StrT::Searcher::Main::Menu::FileClose.Get() );

  // -- �c�[��
  this->AppendMenu( tool_menu_, StrT::Searcher::Main::Menu::Tool.Get() );
  add_menu( tool_menu_, CommandID::Settings, Index::None, StrT::Searcher::Main::Menu::ToolSettings.Get() );

  // -- �e�X�g
  if ( IniFileOperation::LoadTestMode() ) {
    this->AppendMenu( prototype_menu_, "�J����" );
    add_menu( prototype_menu_, CommandID::Test1, Index::None, "Test1" );
    add_menu( prototype_menu_, CommandID::Test2, Index::None, "Test2" );
    add_menu( prototype_menu_, CommandID::Test3, Index::None, "Test3" );
  }
}


// -- TreeMenu -----------------------------------------------------------
Searcher::TreeMenu::TreeMenu( void ) :
TtSubMenu( TtSubMenuCommand::Create() )
{
  this->AppendNewItem( CommandID::OpenTreeDirectory, StrT::Searcher::Main::PopupTree::OpenDirectory.Get() );
}


// -- EntryMenu ----------------------------------------------------------
Searcher::EntryMenu::EntryMenu( void ) :
TtSubMenu( TtSubMenuCommand::Create() ),
entry_processor_menu_( TtSubMenuCommand::Create() ),
last_selected_item_( nullptr, TtListViewItem::INVALID_INDEX )
{
  this->AppendNewItem( CommandID::HideEntry, StrT::Searcher::Main::PopupEntry::HideEntry.Get() );
  this->AppendSeparator();
  this->AppendMenu( entry_processor_menu_, StrT::Searcher::Main::PopupEntry::ProcessorMenu.Get() );
}


void
Searcher::EntryMenu::SetEntryProcessorMenu( TtSubMenuCommand& menu )
{
  const int EntryProcessorElement = 2;
  this->InsertMenu( EntryProcessorElement, menu, this->GetItemAt( EntryProcessorElement ).GetText() );
  this->DeleteAt( EntryProcessorElement + 1 );
  entry_processor_menu_ = menu;
}


// -- MainToolBar --------------------------------------------------------
Searcher::MainToolBar::MainToolBar( void )
{
}

bool
Searcher::MainToolBar::CreatedInternal( void )
{
  this->TtToolBar::CreatedInternal();

  this->SetExtendedStyle( TtToolBar::ExtendedStyle::DrawDropDwonArrows | TtToolBar::ExtendedStyle::MixedButtons );
  this->SetImageList( *Image::LIST );

  namespace ID = CommandID::MainToolBar;
  using Index = Image::Index;

  auto add_button = [this] ( int command_id, int image_index, const std::string& str, int style ) {
    this->AddButtonWithString( command_id, Image::LIST->GetOffsetIndex( image_index ), str, style );
  };
  auto add_standard_button = [this, add_button] ( int command_id, int image_index, const std::string& str ) {
    add_button( command_id, image_index, str, TtToolBar::Button::Style::Standard );
  };
  auto add_drop_down_button = [this, add_button] ( int command_id, int image_index, const std::string& str, bool is_whole ) {
    add_button( command_id, image_index, str,
                TtToolBar::Button::Style::Standard | (is_whole ? TtToolBar::Button::Style::WholeDropDown : TtToolBar::Button::Style::DropDown ) );
  };
  auto add_drop_down_box = [this] ( int command_id, auto& box, int width, int height ) {
    // For ComboBox
    this->AddSeparator( command_id );
    // �Z�p���[�^�ɑ΂��Ă͕��ύX�ɂȂ�
    this->GetButton( command_id ).SetBmpImageIndex( width + 8 );

    box.Create( {this, command_id} );
    box.SetPositionSize( this->GetButton( command_id ).GetRectangle().left + 4, 2, width, height );
    box.Show();
  };
  auto add_label = [this] ( int command_id, TtStatic& label, const std::string& str, int width ) {
    // For Label
    this->AddSeparator( command_id );
    // �Z�p���[�^�ɑ΂��Ă͕��ύX�ɂȂ�
    this->GetButton( command_id ).SetBmpImageIndex( width + 4 );

    label.Create( {this} );
    label.SetPositionSize( this->GetButton( command_id ).GetRectangle().left + 4, 0, width, 22 );
    label.SetText( str );
    label.Show();
  };

  add_standard_button( ID::ReloadSquirrelScript,     Index::ReloadSquirrelScript,     StrT::Searcher::Main::Toolbar::ReloadSquirrelScript.Get() );
  add_standard_button( ID::ShowSquirrelOutputDialog, Index::ShowSquirrelOutputDialog, StrT::Searcher::Main::Toolbar::ShowSquirrelOutputDialog.Get() );
  this->AddSeparator();

  add_drop_down_button( ID::Export,     Index::Export,     StrT::Searcher::Main::Toolbar::Export.Get(), true );
  add_standard_button(  ID::HomeFolder, Index::HomeFolder, StrT::Searcher::Main::Toolbar::HomeFolder.Get() );
  this->AddSeparator();

  add_label( ID::FilterLabel, filter_label_, StrT::Searcher::Main::Toolbar::FilterLabel.Get(), 48 );
  add_drop_down_box( ID::SelectFilter, select_filter_box_, 140, 120 );
  this->AddSeparator();

  this->AddButtonWithString( ID::AutoDisplayCells, Image::LIST->GetOffsetIndex( Index::AutoDisplayCells ),
                             StrT::Searcher::Main::Toolbar::AutoDisplayCells.Get(), TtToolBar::Button::Style::Standard | TtToolBar::Button::Style::Check );

  add_label( ID::SelectColumnLabel, select_column_label_, StrT::Searcher::Main::Toolbar::SelectColumnLabel.Get(), 60 );
  add_drop_down_box( ID::SelectColumnGroup, select_column_group_box_, 120, 120 );
  add_standard_button( ID::DisplayCells, Index::DisplayCells, StrT::Searcher::Main::Toolbar::DisplayCells.Get() );
  this->AddSeparator();

  add_label( ID::ExecuteSearchLabel, execute_search_label_, StrT::Searcher::Main::Toolbar::ExecuteSearchLabel.Get(), 48 );
  add_drop_down_box( ID::SelectSearch, select_search_box_, 220, 120 );
  add_standard_button( ID::ExecuteSearch, Index::ExecuteSearch, StrT::Searcher::Main::Toolbar::ExecuteSearch.Get() );

  return true;
}

// -- MainTree -----------------------------------------------------------
Searcher::MainTree::MainTree( TtWindow& parent_window ) :
parent_window_( parent_window ),
item_id_list_pool_()
{
}


bool
Searcher::MainTree::CreatedInternal( void )
{
  this->SetImageList( *Image::LIST );
  this->SetItemHeight( this->GetItemHeight() + 2 );

  return true;
}

void
Searcher::MainTree::SetRoot( void )
{
  auto tmp = ItemIDList::GetDrivesItemIDList( parent_window_ );
  item_id_list_pool_.push_back( tmp );
  this->AddItemIDList( this->GetOrigin(), tmp.get() );
}

void
Searcher::MainTree::AddItemIDList( Item parent, ItemIDList* child_id )
{
  Item item = parent.AddChildLast( child_id->GetDisplayName() );
  item.SetParameter( child_id );
  item.SetImageIndex( child_id->GetIconIndex() );
  item.SetSelectedImageIndex( child_id->GetOpenIconIndex() );
  {
    TVITEM tmp = {0, item.GetHandle()};
    if ( child_id->HasSubfolder() ) {
      auto folders = child_id->GetChildrenFolders( parent_window_ );
      if ( NOT( std::all_of( folders.begin(), folders.end(), [] ( auto& grand_child_id ) {
        return TtString::EndWith( grand_child_id->GetPathSafety(), ".zip" );
      } ) ) ) {
        tmp.mask |= TVIF_CHILDREN;
        tmp.cChildren = 1; // true
      }
    }
    if ( child_id->IsGhosted() ) {
      tmp.mask      |= TVIF_STATE;
      tmp.state     |= TVIS_CUT;
      tmp.stateMask |= TVIS_CUT;
    }
    if ( child_id->IsShare() ) {
      tmp.mask      |= TVIF_STATE;
      tmp.state     |= INDEXTOOVERLAYMASK( 1 );
      tmp.stateMask |= TVIS_OVERLAYMASK;
    }
    if ( child_id->IsLink() ) {
      tmp.mask      |= TVIF_STATE;
      tmp.state     |= INDEXTOOVERLAYMASK( 2 );
      tmp.stateMask |= TVIS_OVERLAYMASK;
    }
    item.SetInfo( tmp );
  }
}


void
Searcher::MainTree::AddChildrenOf( Item parent )
{
  for ( std::shared_ptr<ItemIDList>& child_id : parent.GetParameter()->GetChildrenFolders( parent_window_ ) ) {
    item_id_list_pool_.push_back( child_id );

    // child_id->IsFileSystem() ���x���׎g��Ȃ��悤�ɂ��Ă���
    std::string path = child_id->GetPathSafety();
    if ( NOT( path.empty() ) && TtPath::IsDirectory( path ) ) {
      this->AddItemIDList( parent, child_id.get() );
    }
  }
}

std::shared_ptr<Searcher::ItemIDList>
Searcher::MainTree::GetSharedPointerFrom( ItemIDList* item_id_list )
{
  for ( auto& one : item_id_list_pool_ ) {
    if ( one.get() == item_id_list ) {
      return one;
    }
  }
  throw BMX2WAV_INTERNAL_EXCEPTION;
}


// -- MainList -----------------------------------------------------------
Searcher::MainList::MainList( void )
{
}

bool
Searcher::MainList::CreatedInternal( void )
{
  this->SetFullRowSelect( true );
  this->SetHeaderDragAndDrop( true );
  this->SetHasGridLines( true );

  this->ResetColumnsBy( {} );
  this->SetSmallImageList( *Image::LIST );
  return true;
}


void
Searcher::MainList::ResetColumnsBy( const std::vector<std::string>& columns )
{
  this->ClearColumns();
  {
    auto column = this->MakeNewColumn();
    column.SetText( StrT::Searcher::Main::DefaultFirstColumnName.Get() );
  }
  for ( auto& name : columns ) {
    auto column = this->MakeNewColumn();
    column.SetText( name );
    column.SetWidth( MainList::FIRST_COLUMN_DEFAULT_WIDTH );
  }
  this->SetWidthOfFirstColumnAuto();
}


void
Searcher::MainList::MoveItem( unsigned int from, unsigned int to )
{
  if ( from >= this->GetItemCount() || to >= this->GetItemCount() ) {
    return;
  }
  if ( from == to || to == from + 1 ) {
    return;
  }

  std::vector<std::string> tmp;
  Item from_item = this->GetItem( from );
  auto from_item_icon_index = from_item.GetImageIndex();
  auto parameter = from_item.GetParameter();

  for ( unsigned int i = 0; i < this->GetColumnCount(); ++i ) {
    tmp.push_back( from_item.GetSubItemText( i ) );
  }
  // ����ȍ~ item �� Index �������̂Œ���
  Item item = this->InsertNewItem( to );
  item.SetParameter( parameter );
  item.SetImageIndex( from_item_icon_index );
  for ( unsigned int i = 0; i < tmp.size(); ++i ) {
    item.SetSubItemText( i, tmp[i] );
  }
  this->GetItem( from + (from > to ? 1 : 0) ).Remove();
}


std::string
Searcher::MainList::GetDataAsCSV( const std::string& delimiter )
{
  std::string tmp;
  auto dq_add = [&tmp] ( const std::string& str ) {
    tmp.append( "\"" );
    tmp.append( TtString::Replace( str, "\"", "\"\"" ) );
    tmp.append( "\"" );
  };
  for ( unsigned int k = 0; k < this->GetColumnCount(); ++k ) {
    dq_add( this->GetColumn( k ).GetText() );
    if ( k != this->GetColumnCount() - 1 ) {
      tmp.append( delimiter );
    }
  }
  tmp.append( "\r\n" );
  for ( unsigned int i = 0; i < this->GetItemCount(); ++i ) {
    for ( unsigned int k = 0; k < this->GetColumnCount(); ++k ) {
      dq_add( this->GetItem( i ).GetSubItemText( k ) );
      if ( k != this->GetColumnCount() - 1 ) {
        tmp.append( delimiter );
      }
    }
    tmp.append( "\r\n" );
  }
  return tmp;
}


void
Searcher::MainList::MakeNewItemFromEntry( Entry* entry )
{
  Item item = this->MakeNewItem();
  item.SetParameter( entry );
  item.SetText( entry->GetDisplayName() );
  item.SetImageIndex( entry->GetIconIndex() );
}


void
Searcher::MainList::SetWidthOfFirstColumnAuto( void )
{
  if ( this->GetItemCount() == 0 ) {
    this->GetColumn( 0 ).SetWidth( MainList::FIRST_COLUMN_DEFAULT_WIDTH );
  }
  else {
    this->GetColumn( 0 ).SetWidthAuto();
  }
}
