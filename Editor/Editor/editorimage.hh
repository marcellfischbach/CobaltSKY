#pragma once


#include <cobalt/core/csclass.hh>
#include <QImage>
#include <editorimage.refl.hh>

class csImage;


CS_CLASS()
class EditorImage : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  EditorImage(csImage *image = 0);
  virtual ~EditorImage();

  void SetImage(csImage *image);

  QImage GetImage() const
  {
    return m_image;
  }
private:

  QImage m_image;
};