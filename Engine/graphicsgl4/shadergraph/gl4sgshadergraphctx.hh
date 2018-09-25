#pragma once

#include <cobalt/cstypes.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.refl.hh>
#include <string>
#include <set>
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
    std::string variableName;
    std::string variableType;
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

  std::string CreateCode(csSGOutput* outputs);

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

  void AddExpression(const std::string &expression);
  std::string AddAssignment(const std::string &type, const std::string &statement);
  void SetOutputValue(csSGOutput *output, const std::string &value);

  bool HasOutputValue(csSGOutput *output) const;
  bool HasInputValue(csSGInput *input) const;
  std::string GetOutputValue(csSGOutput *output);
  std::string GetInputValue(csSGInput *input);

  std::string GetFullOutputValue(csSGOutput *output);
  std::string GetFullInputValue(csSGInput *input);

  std::string GetNextVariable();

  void AddBinding(csSGNode *node, const std::string &variableType, const std::string &variableName);
  bool IsBindingApplyingFor(const std::string &bindingName, csSGNode *node) const;
  bool IsBindingApplyingFor(const std::string &bindingName, csSGOutput *output) const;
  bool IsBindingApplyingFor(const std::string &bindingName, csSGInput *input) const;
  std::set<ExternalBinding> GetBindingsFor(std::set<csSGOutput*> outputs) const;

  void SetDefaultTextureCoordinate(const std::string &defaultTextureCoordinat);
  const std::string &GetDefaultTextureCoordinate() const;

  const std::string &GetCode() const
  {
    return m_code;
  }

private:
  csShaderGraphGL4 *m_graph;

  std::string m_code;
  csUInt32 m_variableCounter;
  std::string m_defaultTextureCoordinate;
  std::map<csSGOutput *, std::string> m_outputValue;
  std::map<csSGNode *, csSGNodeGL4*> m_nodes;
  std::map<csSGOutput *, std::string> m_outputToVar;


  std::map<std::string, ExternalBinding> m_unisformBindingNames;
};
