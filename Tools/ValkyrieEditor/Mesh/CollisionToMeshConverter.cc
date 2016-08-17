
#include <Mesh/CollisionToMeshConverter.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Physics/PhysicsShapeContainer.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>

namespace mesh
{


vkMesh *CollisionToMeshConverter::CreateMesh(vkPhysicsShapeContainer *shapes)
{
  vkMesh *mesh = new vkMesh();

  for (vkSize i = 0, in = shapes->GetNumberOfShapes(); i < in; ++i)
  {
    IPhysicsShape *shape = shapes->GetShape(i);
    if (shape)
    {
      vkSubMesh *subMesh = CollisionToMeshConverter::CreateSubMesh(shape);
      if (subMesh)
      {
        mesh->AddMesh(subMesh);
      }
    }
  }
  mesh->AddMaterialName("Material");
  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();
  return mesh;
}

vkMesh *CollisionToMeshConverter::CreateMesh(IPhysicsShape *shape)
{

  vkMesh *mesh = new vkMesh();

  vkSubMesh *subMesh = CollisionToMeshConverter::CreateSubMesh(shape);
  mesh->AddMesh(subMesh);
  mesh->AddMaterialName("Material");
  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();

  return mesh;
}

vkSubMesh *CollisionToMeshConverter::CreateSubMesh(IPhysicsShape *shape)
{
  const vkPhysGeometry &geometry = shape->GetGeometry();
  switch (geometry.Type)
  {
  case ePGT_CylinderZ:
    return CollisionToMeshConverter::CreateCylinderZSubMesh(geometry, shape->GetLocalTransform());
  }
  return 0;
}

vkSubMesh *CollisionToMeshConverter::CreateCylinderZSubMesh(const vkPhysGeometry &geometry, const vkMatrix4f &localTransform)
{
  unsigned numRads = 16;
  unsigned numVertices = numRads * 2;
  unsigned numIndices = numRads * (2 + 2 + 2);
  vkVector3f *vertices = new vkVector3f[numVertices];
  vkUInt16 *indices = new vkUInt16[numIndices];
  vkUInt16 *iptr = indices;
  vkBoundingBox bbox;
  for (unsigned i = 0; i < numRads; ++i)
  {
    float f = (float)i / (float)numRads * 3.14156f * 2.0f;
    float c = cos(f);
    float s = sin(f);
    vertices[i] = vkVector3f(c * geometry.Radius, s * geometry.Radius, geometry.Height / 2.0f);
    vertices[numRads + i] = vkVector3f(c * geometry.Radius, s * geometry.Radius, -geometry.Height / 2.0f);

    vkMatrix4f::Transform(localTransform, vertices[i], vertices[i]);
    vkMatrix4f::Transform(localTransform, vertices[numRads + i], vertices[numRads + i]);

    bbox.Add(vertices[i]);
    bbox.Add(vertices[numRads + i]);

    unsigned i1 = (i + 1) % numRads;
    *iptr++ = i;
    *iptr++ = i1;

    *iptr++ = numRads + i;
    *iptr++ = numRads + i1;

    *iptr++ = i;
    *iptr++ = numRads + i;
  }
  bbox.Finish();

  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 3, 0, sizeof(vkVector3f), 0),
    vkVertexElement()
  };

  IVertexBuffer *vb = vkEng->CreateVertexBuffer(numVertices * sizeof(vkVector3f), vertices, eBDM_Static);
  IIndexBuffer *ib = vkEng->CreateIndexBuffer(numIndices * sizeof(vkUInt16), indices, eBDM_Static);
  IVertexDeclaration *vd = vkEng->CreateVertexDeclaration(elements);

  vkSubMesh *subMesh = new vkSubMesh();
  subMesh->SetPrimitiveType(ePT_Lines);
  subMesh->AddVertexBuffer(vb);
  subMesh->SetIndexBuffer(ib, numIndices, 0);
  subMesh->SetVertexDeclaration(vd);
  subMesh->SetBoundingBox(bbox);
  subMesh->SetIndexType(eDT_UnsignedShort);

  return subMesh;
}


}