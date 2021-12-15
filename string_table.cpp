// string_table.cpp

#include "tt_dynamic_link_library.h"
#include "tt_path.h"

#include "utility.h"
#include "exception.h"

#include "string_table.h"

using namespace BMX2WAV;


// -- StringTable --------------------------------------------------------
StringTable::ID::ID( unsigned int number ) :
number_( number )
{
}


std::string
StringTable::ID::Get( void ) const
{
  return StringTable::Get( *this );
}

namespace {
  std::optional<TtDynamicLinkLibrary> lib;
}

void
StringTable::Initialize( const std::string& language )
{
  std::string path = TtPath::GetExecutingDirectoryPath() + "\\lang\\" + language + ".dll";
  try {
    lib = TtDynamicLinkLibrary::LoadLibrary( path );
  }
  catch ( TtWindowsSystemCallException& ) {
    throw LanguageException( language );
  }
}

std::string
StringTable::Get( ID id )
{
  try {
    return lib->GetString( id.number_ );
  }
  catch ( ... ) {
    // �����͐����b�Z�[�W
    if ( Utility::UserDefaultLocaleIsJapanese() ) {
      return "���b�Z�[�W�̎擾�Ɏ��s���܂����BID = " + TtUtility::ToStringFrom(  id.number_ );
    }
    else {
      return "Failed to get the message. ID = " + TtUtility::ToStringFrom(  id.number_ );
    }
  }
}
