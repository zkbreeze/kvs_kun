/****************************************************************************/
/**
 *  @file Entry.h
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
#ifndef KVS__TIFF__ENTRY_H_INCLUDE
#define KVS__TIFF__ENTRY_H_INCLUDE

#include <kvs/Type>
#include <kvs/AnyValueArray>
#include <kvs/Indent>
#include <iostream>
#include <fstream>
#include <string>


namespace kvs
{

namespace tiff
{

class Entry
{
protected:

    kvs::UInt16 m_tag; ///< tag
    kvs::UInt16 m_type; ///< value type
    kvs::UInt32 m_count; ///< value count
    kvs::AnyValueArray m_values; ///< value array

public:

    Entry( const kvs::UInt16 tag );
    Entry( std::ifstream& ifs );

    friend const bool operator == ( const Entry& lhs, const Entry& rhs );
    friend std::ostream& operator << ( std::ostream& os, const Entry& entry );

    kvs::UInt16 tag() const;
    kvs::UInt16 type() const;
    kvs::UInt32 count() const;
    std::string tagDescription() const;
    std::string typeName() const;
    const kvs::AnyValueArray& values() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( std::ifstream& ifs );

private:

    void* allocate_values( const size_t nvalues, const size_t value_type );
};

} // end of namespace tiff

} // end of namespace kvs

#endif // KVS__TIFF__ENTRY_H_INCLUDE
