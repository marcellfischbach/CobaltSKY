
#include <editor/assetmanager/actions/import/assetmanagerimporterpage.hh>
#include <editor/iassetimporter.hh>

AssetManagerImporterPage::AssetManagerImporterPage(QWidget *parent)
  : QWidget(parent)
  , m_importer(0)
{
}

AssetManagerImporterPage::~AssetManagerImporterPage()
{
  delete m_importer;
}

void AssetManagerImporterPage::Setup(iAssetImporter *importer)
{
  std::filesystem::path path = importer->GetFile();
  std::string fileName = path.filename().string();
  std::string extension = path.extension().string();
  fileName = fileName.substr(0, fileName.length() - extension.length());
  m_importer = importer;
  m_name = QString(fileName.c_str());
  m_import = true;

  m_gui.setupUi(this);
  m_gui.label ->setText(QString(fileName.c_str()));

  QFont font = m_gui.label->font();
  font.setPointSize(font.pointSize() * 2);
  QFontMetrics fm(font);
  int fontHeight = fm.height();

  m_gui.label->setFont(font);
  m_gui.label->setBackgroundRole(QPalette::Highlight);
  m_gui.label->setForegroundRole(QPalette::HighlightedText);
  m_gui.label->setMargin(fontHeight/2);

  m_gui.leName->setText(QString(fileName.c_str()));

  QGridLayout *layout = new QGridLayout(m_gui.widget);
  layout->setMargin(0);
  layout->setSpacing(0);

  QWidget *widget = importer->GetWidget();
  if (widget)
  {
    layout->addWidget(widget, 0, 0, 1, 1);
  }
}


iAssetImporter *AssetManagerImporterPage::GetImporter() const
{
  return m_importer;
}

const QString &AssetManagerImporterPage::GetName() const
{
  return m_name;
}

bool AssetManagerImporterPage::IsImporting() const
{
  return m_import;
}

void AssetManagerImporterPage::on_cbImport_stateChanged(int state)
{
  m_import = state == Qt::Checked;
  QFont fnt = m_gui.label->font();
  fnt.setItalic(!m_import);
  m_gui.label->setFont(fnt);
  emit Changed(this);
}

void AssetManagerImporterPage::on_leName_textChanged(const QString &name)
{
  m_gui.label->setText(name);
  m_name = name;
  m_importer->SetImportName(std::string(name.toLatin1().data()));
  emit Changed(this);
}
