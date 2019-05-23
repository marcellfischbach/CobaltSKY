#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <map>

class csSGNodeGL4;

namespace cs
{
class GraphicsGL4;
}

class CSGRAPHICSGL4_API csShaderGraphGL4 : public CS_SUPER(cs::iSGShaderGraphFactory)
{
public:
  csShaderGraphGL4(cs::GraphicsGL4 *graphics);
  ~csShaderGraphGL4();


  std::string CreateCode(cs::SGNode *node, csSize output);
  std::string CreateCode(cs::SGNode *node, const std::string &outputName);

  std::string CreateCode(cs::SGOutput *output);


  csSGNodeGL4 *CreateNode(const cs::Class *nodeClass);


  virtual bool GenerateShaderGraph(cs::SGShaderGraph *graph, cs::iSGShaderGraphLogger *logger);

private:
  void GenerateGBuffer(cs::SGShaderGraph *graph, cs::iSGShaderGraphLogger *logger);
  void GenerateShadow(cs::SGShaderGraph *graph, unsigned layers, cs::eRenderPass renderPass, cs::iSGShaderGraphLogger *logger);

  void DebugCode(const std::string &title, const std::string &source);

private:
  struct Map 
  {
    const cs::Class *nodeClass;
    const cs::Class *nodeClassGL4;
    Map(const cs::Class *nodeClass, const cs::Class *nodeClassGL4)
      : nodeClass(nodeClass)
      , nodeClassGL4(nodeClassGL4)
    {

    }
  };
  cs::GraphicsGL4 *m_graphics;
  std::vector<Map> m_classMapping;
  //std::map<const cs::Class*, const cs::Class*> m_classMapping;
};
