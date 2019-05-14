#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/cstypes.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/graphics/ivertexdeclaration.refl.hh>

/**
* @brief One element within the entire vertex declaration
*/
struct csVertexElement
{
  csVertexStreamType    StreamDefinition;
  csDataType                  Type;
  csSize                      Size;
  csUInt32                    Offset;
  csSize                      Stride;
  csUInt8                    Stream;
  bool                        Valid;

  csVertexElement()
    : Valid(false)
  {

  }

  csVertexElement(csVertexStreamType streamDefinition, csDataType type, csSize size, csUInt32 offset, csSize stride, csUInt8 stream)
    : StreamDefinition(streamDefinition)
    , Type(type)
    , Size(size)
    , Offset(offset)
    , Stride(stride)
    , Stream(stream)
    , Valid(true)
  {

  }

};

/**
* @brief Interface for controlling how the vertex data should be passed into the shading system.
*/
CS_CLASS();
struct CSE_API iVertexDeclaration : public cs::iObject
{
  CS_CLASS_GEN;

  virtual unsigned GetNumberOfStreams() const = 0;
  virtual const csVertexElement* GetElements(csUInt8 stream) const = 0;
  virtual const csVertexElement* GetElement(csVertexStreamType streamDefinition, csUInt8 stream) const = 0;
  virtual csSize GetStride(csUInt8 stream) const = 0;
  virtual csSize GetTotalSize() const = 0;
};

