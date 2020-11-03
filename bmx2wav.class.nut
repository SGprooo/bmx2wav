// bmx2wav.class.nut

// -- ParameterColumn
class ParameterColumn extends Column {
  constructor() {
    base.constructor( "�ϊ��ݒ�" );
  }

  function display_cell( entry ) {
    local parameter = entry.get_individual_parameter();

    if ( parameter ) {
      return parameter.name + "�i�ʁj"
    }
    else {
      return Main.get_common_parameter().name + "�i���ʁj"
    }
  }

  function compare( x, y ) {
    return ( x.get_individual_parameter() == null ) <=> ( y.get_individual_parameter() == null );
  }
}


// -- �o�̓e���v���[�g�֌W -----
Main.translate_template_path_by_parameter <- function ( parameter, bms_data ) {
  return Main.translate_template_path( parameter.output_file_template,
                                       parameter.input_file_path,
                                       parameter.output_as_ogg,
                                       bms_data );
}

Main.Converter.get_translated_template_path <- function () {
  return Main.translate_template_path_by_parameter( this.parameter, this.bms_data );
}

// -- callback -----
// -- tools
Main.callback_current_end_string <- null;
Main.puts_with_end_string <- function ( str ) {
  if ( Main.callback_current_end_string ) {
    str += ": " + Main.callback_current_end_string;
  }
  ::puts( str );
}


// abort
Main.callback_table.aborted = function ( converter, user_abort ) {
  if ( user_abort ) {
    ::puts( "���[�U�ɂ��ϊ��̒��f�v���B" );
  }
  ::puts( "�ϊ��͒��f����܂����B" );
}

// exception
Main.callback_table.exception_occurred = function ( converter, exception ) {
  local tmp = ErrorLevel.error_level_to_string( exception.get_error_level() );
  ::puts( tmp + " : " + exception.message );

  if ( exception.get_error_level() <= ErrorLevel.Tiny ) {
    Main.callback_current_end_string = "�G���[�L";
  }
}

Main.callback_table.parser_exception_occurred = function ( converter, exception ) {
  local tmp = converter.parameter.input_file_path;
  tmp = tmp + "(" + exception.line + "): ";
  tmp = tmp + ErrorLevel.error_level_to_string( exception.get_error_level() );
  ::puts( tmp + " : " + exception.message );

  if ( exception.get_error_level() <= ErrorLevel.Tiny ) {
    Main.callback_current_end_string = "�G���[�L";
  }
}

// progress
Main.callback_table.before_initialize = function ( converter ) {
  ::puts( "== �ϊ��J�n =====" );
  ::puts( "BMS File Path : " + converter.parameter.input_file_path );
  ::puts( "�������E�O����: �J�n" );
  Main.callback_current_end_string = null;
}

Main.callback_table.after_initialize = function ( converter ) {
  Main.puts_with_end_string( "�������E�O����: ����" );
}

Main.callback_table.before_parse = function ( converter ) {
  ::puts( "BMS �\�����: �J�n" );
  Main.callback_current_end_string = null;
}

Main.callback_table.decide_output_file_path = function ( converter ) {
  local tmp = converter.get_translated_template_path();
  if ( converter.parameter.remove_can_not_use_character_as_file_path ) {
    tmp = Path.remove_can_not_use_character_as_file_path_from( tmp );
  }
  if ( converter.parameter.never_overwrite_output_file ) {
    tmp = Path.get_not_exist_path_from( tmp );
  }
  return tmp;
}

Main.callback_table.after_parse = function ( converter ) {
  Main.puts_with_end_string( "BMS �\�����: ����" );
  ::puts( "�ő啪��\ : " + converter.bms_data.max_resolution + "; That Bar Number : " + converter.bms_data.bar_number_of_max_resolution );
}

Main.callback_table.before_read_audio_files = function ( converter ) {
  ::puts( "�����t�@�C���ǂݍ���: �J�n" );
  Main.callback_current_end_string = null;
}

Main.callback_table.decide_audio_file_path = function ( converter, filename ) {
  local base_path = Path.dirname( converter.parameter.input_file_path ) + "\\" + filename;
  local wav_path = Path.change_extension( base_path, "wav" );
  local ogg_path = Path.change_extension( base_path, "ogg" );

  if ( Path.file_exist( wav_path ) ) {
    return wav_path;
  }
  else if ( Path.file_exist( ogg_path ) ) {
    return ogg_path;
  }
  return base_path;
}

Main.callback_table.audio_file_read_start = function( converter, word, path ) {
  ::print( "WAV" + BMX2WAV.integer_to_word_string( word ) + " read : " + path + "  start ... " );
}

Main.callback_table.audio_file_read_end = function( converter, word, path ) {
  ::puts( "end" );
}

Main.callback_table.after_read_audio_files = function ( converter ) {
  Main.puts_with_end_string( "�����t�@�C���ǂݍ���: ����" );
}

Main.callback_table.before_mixin_waves = function ( converter ) {
  ::puts( "��������: �J�n" );
  Main.callback_current_end_string = null;
}

Main.callback_table.after_mixin_waves = function ( converter ) {
  Main.puts_with_end_string( "��������: ����" );
}

Main.callback_table.before_affect_wave = function ( converter ) {
  ::puts( "���ʒ���: �J�n" );
  Main.callback_current_end_string = null;
}

Main.callback_table.after_affect_wave = function ( converter ) {
  Main.puts_with_end_string( "���ʒ���: ����" );
}

Main.callback_table.complete_normalize = function ( converter, ratio ) {
  local tmp = NormalizeKind.normalize_kind_to_string( converter.parameter.normalize_kind );
  if ( converter.parameter.normalize_kind == NormalizeKind.Over || converter.parameter.normalize_kind == NormalizeKind.Default ) {
    tmp += " : OverPPM : " + converter.parameter.normalize_over_ppm;
  }
  ::puts( "�m�[�}���C�Y��� : " + tmp );
  ::puts( "�m�[�}���C�Y�̉��ʒ����� : " + ratio );
}

Main.callback_table.before_output_to_file = function ( converter ) {
  ::puts( "�t�@�C���o��: �J�n" );
  Main.callback_current_end_string = null;
}

Main.callback_table.after_output_to_file = function ( converter ) {
  if ( converter.output_file_path == null || converter.output_file_path == "" ) {
    ::puts( "�o�͂��w�肳��Ă��Ȃ��̂Ńt�@�C���ɂ͏o�͂���܂���ł����B" );
    Main.puts_with_end_string( "�t�@�C���o��: ����: �t�@�C�����o��" );
  }
  else {
    ::puts( "�o�̓t�@�C�� : " + converter.output_file_path );
    Main.puts_with_end_string( "�t�@�C���o��: ����" );
  }

  ::puts( ::format( "�ϊ����� : %.2f second.", converter.get_processing_time() ) );
}

Main.callback_table.after_process = function ( converter ) {
  if ( converter.parameter.execute_after_process ) {
    BMX2WAV.create_process(
      converter.parameter.after_process_execute_file,
      BMX2WAV.replace_string( converter.parameter.after_process_execute_arguments, "@@output_file_path@@", converter.output_file_path ),
      Path.dirname( converter.parameter.after_process_execute_file ),
      converter.parameter.after_process_start_on_background ? BMX2WAV.ShowState.SHOW_MINIMIZED_NO_ACTIVE : BMX2WAV.ShowState.SHOW,
      converter.parameter.after_process_wait_for_process_exit );
  }
  if ( converter.parameter.after_process_delete_output_file ) {
    // remove() is squirrel system function
    ::remove( converter.output_file_path );
  }
}

Main.callback_table.before_finalize = function ( converter ) {}
Main.callback_table.after_finalize = function ( converter ) {}
