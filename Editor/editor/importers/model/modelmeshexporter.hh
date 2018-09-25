
#pragma once

#include <editor/editorexport.hh>
#include <vector>
#include <string>

struct AssimpMeshData;

struct ModelMeshExporterData
{
  std::string name;
};

class ModelMeshExporter
{
public:
  ModelMeshExporter();

  void Add(AssimpMeshData mesh);
  void Finish();

  const std::vector<ModelMeshExporterData*> &GetData () const;


private:
  std::vector<ModelMeshExporterData*> m_data;
  std::vector<AssimpMeshData> m_meshes;

  ModelMeshExporterData *GetData(unsigned materialIdx);

};
