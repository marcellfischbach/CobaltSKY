
#include <imageimporter/imageimporter.hh>
#include <csfile/csffile.hh>
#include <editor/assetmodel/folder.hh>
#include <cobalt/core/csvfs.hh>
#include <QImage>
#include <QByteArray>
#include <QBuffer>

ImageImporter::ImageImporter(const std::filesystem::path &path)
  : iAssetImporter()
  , m_path(path)
  , m_importName(path.filename().string())
{
}


const std::filesystem::path &ImageImporter::GetFile() const
{
  return m_path;
}

const std::string ImageImporter::GetType() const
{
  return std::string("Image");
}

QWidget *ImageImporter::GetWidget()
{
  return 0;
}

const std::string &ImageImporter::GetImportName() const
{
  return m_importName;
}

void ImageImporter::SetImportName(const std::string &importName)
{
  m_importName = importName;
}

void ImageImporter::SetEnabled(bool enable)
{
}

cs::ResourceLocator ImageImporter::Import(const asset::model::Folder *folder)
{
  cs::file::File file;
  cs::file::Entry *assetEntry = file.CreateEntry("asset");
  cs::file::Entry *dataEntry = file.CreateEntry("data");
  cs::file::Entry *texture2dEntry = file.CreateEntry("texture2d");
  cs::file::Entry *samplerEntry = file.CreateEntry("sampler");
  cs::file::Entry *imageEntry = file.CreateEntry("image");
  cs::file::Entry *previewEntry = file.CreateEntry("preview");
  cs::file::Entry *editorIconEntry = file.CreateEntry("editorIcon");
  cs::file::Entry *editorIconImageEntry = file.CreateEntry("image");


  file.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(texture2dEntry);
  texture2dEntry->AddChild(samplerEntry);
  texture2dEntry->AddChild(imageEntry);

  samplerEntry->AddAttribute("locator", "DefaultSampler.asset");
  samplerEntry->AddAttribute("resourceMode", "shared");

  imageEntry->AddAttribute("DATA");
  imageEntry->AddAttribute("mipmap", "true");


  assetEntry->AddChild(previewEntry);
  previewEntry->AddChild(editorIconEntry);
  editorIconEntry->AddChild(editorIconImageEntry);

  editorIconImageEntry->AddAttribute("EDITOR_ICON");

  QImage qImage(QString(m_path.string().c_str()));

  //
  // write the origin image to the asset
  {
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    qImage.save(&buffer, "PNG"); // writes image into ba in PNG format

    cs::file::Blob *dataBlob = file.CreateBlob();
    dataBlob->SetName("DATA");
    dataBlob->SetType("PNG");
    dataBlob->SetBuffer(ba.data(), ba.size());

    file.AddBlob(dataBlob);
  }
  // 
  // make a 64x64 preview image
  {
    QImage prevImage = qImage.scaled(64, 64);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    prevImage.save(&buffer, "PNG"); // writes image into ba in PNG format

    cs::file::Blob *editorIconBlob = file.CreateBlob();
    editorIconBlob->SetName("EDITOR_ICON");
    editorIconBlob->SetType("PNG");
    editorIconBlob->SetBuffer(ba.data(), ba.size());

    file.AddBlob(editorIconBlob);
  }
  cs::ResourceLocator locator = folder->CreateResourceLocator(m_importName + ".asset");

  std::string fileName = cs::VFS::Get()->GetAbsolutePath(locator, cs::VFS::DontCheckExistence);

  if (file.Output(fileName, false, 2))
  {
    return locator;
  }

  return cs::ResourceLocator();
}


