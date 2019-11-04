#pragma once

#include <QOpenGLWidget>

namespace cs
{
struct iTexture;
struct iShader;
}

namespace cs::editor::plugin::textureeditor
{

class TextureEditorPreview : public QOpenGLWidget
{
public:
  TextureEditorPreview(QWidget * parent = nullptr);

  void SetTexture (iTexture *texture);

protected:
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;

private:
  iTexture* m_texture;
};

}
