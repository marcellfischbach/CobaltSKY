
#include <editorimage.hh>
#include <cobalt/graphics/csimage.hh>

EditorImage::EditorImage(csImage *image)
{
  CS_CLASS_GEN_CONSTR;
  SetImage(image);
}

EditorImage::~EditorImage()
{
}

void editor_image_clean_up(void *data)
{
  unsigned char *imageBuffer = reinterpret_cast<unsigned char*>(data);
  delete[] imageBuffer;
}

void EditorImage::SetImage(csImage *image)
{
  if (!image)
  {
    m_image = QImage();
  }
  else
  {
    size_t size = image->GetSize();
    unsigned char *buffer = new unsigned char [size];
    memcpy(buffer, image->GetData(), size);

    QImage::Format format;
    switch (image->GetPixelFormat())
    {
    case ePF_R8G8B8A8U:
      format = QImage::Format_RGBA8888;
      break;
    case ePF_R8G8B8U:
      format = QImage::Format_RGB888;
      break;
    }
    m_image = QImage((const uchar *)buffer, image->GetWidth(), image->GetHeight(), format, editor_image_clean_up, buffer);
  }
}