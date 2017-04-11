#pragma once


#include <valkyrie/core/vkclass.hh>
#include <QImage>
#include <editorimage.refl.hh>

class vkImage;


VK_CLASS()
class EditorImage : public VK_SUPER(iObject)
{
  VK_CLASS_GEN_OBJECT;
public:
  EditorImage(vkImage *image = 0);
  virtual ~EditorImage();

  void SetImage(vkImage *image);

  QImage GetImage() const
  {
    return m_image;
  }
private:

  QImage m_image;
};