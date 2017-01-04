#pragma once

#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgshadergraph.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgnode.hh>
#include <map>

class vkSGNodeGL4;
class vkGraphicsGL4;

class VKGL4_API vkShaderGraphGL4 : public VK_SUPER(ISGShaderGraphFactory)
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
  void GenerateShadow(vkSGShaderGraph *graph, unsigned layers, vkRenderPass renderPass);

  void DebugCode(const vkString &title, const vkString &source);

private:
  struct Map 
  {
    const vkClass *nodeClass;
    const vkClass *nodeClassGL4;
    Map(const vkClass *nodeClass, const vkClass *nodeClassGL4)
      : nodeClass(nodeClass)
      , nodeClassGL4(nodeClassGL4)
    {

    }
  };
  vkGraphicsGL4 *m_graphics;
  std::vector<Map> m_classMapping;
  //std::map<const vkClass*, const vkClass*> m_classMapping;
};
