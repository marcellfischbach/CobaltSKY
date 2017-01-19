
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagercontentmodel.hh>
#include <assetmanager/assetmanagerfoldermodel.hh>
#include <assetdescriptor.hh>
#include <editor.hh>
#include <QDomDocument>
#include <QFile>


AssetManagerWidget::AssetManagerWidget()
  : QWidget()
{
  m_gui.setupUi(this);

  m_folderModel = new AssetManagerFolderModel();
  m_contentModel = new AssetManagerContentModel();

  m_gui.treeView->setModel(m_folderModel);
  m_gui.listView->setModel(m_contentModel);



  QRect size = Editor::Get()->GetScreenSize();
  int width = size.width();
  int treeSize = 300;

  m_gui.splitter->setSizes(QList<int>() << treeSize << width - treeSize);
  m_gui.splitter->setStretchFactor(0, 0);
  m_gui.splitter->setStretchFactor(1, 1);
}

AssetManagerWidget::~AssetManagerWidget()
{

}


void AssetManagerWidget::on_treeView_activated(const QModelIndex &index)
{
  SelectIndex(index);
}

void AssetManagerWidget::on_treeView_clicked(const QModelIndex &index)
{
  SelectIndex(index);
}

void AssetManagerWidget::SelectIndex(const QModelIndex &index)
{
  QDir dir = m_folderModel->GetDir(index);
  m_contentModel->SetDir(dir);
}


void AssetManagerWidget::on_listView_doubleClicked(const QModelIndex &index)
{
  vkString fileName = m_contentModel->GetEntry(index);
  if (fileName.length() == 0)
  {
    return;
  }

  OpenAsset(fileName);
}

void AssetManagerWidget::OpenAsset(const vkString &fileName)
{
  QDomDocument doc("mydocument");
  QFile file(QString(fileName.c_str()));
  if (!file.open(QIODevice::ReadOnly))
  {
    return;
  }
  if (!doc.setContent(&file)) 
  {
    file.close();
    return;
  }
  file.close();

  QDomElement assetElement = doc.documentElement();
  if (assetElement.isNull() || assetElement.tagName() != QString("asset"))
  {
    return;
  }

  QDomElement dataElement = assetElement.firstChildElement("data");
  if (dataElement.isNull())
  {
    return;
  }

  QDomElement typeElement = dataElement.firstChildElement();
  vkString type = (const char*)typeElement.tagName().toLatin1();

  AssetDescriptor descriptor(fileName, type);
  Editor::Get()->OpenAsset(descriptor);
}




