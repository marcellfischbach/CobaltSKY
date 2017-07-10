
#include <textureeditor/textureeditorimporter.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagerassetwriter.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>
#include <QGraphicsPixmapItem>
#include <QBuffer>
#include <QImageWriter>
#include <QDomDocument>
#include <QDomElement>


TextureEditorImportData::TextureEditorImportData()
  : AssetManagerImportData()
  , m_name("")
{
  m_view = new QGraphicsView();
  QGraphicsScene *scene = new QGraphicsScene(m_view);
  m_view->setScene(scene);
}

TextureEditorImportData::~TextureEditorImportData()
{
  delete m_view;
}

void TextureEditorImportData::Import(const QString &fileName)
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

const QString &TextureEditorImportData::GetName() const
{
  return m_name;// QObject::tr("Texture: %1").arg(m_name);
}

QWidget *TextureEditorImportData::GetWidget() const
{
  return m_view;
}

void TextureEditorImportData::Import(AssetManagerWidget *assetManager)
{
  csResourceLocator locator = assetManager->GetContentResource();
  QString typeID = GetType();
  if (typeID.isNull())
  {
    return;
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
      csString((const char*)typeID.toLatin1()),
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
    csString("PNG"),
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
  
}

QString TextureEditorImportData::GetType() const
{
  csFileInfo fi((const char*)m_fileName.toLatin1());
  fi.Upper();
  return QString(fi.GetExtension().c_str());
}


TextureEditorImporter::TextureEditorImporter()
  : AssetManagerImporter()
{

}


TextureEditorImporter::~TextureEditorImporter()
{

}

const QStringList TextureEditorImporter::GetFilters() const
{
  QStringList res;
  res << QString("Texture (*.png)");
  return res;
}

bool TextureEditorImporter::CanImport(const QString &fileName) const
{
  csFileInfo fi((const char*)fileName.toLatin1());
  return fi.GetExtension() == std::string("png");
}

AssetManagerImportData *TextureEditorImporter::Import(const QString &fileName) const
{
  TextureEditorImportData *data = new TextureEditorImportData();
  data->Import(fileName);
  return data;
}
