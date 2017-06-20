#pragma once

#include <cobalt/core/csstring.hh>
#include <cobalt/cstypes.hh>
#include <set>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.refl.hh>
#include <map>

class csShaderGraphGL4;
class csSGNode;
class csSGOutput;
class csSGInput;
class csSGNodeGL4;

class csShaderGraphCtx
{
public:
  struct ExternalBinding
  {
    csString variableName;
    csString variableType;
    csSGNode *node;
    friend bool operator<(const ExternalBinding &b0, const ExternalBinding &b1)
    {
      return b0.variableName < b1.variableName;
    }
  };

public:
  csShaderGraphCtx(csShaderGraphGL4 *graph);

  csSGNodeGL4 *GetNode(csSGNode *node);
  csSGNodeGL4 *GetNode(csSGOutput *output);
  csSGNodeGL4 *GetNode(csSGInput *input);

  csString CreateCode(csSGOutput* outputs);

  void EvaluateInlines(std::set<csSGOutput*> outputs);
  void GenerateCode(std::set<csSGOutput*> outputs);

  void Evaluate(csSGNodeGL4 *node);
  void Evaluate(csSGNode *node);
  void Evaluate(csSGOutput *node);
  void Evaluate(csSGInput *node);

  void EvaluateInline(csSGNodeGL4 *node);
  void EvaluateInline(csSGNode *node);
  void EvaluateInline(csSGOutput *node);
  void EvaluateInline(csSGInput *node);

  void AddExpression(const csString &expression);
  csString AddAssignment(const csString &type, const csString &statement);
  void SetOutputValue(csSGOutput *output, const csString &value);

  bool HasOutputValue(csSGOutput *output) const;
  bool HasInputValue(csSGInput *input) const;
  csString GetOutputValue(csSGOutput *output);
  csString GetInputValue(csSGInput *input);

  csString GetFullOutputValue(csSGOutput *output);
  csString GetFullInputValue(csSGInput *input);

  csString GetNextVariable();

  void AddBinding(csSGNode *node, const csString &variableType, const csString &variableName);
  bool IsBindingApplyingFor(const csString &bindingName, csSGNode *node) const;
  bool IsBindingApplyingFor(const csString &bindingName, csSGOutput *output) const;
  bool IsBindingApplyingFor(const csString &bindingName, csSGInput *input) const;
  std::set<ExternalBinding> GetBindingsFor(std::set<csSGOutput*> outputs) const;

  void SetDefaultTextureCoordinate(const csString &defaultTextureCoordinat);
  const csString &GetDefaultTextureCoordinate() const;

  const csString &GetCode() const
  {
    return m_code;
  }

private:
  csShaderGraphGL4 *m_graph;

  csString m_code;
  csUInt32 m_variableCounter;
  csString m_defaultTextureCoordinate;
  std::map<csSGOutput *, csString> m_outputValue;
  std::map<csSGNode *, csSGNodeGL4*> m_nodes;
  std::map<csSGOutput *, csString> m_outputToVar;


  std::map<csString, ExternalBinding> m_unisformBindingNames;
};
