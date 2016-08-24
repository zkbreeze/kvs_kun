/*****************************************************************************/
/**
 *  @file   PixelTag.h
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
#ifndef KVS__KVSML__PIXEL_TAG_H_INCLUDE
#define KVS__KVSML__PIXEL_TAG_H_INCLUDE

#include <string>
#include <kvs/XMLNode>
#include <kvs/Vector3>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <Pixel>
 */
/*===========================================================================*/
class PixelTag : public kvs::kvsml::TagBase
{
public:

    typedef kvs::kvsml::TagBase BaseClass;

private:

    bool m_has_type; ///< flag to check whether 'type' is specified or not
    std::string m_type; ///< pixel type

public:

    PixelTag();

public:

    bool hasType() const;
    const std::string& type() const;

public:

    void setType( const std::string& type );

public:

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs

#endif // KVS__KVSML__PIXEL_TAG_H_INCLUDE
