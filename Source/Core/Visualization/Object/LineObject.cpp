/*****************************************************************************/
/**
 *  @file   LineObject.cpp
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
#include "LineObject.h"
#include <string>
#include <kvs/PolygonObject>
#include <kvs/Assert>
#include <kvs/Type>
#include <map>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns the name of line type as string.
 *  @param  type [in] line type
 *  @return name of line type as string
 */
/*===========================================================================*/
const std::string GetLineTypeName( const kvs::LineObject::LineType type )
{
    switch( type )
    {
    case kvs::LineObject::Strip: return "strip";
    case kvs::LineObject::Uniline: return "uniline";
    case kvs::LineObject::Polyline: return "polyline";
    case kvs::LineObject::Segment: return "segment";
    default: return "unknown line type";
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the name of color type as string.
 *  @param  type [in] color type
 *  @return name of color type as string
 */
/*===========================================================================*/
const std::string GetColorTypeName( const kvs::LineObject::ColorType type )
{
    switch( type )
    {
    case kvs::LineObject::VertexColor: return "vertex color";
    case kvs::LineObject::LineColor: return "line color";
    default: return "unknown color type";
    }
}

/*===========================================================================*/
/**
 *  @brief  Edge map class (hash map table for the edge data).
 */
/*===========================================================================*/
class EdgeMap
{
public:

    typedef kvs::UInt32 Key;
    typedef std::pair<kvs::UInt32,kvs::UInt32> Value;
    typedef std::multimap<Key,Value> Bucket;

private:

    size_t m_nvertices; ///< number of vertices of the original data
    Bucket m_bucket; ///< bucket for the edge data

public:

    EdgeMap( const size_t nvertices ):
        m_nvertices( nvertices ) {}

    void insert( const kvs::UInt32 v0, const kvs::UInt32 v1 )
    {
        const Key key = ( v0 + v1 ) % kvs::UInt32( m_nvertices );

        Bucket::iterator e = m_bucket.find( key );
        Bucket::const_iterator last = m_bucket.end();
        if ( e != last )
        {
            Bucket::const_iterator upper = m_bucket.upper_bound( key );
            while ( e != upper )
            {
                if ( ( e->second.first == v0 && e->second.second == v1 ) ||
                     ( e->second.first == v1 && e->second.second == v0 ) )
                {
                    // The edge has been already inserted in the bucket.
                    return;
                }
                e++;
            }
        }

        m_bucket.insert( std::make_pair( key, std::make_pair( v0, v1 ) ) );
    }

    const kvs::ValueArray<kvs::UInt32> serialize()
    {
        kvs::ValueArray<kvs::UInt32> connections( 2 * m_bucket.size() );

        Bucket::const_iterator e = m_bucket.begin();
        Bucket::const_iterator last = m_bucket.end();
        size_t connection_index = 0;
        while ( e != last )
        {
            connections[ connection_index++ ] = e->second.first;
            connections[ connection_index++ ] = e->second.second;
            e++;
        }

        return connections;
    }
};

kvs::ValueArray<kvs::UInt32> GetConnections( const kvs::PolygonObject& polygon )
{
    const size_t nconnections = polygon.numberOfConnections();
    const size_t ncorners = size_t( polygon.polygonType() );
    const size_t npolygons = ( nconnections == 0 ) ?
        polygon.numberOfVertices() / ncorners : nconnections;

    EdgeMap edge_map( polygon.numberOfVertices() );
    if ( nconnections > 0 )
    {
        if ( polygon.polygonType() == kvs::PolygonObject::Triangle )
        {
            for ( size_t i = 0; i < npolygons; i++ )
            {
                const kvs::UInt32 v0 =  polygon.connections().at( 3 * i );
                const kvs::UInt32 v1 =  polygon.connections().at( 3 * i + 1 );
                const kvs::UInt32 v2 =  polygon.connections().at( 3 * i + 2 );
                edge_map.insert( v0, v1 );
                edge_map.insert( v1, v2 );
                edge_map.insert( v2, v0 );
            }
        }
        else if ( polygon.polygonType() == kvs::PolygonObject::Quadrangle )
        {
            for ( size_t i = 0; i < npolygons; i++ )
            {
                const kvs::UInt32 v0 =  polygon.connections().at( 4 * i );
                const kvs::UInt32 v1 =  polygon.connections().at( 4 * i + 1 );
                const kvs::UInt32 v2 =  polygon.connections().at( 4 * i + 2 );
                const kvs::UInt32 v3 =  polygon.connections().at( 4 * i + 3 );
                edge_map.insert( v0, v1 );
                edge_map.insert( v1, v2 );
                edge_map.insert( v2, v3 );
                edge_map.insert( v3, v0 );
            }
        }
    }
    else
    {
        if ( polygon.polygonType() == kvs::PolygonObject::Triangle )
        {
            for ( size_t i = 0; i < npolygons; i++ )
            {
                const kvs::UInt32 v0 =  3 * i;
                const kvs::UInt32 v1 =  3 * i + 1;
                const kvs::UInt32 v2 =  3 * i + 2;
                edge_map.insert( v0, v1 );
                edge_map.insert( v1, v2 );
                edge_map.insert( v2, v0 );
            }
        }
        else if ( polygon.polygonType() == kvs::PolygonObject::Quadrangle )
        {
            for ( size_t i = 0; i < npolygons; i++ )
            {
                const kvs::UInt32 v0 =  4 * i;
                const kvs::UInt32 v1 =  4 * i + 1;
                const kvs::UInt32 v2 =  4 * i + 2;
                const kvs::UInt32 v3 =  4 * i + 3;
                edge_map.insert( v0, v1 );
                edge_map.insert( v1, v2 );
                edge_map.insert( v2, v3 );
                edge_map.insert( v3, v0 );
            }
        }
    }

    return edge_map.serialize();
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new LineObject class.
 */
/*===========================================================================*/
LineObject::LineObject()
{
    BaseClass::setGeometryType( Line );
    this->setSize( 1 );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LineObject class.
 *  @param  polygon [in] polygon object
 */
/*===========================================================================*/
LineObject::LineObject( const kvs::PolygonObject& polygon )
{
    BaseClass::setGeometryType( Line );
    BaseClass::setCoords( polygon.coords() );

    if( polygon.colorType() == kvs::PolygonObject::VertexColor )
    {
        this->setColorType( LineObject::VertexColor );
        BaseClass::setColors( polygon.colors() );
    }
    else
    {
        this->setColorType( LineObject::LineColor );
        BaseClass::setColor( polygon.color() );
    }

    this->setSize( 1.0f );
    this->setLineType( LineObject::Segment );
    this->setConnections( ::GetConnections( polygon ) );

    BaseClass::setMinMaxObjectCoords(
        polygon.minObjectCoord(),
        polygon.maxObjectCoord() );

    BaseClass::setMinMaxExternalCoords(
        polygon.minExternalCoord(),
        polygon.maxExternalCoord() );
}

/*===========================================================================*/
/**
 *  @brief  Shallow copys.
 *  @param  object [in] line object
 */
/*===========================================================================*/
void LineObject::shallowCopy( const LineObject& object )
{
    BaseClass::shallowCopy( object );
    m_line_type = object.lineType();
    m_color_type = object.colorType();
    m_connections = object.connections();
    m_sizes = object.sizes();
}

/*===========================================================================*/
/**
 *  @brief  Deep copys.
 *  @param  object [in] line object
 */
/*===========================================================================*/
void LineObject::deepCopy( const LineObject& object )
{
    BaseClass::deepCopy( object );
    m_line_type = object.lineType();
    m_color_type = object.colorType();
    m_connections = object.connections().clone();
    m_sizes = object.sizes().clone();
}

/*===========================================================================*/
/**
 *  @brief  Clears the data of the line object.
 */
/*===========================================================================*/
void LineObject::clear()
{
    BaseClass::clear();
    m_connections.release();
    m_sizes.release();
}

/*===========================================================================*/
/**
 *  @brief  Prints the information of the line object.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void LineObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Object type : " << "line object" << std::endl;
    BaseClass::print( os, indent );
    os << indent << "Number of connections : " << this->numberOfConnections() << std::endl;
    os << indent << "Number of sizes : " << this->numberOfSizes() << std::endl;
    os << indent << "Line type : " << ::GetLineTypeName( this->lineType() ) << std::endl;
    os << indent << "Color type : " << ::GetColorTypeName( this->colorType() ) << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Sets a color value.
 *  @param  color [in] color value
 */
/*===========================================================================*/
void LineObject::setColor( const kvs::RGBColor& color )
{
    BaseClass::setColor( color );
    m_color_type = LineObject::LineColor;
}

/*===========================================================================*/
/**
 *  @brief  Sets a size value.
 *  @param  size [in] size value
 */
/*===========================================================================*/
void LineObject::setSize( const kvs::Real32 size )
{
    m_sizes.allocate( 1 );
    m_sizes[0] = size;
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of connections.
 *  @return number of connections
 */
/*===========================================================================*/
size_t LineObject::numberOfConnections() const
{
    return
        m_line_type == LineObject::Uniline ?
        m_connections.size() :
        m_connections.size() / 2;
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of sizes.
 *  @return number of sizes
 */
/*===========================================================================*/
size_t LineObject::numberOfSizes() const
{
    return m_sizes.size();
}

/*===========================================================================*/
/**
 *  @brief  Returns the connection specified by the index.
 *  @param  index [in] index of connection array
 *  @return connection
 */
/*===========================================================================*/
const kvs::Vec2ui LineObject::connection( const size_t index ) const
{
    return kvs::Vec2ui( (unsigned int*)m_connections.data() + 2 * index );
}

/*===========================================================================*/
/**
 *  @brief  Returns the size.
 *  @param  index [in] index of size array
 *  @return size
 */
/*===========================================================================*/
kvs::Real32 LineObject::size( const size_t index ) const
{
    return m_sizes[index];
}

std::ostream& operator << ( std::ostream& os, const LineObject& object )
{
    os << "Object type:  " << "line object" << std::endl;
#ifdef KVS_COMPILER_VC
#if KVS_COMPILER_VERSION_LESS_OR_EQUAL( 8, 0 )
    // @TODO Cannot instance the object that is a abstract class here (error:C2259).
#endif
#else
    os << static_cast<const kvs::GeometryObjectBase&>( object ) << std::endl;
#endif
    os << "Number of connections:  " << object.numberOfConnections() << std::endl;
    os << "Number of sizes:  " << object.numberOfSizes() << std::endl;
    os << "Line type:  " << ::GetLineTypeName( object.lineType() ) << std::endl;
    os << "Color type:  " << ::GetColorTypeName( object.colorType() );

    return os;
}

} // end of namespace kvs
