/****************************************************************************/
/**
 *  @file   RendererBase.h
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
#ifndef KVS__RENDERER_BASE_H_INCLUDE
#define KVS__RENDERER_BASE_H_INCLUDE

#include <string>
#include <kvs/Timer>
#include <kvs/Module>
#include <kvs/Deprecated>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*==========================================================================*/
/**
 *  Renderer base class.
 */
/*==========================================================================*/
class RendererBase
{
    kvsModuleBase( kvs::RendererBase );

private:

    std::string m_name; ///< renderer name
    kvs::Timer m_timer; ///< timer
    mutable bool m_enable_shading; ///< shading flag

public:

    RendererBase(): m_name( "unknown" ), m_enable_shading( true ) {}
    virtual ~RendererBase() {}

    void setName( const std::string& name ) { m_name = name; }
    void setEnabledShading( const bool enable ) { m_enable_shading = enable; }
    const std::string& name() const { return m_name; }
    const kvs::Timer& timer() const { return m_timer; }
    bool isEnabledShading() const { return m_enable_shading; }
    void enableShading() const { m_enable_shading = true; }
    void disableShading() const { m_enable_shading = false; }
    virtual void exec( kvs::ObjectBase* object, kvs::Camera* camera = NULL, kvs::Light* light  = NULL ) = 0;

protected:

    void startTimer() { m_timer.start(); }
    void stopTimer() { m_timer.stop(); }

protected:

    //KVS_DEPRECATED( kvs::Timer& timer() ) { return m_timer; }
    //KVS_DEPRECATED( void initialize() ) { initialize_projection(); initialize_modelview(); }
    KVS_DEPRECATED( bool isShading() const ) { return this->isEnabledShading(); }
    KVS_DEPRECATED( virtual void initialize_projection() ) {}
    KVS_DEPRECATED( virtual void initialize_modelview() ) {}
};

} // end of namespace kvs

#endif // KVS__RENDERER_BASE_H_INCLUDE
