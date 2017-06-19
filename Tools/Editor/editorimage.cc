
#include <editorimage.hh>
#include <valkyrie/graphics/csimage.hh>

EditorImage::EditorImage(csImage *image)
{
  CS_CLASS_GEN_CONSTR;
  SetImage(image);
}

EditorImage::~EditorImage()
{
}

void EditorImage::SetImage(csImage *image)
{
  if (!image)
  {
    m_image = QImage();
  }
  else
  {
    QImage::Format format;
    switch (image->GetPixelFormat())
    {
    case ePF_R8G8B8A8U:
      format = QImage::Format_ARGB32;
      break;
    case ePF_R8G8B8U:
      format = QImage::Format_RGB888;
      break;
    }
    m_image = QImage((const uchar *)image->GetData(), image->GetWidth(), image->GetHeight(), format);
  }
}