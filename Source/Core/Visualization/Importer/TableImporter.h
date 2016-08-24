/*****************************************************************************/
/**
 *  @file   TableImporter.h
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
/*****************************************************************************/
#ifndef KVS__TABLE_IMPORTER_H_INCLUDE
#define KVS__TABLE_IMPORTER_H_INCLUDE

#include <kvs/ImporterBase>
#include <kvs/Module>
#include <kvs/TableObject>
#include <kvs/KVSMLObjectTable>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Importer class for TableObject.
 */
/*===========================================================================*/
class TableImporter : public kvs::ImporterBase, public kvs::TableObject
{
    kvsModule( kvs::TableImporter, Importer );
    kvsModuleBaseClass( kvs::ImporterBase );
    kvsModuleSuperClass( kvs::TableObject );

public:

    TableImporter();
    TableImporter( const std::string& filename );
    TableImporter( const kvs::FileFormatBase* file_format );

    SuperClass* exec( const kvs::FileFormatBase* file_format );

private:

    void import( const kvs::KVSMLObjectTable* kvsml );
};

} // end of namespace kvs

#endif // KVS__TABLE_IMPORTER_H_INCLUDE
