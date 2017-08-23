#ifndef BASICDOCKITEM_HH
#define BASICDOCKITEM_HH

#include <abstractdockitem.hh>
#include <dockitems.hh>

class QGridLayout;
class QFrame;
class QLabel;
class BasicDockItem : public AbstractDockItem
{
public:
  BasicDockItem(const std::string &dockName, const QString &dockTitle, Qt::DockWidgetArea dockArea);
  virtual ~BasicDockItem();

  void SetContent(QWidget *widget);

  void SetEmptyContent() override;


private:
  QFrame *m_frame;
  QWidget *m_content;
  QGridLayout *m_gridLayout;
  QLabel *m_emptyLabel;
};
#endif // BASICDOCKITEM_HH
