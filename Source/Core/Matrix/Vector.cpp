/****************************************************************************/
/**
 *  @file Vector.cpp
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
#include "Vector.h"
#include <kvs/Type>

namespace kvs
{

// Template instantiation.
template class Vector<kvs::Int8>;
template class Vector<kvs::UInt8>;
template class Vector<kvs::Int16>;
template class Vector<kvs::UInt16>;
template class Vector<kvs::Int32>;
template class Vector<kvs::UInt32>;
template class Vector<kvs::Int64>;
template class Vector<kvs::UInt64>;
template class Vector<kvs::Real32>;
template class Vector<kvs::Real64>;

} // end of namespace kvs
