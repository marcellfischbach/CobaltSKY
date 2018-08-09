

#pragma once

#include <QMainWindow>
#include <ui_guitest.h>

class GUITest : public QMainWindow
{
  Q_OBJECT
public:
  GUITest();

private:
  Ui::GUITest m_gui;
};