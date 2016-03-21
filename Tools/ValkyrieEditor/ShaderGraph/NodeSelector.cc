
#include <ShaderGraph/NodeSelector.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>

#include <qlayout.h>
#include <qtimer.h>

NodeSelector::NodeSelector(QWidget *parent)
  : QDialog(parent)
{
  setModal(true);


  m_listView = new QListView(this);

  QGridLayout *layout = new QGridLayout(this);
  layout->setMargin(0);
  layout->addWidget(m_listView, 0, 0, 1, 1);

  QTimer::singleShot(2000, this, SLOT(timed()));
}

void NodeSelector::timed()
{
  printf ("Timedout\n");
  fflush(stdout);

  emit(addNode(vkSGConstFloat4::GetStaticClass()));
}

