
#pragma once

namespace asset::model
{
  class Asset;
  class Folder;
  class Model;
}

namespace asset::model::sync
{
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
