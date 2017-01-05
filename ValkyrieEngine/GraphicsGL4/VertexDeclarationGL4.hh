#pragma once

#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Graphics/ivertexdeclaration.hh>
#include <GraphicsGL4/VertexDeclarationGL4.refl.hh>


class vkProgramGL4;
VK_CLASS();
class vkVertexDeclarationGL4 : public VK_SUPER(IVertexDeclaration)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkVertexDeclarationGL4();
  virtual ~vkVertexDeclarationGL4();

  bool Create(const vkVertexElement* elements);

  unsigned GetNumberOfStreams() const;
  const vkVertexElement* GetElements(vkUInt8 stream) const;
  const vkVertexElement* GetElement(vkVertexStreamType streamDefinition, vkUInt8 stream) const;
  void BindStream(vkProgramGL4* program, vkUInt8 stream, void* ptr);
  void UnbindStream(vkProgramGL4* program, vkUInt8 stream);
  virtual vkSize GetTotalSize() const;
  virtual vkSize GetStride(vkUInt8 stream) const;

private:
  vkUInt32					m_streams;
  vkVertexElement** m_elements;
  vkSize m_totalSize;
};
