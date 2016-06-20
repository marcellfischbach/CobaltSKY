#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Graphics/Particle.refl.hh>

struct IGraphics;
struct IVertexBuffer;
struct IVertexDeclaration;

class vkMaterialInstance;

VK_CLASS()
class VKE_API vkParticle : public vkObject
{
  VK_CLASS_GEN;
public:
  vkParticle();
  virtual ~vkParticle();

  bool Initialize(IGraphics *renderer, vkSize numberOfParticles);

  void SetPosition(IVertexBuffer *vertexBuffer);
  IVertexBuffer *GetPositions();
  const IVertexBuffer *GetPositions() const;

  void SetSizes(IVertexBuffer *vertexBuffer);
  IVertexBuffer *GetSizes();
  const IVertexBuffer *GetSizes() const;

  void SetRotations(IVertexBuffer *vertexBuffer);
  IVertexBuffer *GetRotations();
  const IVertexBuffer *GetRotations() const;

  void SetTexCoords(IVertexBuffer *vertexBuffer);
  IVertexBuffer *GetTexCoords();
  const IVertexBuffer *GetTexCoords() const;

  void SetNumberOfParticles(vkSize numParticles);
  vkSize GetNumberOfParticles() const;

  void SetNumberOfRenderParticles(vkSize numParticles);
  vkSize GetNumberOfRenderParticles() const;


  void Render(IGraphics *renderer, vkRenderPass pass, vkMaterialInstance *material);

private:
  IVertexDeclaration *m_vertexDeclaration;
  IVertexBuffer *m_positions;
  IVertexBuffer *m_sizes;
  IVertexBuffer *m_rotations;
  IVertexBuffer *m_texCoords;

  vkSize m_numParticles;
  vkSize m_numRenderParticles;
};