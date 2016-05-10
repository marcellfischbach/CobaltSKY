#pragma once


#include <Valkyrie/Core/Object.hh>
#include <qimage.h>
#include <AssetManager/EditorIcon.refl.hh>

VK_CLASS()
class EditorIcon : public vkObject
{
  VK_CLASS_GEN;
public:
  EditorIcon();
  virtual ~EditorIcon();

  void SetImage(QImage image);
  QImage GetImage();
  const QImage GetImage() const;

private:
  QImage m_image;

};