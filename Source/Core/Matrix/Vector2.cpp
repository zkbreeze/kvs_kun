/****************************************************************************/
/**
 *  @file Vector2.cpp
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
#include "Vector2.h"
#include <kvs/Type>


namespace kvs
{

// Template instantiation.
template class Vector2<kvs::Int8>;
template class Vector2<kvs::UInt8>;
template class Vector2<kvs::Int16>;
template class Vector2<kvs::UInt16>;
template class Vector2<kvs::Int32>;
template class Vector2<kvs::UInt32>;
template class Vector2<kvs::Int64>;
template class Vector2<kvs::UInt64>;
template class Vector2<kvs::Real32>;
template class Vector2<kvs::Real64>;

} // end of namespace kvs
