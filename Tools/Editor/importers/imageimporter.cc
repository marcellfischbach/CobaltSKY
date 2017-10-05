
#include <importers/imageimporter.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagerassetwriter.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>
#include <csfile/csffile.hh>
#include <QGraphicsPixmapItem>
#include <QBuffer>
#include <QImageWriter>
#include <QDomDocument>
#include <QDomElement>


ImageImporterData::ImageImporterData()
  : AssetManagerImportData()
  , m_name("")
{
  m_view = new QGraphicsView();
  QGraphicsScene *scene = new QGraphicsScene(m_view);
  m_view->setScene(scene);
}

ImageImporterData::~ImageImporterData()
{
  delete m_view;
}

void ImageImporterData::Import(const QString &fileName)
{
  csFileInfo fi((const char*)fileName.toLatin1());
  m_name = QString(fi.GetName().c_str());
  m_fileName = fileName;
  QPixmap pixmap;
  if (pixmap.load(fileName))
  {
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
    m_view->scene()->addItem(item);
  }

}

void ImageImporterData::SetName(const QString &name)
{
  m_name = name;
}


const QString &ImageImporterData::GetName() const
{
  return m_name;
}

QWidget *ImageImporterData::GetWidget() const
{
  return m_view;
}

csResourceLocator ImageImporterData::Import(AssetManagerWidget *assetManager)
{
  csResourceLocator locator = assetManager->GetContentResource();
  QString typeID = GetType();
  if (typeID.isNull())
  {
    return csResourceLocator();
  }

  QString xAssetName = assetManager->GetNewAssetName(m_name);
  csResourceLocator xassetLocator(
    locator.GetResourceFile() + "/" + (const char*)xAssetName.toLatin1(),
    locator.GetResourceName(),
    locator.GetResourceEntry());

  QString dataName = xAssetName;
  dataName = dataName.left(dataName.length() - 6).append("data");
  csResourceLocator dataLocator(
    locator.GetResourceFile() + "/" + (const char*)dataName.toLatin1(),
    locator.GetResourceName(),
    locator.GetResourceEntry());

  QString csfName = xAssetName;
  csfName = csfName.left(csfName.length() - 6).append("csf");
  csResourceLocator csfLocator(
    locator.GetResourceFile() + "/" + (const char*)csfName.toLatin1(),
    locator.GetResourceName(),
    locator.GetResourceEntry());

  xAssetName = assetManager->GetFilePath(xAssetName);
  dataName = assetManager->GetFilePath(dataName);
  csfName = assetManager->GetFilePath(csfName);

  printf("Import:\n");
  printf("  XAsset: %s\n", (const char*)xAssetName.toLatin1());
  printf("    Data: %s\n", (const char*)dataName.toLatin1());
  printf("     CSF: %s\n", (const char*)csfName.toLatin1());


  QDomDocument doc;
  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement texture2dElement = doc.createElement("texture2d");
  QDomElement samplerElement = doc.createElement("sampler");
  QDomElement imageElement = doc.createElement("image");

  QDomElement previewElement = doc.createElement("preview");
  QDomElement editorIconElement = doc.createElement("editorIcon");
  QDomElement previewImageElement = doc.createElement("image");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  dataElement.appendChild(texture2dElement);
  texture2dElement.appendChild(samplerElement);
  texture2dElement.appendChild(imageElement);

  assetElement.appendChild(previewElement);
  previewElement.appendChild(editorIconElement);
  editorIconElement.appendChild(previewImageElement);

  samplerElement.setAttribute("resourceMode", "shared");
  samplerElement.appendChild(doc.createTextNode(QString("DefaultSampler.xasset")));

  csResourceLocator anonDataLocator = dataLocator.AsAnonymous();
  imageElement.appendChild(doc.createTextNode(QString(anonDataLocator.GetText().c_str())));
  imageElement.setAttribute("mipmap", "true");

  csResourceLocator previewLocator(anonDataLocator, "EDITOR_ICON");
  previewImageElement.appendChild(doc.createTextNode(QString(previewLocator.GetText().c_str())));
  QString xml = doc.toString(2);
  printf("Xml:\n%s\n", (const char*)xml.toLatin1());

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

  samplerEntry->AddAttribute("DefaultSampler.csf");
  samplerEntry->AddAttribute("resourceMode", "shared");
  imageEntry->AddAttribute("DATA");
  imageEntry->AddAttribute("mipmap", "true");

  previewImageEntry->AddAttribute("EDITOR_ICON");



  AssetManagerAssetWriter writer;

  QFile file(m_fileName);
  if (file.open(QIODevice::ReadOnly))
  {
    QByteArray ba = file.readAll();
    file.close();

    writer.AddEntry(
      "DATA",
      std::string((const char*)typeID.toLatin1()),
      ba.length(),
      reinterpret_cast<const csUInt8*>(ba.constData())
    );

    csfBlob *blobData = outputFile.CreateBlob();
    blobData->SetName("DATA");
    blobData->SetType(std::string((const char*)typeID.toLatin1()));
    blobData->SetBuffer(reinterpret_cast<const csUInt8*>(ba.constData()), ba.length());
    outputFile.AddBlob(blobData);
  }

  QImage image(m_fileName);
  image = image.scaled(QSize(64, 64));
  QBuffer buffer;
  QImageWriter qimagewriter(&buffer, QString("PNG").toLatin1());
  qimagewriter.write(image);

  writer.AddEntry(
    "EDITOR_ICON",
    std::string("PNG"),
    buffer.data().length(),
    reinterpret_cast<const csUInt8*>(buffer.data().constData())
  );

  csfBlob *blobEditorIcon = outputFile.CreateBlob();
  blobEditorIcon->SetName("EDITOR_ICON");
  blobEditorIcon->SetType(std::string("PNG"));
  blobEditorIcon->SetBuffer(reinterpret_cast<const csUInt8*>(buffer.data().constData()), buffer.data().length());
  outputFile.AddBlob(blobEditorIcon);

  outputFile.Output(std::string((const char*)csfName.toLatin1()), false, 2);
  //
  // write the xml document
  
  QFile xmlFile(xAssetName);
  if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
  {
    xmlFile.write(doc.toString(2).toLatin1());
    xmlFile.close();
  }
  

  //
  // write the data document
  iFile *dataFile = csVFS::Get()->Open(dataLocator, eOM_Write);
  if (dataFile) 
  {
    writer.Output(dataFile);
    dataFile->Close();
  }
  
  return xassetLocator;
}

QString ImageImporterData::GetType() const
{
  csFileInfo fi((const char*)m_fileName.toLatin1());
  fi.Upper();
  return QString(fi.GetExtension().c_str());
}


ImageImporter::ImageImporter()
  : AssetManagerImporter()
{

}


ImageImporter::~ImageImporter()
{

}

const QStringList ImageImporter::GetFilters() const
{
  QStringList res;
  res << QString("Texture (*.png)");
  return res;
}

bool ImageImporter::CanImport(const QString &fileName) const
{
  csFileInfo fi((const char*)fileName.toLatin1());
  return fi.GetExtension() == std::string("png");
}

const std::vector<AssetManagerImportData*> ImageImporter::Import(const QString &fileName) const
{
  ImageImporterData *data = new ImageImporterData();
  data->Import(fileName);

  std::vector<AssetManagerImportData*> result;
  result.push_back(data);
  return result;
}
