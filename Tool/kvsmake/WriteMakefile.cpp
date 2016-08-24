/****************************************************************************/
/**
 *  @file   WriteMakefile.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/****************************************************************************/
#include "WriteMakefile.h"
#include <string>
#include <fstream>
#include <kvs/Message>
#include <kvs/Compiler>
#include <kvs/Directory>
#include <kvs/FileList>
#include <kvs/File>
#include <kvs/String>
#include "Constant.h"


namespace
{

/*===========================================================================*/
/**
 *  @brief  Writes a makefile.
 *  @param  in [in] input stream
 *  @param  out [in] output stream
 *  @param  project_name [in] project name
 */
/*===========================================================================*/
void Write( std::ifstream& in, std::ofstream& out, const std::string& project_name )
{
#if defined ( KVS_COMPILER_VC )
    // Search cpp files.
    std::string sources( "" );

    const kvs::Directory current_dir( "." );
    const kvs::FileList& file_list = current_dir.fileList();

    kvs::FileList::const_iterator iter = file_list.begin();
    const kvs::FileList::const_iterator end  = file_list.end();
    while ( iter != end )
    {
        if ( iter->extension() == "cpp" )
        {
            sources += ( iter->fileName() + " \\\n" );
        }
        ++iter;
    }
#endif

    // Write a Makefile.
    while ( !in.eof() )
    {
        std::string line( "" );
        std::getline( in, line );
        line = kvs::String::Replace( line, "PROJECT_NAME_REPLACED_BY_KVSMAKE", project_name );
#if defined ( KVS_COMPILER_VC )
        line = kvs::String::Replace( line, "SOURCES_REPLACED_BY_KVSMAKE", sources );
#endif
        out << line << std::endl;
    }
}

}

namespace kvsmake
{

/*===========================================================================*/
/**
 *  @brief  Writes a makefile.
 *  @param  project_name [in] project name
 */
/*===========================================================================*/
bool WriteMakefile( const std::string& project_name )
{
    //  Open a template file.
    std::ifstream in( kvsmake::MakefileTemplate.c_str() );
    if ( !in.is_open() )
    {
        kvsMessageError( "Cannot open %s.", kvsmake::MakefileTemplate.c_str() );
        return false;
    }

    //  Open a Makefile.
    std::ofstream out( kvsmake::Makefile.c_str() );
    if ( !out.is_open() )
    {
        kvsMessageError( "Cannot open %s.", kvsmake::Makefile.c_str() );
        return false;
    }

    ::Write( in, out, project_name );
    return true;
}

} // end of namespace kvsmake
