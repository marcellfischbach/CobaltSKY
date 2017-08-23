
#include <importers/imageimporter.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagerassetwriter.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>
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

  xAssetName = assetManager->GetFilePath(xAssetName);
  dataName = assetManager->GetFilePath(dataName);

  printf("Import:\n");
  printf("  XAsset: %s\n", (const char*)xAssetName.toLatin1());
  printf("    Data: %s\n", (const char*)dataName.toLatin1());

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
