/*****************************************************************************/
/**
 *  @file   ProgramObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ProgramObject.cpp 1149 2012-06-02 13:27:56Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "ProgramObject.h"
#include <kvs/DebugNew>
#include <kvs/Exception>
#include <kvs/OpenGL>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ProgramObject class.
 */
/*===========================================================================*/
ProgramObject::ProgramObject():
    m_id( 0 ),
    m_geom_input_type( 0 ),
    m_geom_output_type( 0 ),
    m_geom_output_vertices( 0 ),
    m_is_bound( false )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the ProgramObject class.
 */
/*===========================================================================*/
ProgramObject::~ProgramObject()
{
    this->release();
}

/*===========================================================================*/
/**
 *  @brief  Returns the program object ID.
 *  @return program object ID
 */
/*===========================================================================*/
GLuint ProgramObject::id() const
{
    return m_id;
}

/*===========================================================================*/
/**
 *  @brief  Returns the information log for a program object.
 *  @return information log
 */
/*===========================================================================*/
std::string ProgramObject::log() const
{
    GLint length = 0;
    glGetProgramiv( m_id, GL_INFO_LOG_LENGTH, &length );
    if ( length == 0 ) return "";

    char* buffer = new char [ length ];
    if ( !buffer )
    {
        kvsMessageError("Cannot allocate memory for the log.");
        return "";
    }

    GLsizei buffer_size = 0;
    glGetProgramInfoLog( m_id, length, &buffer_size, buffer );

    std::string log( buffer );
    delete [] buffer;

    return log;
}

/*===========================================================================*/
/**
 *  @brief  Creates a empty program object.
 */
/*===========================================================================*/
void ProgramObject::create()
{
    this->createID();
}

/*===========================================================================*/
/**
 *  @brief  Clears the program object.
 */
/*===========================================================================*/
void ProgramObject::release()
{
    this->deleteID();
}

/*===========================================================================*/
/**
 *  @brief  Attaches a shader object to a program object.
 *  @param  shader [in] shader object that is to be attached
 */
/*===========================================================================*/
void ProgramObject::attach( const kvs::ShaderObject& shader ) const
{
    KVS_GL_CALL( glAttachShader( m_id, shader.id() ) );
}

/*===========================================================================*/
/**
 *  @brief  Detaches the shader object from the program object.
 *  @param  shader [in] shader object
 */
/*===========================================================================*/
void ProgramObject::detach( const kvs::ShaderObject& shader ) const
{
    KVS_GL_CALL( glDetachShader( m_id, shader.id() ) );
}

/*===========================================================================*/
/**
 *  @brief  Links the program object.
 *  @return true if the program object is linked successfully
 */
/*===========================================================================*/
bool ProgramObject::link() const
{
    KVS_GL_CALL( glLinkProgram( m_id ) );
    return this->isLinked();
}

void ProgramObject::build( const kvs::ShaderSource& vert_src, const kvs::ShaderSource& frag_src )
{
    kvs::VertexShader vert( vert_src );
    if ( !vert.compile() )
    {
        GLenum error = glGetError();
        kvsMessageError( "VertexShader compile failed: %s(%d)\n", gluErrorString(error), error );
        std::cout << "error log:" << std::endl;
        std::cout << vert.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "VertexShader compile failed" );
    }

    kvs::FragmentShader frag( frag_src );
    if ( !frag.compile() )
    {
        GLenum error = glGetError();
        kvsMessageError( "FragmentShader compile failed: %s(%d)\n", gluErrorString(error), error );
        std::cout << "error log:" << std::endl;
        std::cout << frag.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "FragmentShader compile failed" );
    }

    this->create();
    this->attach( vert );
    this->attach( frag );
    if ( !this->link() )
    {
        GLenum error = glGetError();
        kvsMessageError( "ProgramObject link failed: %s(%d)\n", gluErrorString(error), error );
        std::cout << "error log:" << std::endl;
        std::cout << this->log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "ProgramObject link failed" );
    }
}

void ProgramObject::build( const kvs::ShaderSource& vert_src, const kvs::ShaderSource& geom_src, const kvs::ShaderSource& frag_src )
{
    kvs::VertexShader vert( vert_src );
    if ( !vert.compile() )
    {
        GLenum error = glGetError();
        kvsMessageError( "VertexShader compile failed: %s(%d)\n", gluErrorString(error), error );
        std::cout << "error log:" << std::endl;
        std::cout << vert.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "VertexShader compile failed" );
    }

    kvs::GeometryShader geom( geom_src );
    if ( !geom.compile() )
    {
        GLenum error = glGetError();
        kvsMessageError( "GeometryShader compile failed: %s(%d)\n", gluErrorString(error), error );
        std::cout << "error log:" << std::endl;
        std::cout << geom.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "GeometryShader compile failed" );
    }

    kvs::FragmentShader frag( frag_src );
    if ( !frag.compile() )
    {
        GLenum error = glGetError();
        kvsMessageError( "FragmentShader compile failed: %s(%d)\n", gluErrorString(error), error );
        std::cout << "error log:" << std::endl;
        std::cout << frag.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "FragmentShader compile failed" );
    }

    this->create();
    this->attach( vert );
    this->attach( geom );
    this->attach( frag );

    // Set parameters for the geometry shader.
    this->setParameter( GL_GEOMETRY_INPUT_TYPE_EXT, m_geom_input_type );
    this->setParameter( GL_GEOMETRY_OUTPUT_TYPE_EXT, m_geom_output_type );
    const GLint max_output_vertices = kvs::OpenGL::Integer( GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT );
    if ( max_output_vertices < m_geom_output_vertices )
    {
        kvsMessageError( "GeometryShader GL_GEOMETRY_VERTICES_OUT require = %d > max = %d", m_geom_output_vertices, max_output_vertices );
        KVS_THROW( kvs::OpenGLException, "GeometryShader cannot be attached" );
    }
    this->setParameter( GL_GEOMETRY_VERTICES_OUT_EXT, m_geom_output_vertices );

    if ( !this->link() )
    {
        GLenum error = glGetError();
        kvsMessageError( "ProgramObject link failed: %s(%d)\n", gluErrorString(error), error );
        std::cout << "error log:" << std::endl;
        std::cout << this->log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "ProgramObject link failed" );
    }
}

/*===========================================================================*/
/**
 *  @brief  Installs the program object as part of current rendering state.
 */
/*===========================================================================*/
void ProgramObject::bind() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glUseProgram( m_id ) );
    m_is_bound = true;
}

/*===========================================================================*/
/**
 *  @brief  Uninstalls the program object as part of current rendering state.
 */
/*===========================================================================*/
void ProgramObject::unbind() const
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glUseProgram( 0 ) );
    m_is_bound = false;
}

bool ProgramObject::isCreated() const
{
    return m_id > 0;
}

bool ProgramObject::isValid() const
{
    GLboolean result;
    KVS_GL_CALL( result = glIsProgram( m_id ) );
    return result == GL_TRUE;
}

bool ProgramObject::isBound() const
{
    return m_is_bound;
}

bool ProgramObject::isLinked() const
{
    GLint error = 0;
    KVS_GL_CALL( glGetProgramiv( m_id, GL_LINK_STATUS, &error ) );
    return error != GL_FALSE;
}

/*===========================================================================*/
/**
 *  @brief  Returns the location of a uniform variable.
 *  @param  name [in] name of the uniform variable
 *  @return location of a specific uniform variable
 */
/*===========================================================================*/
GLint ProgramObject::uniformLocation( const GLchar *name )
{
    GLint result = 0;
    KVS_GL_CALL( result = glGetUniformLocation( m_id, name ) );
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Returns the location of a attribute variable.
 *  @param  name [in] name of the attribute variable
 *  @return location of a specific attribute variable
 */
/*===========================================================================*/
GLint ProgramObject::attributeLocation( const GLchar *name )
{
//    return glGetAttribLocation( m_id, name );
    GLint result = 0;
    KVS_GL_CALL( result = glGetAttribLocation( m_id, name ) );
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const GLint value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform1i( location, value ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 2D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector2i& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform2iv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 3D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector3i& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform3iv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 4D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector4i& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform4iv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const GLfloat value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform1f( location, value ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 2D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector2f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform2fv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 3D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector3f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform3fv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 4D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector4f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform4fv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 2x2 matrix
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Matrix22f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniformMatrix2fv( location, 1, GL_TRUE, &value[0][0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 3x3 matrix
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Matrix33f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniformMatrix3fv( location, 1, GL_TRUE, &value[0][0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 4x4 matrix
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Matrix44f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniformMatrix4fv( location, 1, GL_TRUE, &value[0][0] ) );
}

void ProgramObject::setGeometryInputType( const GLint type )
{
    m_geom_input_type = type;
}

void ProgramObject::setGeometryOutputType( const GLint type )
{
    m_geom_output_type = type;
}

void ProgramObject::setGeometryOutputVertices( const GLint value )
{
    m_geom_output_vertices = value;
}

void ProgramObject::createID()
{
    if ( !this->isValid() )
    {
        KVS_GL_CALL( m_id = glCreateProgram() );
    }
}

void ProgramObject::deleteID()
{
    if ( this->isValid() )
    {
        KVS_GL_CALL( glDeleteProgram( m_id ) );
    }
    m_id = 0;
}

void ProgramObject::setParameter( GLenum pname, GLint value )
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glProgramParameteriEXT( this->id(), pname, value ) );
}

ProgramObject::Binder::Binder( const ProgramObject& po ) :
    m_po( po )
{
    KVS_ASSERT( po.isCreated() );
    po.bind();
}

ProgramObject::Binder::~Binder()
{
    KVS_ASSERT( m_po.isCreated() );
    KVS_GL_CALL( glUseProgram( 0 ) );
}



/*===========================================================================*/
/**
 *  @brief  Attaches a vertex shader and a fragment shader and links the program object.
 *  @param  vertex_shader [in] vertex shader that is to be attached
 *  @param  fragment_shader [in] fragment shader that is to be attached
 *  @return <ReturnValue>
 */
/*===========================================================================*/
bool ProgramObject::link(
    const kvs::VertexShader &vertex_shader,
    const kvs::FragmentShader &fragment_shader )
{
    this->create();
    this->attach( fragment_shader );
    this->attach( vertex_shader );
    return this->link();
}

void ProgramObject::create(
    const kvs::ShaderSource& vertex_source,
    const kvs::ShaderSource& fragment_source )
{
    // Vertex shader.
    kvs::VertexShader vertex_shader( vertex_source );
    if ( !vertex_shader.compile() )
    {
        GLenum error = glGetError();
        kvsMessageError( "VertexShader compile failed: %s(%d)\n", gluErrorString(error), error );
        std::cout << "error log:" << std::endl;
        std::cout << vertex_shader.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "VertexShader compile failed" );
    }

    // Fragment shader.
    kvs::FragmentShader fragment_shader( fragment_source );
    if ( !fragment_shader.compile() )
    {
        GLenum error = glGetError();
        kvsMessageError( "FragmentShader compile failed: %s(%d)\n", gluErrorString(error), error );
        std::cout << "error log:" << std::endl;
        std::cout << fragment_shader.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "FragmentShader compile failed" );
    }

    // Link the shaders.
    if ( !this->link( vertex_shader, fragment_shader ) )
    {
        GLenum error = glGetError();
        kvsMessageError( "ShaderProgram link failed: %s(%d)\n", gluErrorString(error), error );
        std::cout << "error log:" << std::endl;
        std::cout << this->log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "ShaderProgram link failed" );
    }
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const GLint v0 )
{
    glUniform1i( this->uniformLocation( name ), v0 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const GLint v0,
    const GLint v1 )
{
    glUniform2i( this->uniformLocation( name ), v0, v1 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 *  @param  v2 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const GLint v0,
    const GLint v1,
    const GLint v2 )
{
    glUniform3i( this->uniformLocation( name ), v0, v1, v2 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 *  @param  v2 [in] value for the uniform variable
 *  @param  v3 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const GLint v0,
    const GLint v1,
    const GLint v2,
    const GLint v3 )
{
    glUniform4i( this->uniformLocation( name ), v0, v1, v2, v3 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (2 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const kvs::Vector2i& v )
{
    const GLint v0 = static_cast<GLint>( v.x() );
    const GLint v1 = static_cast<GLint>( v.y() );
    glUniform2i( this->uniformLocation( name ), v0, v1 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (3 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const kvs::Vector3i& v )
{
    const GLint v0 = static_cast<GLint>( v.x() );
    const GLint v1 = static_cast<GLint>( v.y() );
    const GLint v2 = static_cast<GLint>( v.z() );
    glUniform3i( this->uniformLocation( name ), v0, v1, v2 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (4 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const kvs::Vector4i& v )
{
    const GLint v0 = static_cast<GLint>( v.x() );
    const GLint v1 = static_cast<GLint>( v.y() );
    const GLint v2 = static_cast<GLint>( v.z() );
    const GLint v3 = static_cast<GLint>( v.w() );
    glUniform4i( this->uniformLocation( name ), v0, v1, v2, v3 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const GLfloat v0 )
{
    glUniform1f( this->uniformLocation( name ), v0 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const GLfloat v0,
    const GLfloat v1 )
{
    glUniform2f( this->uniformLocation( name ), v0, v1 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 *  @param  v2 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const GLfloat v0,
    const GLfloat v1,
    const GLfloat v2 )
{
    glUniform3f( this->uniformLocation( name ), v0, v1, v2 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 *  @param  v2 [in] value for the uniform variable
 *  @param  v3 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const GLfloat v0,
    const GLfloat v1,
    const GLfloat v2,
    const GLfloat v3 )
{
    glUniform4f( this->uniformLocation( name ), v0, v1, v2, v3 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (2 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const kvs::Vector2f& v )
{
    const GLfloat v0 = static_cast<GLfloat>( v.x() );
    const GLfloat v1 = static_cast<GLfloat>( v.y() );
    glUniform2f( this->uniformLocation( name ), v0, v1 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (3 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const kvs::Vector3f& v )
{
    const GLfloat v0 = static_cast<GLfloat>( v.x() );
    const GLfloat v1 = static_cast<GLfloat>( v.y() );
    const GLfloat v2 = static_cast<GLfloat>( v.z() );
    glUniform3f( this->uniformLocation( name ), v0, v1, v2 );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (4 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const kvs::Vector4f& v )
{
    const GLfloat v0 = static_cast<GLfloat>( v.x() );
    const GLfloat v1 = static_cast<GLfloat>( v.y() );
    const GLfloat v2 = static_cast<GLfloat>( v.z() );
    const GLfloat v3 = static_cast<GLfloat>( v.w() );
    glUniform4f( this->uniformLocation( name ), v0, v1, v2, v3 );
}

} // end of namespace kvs
