
#include <graphicsgl4/gl4vertexdeclaration.hh>
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4shader.hh>
#include <assert.h>
#include <map>


#define VOIDPTR(base, offset) (void*)(((long)base) + offset)

namespace
{


const vkVertexElement* find_element(const vkVertexElement* elems, vkUInt32 stream, vkUInt32 idx)
{
  unsigned v = 0;
  for (const vkVertexElement* ve = elems; ve->Valid; ve++)
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


void copy_element(vkVertexElement* dst, const vkVertexElement* src)
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

vkVertexDeclarationGL4::vkVertexDeclarationGL4()
  : m_streams(0)
  , m_elements(0)
  , m_totalSize(0)
{
  VK_CLASS_GEN_CONSTR;
}

bool vkVertexDeclarationGL4::Create(const vkVertexElement* elements)
{

  // evaluate the number of streams and the number of elements per stream
  std::map<unsigned, unsigned> str;
  std::map<unsigned, vkSize> strSize;
  for (const vkVertexElement* vptr = elements; vptr->Valid; vptr++)
  {
    (str[vptr->Stream])++;
    m_streams = m_streams < vptr->Stream ? vptr->Stream : m_streams;
    strSize[vptr->Stream] = vptr->Stride;
  }
  m_streams++;



  // now create elements grouped by the streams.
  m_elements = new vkVertexElement*[m_streams];
  for (unsigned i = 0; i<m_streams; i++)
  {
    unsigned numElems = str[i] + 1;
    // create the elements for this stream
    m_elements[i] = new vkVertexElement[numElems];

    for (unsigned j = 0; j<numElems; j++)
    {
      copy_element(&m_elements[i][j], find_element(elements, i, j));
    }
    m_totalSize += strSize[i];
  }

  return true;
}

vkVertexDeclarationGL4::~vkVertexDeclarationGL4()
{
  for (unsigned i = 0; i<m_streams; i++)
  {
    delete[] m_elements[i];
  }
  delete[] m_elements;
}


unsigned vkVertexDeclarationGL4::GetNumberOfStreams() const
{
  return m_streams;
}

const vkVertexElement* vkVertexDeclarationGL4::GetElements(vkUInt8 stream) const
{
  assert(stream < m_streams);

  return m_elements[stream];
}

const vkVertexElement* vkVertexDeclarationGL4::GetElement(vkVertexStreamType streamDefinition, vkUInt8 stream) const
{
  const vkVertexElement* elements = GetElements(stream);
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

void vkVertexDeclarationGL4::BindStream(vkProgramGL4* shader, vkUInt8 stream, void* ptr)
{
  assert(stream < m_streams);

  vkVertexElement* elements = m_elements[stream];

  if (shader)
  {
    while (elements && elements->Valid)
    {
      IShaderStream *s = shader->GetStream(elements->StreamDefinition);
      if (s)
      {
        s->Set(elements->Size,
               elements->Stride,
               VOIDPTR(ptr, elements->Offset),
               elements->Type);
        s->Enable();
      }
      elements++;
    }
  }
}

void vkVertexDeclarationGL4::UnbindStream(vkProgramGL4* shader, vkUInt8 stream)
{
  assert(stream < m_streams);

  vkVertexElement* elements = m_elements[stream];

  if (shader)
  {
    while (elements && elements->Valid)
    {
      IShaderStream* s = shader->GetStream(elements->StreamDefinition);
      if (s)
      {
        s->Disable();
      }
      elements++;
    }
  }
}

vkSize vkVertexDeclarationGL4::GetTotalSize() const
{
  return m_totalSize;
}


vkSize vkVertexDeclarationGL4::GetStride(vkUInt8 stream) const
{
  const vkVertexElement* elements = GetElements(stream);
  if (!elements)
  {
    return 0;
  }

  vkSize size = 0;
  while (elements && elements->Valid)
  {
    size = size > elements->Stride ? size : elements->Stride;
    elements++;
  }
  return size;

}
