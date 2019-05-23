#pragma once


#include <editor/editorexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <QImage>
#include <editor/editorimage.refl.hh>

namespace cs
{
class Image;
}


CS_CLASS()
class EDITOR_API EditorImage : public CS_SUPER(cs::iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  EditorImage(cs::Image *image = 0);
  virtual ~EditorImage();

  void SetImage(cs::Image *image);

  QImage GetImage() const
  {
    return m_image;
  }
private:

  QImage m_image;
};

CS_CLASS()
class EDITOR_API EditorImageWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(EditorImage, EditorImageWrapper, cs::ResourceWrapper);
};


