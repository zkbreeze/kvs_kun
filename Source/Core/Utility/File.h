/****************************************************************************/
/**
 *  @file File.h
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
#ifndef KVS__FILE_H_INCLUDE
#define KVS__FILE_H_INCLUDE

#include <string>
#if KVS_ENABLE_DEPRECATED
#include <cstdio>
#include <cstdlib>
#include <climits>
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  File class
 */
/*==========================================================================*/
class File
{
#if KVS_ENABLE_DEPRECATED
protected:
#else
private:
#endif
    std::string m_file_path; ///< file path (ex. "/tmp/abc/xyz.tar.gz")
    std::string m_path_name; ///< path name (ex. "/tmp/abc")
    std::string m_file_name; ///< file name (ex. "xyz.tar.gz")
    std::string m_base_name; ///< base name (ex. "xyz")
    std::string m_extension; ///< complete extension (ex. "tar.gz")

public:
    File();
    File( const std::string& file_path );
    virtual ~File() {}

public:
    bool operator <( const File& file ) const;
    bool operator ==( const File& file ) const;

public:
    std::string filePath( bool absolute = false ) const;
    std::string pathName( bool absolute = false ) const;
    std::string fileName() const;
    std::string baseName() const;
    std::string extension( bool complete = false ) const;

    size_t byteSize() const;
    bool isFile() const;
    bool exists() const;
    bool parse( const std::string& file_path );

public:
    static std::string Separator();

#if 1
    bool isExisted() const
    {
        return this->exists();
    }
#endif
};

} // end of namespace kvs

#endif // KVS__FILE_H_INCLUDE
