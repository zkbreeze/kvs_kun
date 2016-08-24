/****************************************************************************/
/**
 *  @file   WriteVCProjectCUDA.cpp
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
#include "WriteVCProjectCUDA.h"
#include <string>
#include <fstream>
#include <kvs/Message>
#include <kvs/Directory>
#include <kvs/FileList>
#include <kvs/File>
#include <kvs/String>
#include "Constant.h"


namespace
{

/*===========================================================================*/
/**
 *  @brief  Writes a VC project file.
 *  @param  in [in] input stream
 *  @param  out [in] output stream
 *  @param  project_name [in] project name
 */
/*===========================================================================*/
void Write( std::ifstream& in, std::ofstream& out, const std::string& project_name )
{
    //  Search the project's condition.
    std::string vc_version( "" );
    std::string cuda_headers( "" );
    std::string cuda_sources( "" );
    std::string headers( "" );
    std::string sources( "" );

    vc_version = KVS_COMPILER_VERSION;

    // Search cpp files and h files.
    const kvs::Directory current_dir( "." );
    const kvs::FileList& file_list = current_dir.fileList();

    kvs::FileList::const_iterator       iter = file_list.begin();
    const kvs::FileList::const_iterator end  = file_list.end();

    while ( iter != end )
    {
        const std::string filename( iter->fileName() );

        if ( iter->extension() == "h" )
        {
            headers += ( "      <File RelativePath=\".\\" + filename + "\"/>\n" );
        }
        else if ( iter->extension() == "cpp" )
        {
            sources += ( "      <File RelativePath=\".\\" + filename + "\"/>\n" );
        }
        else if ( iter->extension() == "cuh" )
        {
            cuda_headers += ( "      <File RelativePath=\".\\" + filename + "\" FileType=\"2\"/>\n" );
        }
        else if ( iter->extension() == "cu" )
        {
            const std::string objectname( iter->baseName() + ".obj" );

            cuda_sources += ( "      <File RelativePath=\".\\" + filename + "\" FileType=\"0\">\n" );

            cuda_sources += ( "        <FileConfiguration Name=\"Debug|Win32\">\n" );
            cuda_sources += ( "          <Tool\n" );
            cuda_sources += ( "            Name=\"VCCustomBuildTool\"\n" );
            cuda_sources += ( "            CommandLine=\"$(KVS_CUDA_DIR)\\bin\\nvcc.exe -ccbin &quot;$(VCInstallDir)bin&quot; -c -D_DEBUG -DWIN32 -D_CONSOLE -D_MBCS -Xcompiler /EHsc,/W3,/nologo,/Wp64,/Od,/Zi,/RTC1,/MTd -I$(KVS_CUDA_DIR)\\include -I$(KVS_CUDA_SDK_DIR)\\common\\inc -o $(ConfigurationName)\\" + objectname + " " + filename + "\"\n" );
            cuda_sources += ( "            Outputs=\"$(ConfigurationName)\\" + objectname + "\"\n" );
            cuda_sources += ( "          />\n" );
            cuda_sources += ( "        </FileConfiguration>\n" );

            cuda_sources += ( "        <FileConfiguration Name=\"Release|Win32\">\n" );
            cuda_sources += ( "          <Tool\n" );
            cuda_sources += ( "            Name=\"VCCustomBuildTool\"\n" );
            cuda_sources += ( "            CommandLine=\"$(KVS_CUDA_DIR)\\bin\\nvcc.exe -ccbin &quot;$(VCInstallDir)bin&quot; -c -DWIN32 -D_CONSOLE -D_MBCS -Xcompiler /EHsc,/W3,/nologo,/Wp64,/O2,/Zi,/MT -I$(KVS_CUDA_DIR)\\include -I$(KVS_CUDA_SDK_DIR)\\common\\inc -o $(ConfigurationName)\\" + objectname + " " + filename + "\"\n" );
            cuda_sources += ( "            Outputs=\"$(ConfigurationName)\\" + objectname + "\"\n" );
            cuda_sources += ( "          />\n" );
            cuda_sources += ( "        </FileConfiguration>\n" );

            cuda_sources += ( "        <FileConfiguration Name=\"EmuDebug|Win32\">\n" );
            cuda_sources += ( "          <Tool\n" );
            cuda_sources += ( "            Name=\"VCCustomBuildTool\"\n" );
            cuda_sources += ( "            CommandLine=\"$(KVS_CUDA_DIR)\\bin\\nvcc.exe -ccbin &quot;$(VCInstallDir)bin&quot; -deviceemu -c -D_DEBUG -DWIN32 -D_CONSOLE -D_MBCS -Xcompiler /EHsc,/W3,/nologo,/Wp64,/Od,/Zi,/RTC1,/MTd -I$(KVS_CUDA_DIR)\\include -I$(KVS_CUDA_SDK_DIR)\\common\\inc -o $(ConfigurationName)\\" + objectname + " " + filename + "\"\n" );
            cuda_sources += ( "            Outputs=\"$(ConfigurationName)\\" + objectname + "\"\n" );
            cuda_sources += ( "          />\n" );
            cuda_sources += ( "        </FileConfiguration>\n" );

            cuda_sources += ( "        <FileConfiguration Name=\"EmuRelease|Win32\">\n" );
            cuda_sources += ( "          <Tool\n" );
            cuda_sources += ( "            Name=\"VCCustomBuildTool\"\n" );
            cuda_sources += ( "            CommandLine=\"$(KVS_CUDA_DIR)\\bin\\nvcc.exe -ccbin &quot;$(VCInstallDir)bin&quot; -deviceemu -c -DWIN32 -D_CONSOLE -D_MBCS -Xcompiler /EHsc,/W3,/nologo,/Wp64,/O2,/Zi,/MT -I$(KVS_CUDA_DIR)\\include -I$(KVS_CUDA_SDK_DIR)\\common\\inc -o $(ConfigurationName)\\" + objectname + " " + filename + "\"\n" );
            cuda_sources += ( "            Outputs=\"$(ConfigurationName)\\" + objectname + "\"\n" );
            cuda_sources += ( "          />\n" );
            cuda_sources += ( "        </FileConfiguration>\n" );

            cuda_sources += ( "      </File>\n" );
        }

        ++iter;
    }

    // Write a project file.
    while ( !in.eof() )
    {
        std::string line( "" );
        std::getline( in, line );
        line = kvs::String::Replace( line, "VC_VERSION_REPLACED_BY_KVSMAKE", vc_version );
        line = kvs::String::Replace( line, "PROJECT_NAME_REPLACED_BY_KVSMAKE", project_name );
        line = kvs::String::Replace( line, "CUDA_HEADERS_REPLACED_BY_KVSMAKE", cuda_headers );
        line = kvs::String::Replace( line, "CUDA_SOURCES_REPLACED_BY_KVSMAKE", cuda_sources );
        line = kvs::String::Replace( line, "HEADERS_REPLACED_BY_KVSMAKE", headers );
        line = kvs::String::Replace( line, "SOURCES_REPLACED_BY_KVSMAKE", sources );
        out << line << std::endl;
    }
}

}


namespace kvsmake
{

/*===========================================================================*/
/**
 *  @brief  Writes a VC project file.
 *  @param  project_name [in] project name
 */
/*===========================================================================*/
bool WriteVCProjectCUDA( const std::string& project_name )
{
    //  Open a template file.
    std::ifstream in( kvsmake::VCProjectCUDATemplate.c_str() );
    if ( !in.is_open() )
    {
        kvsMessageError( "Cannot open %s.", kvsmake::VCProjectCUDATemplate.c_str() );
        return false;
    }

    //  Open a project file.
    const std::string filename( project_name + ".vcproj" );

    std::ofstream out( filename.c_str() );
    if ( !out.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        return false;
    }

    ::Write( in, out, project_name );
    return true;
}

} // end of namespace kvsmake
