// main.cpp

#include "ttl_define.h"
#include "tt_command_line_analyzer.h"
#include "tt_message_box.h"

#include "common.h"
#include "mainstay/main_frame.h"

static void show_unexpected_error_message_box( void );


int WINAPI
WinMain( HINSTANCE h_instance,
         HINSTANCE h_prev_instance,
         PSTR lp_cmd_line,
         int n_cmd_show )
{
  NOT_USE( h_instance );
  NOT_USE( h_prev_instance );
  NOT_USE( n_cmd_show );

  try {
    BMX2WAV::Image::Initialize();
    BMX2WAV::Mainstay::MainFrame frame;
    frame.Create();
    frame.LoadPlacementFromIniFile();
    if ( frame.IsMinimized() ) {
      frame.Restore();
    }

    try {
      TtCommandLine::Analyzer analyzer;
      auto rest = analyzer.AnalyzeForWindows( lp_cmd_line );
      for ( auto& one : rest ) {
        frame.AddEntry( one );
      }
      frame.IfToggleButtonIsPressedDisplayCell();
    }
    catch ( TtCommandLine::Exception& e ) {
      TtMessageBoxOk box;
      ( box.AppendMessage() << "�R�}���h���C�������ŃG���[���������܂����B\r\n" <<
        "��� : " << e.Dump() << "\r\n\r\n" <<
        "�R�}���h���C�������͖�������܂��B" );
      box.SetCaption( "�R�}���h���C���G���[" );
      box.SetIcon( TtMessageBox::Icon::ERROR );
      box.ShowDialog();
    }

    frame.Show();
    return TtForm::LoopDispatchMessage();
  }
  catch ( TtException& e ) {
    BMX2WAV::IniFileOperation::SaveErrorLogDump( e );
    show_unexpected_error_message_box();
    return 2;
  }
  catch ( ... ) {
    show_unexpected_error_message_box();
    return 1;
  }
}

static void
show_unexpected_error_message_box( void )
{
  TtMessageBoxOk box;
  box.SetMessage( "�\�����Ȃ��G���[���������܂����B�A�v���P�[�V�������I�������܂��B" );
  box.SetCaption( "�\�����Ȃ��G���[" );
  box.SetIcon( TtMessageBox::Icon::ERROR );
  box.ShowDialog();
}
