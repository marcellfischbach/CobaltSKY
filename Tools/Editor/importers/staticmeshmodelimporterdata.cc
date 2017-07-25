
#include <importers/staticmeshmodelimporterdata.hh>
#include <staticmesheditor/staticmesheditorwidget.hh>
#include <cobalt/entity/csstaticmeshstate.hh>
#include <cobalt/math/csvector2f.hh>
#include <cobalt/math/csvector3f.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/csengine.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <QString>
#include <assimp/scene.h>
#include <map>


StaticMeshModelImporterData::StaticMeshModelImporterData()
  : ModelImporterData(Mesh)
{
  m_staticMeshEditorWidget = new StaticMeshEditorWidget(0);
  SetView(m_staticMeshEditorWidget);
}

StaticMeshModelImporterData::~StaticMeshModelImporterData()
{

}

void StaticMeshModelImporterData::SetName(const QString &name)
{
  m_name = name + "_Mesh";
}

#define AI2CSVECTOR3(aiVector) csVector3f(aiVector.x, aiVector.y, aiVector.z)
#define AI2CSVECTOR2(aiVector) csVector2f(aiVector.x, aiVector.y)

void StaticMeshModelImporterData::GenerateData(const aiScene *scene)
{
  printf("Meshes: %d\n", m_meshNodes.size());

  for (auto node : m_meshNodes)
  {
    for (unsigned i = 0; i < node->mNumMeshes; ++i)
    {
      int idx = node->mMeshes[i];
      aiMesh *mesh = scene->mMeshes[idx];

      unsigned lod = 0;
      unsigned localMaterialIndex = GetLocalMaterialIndex(mesh->mMaterialIndex);

      Data *data = GetData(lod, localMaterialIndex);
      SubMesh *subMesh = data->subMesh;

      unsigned vOffset = subMesh->vertices.size();
      for (unsigned vi = 0; vi < mesh->mNumVertices; ++vi)
      {
        aiVector3D &pos = mesh->mVertices[vi];

        Vertex v;
        v.position = AI2CSVECTOR3(pos);
        if (mesh->mNormals)
        {
          aiVector3D &norm = mesh->mNormals[vi];
          v.normal = AI2CSVECTOR3(norm);
        }
        if (mesh->mTextureCoords[0])
        {
          aiVector3D &txt = mesh->mTextureCoords[0][vi];
          v.texCoord = AI2CSVECTOR2(txt);
        }
        if (mesh->mTangents)
        {
          aiVector3D &tangent = mesh->mTangents[vi];
          v.tangent = AI2CSVECTOR3(tangent);
        }
        if (mesh->mBitangents)
        {
          aiVector3D &bitangent = mesh->mBitangents[vi];
          v.binormal = AI2CSVECTOR3(bitangent);
        }
        subMesh->boundingBox.Add(v.position);
        subMesh->vertices.push_back(v);
      }

      for (unsigned fi = 0; fi < mesh->mNumFaces; ++fi)
      {
        aiFace &face = mesh->mFaces[fi];
        if (face.mNumIndices != 3)
        {
          continue;
        }
        subMesh->trigonIndices.push_back(vOffset + face.mIndices[0]);
        subMesh->trigonIndices.push_back(vOffset + face.mIndices[1]);
        subMesh->trigonIndices.push_back(vOffset + face.mIndices[2]);
      }
    }
  }

  iGraphics *gfx = csEng->GetRenderer();
  csMesh *mesh = new csMesh();
  for (auto lod : m_lods)
  {
    for (auto d : lod->datas)
    {
      SubMesh *sm = d->subMesh;
      sm->boundingBox.Finish();
     
      csVertexElement elements[] = {
        csVertexElement(eVST_Position, eDT_Float, 3, 0, sizeof(Vertex), 0),
        csVertexElement(eVST_Normal, eDT_Float, 3, sizeof(float) * 3, sizeof(Vertex), 0),
        csVertexElement(eVST_Tangent, eDT_Float, 3, sizeof(float) * 6, sizeof(Vertex), 0),
        csVertexElement(eVST_BiNormal, eDT_Float, 3, sizeof(float) * 9, sizeof(Vertex), 0),
        csVertexElement(eVST_TexCoord0, eDT_Float, 2, sizeof(float) * 12, sizeof(Vertex), 0),
        csVertexElement()
      };
      iVertexDeclaration *vd = gfx->CreateVertexDeclaration(elements);
      iVertexBuffer *vb = gfx->CreateVertexBuffer(sm->vertices.size() * sizeof(Vertex), &sm->vertices[0], eBDM_Static);
      iIndexBuffer *ib = gfx->CreateIndexBuffer(sm->trigonIndices.size() * sizeof(unsigned), &sm->trigonIndices[0], eBDM_Static);

      csSubMesh *subMesh = new csSubMesh();
      subMesh->SetBoundingBox(sm->boundingBox);
      subMesh->SetVertexDeclaration(vd);
      subMesh->AddVertexBuffer(vb);
      subMesh->SetIndexBuffer(ib, sm->trigonIndices.size());
      subMesh->SetIndexType(eDT_UnsignedInt);
      subMesh->SetPrimitiveType(ePT_Triangles);

      mesh->AddMesh(subMesh, d->materialIndex, lod->lod);
    }
  }

  mesh->UpdateBoundingBox();
  mesh->OptimizeDataStruct();

  csStaticMeshState *state = new csStaticMeshState();
  state->SetCastShadow(true);
  state->SetMesh(mesh);

  csMaterial *material = csResourceManager::Get()->Aquire<csMaterial>(csResourceLocator("materials/DefaultMaterial.xasset"));
  for (unsigned i = 0, in = mesh->GetNumberOfMaterials(); i < in; ++i)
  {
    state->SetMaterial(material, i);
  }

  m_staticMeshEditorWidget->SetStaticMeshState(state);

}


csResourceLocator StaticMeshModelImporterData::Import(AssetManagerWidget *assetManager)
{
  return csResourceLocator();
}




StaticMeshModelImporterData::LOD *StaticMeshModelImporterData::GetLOD(unsigned lod)
{
  for (auto l : m_lods)
  {
    if (l->lod == lod)
    {
      return l;
    }
  }

  LOD *l = new LOD;
  l->lod = lod;
  m_lods.push_back(l);
  return l;
}

StaticMeshModelImporterData::Data *StaticMeshModelImporterData::GetData(unsigned lod, unsigned materialIndex)
{
  LOD *l = GetLOD(lod);
  return GetData(l, materialIndex);
}

StaticMeshModelImporterData::Data *StaticMeshModelImporterData::GetData(LOD *lod, unsigned materialIndex)
{
  if (!lod)
  {
    return 0;
  }

  for (auto d : lod->datas)
  {
    if (d->materialIndex == materialIndex)
    {
      return d;
    }
  }

  Data *data = new Data;
  data->materialIndex = materialIndex;
  data->subMesh = new SubMesh;
  lod->datas.push_back(data);
  return data;
}


unsigned StaticMeshModelImporterData::GetLocalMaterialIndex(unsigned modelMaterialIndex)
{
  auto it = m_modelToLocalMap.find(modelMaterialIndex);
  if (it != m_modelToLocalMap.end())
  {
    return it->second;
  }

  unsigned nextLocalMaterialIndex = m_modelToLocalMap.size();
  m_modelToLocalMap[modelMaterialIndex] = nextLocalMaterialIndex;
  return nextLocalMaterialIndex;
}