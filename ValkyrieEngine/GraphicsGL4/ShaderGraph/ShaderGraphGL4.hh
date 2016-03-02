#pragma once

#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <map>

class vkSGNodeGL4;
class vkGraphicsGL4;

class VKGL4_API vkShaderGraphGL4 : public ISGShaderGraphFactory
{
public:
  vkShaderGraphGL4(vkGraphicsGL4 *graphics);
  ~vkShaderGraphGL4();


  vkString CreateCode(vkSGNode *node, vkSize output);
  vkString CreateCode(vkSGNode *node, const vkString &outputName);

  vkString CreateCode(vkSGOutput *output);


  vkSGNodeGL4 *CreateNode(const vkClass *nodeClass);


  virtual bool GenerateShaderGraph(vkSGShaderGraph *graph);

private:
  void GenerateGBuffer(vkSGShaderGraph *graph);

private:
  vkGraphicsGL4 *m_graphics;
  std::map<const vkClass*, const vkClass*> m_classMapping;
};
