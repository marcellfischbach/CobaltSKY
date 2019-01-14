#include <editorcore/assetmanager/assetmanagerview.hh>

namespace editor
{

AssetManagerView::AssetManagerView()
  : iView ()
{
  m_name = "Assets";
  m_widget = new Gtk::Button("AssetManager");
}

AssetManagerView::~AssetManagerView()
{

}

AssetManagerView *AssetManagerView::Get()
{
  static AssetManagerView assetManagerView;
  return &assetManagerView;
}


const Glib::ustring &AssetManagerView::GetName () const
{
  return m_name;
}

Gtk::Image *AssetManagerView::GetImage() const
{
  return nullptr;
}

Gtk::Widget *AssetManagerView::GetWidget() const
{
  return m_widget;
}


}
