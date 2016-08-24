/****************************************************************************/
/**
 *  @file   Camera.cpp
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
#include "Camera.h"
#include <utility>
#include <kvs/OpenGL>
#include <kvs/ColorImage>
#include <kvs/Matrix44>
#include <kvs/ViewingMatrix44>
#include <kvs/FrustumMatrix44>
#include <kvs/PerspectiveMatrix44>
#include <kvs/OrthogonalMatrix44>


namespace
{

const kvs::Xform MakeCameraXform(
    const kvs::Vec3& eye,
    const kvs::Vec3& center,
    const kvs::Vec3& up )
{
    const kvs::Vec3 z = ( eye - center ).normalized();
    const kvs::Vec3 x = up.cross( z ).normalized();
    const kvs::Vec3 y = z.cross( x ).normalized();
    const kvs::Vec3 t = eye;

    const kvs::Mat4 m(
        x[0], y[0], z[0], t[0],
        x[1], y[1], z[1], t[1],
        x[2], y[2], z[2], t[2],
        0,    0,    0,    1 );

    return kvs::Xform( m );
}

}

namespace kvs
{

/*==========================================================================*/
/**
 *  Default constructor.
 *  @param collision [in] collision dectection flag
 */
/*==========================================================================*/
Camera::Camera()
{
    this->initialize();
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
Camera::~Camera()
{
}

/*==========================================================================*/
/**
 *  Set a window size.
 *  @param width [in] window width
 *  @param height [in] window height
 */
/*==========================================================================*/
void Camera::setWindowSize( const size_t width, const size_t height )
{
    m_window_width  = width;
    m_window_height = height;
}

/*==========================================================================*/
/**
 *  Set a camera position in the world coordinate system.
 *  @param position [in] camera position
 */
/*==========================================================================*/
void Camera::setPosition( const kvs::Vec3& position )
{
    this->setPosition( position, this->lookAt(), this->upVector() );
}

/*===========================================================================*/
/**
 *  @brief  Sets a camera position with a target position in the world coordinates.
 *  @param  position [in] camera position
 *  @param  look_at [in] target position
 */
/*===========================================================================*/
void Camera::setPosition( const kvs::Vec3& position, const kvs::Vec3& look_at )
{
    this->setPosition( position, look_at, this->upVector() );
}

/*===========================================================================*/
/**
 *  @brief  Sets a camera position with a target position in the world coordinates.
 *  @param  position [in] camera position
 *  @param  look_at [in] target position
 *  @param  up [in] camera's up vector
 */
/*===========================================================================*/
void Camera::setPosition( const kvs::Vec3& position, const kvs::Vec3& look_at, const kvs::Vec3& up )
{
    m_transform_center = look_at;
    this->setXform( ::MakeCameraXform( position, look_at, up ) );
}

/*==========================================================================*/
/**
 *  Set a up vector in the world coordinate system.
 *  @param up [in] up vector
 */
/*==========================================================================*/
void Camera::setUpVector( const kvs::Vec3& up_vector )
{
    this->setPosition( this->position(), this->lookAt(), up_vector );
}

/*==========================================================================*/
/**
 *  Set a look-at point in the world coordinate system.
 *  @param at [in] look-at point
 */
/*==========================================================================*/
void Camera::setLookAt( const kvs::Vec3& look_at )
{
    this->setPosition( this->position(), look_at, this->upVector() );
}

/*==========================================================================*/
/**
 *  Get the camera position.
 */
/*==========================================================================*/
const kvs::Vec3 Camera::position() const
{
    return this->xform().translation();
}

/*==========================================================================*/
/**
 *  Get the up vector.
 */
/*==========================================================================*/
const kvs::Vec3 Camera::upVector() const
{
    const kvs::Mat4 m = this->xform().toMatrix();
    return kvs::Vec3( m[0][1], m[1][1], m[2][1] ).normalized();
}

/*==========================================================================*/
/**
 *  Get the look-at point.
 */
/*==========================================================================*/
const kvs::Vec3 Camera::lookAt() const
{
    return m_transform_center;
}

/*===========================================================================*/
/**
 *  @brief  Returns projection matrix.
 *  @return projection matrix
 */
/*===========================================================================*/
const kvs::Mat4 Camera::projectionMatrix() const
{
    const float aspect = static_cast<float>( m_window_width ) / m_window_height;

    if ( m_projection_type == Camera::Perspective )
    {
        return kvs::PerspectiveMatrix44<float>( m_field_of_view, aspect, m_front, m_back );
    }
    else if ( m_projection_type == Camera::Orthogonal )
    {
        // Orthogonal camera mode
        if ( aspect >= 1.0f )
        {
            return kvs::OrthogonalMatrix44<float>(
                m_left * aspect,
                m_right * aspect,
                m_bottom, m_top,
                m_front, m_back );
        }
        else
        {
            return kvs::OrthogonalMatrix44<float>(
                m_left, m_right, 
                m_bottom / aspect, m_top / aspect,
                m_front, m_back );
        }
    }
    else
    {
        // Frustum camera mode
        if ( aspect >= 1.0f )
        {
            return kvs::FrustumMatrix44<float>(
                m_left * aspect, m_right * aspect,
                m_bottom, m_top,
                m_front, m_back );
        }
        else
        {
            return kvs::FrustumMatrix44<float>(
                m_left, m_right,
                m_bottom / aspect, m_top / aspect,
                m_front, m_back );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns viewing matrix.
 *  @return viewing matrix
 */
/*===========================================================================*/
const kvs::Mat4 Camera::viewingMatrix() const
{
    return this->xform().inverse().toMatrix();
}

/*==========================================================================*/
/**
 *  Get a look-at point in device coordinate system.
 *  @return look-at point in the device coordinate system
 */
/*==========================================================================*/
const kvs::Vec2 Camera::lookAtInDevice() const
{
    return kvs::Vec2( m_window_width / 2.0f, m_window_height / 2.0f );
}

/*==========================================================================*/
/**
 *  Initialize the member parameters.
 */
/*==========================================================================*/
void Camera::initialize()
{
    m_projection_type = Camera::Perspective;

    const kvs::Vec3 init_pos( 0, 0, 12 );
    const kvs::Vec3 init_look_at( 0, 0, 0 );
    const kvs::Vec3 init_up( 0, 1, 0 );

    this->setPosition( init_pos, init_look_at, init_up );
    this->saveXform();
    m_transform_center = init_look_at;

    m_field_of_view = 45.0;
    m_front = 1.0;
    m_back = 2000.0;
    m_left = -5.0;
    m_right = 5.0;
    m_bottom = -5.0;
    m_top = 5.0;
    m_window_width = 512;
    m_window_height = 512;
}

/*==========================================================================*/
/**
 *  Update the camera.
 */
/*==========================================================================*/
void Camera::update()
{
    float ary[16];

    KVS_GL_CALL( glMatrixMode( GL_PROJECTION ) );
    KVS_GL_CALL( glLoadIdentity() );
    kvs::Xform p( this->projectionMatrix() );
    p.toArray( ary );
    KVS_GL_CALL( glMultMatrixf( ary ) );

    KVS_GL_CALL( glMatrixMode( GL_MODELVIEW ) );
    KVS_GL_CALL( glLoadIdentity() );
    kvs::Xform v( this->viewingMatrix() );
    v.toArray( ary );
    KVS_GL_CALL( glMultMatrixf( ary ) );
}

/*==========================================================================*/
/**
 *  Get a snapshot image.
 *  @return snapshot image
 */
/*==========================================================================*/
kvs::ColorImage Camera::snapshot()
{
    const int width = static_cast<int>( m_window_width );
    const int height = static_cast<int>( m_window_height );
    const int size = height * width * 3;
    kvs::ValueArray<kvs::UInt8> buffer( size );

    KVS_GL_CALL( glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
    KVS_GL_CALL( glReadBuffer( GL_FRONT ) );
    KVS_GL_CALL( glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data() ) );

    kvs::ColorImage ret( width, height, buffer );
    ret.flip();
    return ret;
}

/*===========================================================================*/
/**
 *  @brief  Resets the xform of the camera.
 */
/*===========================================================================*/
void Camera::resetXform()
{
    kvs::XformControl::resetXform();
    m_transform_center.set( 0, 0, 0 );
}

/*==========================================================================*/
/**
 *  Rotate the camera.
 *  @param rot [in] rotation matrix.
 */
/*==========================================================================*/
void Camera::rotate( const kvs::Mat3& rotation )
{
    const kvs::Vec3 t = m_transform_center;
    const kvs::Xform x = kvs::Xform::Translation( t )
                       * kvs::Xform::Rotation( rotation )
                       * kvs::Xform::Translation( -t );
    this->multiplyXform( x );
}

/*==========================================================================*/
/**
 *  Translate the camera.
 *  @param translation [in] translation vector
 */
/*==========================================================================*/
void Camera::translate( const kvs::Vec3& translation )
{
    this->multiplyXform( kvs::Xform::Translation( translation ) );
    m_transform_center += translation;
}

/*==========================================================================*/
/**
 *  Scale the camera.
 *  @param scaling [in] scaling vector
 */
/*==========================================================================*/
void Camera::scale( const kvs::Vec3& scaling )
{
    this->multiplyXform( kvs::Xform::Scaling( scaling ) );
}

const kvs::Mat4 Camera::modelViewMatrix() const
{
    float m[16];
    this->getModelViewMatrix( &m );
    return kvs::Xform::FromArray( m ).toMatrix();
}

const kvs::Mat4 Camera::projectionModelViewMatrix() const
{
    return this->projectionMatrix() * this->modelViewMatrix();
}

void Camera::getProjectionModelViewMatrix( float (*projection_modelview)[16] ) const
{
    kvs::Mat4 M = this->modelViewMatrix();
    kvs::Mat4 P = this->projectionMatrix();
    kvs::Xform x( P * M );
    x.toArray( *projection_modelview );
}

void Camera::getProjectionModelViewMatrix(
    const float projection[16],
    const float modelview[16],
    float (*projection_modelview)[16] ) const
{
    kvs::Xform M = kvs::Xform::FromArray( modelview );
    kvs::Xform P = kvs::Xform::FromArray( projection );
    kvs::Xform x = P * M;
    x.toArray( *projection_modelview );
}

/*==========================================================================*/
/**
 *  Get a projection matrix.
 *  @param  projection [out] projection matrix
 */
/*==========================================================================*/
void Camera::getProjectionMatrix( float (*projection)[16] ) const
{
    KVS_GL_CALL( glGetFloatv( GL_PROJECTION_MATRIX, (GLfloat*)*projection ) );
}

/*==========================================================================*/
/**
 *  Get a modelview matrix.
 *  @param  modelview [out] modelview matrix
 */
/*==========================================================================*/
void Camera::getModelViewMatrix( float (*modelview)[16] ) const
{
    KVS_GL_CALL( glGetFloatv( GL_MODELVIEW_MATRIX, (GLfloat*)*modelview ) );
}

/*==========================================================================*/
/**
 *  Get a combined matrix (<projection matrix> x <modelview matrix>).
 *  @param  combined [out] combined matrix
 */
/*==========================================================================*/
void Camera::getCombinedMatrix( float (*combined)[16] ) const
{
    this->getProjectionModelViewMatrix( combined );
}

/*==========================================================================*/
/**
 *  Get a combined matrix (<projection matrix> x <modelview matrix>).
 *  @param projection [in] projection matrix
 *  @param modelview [in] modelview matrix
 *  @param  combined   [out] combined matrix
 */
/*==========================================================================*/
void Camera::getCombinedMatrix(
    const float projection[16],
    const float modelview[16],
    float (*combined)[16] ) const
{
    this->getProjectionModelViewMatrix( projection, modelview, combined );
}

/*==========================================================================*/
/**
 *  Get a coordinate value in the window coordinate system.
 *  @param p_obj_x [in] x coordinate value in the object coordinate system
 *  @param p_obj_y [in] y coordinate value in the object coordinate system
 *  @param p_obj_z [in] z coordinate value in the object coordinate system
 *  @param  depth   [out] depth value of projected point
 *  @return projected coordinate value in the window coordinate system
 *
 *  Same as gluProject() in OpenGL.
 */
/*==========================================================================*/
const kvs::Vec2 Camera::projectObjectToWindow(
    float  p_obj_x,
    float  p_obj_y,
    float  p_obj_z,
    float* depth ) const
{
    return this->projectObjectToWindow( kvs::Vec3( p_obj_x, p_obj_y, p_obj_z ), depth );
}

/*==========================================================================*/
/**
 *  Get a coordinate value in the window coordinate system.
 *  @param p_obj [in] coordinate value in the object coordinate system
 *  @param  depth [out] depth value of projected point
 *  @return projected coordinate value in the window coordinate system
 *
 *  Same as gluProject() in OpenGL.
 */
/*==========================================================================*/
const kvs::Vec2 Camera::projectObjectToWindow( const kvs::Vec3& p_obj, float* depth ) const
{
    const kvs::Xform pvm( this->projectionModelViewMatrix() );
    const kvs::Vec3 p = pvm.project( p_obj );

    if ( depth ) *depth = ( 1.0f + p[2] ) * 0.5f;

    return kvs::Vec2( ( 1.0f + p[0] ) * m_window_width * 0.5f,
                      ( 1.0f + p[1] ) * m_window_height * 0.5f );
}

/*==========================================================================*/
/**
 *  Get a coordinate value in the object coordinate system.
 *  @param p_win [in] point in the window coordinate system
 *  @param depth [in] depth value
 *  @return point in the object coordinate system
 */
/*==========================================================================*/
const kvs::Vec3 Camera::projectWindowToObject( const kvs::Vec2& p_win, float depth ) const
{
    return this->projectCameraToObject( this->projectWindowToCamera( p_win, depth ) );
}

/*==========================================================================*/
/**
 *  Get a point in the camera coordinate system.
 *  @param p_win [in] a point in the window coordinate system.
 *  @param depth [in] depth value
 *  @return point in the object coordinate system.
 */
/*==========================================================================*/
const kvs::Vec3 Camera::projectWindowToCamera( const kvs::Vec2& p_win, float depth ) const
{
    GLdouble m[16] = { 1.0, 0.0, 0.0, 0.0,
                       0.0, 1.0, 0.0, 0.0,
                       0.0, 0.0, 1.0, 0.0,
                       0.0, 0.0, 0.0, 1.0 };

    GLdouble p[16]; KVS_GL_CALL( glGetDoublev(  GL_PROJECTION_MATRIX, p ) );
    GLint    v[4];  KVS_GL_CALL( glGetIntegerv( GL_VIEWPORT,          v ) );

    double x = 0;
    double y = 0;
    double z = 0;
    KVS_GL_CALL( gluUnProject( p_win.x(), p_win.y(), depth, m, p, v, &x, &y, &z ) );

    return kvs::Vec3( (float)x, (float)y, (float)z );
}

/*==========================================================================*/
/**
 *  Get a point in the world coordinate system.
 *  @param win [in] point in the window coordinate system
 *  @param depth [in] depth value
 *  @return a point in the world coordinate system
 */
/*==========================================================================*/
const kvs::Vec3 Camera::projectWindowToWorld( const kvs::Vec2& p_win, float depth ) const
{
    return this->projectCameraToWorld( this->projectWindowToCamera( p_win, depth ) );
}

/*==========================================================================*/
/**
 *  Get a point in the camera coodinate system.
 *  @param p_obj [in] point in the object coordinate system
 *  @return point in the object coordinate system
 */
/*==========================================================================*/
const kvs::Vec3 Camera::projectObjectToCamera( const kvs::Vec3& p_obj ) const
{
    const kvs::Xform modelview( this->modelViewMatrix() );
    return modelview.transform( p_obj );
}

/*==========================================================================*/
/**
 *  Get a point in the object coordinate system.
 *  @param p_cam [in] point in the camera coordinate system
 *  @return point in the object coordinate system
 */
/*==========================================================================*/
const kvs::Vec3 Camera::projectCameraToObject(
    const kvs::Vec3& p_cam ) const
{
    const kvs::Xform modelview( this->modelViewMatrix() );
    return modelview.inverse().transform( p_cam );
}

/*==========================================================================*/
/**
 *  Get a point in the camera coordinate system.
 *  @param p_wld [in] point in the world coordinate system
 *  @return point in the camera coordinate system
 */
/*==========================================================================*/
const kvs::Vec3 Camera::projectWorldToCamera( const kvs::Vec3& p_wld ) const
{
    const kvs::Xform viewing( this->viewingMatrix() );
    return viewing.transform( p_wld );
}

/*==========================================================================*/
/**
 *  Get a point in the world coordinate system.
 *  @param p_cam [in] point in the camera coordinate system
 *  @return point in the world coordinate system
 */
/*==========================================================================*/
const kvs::Vec3 Camera::projectCameraToWorld( const kvs::Vec3& p_cam ) const
{
    const kvs::Xform inv_viewing( this->xform() );
    return inv_viewing.transform( p_cam );
}

/*==========================================================================*/
/**
 *  Get a point in the object coordinate system.
 *  @param p_wld [in] point in the world coordinate system
 *  @return point in the object coordinate system
 */
/*==========================================================================*/
const kvs::Vec3 Camera::projectWorldToObject( const kvs::Vec3& p_wld ) const
{
    const kvs::Xform inv_modeling( this->modelViewMatrix().inverted() * this->viewingMatrix() );
    return inv_modeling.transform( p_wld );
}

/*==========================================================================*/
/**
 *  Get a point in the world coordinate system.
 *  @param p_obj [in] point in the object coordinate system
 *  @return point in the world coordinate system
 */
/*==========================================================================*/
const kvs::Vec3 Camera::projectObjectToWorld( const kvs::Vec3& p_obj ) const
{
    const kvs::Xform modeling( this->xform().toMatrix() * this->modelViewMatrix() );
    return modeling.transform( p_obj );
}

} // end of namespace kvs
