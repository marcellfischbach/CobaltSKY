
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
  if (node->mNumMeshes > 0 || true)
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


  csFileInfo fi((const char*)fileName.toLatin1());
  data->m_name = QString(fi.GetName().c_str());
  data->m_fileName = fileName;

  Scan(data, scene);


  return data;
}


void ModelImporter::Scan(ModelImporterData *data, const aiScene *scene) const
{
  aiNode *root = scene->mRootNode;
  Scan(data, scene, root, 0);

}


static void ind(int indent)
{
  for (int i = 0; i < indent; ++i)
  {
    printf("  ");
  }
}

void ModelImporter::Scan(ModelImporterData *data, const aiScene *scene, aiNode *node, int indent) const
{
  std::string nodeName(node->mName.C_Str());
  ind(indent); printf("%s", nodeName.c_str());

  if (IsSkeleton(nodeName))
  {
    printf(" => Skeleton\n");
    data->m_skeletons.push_back(node);
    return;
  }
  else if (node->mNumMeshes > 0)
  {
    printf(" => Mesh %d\n", node->mNumMeshes);
    data->m_meshNodes.push_back(node);
  }
  else {
    printf("\n");
  }

  for (unsigned i = 0; i < node->mNumChildren; ++i)
  {
    aiNode *childNode = node->mChildren[i];
    Scan(data, scene, childNode, indent+1);
  }
}


bool ModelImporter::IsSkeleton(const std::string &name) const
{
  const unsigned ArmatureLength = 8;
  if (name.length() >= ArmatureLength)
  {
    std::string sub = name.substr(0, ArmatureLength);
    if (sub == std::string("Armature"))
    {
      return true;
    }
  }

  const unsigned SkeletonLength = 8;
  if (name.length() >= SkeletonLength)
  {
    std::string sub = name.substr(0, SkeletonLength);
    if (sub == std::string("Skeleton"))
    {
      return true;
    }
  }
  return false;
}

