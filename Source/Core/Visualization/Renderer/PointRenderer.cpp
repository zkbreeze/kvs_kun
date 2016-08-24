/****************************************************************************/
/**
 *  @file   PointRenderer.cpp
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
#include "PointRenderer.h"
#include "PointRenderingFunction.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/PointObject>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
PointRenderer::PointRenderer():
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
PointRenderer::~PointRenderer()
{
}

/*==========================================================================*/
/**
 *  Point rendering method.
 *  @param object [in] pointer to the object
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void PointRenderer::exec( ObjectBase* object, Camera* camera, Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    kvs::IgnoreUnusedVariable( camera );

    kvs::PointObject* point = kvs::PointObject::DownCast( object );

    BaseClass::startTimer();

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    if ( point->normals().size() == 0 ) { BaseClass::disableShading(); }

    this->initialize();

#if KVS_ENABLE_DEPRECATED
    point->applyMaterial();
#endif

    glEnable( GL_DEPTH_TEST );
    ::PointRenderingFunction( point );
    glDisable( GL_DEPTH_TEST );

    glPopAttrib();

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  Enables anti-aliasing.
 */
/*===========================================================================*/
void PointRenderer::enableAntiAliasing( const bool multisample ) const
{
    m_enable_anti_aliasing = true;
    m_enable_multisample_anti_aliasing = multisample;
}

/*===========================================================================*/
/**
 *  Disables anti-aliasing.
 */
/*===========================================================================*/
void PointRenderer::disableAntiAliasing() const
{
    m_enable_anti_aliasing = false;
    m_enable_multisample_anti_aliasing = false;
}

void PointRenderer::enableTwoSideLighting() const
{
    m_enable_two_side_lighting = true;
}

void PointRenderer::disableTwoSideLighting() const
{
    m_enable_two_side_lighting = false;
}

bool PointRenderer::isTwoSideLighting() const
{
    return m_enable_two_side_lighting;
}

void PointRenderer::initialize()
{
    glShadeModel( GL_SMOOTH );

    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );

    if ( !this->isEnabledShading() )
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
            glEnable( GL_POINT_SMOOTH );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
        }
    }
}

} // end of namespace kvs
