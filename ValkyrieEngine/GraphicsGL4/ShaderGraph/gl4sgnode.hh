#pragma once

#include <GraphicsGL4/gl4export.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgnode.refl.hh>

class vkShaderGraphCtx;



VK_INTERFACE()
class VKGRAPHICSGL4_API vkSGNodeGL4 : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkSGNodeGL4();
  virtual ~vkSGNodeGL4();

  void SetNode(vkSGNode *node)
  {
    m_node = node;
  }

  virtual bool EvaluateInline(vkShaderGraphCtx &ctx);
  virtual bool Evaluate(vkShaderGraphCtx &ctx);

  vkSGNode* GetNode()
  {
    return m_node;
  }

  void SetForceInline(bool forceInline)
  {
    m_forceInline = forceInline;
  }
  void SetInline(bool iinline)
  {
    m_inline = iinline;
  }
  bool IsInline() const
  {
    return m_forceInline || m_inline;
  }


  vkString AssignOutput(vkShaderGraphCtx &ctx, vkSGOutput *output, const vkString &exp, const vkString &type);

  static vkSGDataType GetHigher(vkSGDataType dtA, vkSGDataType dtB);
  static vkString GetDataTypeVar(vkSGDataType dt);

protected:
  virtual void PrivEvaluate(vkShaderGraphCtx &ctx) = 0;
  void SetDoubleInlineEvaluateInput();

private:
  bool m_forceInline;
  bool m_inline;
  vkSGNode *m_node;
  bool m_inlineEvaluated;
  bool m_evaluated;
  bool m_doubleInlineEvaluateInput;

  vkString GetFloat(vkShaderGraphCtx &ctx, int x);
  vkString GetInt(vkShaderGraphCtx &ctx, int x);

};
