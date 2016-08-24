/****************************************************************************/
/**
 *  @file Xorshift128.cpp
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
#include "Xorshift128.h"
#include <ctime>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new Xorshift128.
 */
/*==========================================================================*/
Xorshift128::Xorshift128()
{
    const kvs::UInt32 seed = static_cast<kvs::UInt32>( time( 0 ) );
    this->setSeed( seed );
}

/*==========================================================================*/
/**
 *  @brief  Sets a seed value.
 *  @param  seed [in] seed value
 */
/*==========================================================================*/
void Xorshift128::setSeed( kvs::UInt32 seed )
{
    m_x = seed = 1812433253UL * ( seed ^ ( seed >> 30 ) ) + 1;
    m_y = seed = 1812433253UL * ( seed ^ ( seed >> 30 ) ) + 2;
    m_z = seed = 1812433253UL * ( seed ^ ( seed >> 30 ) ) + 3;
    m_w = seed = 1812433253UL * ( seed ^ ( seed >> 30 ) ) + 4;
}

} // end of namespace kvs
