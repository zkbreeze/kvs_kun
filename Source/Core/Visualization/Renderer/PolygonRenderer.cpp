/****************************************************************************/
/**
 *  @file   PolygonRenderer.cpp
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
#include "PolygonRenderer.h"
#include "PolygonRenderingFunction.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/PolygonObject>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
PolygonRenderer::PolygonRenderer():
    m_enable_anti_aliasing( false ),
    m_enable_multisample_anti_aliasing( false ),
    m_enable_two_side_lighting( true )
{
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
PolygonRenderer::~PolygonRenderer()
{
}

/*==========================================================================*/
/**
 *  Polygon rendering method.
 *  @param object [in] pointer to the object
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void PolygonRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    kvs::IgnoreUnusedVariable( camera );

    kvs::PolygonObject* polygon = reinterpret_cast<kvs::PolygonObject*>( object );

    BaseClass::startTimer();

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

/*
    if ( this->isEnabledShading() )
    {
        if ( polygon->normals().size() == 0 )
        {
            glEnable( GL_AUTO_NORMAL );
        }
    }
*/

    this->initialize();
#if KVS_ENABLE_DEPRECATED
    polygon->applyMaterial();
#endif

    // Anti-aliasing.
    if ( m_enable_anti_aliasing )
    {
#if defined ( GL_MULTISAMPLE )
        if ( m_enable_multisample_anti_aliasing )
        {
            GLint buffers = 0;
            GLint samples = 0;
            glGetIntegerv( GL_SAMPLE_BUFFERS, &buffers );
            glGetIntegerv( GL_SAMPLES, &samples );
            if ( buffers > 0 && samples > 1 ) glEnable( GL_MULTISAMPLE );
        }
        else
#endif
        {
            glEnable( GL_POLYGON_SMOOTH );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
        }
    }

    glEnable( GL_DEPTH_TEST );
    ::PolygonRenderingFunction( polygon );
    glDisable( GL_DEPTH_TEST );

    glPopAttrib();

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  Enables anti-aliasing.
 */
/*===========================================================================*/
void PolygonRenderer::enableAntiAliasing( const bool multisample ) const
{
    m_enable_anti_aliasing = true;
    m_enable_multisample_anti_aliasing = multisample;
}

/*===========================================================================*/
/**
 *  Disables anti-aliasing.
 */
/*===========================================================================*/
void PolygonRenderer::disableAntiAliasing() const
{
    m_enable_anti_aliasing = false;
    m_enable_multisample_anti_aliasing = false;
}

void PolygonRenderer::enableTwoSideLighting() const
{
    m_enable_two_side_lighting = true;
}

void PolygonRenderer::disableTwoSideLighting() const
{
    m_enable_two_side_lighting = false;
}

bool PolygonRenderer::isTwoSideLighting() const
{
    return( m_enable_two_side_lighting );
}

/*==========================================================================*/
/**
 *  Initialize OpenGL status for the rendering.
 */
/*==========================================================================*/
void PolygonRenderer::initialize()
{
    glDisable( GL_LINE_SMOOTH );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glShadeModel( GL_SMOOTH );

    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );

    if( !this->isEnabledShading() )
    {
        glDisable( GL_NORMALIZE );
        glDisable( GL_LIGHTING );
    }
    else
    {
        glEnable( GL_NORMALIZE );
        glEnable( GL_LIGHTING );
    }

    kvs::Light::SetModelTwoSide( this->isTwoSideLighting() );
}

} // end of namespace kvs
