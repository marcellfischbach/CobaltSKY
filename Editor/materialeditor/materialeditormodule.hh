
#pragma once

#include <materialeditor/materialeditorexport.hh>

class MATERIALEDITOR_API MaterialEditorModule
{
public:
  static void Initialize(int argc, char **argv);

private:
  MaterialEditorModule()  { }
};