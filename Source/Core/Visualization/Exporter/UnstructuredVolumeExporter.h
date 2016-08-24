/*****************************************************************************/
/**
 *  @file   UnstructuredVolumeExporter.h
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
#ifndef KVS__UNSTRUCTURED_VOLUME_EXPORTER_H_INCLUDE
#define KVS__UNSTRUCTURED_VOLUME_EXPORTER_H_INCLUDE

#include <kvs/ObjectBase>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/KVSMLObjectUnstructuredVolume>
#include <kvs/AVSUcd>
#include <kvs/ExporterBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Unstructured volume exporter class.
 */
/*===========================================================================*/
template <typename FileFormatType>
class UnstructuredVolumeExporter : public kvs::ExporterBase, public FileFormatType
{
    kvsModule( kvs::UnstructuredVolumeExporter<FileFormatType>, Exporter );
    kvsModuleBaseClass( kvs::ExporterBase );

public:
    FileFormatType* exec( const kvs::ObjectBase* ) { return NULL; }
};

/*===========================================================================*/
/**
 *  @brief  Specialized structured volume exporter class for kvs::KVSMLObjectUnstructuredVolume.
 */
/*===========================================================================*/
template <>
class UnstructuredVolumeExporter<kvs::KVSMLObjectUnstructuredVolume> :
        public kvs::ExporterBase,
        public kvs::KVSMLObjectUnstructuredVolume
{
public:
    UnstructuredVolumeExporter( const kvs::UnstructuredVolumeObject* object );
    kvs::KVSMLObjectUnstructuredVolume* exec( const kvs::ObjectBase* object );
};

/*===========================================================================*/
/**
 *  @brief  Specialized structured volume exporter class for kvs::AVSUcd.
 */
/*===========================================================================*/
template <>
class UnstructuredVolumeExporter<kvs::AVSUcd> :
        public kvs::ExporterBase,
        public kvs::AVSUcd
{
public:
    UnstructuredVolumeExporter( const kvs::UnstructuredVolumeObject* object );
    kvs::AVSUcd* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs

#endif // KVS__UNSTRUCTURED_VOLUME_EXPORTER_H_INCLUDE
