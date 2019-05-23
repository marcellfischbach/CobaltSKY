#pragma once

#include <graphicsgl4/gl4export.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.refl.hh>

namespace cs
{
class ShaderGraphCtx;


CS_CLASS()
class CSGRAPHICSGL4_API SGNodeGL4 : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  SGNodeGL4();
  virtual ~SGNodeGL4();

  void SetNode(cs::SGNode * node)
  {
    m_node = node;
  }

  virtual bool EvaluateInline(cs::ShaderGraphCtx & ctx);
  virtual bool Evaluate(cs::ShaderGraphCtx & ctx);

  cs::SGNode* GetNode()
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


  std::string AssignOutput(cs::ShaderGraphCtx & ctx, cs::SGOutput * output, const std::string & exp, const std::string & type);

  static cs::eSGDataType GetHigher(cs::eSGDataType dtA, cs::eSGDataType dtB);
  static std::string GetDataTypeVar(cs::eSGDataType dt);

protected:
  virtual void PrivEvaluate(cs::ShaderGraphCtx & ctx) = 0;
  void SetDoubleInlineEvaluateInput();

private:
  bool m_forceInline;
  bool m_inline;
  cs::SGNode* m_node;
  bool m_inlineEvaluated;
  bool m_evaluated;
  bool m_doubleInlineEvaluateInput;

  std::string GetFloat(cs::ShaderGraphCtx & ctx, int x);
  std::string GetInt(cs::ShaderGraphCtx & ctx, int x);

};

}