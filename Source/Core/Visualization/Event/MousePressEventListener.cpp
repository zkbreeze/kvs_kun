/*****************************************************************************/
/**
 *  @file   MousePressEventListener.cpp
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
#include "MousePressEventListener.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new MousePressEventListener class.
 */
/*===========================================================================*/
MousePressEventListener::MousePressEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::MousePressEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the MousePressEventListener class.
 */
/*===========================================================================*/
MousePressEventListener::~MousePressEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void MousePressEventListener::onEvent( kvs::EventBase* event )
{
    this->update( static_cast<kvs::MouseEvent*>(event) );
}

} // end of namespace kvs
