/****************************************************************************/
/**
 *  @file   PointImporter.h
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
#ifndef KVS__POINT_IMPORTER_H_INCLUDE
#define KVS__POINT_IMPORTER_H_INCLUDE

#include "ImporterBase.h"
#include <kvs/Module>
#include <kvs/PointObject>
#include <kvs/KVSMLObjectPoint>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Point object importer class.
 */
/*==========================================================================*/
class PointImporter : public kvs::ImporterBase, public kvs::PointObject
{
    kvsModule( kvs::PointImporter, Importer );
    kvsModuleBaseClass( kvs::ImporterBase );
    kvsModuleSuperClass( kvs::PointObject );

public:

    PointImporter();
    PointImporter( const std::string& filename );
    PointImporter( const kvs::FileFormatBase* file_format );
    virtual ~PointImporter();

    SuperClass* exec( const kvs::FileFormatBase* file_format );

private:

    void import( const kvs::KVSMLObjectPoint* kvsml );
    void set_min_max_coord();
};

} // end of namespace kvs

#endif // KVS__POINT_IMPORTER_H_INCLUDE
