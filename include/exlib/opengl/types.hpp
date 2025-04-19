#pragma once

#include <GL/glew.h>

#include "exlib/core/exception.hpp"

namespace ex::gl {

enum class BufferUsage : GLenum {
    Static = GL_STATIC_DRAW,
    Dynamic = GL_DYNAMIC_DRAW,
    Stream = GL_STREAM_DRAW
};

enum class BufferAccess : GLenum {
    Read = GL_READ_ONLY,
    Write = GL_WRITE_ONLY,
    ReadWrite = GL_READ_WRITE
};

enum class PrimitiveType : GLenum {
    Points = GL_POINTS,
    LineStrip = GL_LINE_STRIP,
    LineLoop = GL_LINE_LOOP,
    Lines = GL_LINES,
    LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
    LinesAdjacency = GL_LINES_ADJACENCY,
    TriangleStrip = GL_TRIANGLE_STRIP,
    TriangleFan = GL_TRIANGLE_FAN,
    Triangles = GL_TRIANGLES,
    TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
    TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
    Patches = GL_PATCHES
};

enum class Type : GLenum {
    Byte = GL_BYTE,
    UnsignedByte = GL_UNSIGNED_BYTE,
    Short = GL_SHORT,
    UnsignedShort = GL_UNSIGNED_SHORT,
    Int = GL_INT,
    UnsignedInt = GL_UNSIGNED_INT,
    Float = GL_FLOAT,
    Double = GL_DOUBLE,
    HalfFloat = GL_HALF_FLOAT,
    Fixed = GL_FIXED,
    Int_2_10_10_10_REV = GL_INT_2_10_10_10_REV,
    UnsignedInt_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
    UnsignedInt_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV
};

inline GLuint get_size_of_type(Type type) {
    switch (type) {
    case Type::Byte:                        return sizeof(GLbyte);
    case Type::UnsignedByte:                return sizeof(GLubyte);
    case Type::Short:                       return sizeof(GLshort);
    case Type::UnsignedShort:               return sizeof(GLushort);
    case Type::Int:                         return sizeof(GLint);
    case Type::UnsignedInt:                 return sizeof(GLuint);
    case Type::Float:                       return sizeof(GLfloat);
    case Type::Double:                      return sizeof(GLdouble);
    case Type::HalfFloat:                   return 2;
    case Type::Fixed:                       return 4;
    case Type::Int_2_10_10_10_REV:          return 4;
    case Type::UnsignedInt_2_10_10_10_REV:  return 4;
    case Type::UnsignedInt_10F_11F_11F_REV: return 4;
    default:                                EX_THROW("Invalid type: " + std::to_string((GLenum) type));
    }
}

}