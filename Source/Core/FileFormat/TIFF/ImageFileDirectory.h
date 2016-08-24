/****************************************************************************/
/**
 *  @file ImageFileDirectory.h
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
#ifndef KVS__TIFF__IMAGE_FILE_DIRECTORY_H_INCLUDE
#define KVS__TIFF__IMAGE_FILE_DIRECTORY_H_INCLUDE

#include "Entry.h"
#include <kvs/Type>
#include <kvs/Indent>
#include <iostream>
#include <fstream>
#include <list>


namespace kvs
{

namespace tiff
{

class ImageFileDirectory
{
public:

    typedef std::list<kvs::tiff::Entry> EntryList;

private:

    EntryList m_entry_list; ///< entry list
    kvs::UInt32 m_offset; ///< bytes offset

public:

    ImageFileDirectory();
    ImageFileDirectory( std::ifstream& ifs );

    const std::list<tiff::Entry>& entryList() const;
    kvs::UInt32 offset() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( std::ifstream& ifs );
};

} // end of namespace tiff

} // end of namespace kvs

#endif // KVS__TIFF__IMAGE_FILE_DIRECTORY_H_INCLUDE
