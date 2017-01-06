#pragma once

#include <valkyrie/core/vkstring.hh>
#include <valkyrie/vktypes.hh>
#include <set>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.refl.hh>
#include <map>

class vkShaderGraphGL4;
class vkSGNode;
class vkSGOutput;
class vkSGInput;
class vkSGNodeGL4;

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

  void EvaluateInlines(std::set<vkSGOutput*> outputs);
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
