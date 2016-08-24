/*****************************************************************************/
/**
 *  @file   DisplayList.cpp
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
#if KVS_ENABLE_DEPRECATED

#include "DisplayList.h"
#include <kvs/OpenGL>
#include <kvs/Message>


namespace kvs
{

/*===========================================================================*/
/**
 *  Constructs a new DisplayList.
 */
/*===========================================================================*/
DisplayList::DisplayList():
    m_id( 0 ),
    m_range( 0 )
{
}

/*===========================================================================*/
/**
 *  Constructs a new DisplayList.
 *  @param  range [in] number of lists
 */
/*===========================================================================*/
DisplayList::DisplayList( const GLsizei range )
{
    this->create( range );
}

/*===========================================================================*/
/**
 *  Destructs the DisplayList.
 */
/*===========================================================================*/
DisplayList::~DisplayList()
{
    this->clear();
}

/*===========================================================================*/
/**
 *  Returns the list ID.
 *  @return list ID
 */
/*===========================================================================*/
GLuint DisplayList::id() const
{
    return m_id;
}

/*===========================================================================*/
/**
 *  Returns the range of list (number of lists).
 *  @return range of list
 */
/*===========================================================================*/
GLsizei DisplayList::range() const
{
    return m_range;
}

/*===========================================================================*/
/**
 *  Begins the registration of the display list.
 *  @param  index [in] index of the list (0 <= index < range)
 *  @param  mode [in] command mode (GL_COMPILE or GL_COMPILE_AND_EXECUTE)
 */
/*===========================================================================*/
void DisplayList::begin( const GLuint index, const GLenum mode )
{
    if ( GLsizei( index ) >= m_range )
    {
        kvsMessageError("Given display-list index is out of range.");
        return;
    }

    glNewList( m_id + index, mode );
}

/*===========================================================================*/
/**
 *  Ends the registrantion of the display list.
 */
/*===========================================================================*/
void DisplayList::end()
{
    glEndList();
}

/*===========================================================================*/
/**
 *  Creates a new display list.
 *  @param  range [in] range of list (number of lists)
 *  @return range of list
 */
/*===========================================================================*/
bool DisplayList::create( const GLsizei range )
{
    m_id = glGenLists( range );
    if ( m_id == 0 ) return false;

    m_range = range;

    return true;
}

/*===========================================================================*/
/**
 *  Clears the display list.
 */
/*===========================================================================*/
void DisplayList::clear()
{
    if ( glIsList( m_id ) == GL_TRUE )
    {
        glDeleteLists( m_id, m_range );
    }
}

/*===========================================================================*/
/**
 *  Render the display list.
 *  @param  index [in] index of the list (0 <= index < range)
 */
/*===========================================================================*/
void DisplayList::render( const GLuint index )
{
    if ( GLsizei( index ) >= m_range )
    {
        kvsMessageError("Given display-list index is out of range.");
        return;
    }

    glCallList( m_id + index );
}

} // end of namespace kvs

#endif
