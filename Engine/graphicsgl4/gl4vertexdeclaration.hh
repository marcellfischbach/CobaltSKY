#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <graphicsgl4/gl4vertexdeclaration.refl.hh>


namespace cs
{

class ProgramGL4;



CS_CLASS()
class VertexDeclarationGL4 : public CS_SUPER(cs::iVertexDeclaration)
{
  CS_CLASS_GEN_OBJECT;
public:
  VertexDeclarationGL4();
  virtual ~VertexDeclarationGL4();

  bool Create(const cs::VertexElement * elements);

  unsigned GetNumberOfStreams() const;
  const cs::VertexElement* GetElements(csUInt8 stream) const;
  const cs::VertexElement* GetElement(cs::eVertexStreamType streamDefinition, csUInt8 stream) const;
  void BindStream(cs::ProgramGL4 * program, csUInt8 stream, void* ptr);
  void UnbindStream(cs::ProgramGL4 * program, csUInt8 stream);
  virtual csSize GetTotalSize() const;
  virtual csSize GetStride(csUInt8 stream) const;

private:
  csUInt32					m_streams;
  cs::VertexElement** m_elements;
  csSize m_totalSize;
};

}
