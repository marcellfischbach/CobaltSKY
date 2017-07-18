
#include <importers/modelimporter.hh>
#include <cobalt/core/csfileinfo.hh>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QLabel>



ModelImporterData::ModelImporterData()
  : AssetManagerImportData()
{
  m_view = new QLabel("Model importer...");
}


ModelImporterData::~ModelImporterData()
{

}

void debug_node(const aiNode *node, int indent)
{
  if (node->mNumMeshes > 0)
  {
    for (int i = 0; i < indent; ++i)
    {
      printf("  ");
    }


    printf("Node: %s [", node->mName.C_Str());
    for (int i = 0; i < node->mNumMeshes; ++i)
    {
      printf(" %d", node->mMeshes[i]);
    }
    printf(" ]\n");
  }
  for (unsigned i = 0; i < node->mNumChildren; ++i)
  {
    debug_node(node->mChildren[i], indent + 1);
  }
}

void ModelImporterData::Import(const QString &fileName)
{
  csFileInfo fi((const char*)fileName.toLatin1());
  m_name = QString(fi.GetName().c_str());
  m_fileName = fileName;

  // Create an instance of the Importer class
  Assimp::Importer importer;
  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll 
  // propably to request more postprocessing than we do in this example.
  const aiScene* scene = importer.ReadFile(
    std::string((const char*)fileName.toLatin1()),
    aiProcess_CalcTangentSpace |
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_SortByPType);

  printf("Import Scene\n");
  debug_node(scene->mRootNode, 1);

}


const QString &ModelImporterData::GetName() const
{
  return m_name;
}

QWidget *ModelImporterData::GetWidget() const
{
  return m_view;
}

csResourceLocator ModelImporterData::Import(AssetManagerWidget *assetManager)
{
  return csResourceLocator();
}




ModelImporter::ModelImporter()
  : AssetManagerImporter()
{

}


ModelImporter::~ModelImporter()
{

}

const QStringList ModelImporter::GetFilters() const
{
  QStringList res;
  res << QString("Model (*.dae *.fbx *.obj)");
  return res;
}

bool ModelImporter::CanImport(const QString &fileName) const
{
  csFileInfo fi((const char*)fileName.toLatin1());
  std::string ext = fi.GetExtension();
  return ext == std::string("dae")
    || ext == std::string("fbx")
    || ext == std::string("obj");
}

AssetManagerImportData *ModelImporter::Import(const QString &fileName) const
{
  ModelImporterData *data = new ModelImporterData();
  data->Import(fileName);
  return data;
}
