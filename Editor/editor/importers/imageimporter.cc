
#include <editor/importers/imageimporter.hh>
#include <editor/assetmanager/assetmanagerwidget.hh>
#include <editor/assetmanager/assetmanagerassetwriter.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csfwriter.hh>
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
  QString typeID = GetType();
  if (typeID.isNull())
  {
    return csResourceLocator();
  }

  csResourceLocator assetLocator;// = assetManager->GetNewResourceLocator(std::string((const char*)m_name.toLatin1()));
  if (!assetLocator.IsValid())
  {
    return csResourceLocator();
  }

  printf("Import:\n");
  printf("  CSF: %s\n", assetLocator.Encode().c_str());



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

  samplerEntry->AddAttribute("locator", "DefaultSampler.csf");
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

  csfBlob *blobEditorIcon = outputFile.CreateBlob();
  blobEditorIcon->SetName("EDITOR_ICON");
  blobEditorIcon->SetType(std::string("PNG"));
  blobEditorIcon->SetBuffer(reinterpret_cast<const csUInt8*>(buffer.data().constData()), buffer.data().length());
  outputFile.AddBlob(blobEditorIcon);

  if (!csfWriter::Write(outputFile, assetLocator, false, 2))
  {
    return csResourceLocator();
  }
  
  return assetLocator;
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
