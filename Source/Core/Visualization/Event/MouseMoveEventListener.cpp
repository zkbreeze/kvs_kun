/*****************************************************************************/
/**
 *  @file   MouseMoveEventListener.cpp
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
#include "MouseMoveEventListener.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new MouseMoveEventListener class.
 */
/*===========================================================================*/
MouseMoveEventListener::MouseMoveEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::MouseMoveEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the MouseMoveEventListener class.
 */
/*===========================================================================*/
MouseMoveEventListener::~MouseMoveEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void MouseMoveEventListener::onEvent( kvs::EventBase* event )
{
    this->update( static_cast<kvs::MouseEvent*>(event) );
}

} // end of namespace kvs
