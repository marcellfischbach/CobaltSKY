#include <editor/importers/model/modelmeshexporter.hh>
#include <editor/importers/model/assimpscenescanner.hh>
#include <assimp/mesh.h>
#include <cobalt/math/csvector.hh>


struct Mesh
{
  unsigned numVertices;
  csVector3f *vertices;
  csVector3f *normals;
  csVector3f *tangents;
  csVector3f *biNormals;
  csVector2f *texCoords;
  csVector3f *colors;

  unsigned numberOfIndices;
  unsigned *indices;
};


struct Data : public ModelMeshExporterData
{
  unsigned materialIdx;
  std::vector<Mesh*> m_meshes;
};

Mesh *create_mesh (aiMesh *mesh, const csMatrix4f &tr);


ModelMeshExporter::ModelMeshExporter()
{

}


void ModelMeshExporter::Add(AssimpMeshData mesh)
{
  m_meshes.push_back(mesh);
}

void ModelMeshExporter::Finish()
{
  for(AssimpMeshData mesh : m_meshes)
  {
    Data *data = static_cast<Data*>(GetData(mesh.mesh->mMaterialIndex));
    Mesh *m = create_mesh(mesh.mesh, mesh.tr);
    data->m_meshes.push_back(m);
  }
}


ModelMeshExporterData* ModelMeshExporter::GetData(unsigned materialIdx)
{
  for (ModelMeshExporterData *data : m_data)
  {
    Data *d = static_cast<Data*>(data);
    if (d->materialIdx == materialIdx)
    {
      return d;
    }
  }

  Data *data = new Data();
  data->name = "MESH" + materialIdx;
  data->materialIdx = materialIdx;

  m_data.push_back(data);
  return data;
}




Mesh *create_mesh (aiMesh *mesh, const csMatrix4f &tr)
{
  Mesh *m;

  return m;
}
