/*****************************************************************************/
/**
 *  @file   PolygonObjectTag.h
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
#ifndef KVS__KVSML__POLYGON_OBJECT_TAG_H_INCLUDE
#define KVS__KVSML__POLYGON_OBJECT_TAG_H_INCLUDE

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
 *  @brief  Tag class for <PolygonObject>
 */
/*===========================================================================*/
class PolygonObjectTag : public kvs::kvsml::TagBase
{
public:

    typedef kvs::kvsml::TagBase BaseClass;

private:

    bool m_has_polygon_type; ///< flag to check whether 'polygon_type' is specified or not
    std::string m_polygon_type; ///< polygon type
    bool m_has_color_type; ///< flag to check whether 'color_type' is specified or not
    std::string m_color_type; ///< color type
    bool m_has_normal_type; ///< flag to check whether 'normal_type' is specified or not
    std::string m_normal_type; ///< normal type

public:

    PolygonObjectTag();

public:

    bool hasPolygonType() const;
    const std::string& polygonType() const;
    bool hasColorType() const;
    const std::string& colorType() const;
    bool hasNormalType() const;
    const std::string& normalType() const;

public:

    void setPolygonType( const std::string& line_type );
    void setColorType( const std::string& color_type );
    void setNormalType( const std::string& normal_type );

public:

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs

#endif // KVS__KVSML__POLYGON_OBJECT_TAG_H_INCLUDE
