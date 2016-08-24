/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for KVS_BREAKPOINT macro.
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
#include <iostream>
#include <kvs/Breakpoint>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    /* KVS_BREAKPOINT is a signal that tells the debugger to temporary suspend
     * execusion of the program here.
     */
    KVS_BREAKPOINT;

    return 0;
}
