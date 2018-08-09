

#pragma once

#include <editor/editorexport.hh>
#include <QMainWindow>
#include <editor/ui_guitest.h>

class GUITest : public QMainWindow
{
  Q_OBJECT
public:
  GUITest();

private:
  Ui::GUITest m_gui;
};