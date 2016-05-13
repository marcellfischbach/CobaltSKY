

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


    vkAssetOutputStream os(buffer.length());
    os << (vkUInt32)VK_VERSION(1, 0, 0)
      << (vkUInt8)eRLM_Inline // we provide the sampler data here >> this must be changed later
      // << (vkString)"Path/To/The/Sampler/Asset" // if the param above would be 0
      << (vkUInt8)eFM_MinMagMipLinear // texture access filtering
      << (vkUInt8)1 // anisotropy
      << (vkInt16)-1000 // min lod
      << (vkInt16)1000 // max lod
      << (vkUInt8)eTAM_Repeat // clamp U
      << (vkUInt8)eTAM_Repeat // clamp V
      << (vkUInt8)eTAM_Repeat // clamp W
      << vkColor4f(0.0f, 0.0f, 0.0f, 0.0f)
      << (vkUInt8)eTCM_None
      << (vkUInt8)eTCF_Less;

    os << vkString((const char*)format.toLatin1())
      << (vkUInt32)buffer.length();
    os.Write(buffer.data(), buffer.length());

      


    writer.AddEntry("TEXTURE2D", "DATA", os.GetSize(), os.GetBuffer());
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
    writer.AddEntry("TEXTURE2D", "EDITOR_ICON", buffer.length(), (vkUInt8*)buffer.data());
  }



  QString fileName = info.completeBaseName() + ".asset";
  QString absFileName = outputDir.absoluteFilePath(fileName);


  QFileInfo rootPathInfo(vkVFS::Get()->GetRootPath().c_str());
  QString rootPath = rootPathInfo.absoluteFilePath();

  if (absFileName.startsWith(rootPath, Qt::CaseInsensitive))
  {
    absFileName = absFileName.right(absFileName.length() - rootPath.length());
  }

  IFile *finalOutputFile = vkVFS::Get()->Open(vkString((const char*)absFileName.toLatin1()), eOM_Write, eTM_Binary);
  if (finalOutputFile)
  {
    writer.Output(finalOutputFile);
    finalOutputFile->Close();
    return true;
  }
  

  return false;
}

}