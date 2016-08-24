/*****************************************************************************/
/**
 *  @file   VertexTag.h
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
#ifndef KVS__KVSML__VERTEX_TAG_H_INCLUDE
#define KVS__KVSML__VERTEX_TAG_H_INCLUDE

#include <kvs/XMLNode>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <Vertex>
 */
/*===========================================================================*/
class VertexTag : public kvs::kvsml::TagBase
{
public:

    typedef kvs::kvsml::TagBase BaseClass;

private:

    bool m_has_nvertices; ///< flag to check whether 'nvertices' is specified or not
    size_t m_nvertices; ///< number of vertices

public:

    VertexTag();

public:

    bool hasNVertices() const;
    size_t nvertices() const;

public:

    void setNVertices( const size_t nvertices );

public:

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs

#endif // KVS__KVSML__VERTEX_TAG_H_INCLUDE
