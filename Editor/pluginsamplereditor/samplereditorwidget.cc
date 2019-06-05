

#include <pluginsamplereditor/samplereditorwidget.hh>
#include <pluginsamplereditor/samplereditor.hh>
#include <pluginsamplereditor/samplereditorproperties.hh>

#include <editorcore/renderwidget.hh>

#include <QFrame>
#include <QGridLayout>
#include <QSplitter>
#include <QTimer>

namespace cs::editor::plugin::samplereditor
{

SamplerEditorWidget::SamplerEditorWidget(SamplerEditor *editor)
  : QWidget()
  , m_editor(editor)
{


  m_properties = new SamplerEditorProperties();

  core::RenderWidget* renderWidget = new core::RenderWidget(this);

  QSplitter* splitter = new QSplitter();
  splitter->addWidget(renderWidget);
  splitter->addWidget(m_properties);

  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 0);

  QGridLayout* layout = new QGridLayout(this);
  setLayout(layout);
  layout->addWidget(splitter, 0, 0);

  QTimer::singleShot(1, [this, splitter]() {
    ;
    splitter->setSizes((QList<int>() << width() - 300 << 300));
    });
}

}