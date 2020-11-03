// exception.cpp

#include "tt_string.h"
#include "tt_enum.h"

#include "exception.h"

using namespace BMX2WAV;


// -- Exception --------------------------------------------------------
std::string
Exception::MakeMessage( const char* format, ... )
{
  va_list args;
  va_start( args, format );
  char tmp[1024];
  ::vsprintf_s( tmp, sizeof( tmp ), format, args );
  va_end( args );
  return tmp;
}

Exception::Exception( void ) :
TtException()
{
}

std::string
Exception::GetMessage( void )
{
  return Exception::MakeMessage( "Error" );
}


// -- InternalException ------------------------------------------------
InternalException::InternalException( const char* file, unsigned int line ) :
file_( file ),
line_( line )
{
}


const char*
InternalException::GetFile( void ) const
{
  return file_;
}

unsigned int
InternalException::GetLine( void ) const
{
  return line_;
}


std::string
InternalException::GetMessage( void )
{
  return Exception::MakeMessage( "�����G���[���������܂����B\r\n�t�@�C�� : %s\r\n�s�ԍ� : %d", file_, line_ );
}


std::string
InternalException::Dump( void ) const
{
  std::ostringstream os;
  os << typeid( *this ).name() << " : " << file_ << "(" << line_ << ")";
  return os.str();
}


// -- ConvertException ---------------------------------------------------
ConvertException::ConvertException( ErrorLevel error_level ) :
error_level_( error_level )
{
}

TtEnum<ErrorLevel>
ConvertException::GetErrorLevel( void ) const
{
  return error_level_;
}

void
ConvertException::ResetErrorLevel( TtEnum<ErrorLevel> error_level )
{
  error_level_ = error_level;
}


// -- MessageOnlyException -------------------------------------------------
MessageOnlyException::MessageOnlyException( const std::string& message ) :
message_( message )
{
}

std::string
MessageOnlyException::GetMessage( void )
{
  return message_;
}


// -- AudioFileError -----------------------------------------------------
AudioFileError::AudioFileError( ErrorLevel error_level, const std::string& path ) :
ConvertException( error_level ),
WithFilePathException( path )
{
}


// -- EntriedAudioFileNotFoundException ----------------------------------
EntriedAudioFileNotFoundException::EntriedAudioFileNotFoundException( BL::Word object_number, const std::string& path ) :
AudioFileErrorWith( path ),
WithObjectNumberException( object_number )
{
}

std::string
EntriedAudioFileNotFoundException::GetMessage( void )
{
  return Exception::MakeMessage( "�w�肳�ꂽ�����t�@�C�������݂��܂���B�w�b�_ : WAV%s ; �t�@�C�� : %s", this->GetObjectNumber().ToCharPointer(), this->GetFilePath().c_str() );
}


// -- AudioFileOpenException ---------------------------------------------
AudioFileOpenException::AudioFileOpenException( const std::string& path, int error_number ) :
AudioFileErrorWith( path ),
WithErrorNumberException( error_number )
{
}


std::string
AudioFileOpenException::GetMessage( void )
{
  return Exception::MakeMessage( "�����t�@�C�����J���̂Ɏ��s���܂����B�t�@�C�� : %s ; ���b�Z�[�W : %s", this->GetFilePath().c_str(), this->GetSystemErrorMessage().c_str() );
}


// -- WavFileReadException -----------------------------------------------
WavFileReadException::WavFileReadException( const std::string& path, int error_number ) :
AudioFileErrorWith( path ),
WithErrorNumberException( error_number )
{
}

std::string
WavFileReadException::GetMessage( void )
{
  return Exception::MakeMessage( "WAV �t�@�C���̓ǂݍ��݂Ɏ��s���܂����B�t�@�C�� : %s ; ���b�Z�[�W : %s", this->GetFilePath().c_str(), this->GetSystemErrorMessage().c_str() );
}


// -- WavFileWriteException ----------------------------------------------
WavFileWriteException::WavFileWriteException( const std::string& path, int error_number ) :
AudioFileErrorWith( path ),
WithErrorNumberException( error_number )
{
}

std::string
WavFileWriteException::GetMessage( void )
{
  return Exception::MakeMessage( "WAV �t�@�C���̏������݂Ɏ��s���܂����B�t�@�C�� : %s ; ���b�Z�[�W : %s", this->GetFilePath().c_str(), this->GetSystemErrorMessage().c_str() );
}



// -- OggFileOpenException ---------------------------------------------
OggFileOpenException::OggFileOpenException( const std::string& path, int error_number ) :
AudioFileErrorWith( path ),
WithErrorNumberException( error_number )
{
}


std::string
OggFileOpenException::GetMessage( void )
{
  return Exception::MakeMessage( "ogg �t�@�C�����J���̂Ɏ��s���܂����B�t�@�C�� : %s ; �G���[�ԍ� : %d", this->GetFilePath().c_str(), error_number_ );
}


// -- OggFileReadException -----------------------------------------------
OggFileReadException::OggFileReadException( const std::string& path ) :
AudioFileErrorWith( path )
{
}

std::string
OggFileReadException::GetMessage( void )
{
  return Exception::MakeMessage( "ogg �t�@�C���̓ǂݍ��݂Ɏ��s���܂����B�t�@�C�� : %s", this->GetFilePath().c_str() );
}


// -- InvalidWavFileFormatException --------------------------------------
InvalidWavFileFormatException::InvalidWavFileFormatException( const std::string& path, const std::string& reason ) :
AudioFileErrorWith( path ),
reason_( reason )
{
}

std::string
InvalidWavFileFormatException::GetMessage( void )
{
  return Exception::MakeMessage( "WAV �t�@�C���̌`�����s���ł��B�t�@�C�� : %s ; ���R : %s", this->GetFilePath().c_str(), reason_.c_str() );
}

const std::string&
InvalidWavFileFormatException::GetReason( void ) const
{
  return reason_;
}

// -- UnsupportedWavFileFormatException ----------------------------------
UnsupportedWavFileFormatException::UnsupportedWavFileFormatException( const std::string& path, const std::string& reason ) :
AudioFileErrorWith( path ),
reason_( reason )
{
}

std::string
UnsupportedWavFileFormatException::GetMessage( void )
{
  return Exception::MakeMessage( "�T�|�[�g���Ă��Ȃ� WAV �t�@�C���̌`���ł��B�t�@�C�� : %s ; ���R : %s", this->GetFilePath().c_str(), reason_.c_str() );
}

const std::string&
UnsupportedWavFileFormatException::GetReason( void ) const
{
  return reason_;
}


// -- BadAllocationException ---------------------------------------------
BadAllocationException::BadAllocationException( std::bad_alloc source ) :
source_( source )
{
}

std::string
BadAllocationException::GetMessage( void )
{
  return Exception::MakeMessage( "�������m�ۂɎ��s���܂����B ���b�Z�[�W : %s", source_.what() );
}


// -- BmsFileAccessException ---------------------------------------------
BmsFileAccessException::BmsFileAccessException( const std::string& path, int error_number ) :
WithFilePathException( path ),
error_number_( error_number )
{
}

int
BmsFileAccessException::GetErrorNumber( void ) const
{
  return error_number_;
}

std::string
BmsFileAccessException::GetMessage( void )
{
  auto tmp = TtUtility::GetANSIErrorMessage( error_number_ );
  return Exception::MakeMessage( "BMS �t�@�C���̃A�N�Z�X�ŃG���[������܂����B�t�@�C�� : %s ; ���b�Z�[�W : %s", this->GetFilePath().c_str(), tmp.c_str() );
}


// -- OutputFileIsInputFilePathException ---------------------------------
OutputFileIsInputFilePathException::OutputFileIsInputFilePathException( BL::Word number, const std::string& path ) :
WithObjectNumberException( number ),
WithFilePathException( path )
{
}

std::string
OutputFileIsInputFilePathException::GetMessage( void )
{
  return Exception::MakeMessage( "�o�̓t�@�C���̃p�X�� BMS �t�@�C���� WAV �Ŏw�肳��Ă�����̓t�@�C���ɂȂ��Ă��܂��B�w�b�_�� : WAV%s ; �t�@�C���p�X : %s", this->GetObjectNumber().ToCharPointer(), this->GetFilePath().c_str() );
}


// -- InvalidFormatAsBpmHeaderException ----------------------------------
InvalidFormatAsBpmHeaderException::InvalidFormatAsBpmHeaderException( void )
{
}

std::string
InvalidFormatAsBpmHeaderException::GetMessage( void )
{
  return Exception::MakeMessage( "�w�肳�ꂽ BPM �̕\�L���s���ł��B" );
}


// -- InvalidFormatAsExtendedBpmChangeValueException ---------------------
InvalidFormatAsExtendedBpmChangeValueException::InvalidFormatAsExtendedBpmChangeValueException( BL::Word object_number ) :
WithObjectNumberException( object_number )
{
}

std::string
InvalidFormatAsExtendedBpmChangeValueException::GetMessage( void )
{
  return Exception::MakeMessage( "�w�肳�ꂽ�g�� BPM �̕\�L���s���ł��B�w�b�_�� BPM%s", this->GetObjectNumber().ToCharPointer() );
}


// -- InvalidFormatAsStopSequenceException -----------------------------
InvalidFormatAsStopSequenceException::InvalidFormatAsStopSequenceException( BL::Word object_number ) :
WithObjectNumberException( object_number )
{
}

std::string
InvalidFormatAsStopSequenceException::GetMessage( void )
{
  return Exception::MakeMessage( "�w�肳�ꂽ�X�g�b�v�V�[�P���X�̕\�L���s���ł��B�w�b�_�� STOP%s", this->GetObjectNumber().ToCharPointer() );
}

// -- InvalidFormatAsLongNoteObjectHeaderException -----------------------
InvalidFormatAsLongNoteObjectHeaderException::InvalidFormatAsLongNoteObjectHeaderException( void )
{
}

std::string
InvalidFormatAsLongNoteObjectHeaderException::GetMessage( void )
{
  return Exception::MakeMessage( "LNOBJ �Ŏw�肳�ꂽ�\�L���s���ł��B" );
}


// -- LongNoteObjectInvalidEncloseException ------------------------------
LongNoteObjectInvalidEncloseException::LongNoteObjectInvalidEncloseException( BL::ObjectWithLocation start, BL::ObjectWithLocation end ) :
start_( start ),
end_( end )
{
}

std::string
LongNoteObjectInvalidEncloseException::GetMessage( void )
{
  auto tmp1 = Exception::MakeMessage( "( ����:%d - ch:%s - object:%s )", start_.bar_number_, start_.channel_number_.ToCharPointer(), start_.object_number_.ToCharPointer() );
  auto tmp2 = Exception::MakeMessage( "( ����:%d - ch:%s - object:%s )", end_.bar_number_,   end_.channel_number_.ToCharPointer(),   end_.object_number_.ToCharPointer() );
  return Exception::MakeMessage( "�����O�m�[�g�̏I�[�̃I�u�W�F�N�g���n�[�ƈقȂ��Ă��܂��B �n�[ : %s ; �I�[ : %s", tmp1.c_str(), tmp2.c_str() );
}

BL::ObjectWithLocation
LongNoteObjectInvalidEncloseException::GetStartObject( void )
{
  return start_;
}

BL::ObjectWithLocation
LongNoteObjectInvalidEncloseException::GetEndObject( void )
{
  return end_;
}


// -- LongNoteObjectNotEnclosedException ---------------------------------
LongNoteObjectNotEnclosedException::LongNoteObjectNotEnclosedException( BL::ObjectWithLocation object ) :
WithObjectWithLocationException( object )
{
}

std::string
LongNoteObjectNotEnclosedException::GetMessage( void )
{
  auto tmp = this->GetObjectWithLocation();
  return Exception::MakeMessage( "�����O�m�[�g�̏I�[������܂���B�n�[���ߔԍ� : %d ; �`�����l�� : %s ; �I�u�W�F�N�g : %s", tmp.bar_number_, tmp.channel_number_.ToCharPointer(), tmp.object_number_.ToCharPointer() );
}


// -- NotEntriedWavWasUsedException --------------------------------------
NotEntriedWavWasUsedException::NotEntriedWavWasUsedException( BL::ObjectWithLocation object ) :
WithObjectWithLocationException( object )
{
}

std::string
NotEntriedWavWasUsedException::GetMessage( void )
{
  auto tmp = this->GetObjectWithLocation();
  return Exception::MakeMessage( "WAV �Ƃ��ēo�^����Ă��Ȃ��I�u�W�F�N�g���g�p����܂����B ���ߔԍ� : %d ; �`�����l�� : %s ; �I�u�W�F�N�g : %s", tmp.bar_number_, tmp.channel_number_.ToCharPointer(), tmp.object_number_.ToCharPointer() );
}

// -- InvalidFormatAsBpmChangeValueException -----------------------------
InvalidFormatAsBpmChangeValueException::InvalidFormatAsBpmChangeValueException( unsigned int bar_number, BL::Word object_number ) :
WithBarNumberException( bar_number ),
WithObjectNumberException( object_number )
{
}

std::string
InvalidFormatAsBpmChangeValueException::GetMessage( void )
{
  return Exception::MakeMessage( "�ʏ�� BPM �ύX�Ɏg�p���ꂽ�I�u�W�F�N�g���s���ł��B ���ߔԍ� : %d ; �I�u�W�F�N�g : %s", this->GetBarNumber(), this->GetObjectNumber().ToCharPointer() );
}

// -- ExtendedBpmChangeEntryNotExistException ----------------------------
ExtendedBpmChangeEntryNotExistException::ExtendedBpmChangeEntryNotExistException( unsigned int bar_number, BL::Word object_number ) :
WithBarNumberException( bar_number ),
WithObjectNumberException( object_number )
{
}

std::string
ExtendedBpmChangeEntryNotExistException::GetMessage( void )
{
  return Exception::MakeMessage( "�g�� BPM �ύX�Ńw�b�_�ɖ��ݒ�̃I�u�W�F�N�g���g�p����܂����B ���ߔԍ� : %d ; �I�u�W�F�N�g : %s", this->GetBarNumber(), this->GetObjectNumber().ToCharPointer() );
}

// -- StopSequenceEntryNotExistException ---------------------------------
StopSequenceEntryNotExistException::StopSequenceEntryNotExistException( unsigned int bar_number, BL::Word object_number ) :
WithBarNumberException( bar_number ),
WithObjectNumberException( object_number )
{
}

std::string
StopSequenceEntryNotExistException::GetMessage( void )
{
  return Exception::MakeMessage( "�X�g�b�v�V�[�P���X�Ńw�b�_�ɖ��ݒ�̃I�u�W�F�N�g���g�p����܂����B ���ߔԍ� : %d ; �I�u�W�F�N�g : %s", this->GetBarNumber(), this->GetObjectNumber().ToCharPointer() );
}

// -- OutputFileAccessException ----------------------------------------
OutputFileAccessException::OutputFileAccessException( AudioFileOpenException& parent ) :
WithFilePathException( parent.GetFilePath() ),
WithErrorNumberException( parent.GetErrorNumber() )
{
}

std::string
OutputFileAccessException::GetMessage( void )
{
  return Exception::MakeMessage( "�o�͗p�̉����t�@�C�����J���̂Ɏ��s���܂����B�t�@�C�� : %s ; ���b�Z�[�W : %s", this->GetFilePath().c_str(), this->GetSystemErrorMessage().c_str() );
}
