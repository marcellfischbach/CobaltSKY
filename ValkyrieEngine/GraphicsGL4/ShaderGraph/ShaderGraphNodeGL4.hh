#pragma once


#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <map>
#include <set>
#include <GraphicsGL4/ShaderGraph/ShaderGraphNodeGL4.refl.hh>

class vkSGNodeGL4;
class vkShaderGraphGL4;


class vkShaderGraphCtx
{
public:
  struct ExternalBinding
  {
    vkString variableName;
    vkString variableType;
    vkSGNode *node;
    friend bool operator<(const ExternalBinding &b0, const ExternalBinding &b1)
    {
      return b0.variableName < b1.variableName;
    }
  };

public:
  vkShaderGraphCtx(vkShaderGraphGL4 *graph);

  vkSGNodeGL4 *GetNode(vkSGNode *node);
  vkSGNodeGL4 *GetNode(vkSGOutput *output);
  vkSGNodeGL4 *GetNode(vkSGInput *input);


  vkString CreateCode(vkSGOutput* outputs);

  void GenerateCode(std::set<vkSGOutput*> outputs);

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

  bool HasOutputValue(vkSGOutput *output) const;
  bool HasInputValue(vkSGInput *input) const;
  vkString GetOutputValue(vkSGOutput *output);
  vkString GetInputValue(vkSGInput *input);

  vkString GetFullOutputValue(vkSGOutput *output);
  vkString GetFullInputValue(vkSGInput *input);

  vkString GetNextVariable();

  void AddBinding(vkSGNode *node, const vkString &variableType, const vkString &variableName);
  bool IsBindingApplyingFor(const vkString &bindingName, vkSGNode *node) const;
  bool IsBindingApplyingFor(const vkString &bindingName, vkSGOutput *output) const;
  bool IsBindingApplyingFor(const vkString &bindingName, vkSGInput *input) const;
  std::set<ExternalBinding> GetBindingsFor(std::set<vkSGOutput*> outputs) const;

  void SetDefaultTextureCoordinate(const vkString &defaultTextureCoordinat);
  const vkString &GetDefaultTextureCoordinate() const;

  const vkString &GetCode() const
  {
    return m_code;
  }

private:
  vkShaderGraphGL4 *m_graph;

  vkString m_code;
  vkUInt32 m_variableCounter;
  vkString m_defaultTextureCoordinate;
  std::map<vkSGOutput *, vkString> m_outputValue;
  std::map<vkSGNode *, vkSGNodeGL4*> m_nodes;
  std::map<vkSGOutput *, vkString> m_outputToVar;


  std::map<vkString, ExternalBinding> m_unisformBindingNames;
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
  void EvaluateFloat2(vkShaderGraphCtx &ctx);
  void EvaluateFloat3(vkShaderGraphCtx &ctx);
  void EvaluateVarFloat(vkShaderGraphCtx &ctx);
  void EvaluateVarFloat2(vkShaderGraphCtx &ctx);
  void EvaluateVarFloat3(vkShaderGraphCtx &ctx);
  void EvaluateVarFloat4(vkShaderGraphCtx &ctx);
  void EvaluateArithFloat(vkShaderGraphCtx &ctx, const char *arith);
  void EvaluateArithFloat2(vkShaderGraphCtx &ctx, const char *arith);
  void EvaluateArithFloat3(vkShaderGraphCtx &ctx, const char *arith);
  void EvaluateArithFloat4(vkShaderGraphCtx &ctx, const char *arith);
  void EvaluateSplitFloat3(vkShaderGraphCtx &ctx);
  void EvaluateSplitFloat4(vkShaderGraphCtx &ctx);
  void EvaluateDefaultTextureCoordinate(vkShaderGraphCtx &ctx);
  void EvaluateTexture2D(vkShaderGraphCtx &ctx);
};

