#pragma once

#include <graphicsgl4/gl4export.hh>
#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <map>
#include <graphicsgl4/shadergraph/gl4sgshadergraph.refl.hh>

class vkSGNodeGL4;
class vkGraphicsGL4;

class VKGRAPHICSGL4_API vkShaderGraphGL4 : public VK_SUPER(ISGShaderGraphFactory)
{
public:
  vkShaderGraphGL4(vkGraphicsGL4 *graphics);
  ~vkShaderGraphGL4();


  vkString CreateCode(vkSGNode *node, vkSize output);
  vkString CreateCode(vkSGNode *node, const vkString &outputName);

  vkString CreateCode(vkSGOutput *output);


  vkSGNodeGL4 *CreateNode(const vkClass *nodeClass);


  virtual bool GenerateShaderGraph(vkSGShaderGraph *graph, iSGShaderGraphLogger *logger);

private:
  void GenerateGBuffer(vkSGShaderGraph *graph, iSGShaderGraphLogger *logger);
  void GenerateShadow(vkSGShaderGraph *graph, unsigned layers, vkRenderPass renderPass, iSGShaderGraphLogger *logger);

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
