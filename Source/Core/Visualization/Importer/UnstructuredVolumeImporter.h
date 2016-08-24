/****************************************************************************/
/**
 *  @file   UnstructuredVolumeImporter.h
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
#ifndef KVS__UNSTRUCTURED_VOLUME_IMPORTER_H_INCLUDE
#define KVS__UNSTRUCTURED_VOLUME_IMPORTER_H_INCLUDE

#include "ImporterBase.h"
#include <kvs/Module>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/KVSMLObjectUnstructuredVolume>
#include <kvs/AVSUcd>
#include <kvs/AVSField>


namespace kvs
{

/*==========================================================================*/
/**
 *  Unstructured volume object importer class.
 */
/*==========================================================================*/
class UnstructuredVolumeImporter : public kvs::ImporterBase, public kvs::UnstructuredVolumeObject
{
    kvsModule( kvs::UnstructuredVolumeImporter, Importer );
    kvsModuleBaseClass( kvs::ImporterBase );
    kvsModuleSuperClass( kvs::UnstructuredVolumeObject );

public:

    UnstructuredVolumeImporter();
    UnstructuredVolumeImporter( const std::string& filename );
    UnstructuredVolumeImporter( const kvs::FileFormatBase* file_format );
    virtual ~UnstructuredVolumeImporter();

    SuperClass* exec( const kvs::FileFormatBase* file_format );

private:

    void import( const kvs::KVSMLObjectUnstructuredVolume* const kvsml );
    void import( const kvs::AVSUcd* const ucd );
    void import( const kvs::AVSField* const field );
};

} // end of namespace kvs

#endif // KVS__UNSTRUCTURED_VOLUME_IMPORTER_H_INCLUDE
