#pragma once

#include <textureeditor/textureeditorexport.hh>
#include <editor/renderwidget.hh>
#include <cobalt/graphics/csshaderattributeid.hh>


class csTexture2DWrapper;
struct iShader;
struct iVertexDeclaration;
struct iVertexBuffer;
struct iIndexBuffer;
class TextureEditorRenderWidget : public RenderWidget
{
public:
  TextureEditorRenderWidget(QWidget *parent);
  ~TextureEditorRenderWidget();

  void SetTexture(csTexture2DWrapper *texture);
  void SetLOD(int lod);
  void SetRed(bool red);
  void SetGreen(bool green);
  void SetBlue(bool blue);
  void SetAlpha(bool alpha);

protected:
  void initializeGL();
  void paintGL();

private:
  csTexture2DWrapper *m_texture;


  iShader *m_shader;
  iVertexDeclaration *m_vertexDeclaration;
  iVertexBuffer *m_vertexBuffer;
  iIndexBuffer *m_indexBuffer;

  csShaderAttributeID m_idOffset;
  csShaderAttributeID m_idScale;
  csShaderAttributeID m_idDiffuse;
  csShaderAttributeID m_idLOD;
  csShaderAttributeID m_idColorTrans;

  float m_lod;
  bool m_red;
  bool m_green;
  bool m_blue;
  bool m_alpha;

};