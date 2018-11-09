#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.refl.hh>

class csShaderGraphCtx;



CS_INTERFACE()
class CSGRAPHICSGL4_API csSGNodeGL4 : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csSGNodeGL4();
  virtual ~csSGNodeGL4();

  void SetNode(csSGNode *node)
  {
    m_node = node;
  }

  virtual bool EvaluateInline(csShaderGraphCtx &ctx);
  virtual bool Evaluate(csShaderGraphCtx &ctx);

  csSGNode* GetNode()
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


  std::string AssignOutput(csShaderGraphCtx &ctx, csSGOutput *output, const std::string &exp, const std::string &type);

  static csSGDataType GetHigher(csSGDataType dtA, csSGDataType dtB);
  static std::string GetDataTypeVar(csSGDataType dt);

protected:
  virtual void PrivEvaluate(csShaderGraphCtx &ctx) = 0;
  void SetDoubleInlineEvaluateInput();

private:
  bool m_forceInline;
  bool m_inline;
  csSGNode *m_node;
  bool m_inlineEvaluated;
  bool m_evaluated;
  bool m_doubleInlineEvaluateInput;

  std::string GetFloat(csShaderGraphCtx &ctx, int x);
  std::string GetInt(csShaderGraphCtx &ctx, int x);

};
