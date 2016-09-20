#pragma once

#include <Valkyrie/Enums.hh>
#include <Valkyrie/Types.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.refl.hh>

/**
* @brief One element within the entire vertex declaration
*/
struct vkVertexElement
{
  vkVertexStreamType    StreamDefinition;
  vkDataType                  Type;
  vkSize                      Size;
  vkUInt32                    Offset;
  vkSize                      Stride;
  vkUInt8                    Stream;
  bool                        Valid;

  vkVertexElement()
    : Valid(false)
  {

  }

  vkVertexElement(vkVertexStreamType streamDefinition, vkDataType type, vkSize size, vkUInt32 offset, vkSize stride, vkUInt8 stream)
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
VK_INTERFACE();
struct VKE_API IVertexDeclaration : public IObject
{
  VK_CLASS_GEN;

  virtual unsigned GetNumberOfStreams() const = 0;
  virtual const vkVertexElement* GetElements(vkUInt8 stream) const = 0;
  virtual const vkVertexElement* GetElement(vkVertexStreamType streamDefinition, vkUInt8 stream) const = 0;
  virtual vkSize GetStride(vkUInt8 stream) const = 0;
  virtual vkSize GetTotalSize() const = 0;
};

