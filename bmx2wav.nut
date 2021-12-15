
// ----- ColumnGroup �֘A ------------------------------------------------
// null �̏ꍇ�̓t���p�X��\������
Main.first_column = null;
// �ȉ��Ɠ���
// Main.first_column = FullPathColumn();

// �J�����O���[�v�ݒ�
{
  local group = ColumnGroup( StrT.Main.ColumnGroupName.Simple.get() );
  group.columns.append( ParameterColumn() );
  group.columns.append( HeaderColumn( "TITLE", HeaderColumn.compare_as_string ) );
  group.columns.append( HeaderColumn( "ARTIST", HeaderColumn.compare_as_string ) );
  group.columns.append( PlayerColumn() );
  group.columns.append( HeaderColumn( "PLAYLEVEL", HeaderColumn.compare_as_integer_desc ) );
  Main.column_groups.append( group );
}

{
  local group = ColumnGroup( StrT.Main.ColumnGroupName.Details.get() );
  group.columns.append( ParameterColumn() );
  group.columns.append( HeaderColumn( "TITLE", HeaderColumn.compare_as_string ) );
  group.columns.append( HeaderColumn( "ARTIST", HeaderColumn.compare_as_string ) );
  group.columns.append( HeaderColumn( "GENRE", HeaderColumn.compare_as_string ) );
  group.columns.append( PlayerColumn() );
  group.columns.append( HeaderColumn( "PLAYLEVEL", HeaderColumn.compare_as_integer_desc ) );
  group.columns.append( HeaderColumn( "BPM", HeaderColumn.compare_as_integer_asc ) );
  group.columns.append( MostSeriousErrorColumn() );
  group.columns.append( PlayingTimeColumn() );
  Main.column_groups.append( group );
}

{
  local group = ColumnGroup( StrT.Main.ColumnGroupName.Various.get() );
  group.columns.append( ParameterColumn() );
  group.columns.append( PlayerColumn() );
  group.columns.append( HeaderColumn( "TITLE", HeaderColumn.compare_as_string ) );
  group.columns.append( HeaderColumn( "ARTIST", HeaderColumn.compare_as_string ) );
  group.columns.append( ObjectCountColumn() );
  group.columns.append( ObjectOf1PCountColumn() );
  group.columns.append( ObjectOf2PCountColumn() );
  group.columns.append( ContainsRandomStatementColumn() );
  group.columns.append( ParentDirectoryColumn() );
  group.columns.append( ParentsParentDirectoryColumn() );
  group.columns.append( FullPathColumn() );
  Main.column_groups.append( group );
}

// -- �G���g���[�ڍ׃_�C�A���O���X�g�\�L�ݒ� -----------------------------
Main.entry_dialog_list_items.append( HeaderColumn( "TITLE" ) );
Main.entry_dialog_list_items.append( HeaderColumn( "ARTIST" ) );
Main.entry_dialog_list_items.append( HeaderColumn( "GENRE" ) );
Main.entry_dialog_list_items.append( PlayerColumn() );
Main.entry_dialog_list_items.append( HeaderColumn( "PLAYLEVEL" ) );
Main.entry_dialog_list_items.append( HeaderColumn( "BPM" ) );
Main.entry_dialog_list_items.append( MostSeriousErrorColumn() );
Main.entry_dialog_list_items.append( PlayingTimeColumn() );
Main.entry_dialog_list_items.append( ContainsRandomStatementColumn() );


// -- EntryProcessors �֘A -----------------------------------------------
{
  local preview = {
    name = StrT.Main.PopupEntry.ProcessorMenuPlayPreview.get(),
    function execute( entry ) {
      entry.parse_as_bms_data_once();
      local key = "PREVIEW";
      if ( key in entry.bms_data.headers ) {
        local path = Path.dirname( entry.path ) + "\\" + entry.bms_data.headers[key];
        BMX2WAV.play_wav_or_ogg_file_async( path, 50 );
      }
    }
  };

  local shell_execute = {
    name = StrT.Main.PopupEntry.ProcessorMenuOpen.get(),
    function execute( entry ) {
      BMX2WAV.shell_execute( "open", entry.path, null, Path.dirname( entry.path ) );
    }
  };

  local notepad = {
    name = StrT.Main.PopupEntry.ProcessorMenuOpenByNotepad.get(),
    function execute( entry ) {
      BMX2WAV.create_process( "notepad.exe", entry.path, null, BMX2WAV.ShowState.SHOW, false );
    }
  };

  Main.entry_processors.items.append( preview );
  local sub = SubMenu( StrT.Main.PopupEntry.ProcessorMenuFileOperations.get(), [shell_execute, notepad] );
  Main.entry_processors.items.append( sub );
}
