
#include <imageimporter/imageimporterexport.hh>
#include <editor/iassetimporter.hh>

#include <filesystem>
#include <imageimporter/imageimporter.refl.hh>

CS_INTERFACE()
class IMAGEIMPORTER_API ImageImporter : public iAssetImporter
{
  CS_CLASS_GEN_OBJECT;
public:
  ImageImporter(const std::filesystem::path &path);

  virtual ~ImageImporter() { }

  virtual const std::filesystem::path &GetFile() const;

  virtual const std::string GetType() const;

  virtual QWidget *GetWidget();

  virtual const std::string &GetImportName() const;
  virtual void SetImportName(const std::string &importName);

  virtual void SetEnabled(bool enable);

  virtual csResourceLocator Import(const asset::model::Folder *folder);

private:
  std::filesystem::path m_path;
  std::string m_importName;
};

