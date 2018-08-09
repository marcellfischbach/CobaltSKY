
#include <editor/assetmanager/import/assetmanagerimporterpage.hh>
#include <editor/assetmanager/import/assetmanagerimporter.hh>

AssetManagerImporterPage::AssetManagerImporterPage(QWidget *parent)
  : QWidget(parent)
  , m_importerData(0)
{
}

AssetManagerImporterPage::~AssetManagerImporterPage()
{
  delete m_importerData;
}

void AssetManagerImporterPage::Setup(AssetManagerImportData *data)
{
  m_importerData = data;
  m_name = data->GetName();
  m_import = true;

  m_gui.setupUi(this);
  m_gui.label ->setText(data->GetName());

  QFont font = m_gui.label->font();
  font.setPointSize(font.pointSize() * 2);
  QFontMetrics fm(font);
  int fontHeight = fm.height();

  m_gui.label->setFont(font);
  m_gui.label->setBackgroundRole(QPalette::Highlight);
  m_gui.label->setForegroundRole(QPalette::HighlightedText);
  m_gui.label->setMargin(fontHeight/2);

  m_gui.leName->setText(data->GetName());

  QGridLayout *layout = new QGridLayout(m_gui.widget);
  layout->setMargin(0);
  layout->setSpacing(0);

  layout->addWidget(data->GetWidget(), 0, 0, 1, 1);
}


AssetManagerImportData *AssetManagerImporterPage::GetData() const
{
  return m_importerData;
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
  m_importerData->SetName(name);
  emit Changed(this);
}
