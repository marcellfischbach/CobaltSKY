
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

csResourceLocator ImageImporter::Import(const asset::model::Folder *folder)
{
  csfFile file;
  csfEntry *assetEntry = file.CreateEntry("asset");
  csfEntry *dataEntry = file.CreateEntry("data");
  csfEntry *texture2dEntry = file.CreateEntry("texture2d");
  csfEntry *samplerEntry = file.CreateEntry("sampler");
  csfEntry *imageEntry = file.CreateEntry("image");
  csfEntry *previewEntry = file.CreateEntry("preview");
  csfEntry *editorIconEntry = file.CreateEntry("editorIcon");
  csfEntry *editorIconImageEntry = file.CreateEntry("image");


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

    csfBlob *dataBlob = file.CreateBlob();
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

    csfBlob *editorIconBlob = file.CreateBlob();
    editorIconBlob->SetName("EDITOR_ICON");
    editorIconBlob->SetType("PNG");
    editorIconBlob->SetBuffer(ba.data(), ba.size());

    file.AddBlob(editorIconBlob);
  }
  csResourceLocator locator = folder->CreateResourceLocator(m_importName + ".asset");

  std::string fileName = csVFS::Get()->GetAbsolutePath(locator, csVFS::DontCheckExistence);

  if (file.Output(fileName, false, 2))
  {
    return locator;
  }

  return csResourceLocator();
}


