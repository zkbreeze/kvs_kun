/****************************************************************************/
/**
 *  @file   VolumeRendererBase.cpp
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
#include "VolumeRendererBase.h"
#include <kvs/Camera>
#include <kvs/Math>
#include <kvs/OpenGL>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
VolumeRendererBase::VolumeRendererBase():
    m_width( 0 ),
    m_height( 0 ),
    m_shader( NULL )
{
    m_depth_buffer.setFormat( GL_DEPTH_COMPONENT );
    m_depth_buffer.setType( GL_FLOAT );

    m_color_buffer.setFormat( GL_RGBA );
    m_color_buffer.setType( GL_UNSIGNED_BYTE );
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
VolumeRendererBase::~VolumeRendererBase()
{
    if ( m_shader ) { delete m_shader; }
}

/*===========================================================================*/
/**
 *  @brief  Allocates a memory for the depth data.
 *  @param  size [in] number of element of the data
 */
/*===========================================================================*/
void VolumeRendererBase::allocateDepthData( const size_t size )
{
    m_depth_data.allocate( size );
}

/*===========================================================================*/
/**
 *  @brief  Allocates a memory for the color data.
 *  @param  size [in] number of element of the data
 */
/*===========================================================================*/
void VolumeRendererBase::allocateColorData( const size_t size )
{
    m_color_data.allocate( size );
}

/*===========================================================================*/
/**
 *  @brief  Fills a value into the depth data.
 *  @param  value [in] depth value
 */
/*===========================================================================*/
void VolumeRendererBase::fillDepthData( const kvs::Real32 value )
{
    m_depth_data.fill( value );
}

/*===========================================================================*/
/**
 *  @brief  Fills a value into the color data.
 *  @param  value [in] color value (single component)
 */
/*===========================================================================*/
void VolumeRendererBase::fillColorData( const kvs::UInt8 value )
{
    m_color_data.fill( value );
}

/*===========================================================================*/
/**
 *  @brief  Reads color and depth buffers.
 */
/*===========================================================================*/
void VolumeRendererBase::readImage()
{
    m_depth_buffer.readPixels( 0, 0, m_width, m_height, m_depth_data.data() );
    m_color_buffer.readPixels( 0, 0, m_width, m_height, m_color_data.data() );
}

/*==========================================================================*/
/**
 *  Draw the rendering image.
 */
/*==========================================================================*/
void VolumeRendererBase::drawImage()
{
    GLint viewport[4];
    kvs::OpenGL::GetViewport( viewport );

    kvs::OpenGL::SetDepthFunc( GL_LEQUAL );
    kvs::OpenGL::SetDepthMask( GL_TRUE );
    kvs::OpenGL::SetColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
    {
        kvs::OpenGL::WithEnabled e( GL_DEPTH_TEST );
        m_depth_buffer.drawPixels( 0, 0, m_width, m_height, m_depth_data.data() );
    }

    kvs::OpenGL::SetBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    kvs::OpenGL::SetColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    {
        kvs::OpenGL::WithEnabled e( GL_BLEND );
        kvs::OpenGL::WithDisabled d( GL_DEPTH_TEST );
        m_color_buffer.drawPixels( 0, 0, m_width, m_height, m_color_data.data() );
    }
}

} // end of namespace kvs
