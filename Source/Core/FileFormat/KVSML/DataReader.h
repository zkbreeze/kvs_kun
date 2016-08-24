/*****************************************************************************/
/**
 *  @file   DataReader.h
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
#ifndef KVS__KVSML__DATA_READER_H_INCLUDE
#define KVS__KVSML__DATA_READER_H_INCLUDE

#include <kvs/ValueArray>
#include <kvs/XMLNode>
#include <kvs/Type>


namespace kvs
{

namespace kvsml
{

bool ReadCoordData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t ncoords,
    kvs::ValueArray<kvs::Real32>* coords );

bool ReadColorData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t ncolors,
    kvs::ValueArray<kvs::UInt8>* colors );

bool ReadNormalData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nnormals,
    kvs::ValueArray<kvs::Real32>* normals );

bool ReadSizeData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nsizes,
    kvs::ValueArray<kvs::Real32>* sizes );

bool ReadConnectionData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nconnections,
    kvs::ValueArray<kvs::UInt32>* connections );

bool ReadOpacityData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nopacities,
    kvs::ValueArray<kvs::UInt8>* opacities );

} // end of namespace kvsml

} // end of namespace kvs

#endif // KVS__KVSML__DATA_READER_H_INCLUDE
