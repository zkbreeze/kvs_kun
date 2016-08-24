/****************************************************************************/
/**
 *  @file   KVSMLObjectImage.cpp
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
#include "KVSMLObjectImage.h"
#include "ImageObjectTag.h"
#include "PixelTag.h"
#include "DataArrayTag.h"
#include <kvs/XMLDocument>
#include <kvs/XMLDeclaration>
#include <kvs/XMLElement>
#include <kvs/XMLComment>
#include <kvs/ValueArray>
#include <kvs/File>
#include <kvs/Type>
#include <kvs/File>
#include <kvs/IgnoreUnusedVariable>
#include <iostream>
#include <fstream>
#include <sstream>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Checks the file extension.
 *  @param  filename [in] filename
 *  @return true, if the given filename has the supported extension
 */
/*===========================================================================*/
bool KVSMLObjectImage::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "kvsml" || file.extension() == "KVSML" ||
         file.extension() == "xml"   || file.extension() == "XML" )
    {
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Check the file format.
 *  @param  filename [in] filename
 *  @return true, if the KVSMLObjectImage class can read the given file
 */
/*===========================================================================*/
bool KVSMLObjectImage::CheckFormat( const std::string& filename )
{
    kvs::XMLDocument document;
    if ( !document.read( filename ) ) return false;

    // <KVSML>
    const std::string kvsml_tag("KVSML");
    const kvs::XMLNode::SuperClass* kvsml_node = kvs::XMLDocument::FindNode( &document, kvsml_tag );
    if ( !kvsml_node ) return false;

    // <Object>
    const std::string object_tag("Object");
    const kvs::XMLNode::SuperClass* object_node = kvs::XMLNode::FindChildNode( kvsml_node, object_tag );
    if ( !object_node ) return false;

    // <ImageObject>
    const std::string image_tag("ImageObject");
    const kvs::XMLNode::SuperClass* image_node = kvs::XMLNode::FindChildNode( object_node, image_tag );
    if ( !image_node ) return false;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML image object class.
 */
/*===========================================================================*/
KVSMLObjectImage::KVSMLObjectImage():
    m_width( 0 ),
    m_height( 0 ),
    m_pixel_type( "" ),
    m_writing_type( kvs::KVSMLObjectImage::Ascii )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML image object class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
KVSMLObjectImage::KVSMLObjectImage( const std::string& filename ):
    m_width( 0 ),
    m_height( 0 ),
    m_pixel_type( "" ),
    m_writing_type( kvs::KVSMLObjectImage::Ascii )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Prints file information.
 *  @param  os [in] output stream
 *  @param  indent [in] indent for each line
 */
/*===========================================================================*/
void KVSMLObjectImage::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Width : " << m_width << std::endl;
    os << indent << "Height : " << m_height << std::endl;
    os << indent << "Pixel type : " << m_pixel_type << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Read a KVSMl point object file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is successfully
 */
/*===========================================================================*/
bool KVSMLObjectImage::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( false );

    // XML document.
    kvs::XMLDocument document;
    if ( !document.read( filename ) )
    {
        kvsMessageError( "%s", document.ErrorDesc().c_str() );
        return false;
    }

    // <KVSML>
    m_kvsml_tag.read( &document );

    // <Object>
    if ( !m_object_tag.read( m_kvsml_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_object_tag.name().c_str() );
        return false;
    }

    // <ImageObject>
    kvs::kvsml::ImageObjectTag image_object_tag;
    if ( !image_object_tag.read( m_object_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", image_object_tag.name().c_str() );
        return false;
    }

    if ( !image_object_tag.hasWidth() )
    {
        kvsMessageError( "'width' is not specified in <%s>.", image_object_tag.name().c_str() );
        return false;
    }
    m_width = image_object_tag.width();

    if ( !image_object_tag.hasHeight() )
    {
        kvsMessageError( "'height' is not specified in <%s>.", image_object_tag.name().c_str() );
        return false;
    }
    m_height = image_object_tag.height();

    // <Pixel>
    kvs::kvsml::PixelTag pixel_tag;
    if ( !pixel_tag.read( image_object_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", image_object_tag.name().c_str() );
        return false;
    }
    else
    {
        if ( !pixel_tag.hasType() )
        {
            kvsMessageError( "'type' is not specified in <%s>.", pixel_tag.name().c_str() );
            return false;
        }
        m_pixel_type = pixel_tag.type();

        // <DataArray>
        const size_t nchannels =
            ( m_pixel_type == "gray" ) ? 1 :
            ( m_pixel_type == "color" ) ? 3 : 0;
        const size_t npixels = m_width * m_height;
        const size_t nelements = npixels * nchannels;
        kvs::kvsml::DataArrayTag data_tag;
        if ( !data_tag.read( pixel_tag.node(), nelements, &m_pixels ) )
        {
            kvsMessageError( "Cannot read <%s> for <%s>.",
                             data_tag.name().c_str(),
                             pixel_tag.name().c_str() );
            return false;
        }
    }

    BaseClass::setSuccess( true );
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the KVSML point object.
 *  @param  filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool KVSMLObjectImage::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( false );

    kvs::XMLDocument document;
    document.InsertEndChild( kvs::XMLDeclaration("1.0") );
    document.InsertEndChild( kvs::XMLComment(" Generated by kvs::KVSMLObjectImage::write() ") );

    // <KVSML>
    kvs::kvsml::KVSMLTag kvsml_tag;
    kvsml_tag.write( &document );

    // <Object type="ImageObject">
    kvs::kvsml::ObjectTag object_tag;
    object_tag.setType( "ImageObject" );
    if ( !object_tag.write( kvsml_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", object_tag.name().c_str() );
        return false;
    }

    // <ImageObject width="xxx" height="xxx">
    kvs::kvsml::ImageObjectTag image_object_tag;
    image_object_tag.setWidth( m_width );
    image_object_tag.setHeight( m_height );
    if ( !image_object_tag.write( object_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", image_object_tag.name().c_str() );
        return false;
    }

    // <Pixel>
    kvs::kvsml::PixelTag pixel_tag;
    pixel_tag.setType( m_pixel_type );
    if ( !pixel_tag.write( image_object_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", pixel_tag.name().c_str() );
        return false;
    }
    else
    {
        if ( m_pixels.size() > 0 )
        {
            // <DataArray>
            kvs::kvsml::DataArrayTag data_tag;
            if ( m_writing_type == kvs::KVSMLObjectImage::ExternalAscii )
            {
                data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "pixel" ) );
                data_tag.setFormat( "ascii" );
            }
            else if ( m_writing_type == kvs::KVSMLObjectImage::ExternalBinary )
            {
                data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "pixel" ) );
                data_tag.setFormat( "binary" );
            }

            const std::string pathname = kvs::File( filename ).pathName();
            if ( !data_tag.write( pixel_tag.node(), m_pixels, pathname ) )
            {
                kvsMessageError( "Cannot write <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 pixel_tag.name().c_str() );
                return false;
            }
        }
    }

    const bool success = document.write( filename );
    BaseClass::setSuccess( success );

    return success;
}

} // end of namespace kvs
