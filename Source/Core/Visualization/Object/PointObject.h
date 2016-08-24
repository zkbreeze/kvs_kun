/****************************************************************************/
/**
 *  @file   PointObject.h
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
#ifndef KVS__POINT_OBJECT_H_INCLUDE
#define KVS__POINT_OBJECT_H_INCLUDE

#include <ostream>
#include <kvs/GeometryObjectBase>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Vector3>
#include <kvs/RGBColor>
#include <kvs/Module>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

class LineObject;
class PolygonObject;

/*==========================================================================*/
/**
 *  Point object class.
 */
/*==========================================================================*/
class PointObject : public kvs::GeometryObjectBase
{
    kvsModule( kvs::PointObject, Object );
    kvsModuleBaseClass( kvs::GeometryObjectBase );

private:

    kvs::ValueArray<kvs::Real32> m_sizes; ///< size array

public:

    PointObject();
    explicit PointObject( const kvs::LineObject& line );
    explicit PointObject( const kvs::PolygonObject& polygon );

    void add( const PointObject& other );
    void shallowCopy( const PointObject& other );
    void deepCopy( const PointObject& other );
    void clear();
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;

    void setSizes( const kvs::ValueArray<kvs::Real32>& sizes ) { m_sizes = sizes; }
    void setSize( const kvs::Real32 size );

    size_t numberOfSizes() const { return m_sizes.size(); }

    kvs::Real32 size( const size_t index = 0 ) const { return m_sizes[index]; }
    const kvs::ValueArray<kvs::Real32>& sizes() const { return m_sizes; }

public:
    KVS_DEPRECATED( PointObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const kvs::ValueArray<kvs::Real32>& sizes ) )
    {
        this->setCoords( coords );
        this->setColors( colors );
        this->setNormals( normals );
        this->setSizes( sizes );
    }

    KVS_DEPRECATED( PointObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const kvs::Real32 size ) )
    {
        this->setCoords( coords );
        this->setColors( colors );
        this->setNormals( normals );
        this->setSize( size );
    }

    KVS_DEPRECATED( PointObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::RGBColor& color,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const kvs::ValueArray<kvs::Real32>& sizes ) )
    {
        this->setCoords( coords );
        this->setColor( color );
        this->setNormals( normals );
        this->setSizes( sizes );
    }

    KVS_DEPRECATED( PointObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const kvs::ValueArray<kvs::Real32>& sizes ) )
    {
        this->setCoords( coords );
        this->setNormals( normals );
        this->setSizes( sizes );
    }

    KVS_DEPRECATED( PointObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::RGBColor& color,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const kvs::Real32 size ) )
    {
        this->setCoords( coords );
        this->setColor( color );
        this->setNormals( normals );
        this->setSize( size );
    }

    KVS_DEPRECATED( PointObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::ValueArray<kvs::Real32>& sizes ) )
    {
        this->setCoords( coords );
        this->setColors( colors );
        this->setSizes( sizes );
    }

    KVS_DEPRECATED( PointObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::RGBColor& color,
                        const kvs::ValueArray<kvs::Real32>& sizes ) )
    {
        this->setCoords( coords );
        this->setColor( color );
        this->setSizes( sizes );
    }

    KVS_DEPRECATED( PointObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt8>&  colors,
                        const kvs::Real32 size ) )
    {
        this->setCoords( coords );
        this->setColors( colors );
        this->setSize( size );
    }

    KVS_DEPRECATED( PointObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::RGBColor& color,
                        const kvs::Real32 size ) )
    {
        this->setCoords( coords );
        this->setColor( color );
        this->setSize( size );
    }

    KVS_DEPRECATED( PointObject(
                        const kvs::ValueArray<kvs::Real32>& coords ) )
    {
        this->setCoords( coords );
        this->setColor( kvs::RGBColor( 255, 255, 255 ) );
        this->setSize( 1 );
    }

    KVS_DEPRECATED( PointObject& operator += ( const PointObject& other ) )
    {
        this->add( other );
        return( *this );
    }

    KVS_DEPRECATED( size_t nsizes() const ) { return this->numberOfSizes(); }
    KVS_DEPRECATED( friend std::ostream& operator << ( std::ostream& os, const PointObject& object ) );
};

} // end of namespace kvs

#endif // KVS__POINT_OBJECT_H_INCLUDE
