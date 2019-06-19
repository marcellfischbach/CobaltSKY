

#include <pluginsamplereditor/samplereditorwidget.hh>
#include <pluginsamplereditor/samplereditor.hh>
#include <pluginsamplereditor/samplereditorfile.hh>
#include <pluginsamplereditor/samplereditorproperties.hh>

#include <editorcore/editor.hh>
#include <editorcore/glcontext.hh>
#include <editorcore/renderwidget.hh>

#include <editormodel/model.hh>

#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/isampler.hh>

#include <QFrame>
#include <QGridLayout>
#include <QSplitter>
#include <QTimer>
#include <QToolBar>

namespace cs::editor::plugin::samplereditor
{

SamplerEditorWidget::SamplerEditorWidget(SamplerEditor* editor, iSampler* sampler)
  : QWidget()
  , m_editor(editor)
  , m_sampler(sampler)
  , m_editorSampler(nullptr)
{
  sampler->AddRef();

  m_editorSampler = Copy(sampler);

  m_properties = new SamplerEditorProperties();
  m_properties->Set(m_editorSampler);

  core::RenderWidget* renderWidget = new core::RenderWidget(this);

  QSplitter* splitter = new QSplitter();
  splitter->addWidget(renderWidget);
  splitter->addWidget(m_properties);

  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 0);

  QToolBar* toolbar = new QToolBar(this);
  QAction *action = toolbar->addAction(QIcon(":/icons32/save"), QString("Save"));
  connect(action, SIGNAL(triggered(bool)), this, SLOT(Save(bool)));


  QGridLayout* layout = new QGridLayout(this);
  layout->setContentsMargins(QMargins(0, 0, 0, 0));
  setLayout(layout);
  layout->addWidget(toolbar, 0, 0);
  layout->addWidget(splitter, 1, 0);

  QTimer::singleShot(1, [this, splitter]() {
    ;
    splitter->setSizes((QList<int>() << width() - 300 << 300));
    });
}


SamplerEditorWidget::~SamplerEditorWidget()
{
  CS_RELEASE(m_sampler);
  CS_RELEASE(m_editorSampler);
}


void SamplerEditorWidget::Save(bool)
{

  if (!core::GLContext::Get().MakeCurrent())
  {
    return;
  }

  m_properties->Get(m_editorSampler);
  Copy(m_editorSampler, m_sampler);


  // 
  // Sync the sampler to the anon node reference if they are the same
  model::Model* model = core::Editor::Get()->GetModel();
  model::AssetNode* assetNode = m_editor->GetAsset();
  if (model->IsEngineFinalNode(assetNode))
  {
    cs::ResourceLocator anon = assetNode->GetResourceLocator().AsAnonymous();
    cs::SamplerWrapper *samplerWrapper = cs::ResourceManager::Get()->Get<cs::SamplerWrapper>(anon);
    if (samplerWrapper)
    {
      Copy(m_editorSampler, samplerWrapper->Get());
    }
  }

  SamplerEditorFile::Save(m_editorSampler, assetNode->GetResourceLocator());


}


iSampler* SamplerEditorWidget::Copy(const iSampler* sampler)
{
  iSampler* copy = core::Editor::Get()->GetGraphics()->CreateSampler();
  return Copy(sampler, copy);
}

iSampler *SamplerEditorWidget::Copy(const iSampler *src, iSampler *dst)
{
  dst->SetFilter(src->GetFilter());
  dst->SetAnisotropy(src->GetAnisotropy());
  dst->SetMinLOD(src->GetMinLOD());
  dst->SetMaxLOD(src->GetMaxLOD());
  dst->SetAddressU(src->GetAddressU());
  dst->SetAddressV(src->GetAddressV());
  dst->SetAddressW(src->GetAddressW());
  dst->SetBorderColor(src->GetBorderColor());
  dst->SetTextureCompareMode(src->GetTextureCompareMode());
  dst->SetTextureCompareFunc(src->GetTextureCompareFunc());
  return dst;

}

}