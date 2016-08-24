/*****************************************************************************/
/**
 *  @file   PointExporter.h
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
#ifndef KVS__POINT_EXPORTER_H_INCLUDE
#define KVS__POINT_EXPORTER_H_INCLUDE

#include <kvs/PointObject>
#include <kvs/KVSMLObjectPoint>
#include "ExporterBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Point exporter class.
 */
/*===========================================================================*/
template <typename FileFormatType>
class PointExporter : public kvs::ExporterBase, public FileFormatType
{
    kvsModule( kvs::PointExporter<FileFormatType>, Exporter );
    kvsModuleBaseClass( kvs::ExporterBase );

public:
    FileFormatType* exec( const kvs::ObjectBase* ) { return NULL; }
};

/*===========================================================================*/
/**
 *  @brief  Point exporter class as KVSMLObjectPoint format.
 */
/*===========================================================================*/
template <>
class PointExporter<kvs::KVSMLObjectPoint> :
        public kvs::ExporterBase,
        public kvs::KVSMLObjectPoint
{
public:
    PointExporter( const kvs::PointObject* object );
    kvs::KVSMLObjectPoint* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs

#endif // KVS__POINT_EXPORTER_H_INCLUDE
