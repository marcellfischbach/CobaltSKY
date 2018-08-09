
#pragma once

#include <editor/editorexport.hh>

namespace asset::model
{
  class Asset;
  class Folder;
  class Model;
  class ModelScanner
  {
  public:
    ModelScanner(asset::model::Model *model);
    ~ModelScanner();

    void Scan();
    void Scan(asset::model::Folder *folder);
    void Scan(asset::model::Asset *asset);
	
  private:
    asset::model::Model * m_model;
  };
}
