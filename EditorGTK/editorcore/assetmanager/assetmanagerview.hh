
#ifndef __ASSETMANAGERVIEW_HH__
#define __ASSETMANAGERVIEW_HH__

#include <editorcore/iview.hh>

namespace editor
{

class AssetManagerView : public iView
{
public:
  virtual ~AssetManagerView();

  static AssetManagerView* Get();

public:
  virtual const Glib::ustring &GetName () const;
  virtual Gtk::Image *GetImage() const;
  virtual Gtk::Widget *GetWidget() const;

private:
  AssetManagerView();

private:
  Glib::ustring m_name;
  Gtk::Button *m_widget;
};

}

#endif // __ASSETMANAGERVIEW_HH__
