// main_frame.h

#pragma once

#include "tt_form.h"


namespace BMX2WAV {
  // -- MainFrame --------------------------------------------------------
  class MainFrame : public TtForm {
  public:
    explicit MainFrame( void );

    virtual DWORD GetStyle( void ) override;
    virtual DWORD GetExtendedStyle( void ) override;
    virtual bool  Created( void ) override;

  private:
    // -- control member -------------------------------------------------
    // -- �S�� -----
    /*
    MainMenu    main_menu_;
    MainToolBar main_tool_bar_;
    MainList    main_list_;
    TtStatusBar status_bar_;

    // ���X�g�r���[���엍��
    MainList::Item::DragHandler drag_handler_;
     */
  };
}
