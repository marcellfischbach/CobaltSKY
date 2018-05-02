
#include <importers/image/imageimporter.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagerassetwriter.hh>
#include <csfile/csffile.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/graphics/isampler.hh>
#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QImage>
#include <QImageWriter>
#include <QString>


ImageImporterGUI::ImageImporterGUI()
  : QWidget()
{
  m_gui.setupUi(this);
  m_gui.sampler->AddValidClass(iSampler::GetStaticClass());
  m_gui.sampler->SetResourceLocator(csResourceLocator("DefaultSampler.csf"));
}

const csResourceLocator &ImageImporterGUI::GetSamplerLocator() const
{
  return m_gui.sampler->GetResourceLocator();
}


newImageImporter::newImageImporter(const std::string &fileName)
  : m_fileName(fileName)
{
  m_gui = new ImageImporterGUI();
  csFileInfo fileInfo (m_fileName);
  m_assetName = fileInfo.GetName() + "." + fileInfo.GetExtension();

}

std::string newImageImporter::GetName () const
{
  return "Image";
}

std::string newImageImporter::GetAssetName () const
{
  return m_assetName;
}

std::string newImageImporter::GetFileName () const
{
  return m_fileName;
}

QWidget *newImageImporter::GetWidget ()
{
  return m_gui;
}


bool newImageImporter::Import(AssetManagerWidget *assetManager)
{
  csResourceLocator locator = assetManager->GetContentResource();
  std::string typeID = GetType();
  if (typeID.empty())
  {
    return false;
  }


  csFileInfo fi (m_fileName);

  QString csfName = assetManager->GetNewAssetName(QString(fi.GetName().c_str()));
  csResourceLocator assetLocator(
    locator.GetResourceFile() + "/" + (const char*)csfName.toLatin1(),
    locator.GetResourceName(),
    locator.GetResourceEntry());

  csfName = assetManager->GetFilePath(csfName);

  printf("Import:\n");
  printf("  CSF: %s\n", (const char*)csfName.toLatin1());



  csfFile outputFile;
  csfEntry *assetEntry = outputFile.CreateEntry("asset");
  csfEntry *dataEntry = outputFile.CreateEntry("data");
  csfEntry *texture2dEntry = outputFile.CreateEntry("texture2d");
  csfEntry *samplerEntry = outputFile.CreateEntry("sampler");
  csfEntry *imageEntry = outputFile.CreateEntry("image");
  csfEntry *previewEntry = outputFile.CreateEntry("preview");
  csfEntry *editorIconEntry = outputFile.CreateEntry("editorIcon");
  csfEntry *previewImageEntry = outputFile.CreateEntry("image");
  outputFile.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(texture2dEntry);
  texture2dEntry->AddChild(samplerEntry);
  texture2dEntry->AddChild(imageEntry);

  assetEntry->AddChild(previewEntry);
  previewEntry->AddChild(editorIconEntry);
  editorIconEntry->AddChild(previewImageEntry);

  const csResourceLocator &samplerLocator = m_gui->GetSamplerLocator();
  if (samplerLocator.IsValid())
  {
    csResourceLocator anonymous = samplerLocator.AsAnonymous();
    samplerEntry->AddAttribute("locator", anonymous.GetText());
  }
  else
  {
    samplerEntry->AddAttribute("locator", "DefaultSampler.csf");
  }
  samplerEntry->AddAttribute("resourceMode", "shared");
  imageEntry->AddAttribute("DATA");
  imageEntry->AddAttribute("mipmap", "true");

  previewImageEntry->AddAttribute("EDITOR_ICON");



  AssetManagerAssetWriter writer;

  QFile file(QString(m_fileName.c_str()));
  if (file.open(QIODevice::ReadOnly))
  {
    QByteArray ba = file.readAll();
    file.close();

    csfBlob *blobData = outputFile.CreateBlob();
    blobData->SetName("DATA");
    blobData->SetType(typeID);
    blobData->SetBuffer(reinterpret_cast<const csUInt8*>(ba.constData()), ba.length());
    outputFile.AddBlob(blobData);
  }

  QImage image(QString(m_fileName.c_str()));
  image = image.scaled(QSize(64, 64));
  QBuffer buffer;
  QImageWriter qimagewriter(&buffer, QString("PNG").toLatin1());
  qimagewriter.write(image);

  csfBlob *blobEditorIcon = outputFile.CreateBlob();
  blobEditorIcon->SetName("EDITOR_ICON");
  blobEditorIcon->SetType(std::string("PNG"));
  blobEditorIcon->SetBuffer(reinterpret_cast<const csUInt8*>(buffer.data().constData()), buffer.data().length());
  outputFile.AddBlob(blobEditorIcon);

  outputFile.Output(std::string((const char*)csfName.toLatin1()), false, 2);

  return true;
}


std::string newImageImporter::GetType() const
{
  csFileInfo fi(m_fileName);
  fi.Upper();
  return fi.GetExtension().c_str();
}


bool ImageImporterFactory::CanImport(const std::string &fileName) const
{
//  QString name (fileName);
  return true;
}


std::map<std::string, std::vector<std::string>> ImageImporterFactory::GetImportFormats() const
{
  std::map<std::string, std::vector<std::string>> result;

  result["JPEG Image file"].push_back("*.jpg");
  result["JPEG Image file"].push_back("*.jpeg");
  result["PNG Image file"].push_back("*.png");
  result["BMP Image file"].push_back("*.bmp");
  return result;
}


iImporter *ImageImporterFactory::CreateImporter (const std::string &fileName) const
{
  return new newImageImporter(fileName);
}
