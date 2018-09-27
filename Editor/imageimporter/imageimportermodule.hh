
#pragma once


#include <imageimporter/imageimporterexport.hh>

class IMAGEIMPORTER_API ImageImporterModule
{
public:
  static void Initialize(int argc, char **argv);

private:
  ImageImporterModule() { }
};