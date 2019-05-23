
#include <editor/editorimage.hh>
#include <cobalt/graphics/csimage.hh>

EditorImage::EditorImage(cs::Image *image)
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
  printf("Cleanupat: %p\n", imageBuffer);
  delete[] imageBuffer;
}

void EditorImage::SetImage(cs::Image *image)
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
    case cs::ePF_R8G8B8A8U:
      format = QImage::Format_RGBA8888;
      break;
    case cs::ePF_R8G8B8U:
      format = QImage::Format_RGB888;
      break;
    }
    printf("Buffer at: %p\n", buffer);
    m_image = QImage((const uchar *)buffer, image->GetWidth(), image->GetHeight(), format, editor_image_clean_up, buffer);
  }
}