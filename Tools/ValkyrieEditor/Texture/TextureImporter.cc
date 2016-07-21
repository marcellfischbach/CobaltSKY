

#include <Texture/TextureImporter.hh>
#include <AssetManager/AssetWriter.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Core/IFile.hh>
#include <Valkyrie/Core/VFS.hh>
#include <Valkyrie/Defs.hh>
#include <Valkyrie/Enums.hh>
#include <qimage.h>
#include <qimagewriter.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qxmlstream.h>
#include <qdom.h>

namespace texture
{

Importer *Importer::Get()
{
  static Importer importer;
  return &importer;
}

bool Importer::CanImport(const QFileInfo &info)
{
  QString fileName = info.fileName();
  return fileName.endsWith(".jpg", Qt::CaseInsensitive)
    || fileName.endsWith(".jpeg", Qt::CaseInsensitive)
    || fileName.endsWith(".png", Qt::CaseInsensitive);
}

bool Importer::Import(const QFileInfo &info, const QDir &outputDir)
{
  QImage image(info.absoluteFilePath());
  if (image.isNull())
  {
    return false;
  }

  QFileInfo tempFile("texture_import_file.file");

  QString format = "JPG";
  switch (image.format())
  {
    case QImage::Format_Invalid:
      return false;

    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
    case QImage::Format_Indexed8:
    case QImage::Format_Alpha8:
    case QImage::Format_Grayscale8:
      // these formats are stored internally as a jpg

    case QImage::Format_RGB32:
    case QImage::Format_RGB16:
    case QImage::Format_RGB666:
    case QImage::Format_RGB555:
    case QImage::Format_RGB888:
    case QImage::Format_RGB444:
    case QImage::Format_BGR30:
    case QImage::Format_RGB30:
    case QImage::Format_RGBX8888:
      // no sensible alpha channel
      format = "JPG";
      format = "PNG"; // yet no png loader 
      break;


    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    case QImage::Format_ARGB8565_Premultiplied:
    case QImage::Format_ARGB6666_Premultiplied:
    case QImage::Format_ARGB8555_Premultiplied:
    case QImage::Format_ARGB4444_Premultiplied:
    case QImage::Format_RGBA8888:
    case QImage::Format_RGBA8888_Premultiplied:
    case QImage::Format_A2BGR30_Premultiplied:
    case QImage::Format_A2RGB30_Premultiplied:
      format = "PNG";
      break; 
  }

  assetmanager::AssetWriter writer;

  {
    QImageWriter writer(tempFile.absoluteFilePath(), format.toLatin1());
    writer.write(image);
  }

  QFile file(tempFile.absoluteFilePath());
  if (file.open(QIODevice::ReadOnly))
  {
    QByteArray buffer = file.readAll();
    file.close();
    file.remove();

    vkString typeID((const char*)format.toLatin1());
    writer.AddEntry("DATA", typeID, buffer.length(), (const vkUInt8*)buffer.data());
  }


  {
    // the icon image is always 64x64 stored as PNG
    QImage iconImage = image.scaled(QSize(64, 64));
    QImageWriter writer (tempFile.absoluteFilePath(), QString("PNG").toLatin1());
    writer.write(iconImage);
  }

  if (file.open(QIODevice::ReadOnly))
  {
    QByteArray buffer = file.readAll();
    file.close();
    file.remove();

    vkString typeID((const char*)format.toLatin1());
    writer.AddEntry("EDITOR_ICON", typeID, buffer.length(), (const vkUInt8*)buffer.data());
  }


  QString dataFileName;
  {
    QString fileName = info.completeBaseName() + ".data";
    QString absFileName = outputDir.absoluteFilePath(fileName);


    QFileInfo rootPathInfo(vkVFS::Get()->GetRootPath().c_str());
    QString rootPath = rootPathInfo.absoluteFilePath();

    if (absFileName.startsWith(rootPath, Qt::CaseInsensitive))
    {
      absFileName = absFileName.right(absFileName.length() - rootPath.length());
    }
    dataFileName = absFileName.right(absFileName.length() - 1);

    IFile *finalOutputFile = vkVFS::Get()->Open(vkString((const char*)absFileName.toLatin1()), eOM_Write, eTM_Binary);
    if (!finalOutputFile)
    {
      return false;
    }
    writer.Output(finalOutputFile);
    finalOutputFile->Close();
  }

  {
    QString fileName = info.completeBaseName() + ".xasset";
    QString absFileName = outputDir.absoluteFilePath(fileName);


    QFileInfo rootPathInfo(vkVFS::Get()->GetRootPath().c_str());
    QString rootPath = rootPathInfo.absoluteFilePath();

    if (absFileName.startsWith(rootPath, Qt::CaseInsensitive))
    {
      absFileName = absFileName.right(absFileName.length() - rootPath.length());
    }


    QDomDocument doc;
    QDomElement assetElement = doc.createElement("asset");
    QDomElement dataElement = doc.createElement("data");
    QDomElement previewElement = doc.createElement("preview");
    QDomElement editorIconElement = doc.createElement("editoricon");
    QDomElement textureElement = doc.createElement("texture2d");
    QDomElement dataImageElement = doc.createElement("image");
    QDomElement dataSamplerElement = doc.createElement("sampler");
    QDomElement editorIconImageElement = doc.createElement("image");
    textureElement.appendChild(dataSamplerElement);
    textureElement.appendChild(dataImageElement);
    dataElement.appendChild(textureElement);
    assetElement.appendChild(dataElement);

    editorIconElement.appendChild(editorIconImageElement);
    previewElement.appendChild(editorIconElement);
    assetElement.appendChild(previewElement);

    doc.appendChild(assetElement);

    dataSamplerElement.setAttribute("resourcemode", "shared");
    QDomText dataSamplerText = doc.createTextNode("default_sampler.xasset");
    dataSamplerElement.appendChild(dataSamplerText);

    dataImageElement.setAttribute("mipmap", "true");
    QDomText dataImageText = doc.createTextNode(dataFileName);
    dataImageElement.appendChild(dataImageText);


    QDomText editorIconImageText = doc.createTextNode(dataFileName + QString(":EDITOR_ICON"));
    editorIconImageElement.appendChild(editorIconImageText);

    QString xmlDoc = doc.toString(2);


    IFile *finalOutputFile = vkVFS::Get()->Open(vkString((const char*)absFileName.toLatin1()), eOM_Write, eTM_Binary);
    if (!finalOutputFile)
    {
      return false;
    }
    finalOutputFile->Write((const char*)xmlDoc.toLatin1(), xmlDoc.length());
    finalOutputFile->Close();
  }



  return true;
}

}