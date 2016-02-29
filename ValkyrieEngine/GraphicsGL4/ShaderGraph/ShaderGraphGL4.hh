#pragma once


#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <map>
#include <set>
#include <GraphicsGL4/ShaderGraph/ShaderGraphGL4.refl.hh>

class vkSGNodeGL4;
class vkShaderGraphGL4;


class VKGL4_API vkShaderGraphGL4
{
public:
  vkShaderGraphGL4();
  ~vkShaderGraphGL4();


  vkString CreateCode(vkSGNode *node, vkSize output);
  vkString CreateCode(vkSGNode *node, const vkString &outputName);

  vkString CreateCode(vkSGOutput *output);

  vkSGNodeGL4 *CreateNode(const vkClass *clazz) const;


private:
  std::map<const vkClass*, const vkClass*> m_nodeMapping;

};

class vkShaderGraphCtx
{
public:
  vkShaderGraphCtx(vkShaderGraphGL4 *graph);

  vkSGNodeGL4 *GetNode(vkSGNode *node);
  vkSGNodeGL4 *GetNode(vkSGOutput *output);
  vkSGNodeGL4 *GetNode(vkSGInput *input);

  vkString CreateCode(vkSGOutput *output);

  void Evaluate(vkSGNodeGL4 *node);
  void Evaluate(vkSGNode *node);
  void Evaluate(vkSGOutput *node);
  void Evaluate(vkSGInput *node);

  void EvaluateInline(vkSGNodeGL4 *node);
  void EvaluateInline(vkSGNode *node);
  void EvaluateInline(vkSGOutput *node);
  void EvaluateInline(vkSGInput *node);

  void AddExpression(const vkString &expression);
  vkString AddAssignment(const vkString &type, const vkString &statement);
  void SetOutputValue(vkSGOutput *output, const vkString &value);

  vkString GetOutputValue(vkSGOutput *output);
  vkString GetInputValue(vkSGInput *input);

  vkString GetFullOutputValue(vkSGOutput *output);
  vkString GetFullInputValue(vkSGInput *input);

  vkString GetNextVariable();


private:
  vkShaderGraphGL4 *m_graph;

  vkString m_code;
  vkUInt32 m_variableCounter;
  std::map<vkSGOutput *, vkString> m_outputValue;
  std::map<vkSGNode *, vkSGNodeGL4*> m_nodes;
  std::map<vkSGOutput *, vkString> m_outputToVar;

};


VK_INTERFACE()
class VKGL4_API vkSGNodeGL4 : public vkObject
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

private:
  bool m_forceInline;
  bool m_inline;
  vkSGNode *m_node;
  bool m_inlineEvaluated;
  bool m_evaluated;

  vkString GetFloat(vkShaderGraphCtx &ctx, int x);
  vkString GetInt(vkShaderGraphCtx &ctx, int x);

private:
  void EvaluateConstFloat(vkShaderGraphCtx &ctx);
  void EvaluateConstFloat3(vkShaderGraphCtx &ctx);
  void EvaluateFloat3(vkShaderGraphCtx &ctx);
  void EvaluateAddFloat(vkShaderGraphCtx &ctx);
  void EvaluateAddFloat3(vkShaderGraphCtx &ctx);
  void EvaluateSplitFloat3(vkShaderGraphCtx &ctx);
};

