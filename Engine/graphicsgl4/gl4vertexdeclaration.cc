
#include <graphicsgl4/gl4vertexdeclaration.hh>
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4defines.hh>
#include <assert.h>
#include <map>


#define VOIDPTR(base, offset) reinterpret_cast<const void*>((long long)base + static_cast<long>(offset))

namespace
{


const cs::VertexElement* find_element(const cs::VertexElement* elems, csUInt32 stream, csUInt32 idx)
{
  unsigned v = 0;
  for (const cs::VertexElement* ve = elems; ve->Valid; ve++)
  {
    if (ve->Stream != stream)
    {
      continue;
    }
    if (v == idx)
    {
      return ve;
    }
    v++;
  }
  return 0;
}


void copy_element(cs::VertexElement* dst, const cs::VertexElement* src)
{
  assert(dst);
  if (src)
  {
    dst->StreamDefinition= src->StreamDefinition;
    dst->Type = src->Type;
    dst->Offset = src->Offset;
    dst->Size = src->Size;
    dst->Stream = src->Stream;
    dst->Stride = src->Stride;
    dst->Valid = src->Valid;
  }
}
}

cs::VertexDeclarationGL4::VertexDeclarationGL4()
  : m_streams(0)
  , m_elements(0)
  , m_totalSize(0)
{
  CS_CLASS_GEN_CONSTR;
}

bool cs::VertexDeclarationGL4::Create(const cs::VertexElement* elements)
{

  // evaluate the number of streams and the number of elements per stream
  std::map<unsigned, unsigned> str;
  std::map<unsigned, csSize> strSize;
  for (const cs::VertexElement* vptr = elements; vptr->Valid; vptr++)
  {
    (str[vptr->Stream])++;
    m_streams = m_streams < vptr->Stream ? vptr->Stream : m_streams;
    strSize[vptr->Stream] = vptr->Stride;
  }
  m_streams++;



  // now create elements grouped by the streams.
  m_elements = new cs::VertexElement*[m_streams];
  for (unsigned i = 0; i<m_streams; i++)
  {
    unsigned numElems = str[i] + 1;
    // create the elements for this stream
    m_elements[i] = new cs::VertexElement[numElems];

    for (unsigned j = 0; j<numElems; j++)
    {
      copy_element(&m_elements[i][j], find_element(elements, i, j));
    }
    m_totalSize += strSize[i];
  }

  return true;
}

cs::VertexDeclarationGL4::~VertexDeclarationGL4()
{
  for (unsigned i = 0; i<m_streams; i++)
  {
    delete[] m_elements[i];
  }
  delete[] m_elements;
}


unsigned cs::VertexDeclarationGL4::GetNumberOfStreams() const
{
  return m_streams;
}

const cs::VertexElement* cs::VertexDeclarationGL4::GetElements(csUInt8 stream) const
{
  assert(stream < m_streams);

  return m_elements[stream];
}

const cs::VertexElement* cs::VertexDeclarationGL4::GetElement(cs::eVertexStreamType streamDefinition, csUInt8 stream) const
{
  const cs::VertexElement* elements = GetElements(stream);
  if (!elements)
  {
    return 0;
  }
  while (elements && elements->Valid)
  {
    if (elements->StreamDefinition== streamDefinition)
    {
      return elements;
    }
    elements++;
  }
  return 0;
}

void cs::VertexDeclarationGL4::BindStream(cs::ProgramGL4* shader, csUInt8 stream, void* ptr)
{
  assert(stream < m_streams);

  cs::VertexElement* elements = m_elements[stream];

  if (shader)
  {
    while (elements && elements->Valid)
    {
      cs::iShaderStream *s = shader->GetStream(elements->StreamDefinition);
      if (s)
      {
        if (ptr)
        {
          s->Set(elements->Size,
                 elements->Stride,
                 VOIDPTR(ptr, elements->Offset),
                 elements->Type);
        }
        else
        {
          s->Set(elements->Size,
                 elements->Stride,
                 elements->Offset,
                 elements->Type);
        }
        s->Enable();
      }
      elements++;
    }
  }
}

void cs::VertexDeclarationGL4::UnbindStream(cs::ProgramGL4* shader, csUInt8 stream)
{
  assert(stream < m_streams);

  cs::VertexElement* elements = m_elements[stream];

  if (shader)
  {
    while (elements && elements->Valid)
    {
      cs::iShaderStream* s = shader->GetStream(elements->StreamDefinition);
      if (s)
      {
        s->Disable();
      }
      elements++;
    }
  }
}

csSize cs::VertexDeclarationGL4::GetTotalSize() const
{
  return m_totalSize;
}


csSize cs::VertexDeclarationGL4::GetStride(csUInt8 stream) const
{
  const cs::VertexElement* elements = GetElements(stream);
  if (!elements)
  {
    return 0;
  }

  csSize size = 0;
  while (elements && elements->Valid)
  {
    size = size > elements->Stride ? size : elements->Stride;
    elements++;
  }
  return size;

}
