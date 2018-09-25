

#include <editor/abstractasseteditor.hh>
#include <editor/assetmanager/assetmanagerassetwriter.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/editor.hh>
#include <editor/project/project.hh>
#include <editor/mainwindow.hh>
#include <editor/eventbus.hh>
#include <editor/events/assetpreviewiconchangedevent.hh>
#include <editor/events/assetrenamedevent.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>

#include <QBuffer>
#include <QFile>
#include <QImage>
#include <QImageWriter>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>

void abstract_asset_editor_asset_renamed(csEvent &event, void *userData)
{
	AssetRenamedEvent &evt = static_cast<AssetRenamedEvent&>(event);
	AbstractAssetEditor *editor = reinterpret_cast<AbstractAssetEditor*>(userData);
	editor->ResourceRenamed(evt.GetFrom(), evt.GetTo());
}


AbstractAssetEditor::AbstractAssetEditor()
	: iAssetEditor()
	, m_dirty(true)
	, m_editObject(0)
{
	EventBus::Get().Register(AssetRenamedEvent::GetStaticClass(), abstract_asset_editor_asset_renamed, this);
}

AbstractAssetEditor::~AbstractAssetEditor()
{
	EventBus::Get().Deregister(abstract_asset_editor_asset_renamed, this);
}

void AbstractAssetEditor::SetObject(iObject *object, asset::model::Asset *asset)
{
	CS_SET(m_editObject, object);
	m_asset = asset;
	UpdateName();
	UpdateAsset();
}

void AbstractAssetEditor::UpdateName()
{

	csFileInfo info(m_asset->GetResourceLocator().GetResourceFile());
	m_name = info.GetName();

}

iObject *AbstractAssetEditor::GetEditObject()
{
	return m_editObject;
}

const iObject *AbstractAssetEditor::GetEditObject() const
{
	return m_editObject;
}



asset::model::Asset *AbstractAssetEditor::GetAsset()
{
	return m_asset;
}

const asset::model::Asset *AbstractAssetEditor::GetAsset() const
{
	return m_asset;
}



void AbstractAssetEditor::SetWidget(QWidget *widget)
{
	m_widget = widget;
}

QWidget *AbstractAssetEditor::GetWidget()
{
	return m_widget;
}

const std::string &AbstractAssetEditor::GetName() const
{
	return m_name;
}

void AbstractAssetEditor::SetDirty(bool dirty)
{
	if (m_dirty != dirty)
	{
		m_dirty = dirty;
		UpdateMainWindow();
	}
}

bool AbstractAssetEditor::IsDirty() const
{
	return m_dirty;
}

void AbstractAssetEditor::CloseRequest()
{
	if (m_dirty)
	{
		int res = QMessageBox::question(Editor::Get()->GetMainWindow(),
			QObject::tr("CobaltSKY Editor"),
			QObject::tr("Close without saving?"),
			QMessageBox::Yes,
			QMessageBox::Cancel);
		if (res != QMessageBox::Yes)
		{
			return;
		}
	}

	Editor::Get()->GetMainWindow()->CloseEditor(this, true);
}


QString AbstractAssetEditor::GetResourceFileName() const
{
	const csResourceLocator &loc = GetAsset()->GetResourceLocator();
	std::string absFileName = csVFS::Get()->GetAbsolutePath(loc.GetResourceFile(), loc.GetResourceEntry());
	QString fileName(absFileName.c_str());
	return fileName;
}


void AbstractAssetEditor::ResourceRenamed(const csResourceLocator &from, const csResourceLocator &to)
{
	UpdateName();
	UpdateMainWindow();
}


void AbstractAssetEditor::ReplacePreviewIcon(QImage image)
{
	/*
	csResourceLocator dataLocator = m_assetDescriptor.GetLocator().AsData();

	printf("image: %s\n", dataLocator.GetDebugName().c_str());
	iFile *file = csVFS::Get()->Open(dataLocator, eOM_ReadWrite);

	AssetManagerAssetWriter writer;
	writer.Import(file);
	file->Close();
	writer.RemoveEntry("EDITOR_ICON");

	QBuffer buffer;
	QImageWriter qimagewriter(&buffer, QString("PNG").toLatin1());
	qimagewriter.write(image);

	writer.AddEntry(
		"EDITOR_ICON",
		std::string("PNG"),
		buffer.data().length(),
		reinterpret_cast<const csUInt8*>(buffer.data().constData())
	);


	iFile *dataFile = csVFS::Get()->Open(dataLocator, eOM_Write);
	if (dataFile)
	{
		writer.Output(dataFile);
		dataFile->Close();
	}

	std::string fileName = csVFS::Get()->GetAbsolutePath(m_assetDescriptor.GetLocator(), csVFS::CheckExistence);
	QFile assetFile(QString(fileName.c_str()));
	QDomDocument doc;
	if (doc.setContent(&assetFile))
	{
		if (assetFile.isOpen())
		{
			assetFile.close();
		}
		QDomElement assetElement = doc.firstChildElement("asset");
		if (!assetElement.isNull())
		{
			QDomElement previewElement = doc.createElement("preview");
			QDomElement editorIconElement = doc.createElement("editorIcon");
			QDomElement imageElement = doc.createElement("image");
			previewElement.appendChild(editorIconElement);
			editorIconElement.appendChild(imageElement);
			imageElement.appendChild(doc.createTextNode(QString("%1:EDITOR_ICON").arg(QString(dataLocator.GetResourceFile().c_str()))));


			QDomElement currentPreviewElement = assetElement.firstChildElement("preview");
			if (currentPreviewElement.isNull())
			{
				assetElement.appendChild(previewElement);
			}
			else
			{
				assetElement.replaceChild(previewElement, currentPreviewElement);
			}
		}

		QString xmlText = doc.toString(2);
		if (assetFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			assetFile.write(xmlText.toLatin1());
		}
	}
	if (assetFile.isOpen())
	{
		assetFile.close();
	}
	AssetPreviewIconChangedEvent evt(m_assetDescriptor.GetLocator());
	EventBus::Get() << evt;
	*/
}

void AbstractAssetEditor::UpdateMainWindow()
{
	Editor::Get()->GetMainWindow()->UpdateEditor(this);
}

