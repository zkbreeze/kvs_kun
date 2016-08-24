/****************************************************************************/
/**
 *  @file   HexahedralCell.h
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
#ifndef KVS__HEXAHEDRAL_CELL_H_INCLUDE
#define KVS__HEXAHEDRAL_CELL_H_INCLUDE

#include <kvs/Type>
#include <kvs/Vector4>
#include <kvs/Matrix44>
#include <kvs/UnstructuredVolumeObject>
#include "CellBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  HexahedralCell class.
 */
/*===========================================================================*/
class HexahedralCell : public kvs::CellBase
{
public:

    enum { NumberOfNodes = kvs::UnstructuredVolumeObject::Hexahedra };
    typedef kvs::CellBase BaseClass;

public:

    HexahedralCell( const kvs::UnstructuredVolumeObject* volume );
    virtual ~HexahedralCell();

    const kvs::Real32* interpolationFunctions( const kvs::Vec3& local ) const;
    const kvs::Real32* differentialFunctions( const kvs::Vec3& local ) const;
    const kvs::Vec3 randomSampling() const;
    const kvs::Real32 volume() const;
};

} // end of namespace kvs

#endif // KVS__HEXAHEDRAL_CELL_H_INCLUDE
