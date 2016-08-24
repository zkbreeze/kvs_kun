/*****************************************************************************/
/**
 *  @file   ExporterBase.h
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
#ifndef KVS__EXPORTER_BASE_H_INCLUDE
#define KVS__EXPORTER_BASE_H_INCLUDE

#include <kvs/FileFormatBase>
#include <kvs/Module>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Exporter base class.
 */
/*===========================================================================*/
class ExporterBase
{
    kvsModuleBase( kvs::ExporterBase );

private:

    bool m_is_success; ///< check flag for exporting

public:

    ExporterBase() {}
    virtual ~ExporterBase() {}

    bool isSuccess() const { return m_is_success; }
    bool isFailure() const { return !m_is_success; }
    virtual kvs::FileFormatBase* exec( const kvs::ObjectBase* object ) = 0;

protected:

    void setSuccess( const bool success ) { m_is_success = success; }
};

} // end of namespace kvs

#endif // KVS__EXPORTER_BASE_H_INCLUDE
