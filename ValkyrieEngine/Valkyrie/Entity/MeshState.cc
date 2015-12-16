
#include <Valkyrie/Entity/MeshState.hh>
#include <Valkyrie/Entity/Scan.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Mesh.hh>

vkStaticMeshState::vkStaticMeshState()
  : vkRenderState()
  , m_mesh (0)
  , m_materials(0)
  , m_numberOfMaterialSlots(0)
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

void vkStaticMeshState::SetMaterial(vkMaterialInstance *material, vkUInt32 slot)
{
  if (slot >= m_numberOfMaterialSlots)
  {
    return;
  }

  VK_SET(m_materials[slot], material);
}


void vkStaticMeshState::FinishTransformation()
{
  vkRenderState::FinishTransformation();

  UpdateBoundingBox();
}


void vkStaticMeshState::UpdateMaterialSlots()
{
  vkUInt32 numberOfSlots = m_mesh->GetNumberOfMaterials();
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

void vkStaticMeshState::UpdateBoundingBox()
{
  m_boundingBox.Clear();
  if (m_mesh)
  {
    m_boundingBox.Add(m_globalMatrix, m_mesh->GetBoundingBox());
    m_boundingBox.Finish();
  }

}

void vkStaticMeshState::Render(IGraphics *graphics, vkRenderPass pass) const
{
  if (m_mesh)
  {
    graphics->SetModelMatrix(GetGlobalTransformation());
    m_mesh->Render(graphics, pass, m_numberOfMaterialSlots, m_materials);
  }
}


void vkStaticMeshState::PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config)
{
  if (m_materials && m_mesh)
  {
    entityScan->ScanRenderState(this);
  }
}