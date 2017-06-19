#pragma once

#include <graphicsgl4/gl4export.hh>
#include <valkyrie/graphics/ivertexdeclaration.hh>
#include <graphicsgl4/gl4vertexdeclaration.refl.hh>


class csProgramGL4;
CS_CLASS()
class csVertexDeclarationGL4 : public CS_SUPER(iVertexDeclaration)
{
  CS_CLASS_GEN_OBJECT;
public:
  csVertexDeclarationGL4();
  virtual ~csVertexDeclarationGL4();

  bool Create(const csVertexElement* elements);

  unsigned GetNumberOfStreams() const;
  const csVertexElement* GetElements(csUInt8 stream) const;
  const csVertexElement* GetElement(csVertexStreamType streamDefinition, csUInt8 stream) const;
  void BindStream(csProgramGL4* program, csUInt8 stream, void* ptr);
  void UnbindStream(csProgramGL4* program, csUInt8 stream);
  virtual csSize GetTotalSize() const;
  virtual csSize GetStride(csUInt8 stream) const;

private:
  csUInt32					m_streams;
  csVertexElement** m_elements;
  csSize m_totalSize;
};
