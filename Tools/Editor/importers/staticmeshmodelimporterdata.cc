
#include <importers/staticmeshmodelimporterdata.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagerassetwriter.hh>
#include <staticmesheditor/staticmesheditorwidget.hh>
#include <cobalt/core/csassetoutputstream.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/entity/csstaticmeshstate.hh>
#include <cobalt/math/csvector2f.hh>
#include <cobalt/math/csvector3f.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/csengine.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <assimp/scene.h>
#include <csfile/csffile.hh>
#include <map>

#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QFile>
#include <QString>


StaticMeshModelImporterData::StaticMeshModelImporterData()
  : ModelImporterData(Mesh)
{
  m_staticMeshEditorWidget = new StaticMeshEditorWidget(0);
  SetView(m_staticMeshEditorWidget);
}

StaticMeshModelImporterData::~StaticMeshModelImporterData()
{

}

void StaticMeshModelImporterData::SetImportName(const QString &importName)
{
  SetName(importName);
}

void StaticMeshModelImporterData::SetName(const QString &name)
{
  m_name = name;
}

void StaticMeshModelImporterData::AddNode(aiNode *node)
{
  if (!node)
  {
    return;
  }

  std::string name(node->mName.C_Str());
  if (name.length() >= 7 && name.substr(0, 7) == std::string("CS_COL_"))
  {
    m_collisionNodes.push_back(node);
  }
  else
  {
    m_meshNodes.push_back(node);
  }
}

#define AI2CSVECTOR3(aiVector) csVector3f(aiVector.x, aiVector.y, aiVector.z)
#define AI2CSVECTOR2(aiVector) csVector2f(aiVector.x, aiVector.y)

void StaticMeshModelImporterData::GenerateData(const aiScene *scene)
{
  printf("Meshes: %d\n", m_meshNodes.size());

  for (auto node : m_meshNodes)
  {
    csMatrix4f trans(&node->mTransformation.a1);
    trans.Transpose();
    csMatrix4f normalTrans = trans;
    normalTrans.Invert();
    normalTrans.Transpose();

    for (unsigned i = 0; i < node->mNumMeshes; ++i)
    {
      int idx = node->mMeshes[i];
      aiMesh *mesh = scene->mMeshes[idx];
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
      aiString materialName;
      material->Get(AI_MATKEY_NAME, materialName);

      unsigned lod = 0;
      unsigned localMaterialIndex = GetLocalMaterialIndex(mesh->mMaterialIndex, std::string(materialName.C_Str()));

      Data *data = GetData(lod, localMaterialIndex);
      if (data->name.length() == 0)
      {
        data->name = std::string(node->mName.C_Str());
      }
      SubMesh *subMesh = data->subMesh;

      unsigned vOffset = subMesh->vertices.size();
      for (unsigned vi = 0; vi < mesh->mNumVertices; ++vi)
      {
        aiVector3D &pos = mesh->mVertices[vi];

        Vertex v;
        csMatrix4f::Transform(trans, AI2CSVECTOR3(pos), v.position);
        if (mesh->mNormals)
        {
          aiVector3D &norm = mesh->mNormals[vi];
          csMatrix4f::Mult(normalTrans, AI2CSVECTOR3(norm), v.normal);
        }
        if (mesh->mTextureCoords[0])
        {
          aiVector3D &txt = mesh->mTextureCoords[0][vi];
          v.texCoord = AI2CSVECTOR2(txt);
        }
        if (mesh->mTangents)
        {
          aiVector3D &tangent = mesh->mTangents[vi];
          csMatrix4f::Transform(normalTrans, AI2CSVECTOR3(tangent), v.tangent);
          csVector3f::Cross(v.normal, v.tangent, v.binormal);
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

  csMaterial *material = csResourceManager::Get()->Aquire<csMaterial>(csResourceLocator("materials/DefaultMaterial.csf"));
  for (unsigned i = 0, in = mesh->GetNumberOfMaterials(); i < in; ++i)
  {
    state->SetMaterial(i, material);
  }

  m_staticMeshEditorWidget->SetStaticMeshState(state);

}

namespace
{
  std::string as_data_name(const std::string& dataName)
  {
    std::string res;
    for (auto ch : dataName)
    {
      if (isalnum(ch))
      {
        res += ch;
      }
      else
      {
        res += '_';
      }
    }
    return res;
  }
}


csResourceLocator StaticMeshModelImporterData::Import(AssetManagerWidget *assetManager)
{
  csResourceLocator meshLocator = ImportMesh(assetManager);
  csResourceLocator colliderMeshLocator = ImportCollisionMesh(assetManager);


  csResourceLocator locator = assetManager->GetContentResource();
  QString csfName = assetManager->GetNewAssetName(m_name);
  csResourceLocator csfLocator(
    locator.GetResourceFile() + "/" + (const char*)csfName.toLatin1(),
    locator.GetResourceName(),
    locator.GetResourceEntry());
  csfName = assetManager->GetFilePath(csfName);
  csfFile outputFile;
  csfEntry *assetEntry = outputFile.CreateEntry("asset");
  csfEntry *dataEntry = outputFile.CreateEntry("data");
  csfEntry *blueprintEntry = outputFile.CreateEntry("blueprint");
  csfEntry *entityEntry = outputFile.CreateEntry("entity");
  csfEntry *entityStateEntry = outputFile.CreateEntry("entityState");

  outputFile.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(blueprintEntry);
  blueprintEntry->AddChild(entityEntry);
  entityEntry->AddChild(entityStateEntry);

  entityEntry->AddAttribute("class", "csEntity");
  entityStateEntry->AddAttribute("class", "csStaticMeshState");
  entityStateEntry->AddAttributeInt("id", 0);
  entityStateEntry->AddAttribute("root", "true");

  if (meshLocator.IsValid())
  {
    csfEntry *meshProperty = outputFile.CreateEntry("property");
    meshProperty->AddAttribute("name", "Mesh");
    meshProperty->AddAttribute("type", "resource");
    meshProperty->AddAttribute("locator", meshLocator.AsAnonymous().GetText());
    entityStateEntry->AddChild(meshProperty);

    for (auto materials : m_materialNames)
    {
      csfEntry *materialProperty = outputFile.CreateEntry("property");
      materialProperty->AddAttribute("name", "Materials");
      materialProperty->AddAttribute("collection", "true");
      materialProperty->AddAttribute("type", "resource");
      materialProperty->AddAttributeInt("idx", materials.first);
      materialProperty->AddAttribute("locator", "materials/DefaultMaterial.csf");
      entityStateEntry->AddChild(materialProperty);
    }
  }

  outputFile.Output(std::string((const char*)csfName.toLatin1()), false, 2);

  return csfLocator;


}

csResourceLocator StaticMeshModelImporterData::ImportMesh(AssetManagerWidget *assetManager)
{
  csResourceLocator locator = assetManager->GetContentResource();

  QString csfName = assetManager->GetNewAssetName(m_name + "_Mesh");
  csResourceLocator csfLocator(
    locator.GetResourceFile() + "/" + (const char*)csfName.toLatin1(),
    locator.GetResourceName(),
    locator.GetResourceEntry());
  csfName = assetManager->GetFilePath(csfName);

  csfFile outputFile;
  csfEntry *assetEntry = outputFile.CreateEntry("asset");
  csfEntry *dataEntry = outputFile.CreateEntry("data");
  csfEntry *meshEntry = outputFile.CreateEntry("mesh");

  outputFile.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(meshEntry);


  csfEntry *materialSlotsEntry = outputFile.CreateEntry("materialSlots");
  meshEntry->AddChild(materialSlotsEntry);
  for (auto materials : m_materialNames)
  {
    csfEntry *materialSlotEntry = outputFile.CreateEntry("materialSlot");
    materialSlotEntry->AddAttributeInt("id", materials.first);
    materialSlotEntry->AddAttribute("name", materials.second.c_str());
    materialSlotsEntry->AddChild(materialSlotEntry);
  }

  csfEntry *globalIndicesEntry = outputFile.CreateEntry("globalIndices");
  meshEntry->AddChild(globalIndicesEntry);

  csfEntry *subMeshesEntry = outputFile.CreateEntry("subMeshes");
  meshEntry->AddChild(subMeshesEntry);

  for (auto lod : m_lods)
  {
    for (auto data : lod->datas)
    {
      SubMesh *subMesh = data->subMesh;
      csAssetOutputStream os;
      Output(subMesh, os);
      std::string dataName = ::as_data_name(data->name);

      csfBlob *blobData = outputFile.CreateBlob();
      blobData->SetName(dataName);
      blobData->SetType(std::string("SUBMESH"));
      blobData->SetBuffer(reinterpret_cast<const csUInt8*>(os.GetBuffer()), os.GetSize());
      outputFile.AddBlob(blobData);


      csfEntry *subMeshEntry = outputFile.CreateEntry("subMesh");
      subMeshesEntry->AddChild(subMeshEntry);

      subMeshEntry->AddAttribute(dataName);
      subMeshEntry->AddAttribute("name", data->name.c_str());
      subMeshEntry->AddAttributeInt("lod", lod->lod);
      subMeshEntry->AddAttributeInt("materialSlot", data->materialIndex);

    }
  }



  outputFile.Output(std::string((const char*)csfName.toLatin1()), false, 2);

  return csfLocator;
}

csResourceLocator StaticMeshModelImporterData::ImportCollisionMesh(AssetManagerWidget *assetManager)
{
  return csResourceLocator();
}



void StaticMeshModelImporterData::Output(SubMesh *subMesh, csAssetOutputStream &os)
{
  os << (csUInt32)(CS_VERSION(1, 0, 0));

  csUInt32 indexType = eDT_UnsignedShort;
  if (subMesh->vertices.size() >= 65536)
  {
    indexType = eDT_UnsignedInt;
  }
  csUInt32 primitiveType = ePT_Triangles;

  os << primitiveType
    << indexType;


  // write the vertex declarations
  os << (csUInt8)5;
  os << (csUInt32)eVST_Position
    << (csUInt32)eDT_Float
    << (csSize)3
    << (csUInt32)0
    << (csSize)sizeof(Vertex)
    << (csUInt8)0;
  os << (csUInt32)eVST_Normal
    << (csUInt32)eDT_Float
    << (csSize)3
    << (csUInt32)(3 * sizeof(float))
    << (csSize)sizeof(Vertex)
    << (csUInt8)0;
  os << (csUInt32)eVST_Tangent
    << (csUInt32)eDT_Float
    << (csSize)3
    << (csUInt32)(6 * sizeof(float))
    << (csSize)sizeof(Vertex)
    << (csUInt8)0;
  os << (csUInt32)eVST_BiNormal
    << (csUInt32)eDT_Float
    << (csSize)3
    << (csUInt32)(9 * sizeof(float))
    << (csSize)sizeof(Vertex)
    << (csUInt8)0;
  os << (csUInt32)eVST_TexCoord0
    << (csUInt32)eDT_Float
    << (csSize)2
    << (csUInt32)(12 * sizeof(float))
    << (csSize)sizeof(Vertex)
    << (csUInt8)0;

  // only one stream here
  os << (csUInt8)1;
  os << (csSize)(subMesh->vertices.size() * sizeof(Vertex));
  for (auto v : subMesh->vertices)
  {
    os << v.position
      << v.normal
      << v.tangent
      << v.binormal
      << v.texCoord;
  }


  os << (csInt8)-1 // shared index buffer
    << (csSize)subMesh->trigonIndices.size() // num indices in buffer
    << (csSize)0; // offset

  if (indexType == eDT_UnsignedShort)
  {
    os << (csSize)(subMesh->trigonIndices.size () * sizeof(csUInt16));
    for (auto idx : subMesh->trigonIndices)
    {
      os << (csUInt16)idx;
    }
  }
  else if (indexType == eDT_UnsignedInt)
  {
    os << (csSize)(subMesh->trigonIndices.size () * sizeof(csUInt32));
    for (auto idx : subMesh->trigonIndices)
    {
      os << (csUInt32)idx;
    }
  }

  os << subMesh->boundingBox.GetMin()
    << subMesh->boundingBox.GetMax();
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


unsigned StaticMeshModelImporterData::GetLocalMaterialIndex(unsigned modelMaterialIndex, const std::string &materialName)
{
  unsigned res = ~0x00;
  auto it = m_modelToLocalMap.find(modelMaterialIndex);
  if (it != m_modelToLocalMap.end())
  {
    res = it->second;
  }
  else
  {
    unsigned nextLocalMaterialIndex = m_modelToLocalMap.size();
    m_modelToLocalMap[modelMaterialIndex] = nextLocalMaterialIndex;
    res = nextLocalMaterialIndex;
  }

  m_materialNames[res] = materialName;
  return res;
}
