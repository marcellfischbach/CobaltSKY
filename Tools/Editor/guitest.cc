#include <guitest.hh>
#include <QFile>

GUITest::GUITest()
  : QMainWindow()
{
  m_gui.setupUi(this);

  QFile qssFile("D://Programming//CobaltSKY//Tools//Editor//cobalt.css");
  if (qssFile.open(QIODevice::ReadOnly))
  {
    QString qss = qssFile.readAll();
    qssFile.close();
    printf("StyleSheet:\n%s\n", (const char *)qss.toLatin1());
    setStyleSheet(qss);
  }
}

