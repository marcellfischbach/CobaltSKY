
#ifndef __IVIEW_HH__
#define __IVIEW_HH__

#include <gtkmm.h>

namespace editor
{


struct iView
{

  virtual const Glib::ustring &GetName () const = 0;

  virtual Gtk::Image *GetImage() const = 0;

  virtual Gtk::Widget *GetWidget() const = 0;

};


}


#endif // __IVIEW_HH__

