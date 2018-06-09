
#pragma once

namespace asset::model
{
  class Asset;
  class Folder;
  class Model;
  class ModelScanner
  {
  public:
    ModelScanner(Model *model);
    ~ModelScanner();

    void Scan();
    void Scan(Folder *folder);
    void Scan(Asset *asset);
	
  private:
    Model * m_model;
  };
}
