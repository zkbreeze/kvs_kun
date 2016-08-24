/*****************************************************************************/
/**
 *  @file   PolygonExporter.h
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
#ifndef KVS__POLYGON_EXPORTER_H_INCLUDE
#define KVS__POLYGON_EXPORTER_H_INCLUDE

#include <kvs/PolygonObject>
#include <kvs/KVSMLObjectPolygon>
#include <kvs/Stl>
#include <kvs/Ply>
#include "ExporterBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Polygon exporter class.
 */
/*===========================================================================*/
template <typename FileFormatType>
class PolygonExporter : public kvs::ExporterBase, public FileFormatType
{
    kvsModule( kvs::PolygonExporter<FileFormatType>, Exporter );
    kvsModuleBaseClass( kvs::ExporterBase );

public:
    FileFormatType* exec( const kvs::ObjectBase* ) { return NULL; }
};

/*===========================================================================*/
/**
 *  @brief  Polygon exporter class as KVSMLObjectPoint format.
 */
/*===========================================================================*/
template <>
class PolygonExporter<kvs::KVSMLObjectPolygon> :
        public kvs::ExporterBase,
        public kvs::KVSMLObjectPolygon
{
public:
    PolygonExporter( const kvs::PolygonObject* object );
    kvs::KVSMLObjectPolygon* exec( const kvs::ObjectBase* object );
};

/*===========================================================================*/
/**
 *  @brief  Polygon exporter class as STL format.
 */
/*===========================================================================*/
template <>
class PolygonExporter<kvs::Stl> :
        public kvs::ExporterBase,
        public kvs::Stl
{
public:
    PolygonExporter( const kvs::PolygonObject* object );
    kvs::Stl* exec( const kvs::ObjectBase* object );
};

/*===========================================================================*/
/**
 *  @brief  Polygon exporter class as PLY format.
 */
/*===========================================================================*/
template <>
class PolygonExporter<kvs::Ply> :
        public kvs::ExporterBase,
        public kvs::Ply
{
public:
    PolygonExporter( const kvs::PolygonObject* object );
    kvs::Ply* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs

#endif // KVS__POLYGON_EXPORTER_H_INCLUDE
