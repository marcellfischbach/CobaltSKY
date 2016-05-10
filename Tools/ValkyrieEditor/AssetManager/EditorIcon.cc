

#include <AssetManager/EditorIcon.hh>


EditorIcon::EditorIcon()
  : vkObject()
{

}

EditorIcon::~EditorIcon()
{
}

void EditorIcon::SetImage(QImage image)
{
  m_image = image;
}

QImage EditorIcon::GetImage()
{
  return m_image;
}

const QImage EditorIcon::GetImage() const
{
  return m_image;
}
