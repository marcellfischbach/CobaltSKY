
#include <Valkyrie/entity/vkstaticmeshstate.hh>
#include <Valkyrie/entity/vkentity.hh>
#include <Valkyrie/entity/ientityscan.hh>
#include <Valkyrie/entity/vkentityscene.hh>
#include <Valkyrie/animation/vkskeleton.hh>
#include <Valkyrie/Graphics/igraphics.hh>
#include <Valkyrie/Graphics/vkmaterialinstance.hh>
#include <Valkyrie/Graphics/vkmesh.hh>
#include <Valkyrie/Physics/IPhysicsScene.hh>
#include <Valkyrie/vkengine.hh>

vkStaticMeshState::vkStaticMeshState()
  : vkRenderState()
  , m_mesh(0)
  , m_materials(0)
  , m_numberOfMaterialSlots(0)
  , m_castShadow(true)
  , m_staticCollider(0)
  , m_shapes(0)
  , m_friction(0.5)
  , m_restitution(0.5)
{

}

vkStaticMeshState::~vkStaticMeshState()
{
  VK_RELEASE(m_mesh);
  m_mesh = 0;

  for (vkUInt32 i = 0; i < m_numberOfMaterialSlots; ++i)
  {
    VK_RELEASE(m_materials[i]);
    m_materials[i] = 0;
  }

  delete[] m_materials;
  m_materials = 0;

  m_numberOfMaterialSlots = 0;
}


void vkStaticMeshState::SetMesh(vkMesh *mesh)
{
  VK_SET(m_mesh, mesh);

  UpdateMaterialSlots();

  UpdateBoundingBox();
}

void vkStaticMeshState::SetMaterial(vkMaterialInstance *material, vkSize slot)
{
  if (slot >= m_numberOfMaterialSlots)
  {
    return;
  }

  VK_SET(m_materials[slot], material);
}


void vkStaticMeshState::UpdateMaterialSlots()
{
  vkSize numberOfSlots = m_mesh->GetNumberOfMaterials();
  if (numberOfSlots == m_numberOfMaterialSlots)
  {
    return;
  }

  // create new material slots
  vkMaterialInstance **materials = new vkMaterialInstance*[numberOfSlots];
  memset(materials, 0, sizeof(vkMaterialInstance*) * numberOfSlots);

  // copy all slots that remain from the old to the new
  for (vkUInt32 i = 0; i < numberOfSlots && i < m_numberOfMaterialSlots; ++i)
  {
    VK_SET(materials[i], m_materials[i]);
  }

  for (vkUInt32 i = 0; i < m_numberOfMaterialSlots; ++i)
  {
    VK_RELEASE(m_materials[i]);
    m_materials[i] = 0;
  }

  delete[] m_materials;
  m_materials = materials;
  m_numberOfMaterialSlots = numberOfSlots;
}

void vkStaticMeshState::SetColliderShape(vkPhysicsShapeContainer *shapes)
{
  if (shapes == m_shapes)
  {
    return;
  }
  if (!m_staticCollider)
  {
    m_staticCollider = vkEng->CreateStaticCollider();
    m_staticCollider->SetFriction(m_friction);
    m_staticCollider->SetRestitution(m_restitution);
  }
  if (m_shapes)
  {
    m_staticCollider->DetachShape(m_shapes);
  }
  if (shapes)
  {
    m_staticCollider->AttachShape(shapes);
  }

  VK_SET(m_shapes, shapes);
}

void vkStaticMeshState::SetFriction(float friction)
{
  if (!m_staticCollider)
  {
    m_staticCollider = vkEng->CreateStaticCollider();
    m_staticCollider->AttachShape(m_shapes);
    m_staticCollider->SetRestitution(m_restitution);
  }

  m_friction = friction;
  m_staticCollider->SetFriction(m_friction);
}


void vkStaticMeshState::SetRestitution(float restitution)
{
  if (!m_staticCollider)
  {
    m_staticCollider = vkEng->CreateStaticCollider();
    m_staticCollider->AttachShape(m_shapes);
    m_staticCollider->SetFriction(m_friction);
  }

  m_restitution = restitution;
  m_staticCollider->SetRestitution(m_restitution);
}

void vkStaticMeshState::UpdateTransformation()
{
  vkSpatialState::UpdateTransformation();

  if (m_staticCollider)
  {
    m_staticCollider->GetTransform().SetGlobalTransformation(GetGlobalTransformation());
    m_staticCollider->FinishTransformation();
  }

}

void vkStaticMeshState::OnAttachedToScene(vkEntityScene *scene)
{
  if (m_staticCollider && scene)
  {
    scene->GetPhysicsScene()->AddStaticCollider(m_staticCollider);
  }
}

void vkStaticMeshState::OnDetachedFromScene(vkEntityScene *scene)
{
  if (m_staticCollider && scene)
  {
    scene->GetPhysicsScene()->RemoveStaticCollider(m_staticCollider);
  }
}

void vkStaticMeshState::FillBoundingBox(vkBoundingBox &bbox)
{
  if (m_mesh)
  {
    bbox.Add(m_globalMatrix, m_mesh->GetBoundingBox());
  }

}

void vkStaticMeshState::Render(IGraphics *graphics, vkRenderPass pass) const
{
  if (m_mesh)
  {
    vkRenderState::Render(graphics, pass);
    graphics->SetModelMatrix(GetGlobalTransformation());
    m_mesh->Render(graphics, pass, m_numberOfMaterialSlots, m_materials);
  }
}


void vkStaticMeshState::PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config)
{
  if (m_materials && m_mesh)
  {
    if (m_castShadow && config.ScanShadowCasters || !m_castShadow && config.ScanNonShadowCasters)
    {
      entityScan->ScanRenderState(this);
    }
  }
}



vkSkinnedMeshState::vkSkinnedMeshState()
  : vkStaticMeshState()
  , m_skeleton(0)
{

}


vkSkinnedMeshState::~vkSkinnedMeshState()
{
  VK_RELEASE(m_skeleton);
}


void vkSkinnedMeshState::SetSkeleton(vkSkeleton *skeleton)
{
  VK_SET(m_skeleton, skeleton);
}


void vkSkinnedMeshState::Render(IGraphics *graphics, vkRenderPass pass) const
{
  if (m_skeleton)
  {
    graphics->SetSkeleton(m_skeleton);

    vkStaticMeshState::Render(graphics, pass);
  }
}
