
#include <RenderGL4/Deferred/DeferredFrameProcessor.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/Scene/Node.hh>


vkDeferredFrameProcessor::vkDeferredFrameProcessor()
  : IFrameProcessor()
{
  VK_CLASS_GEN_CONSTR;
}

vkDeferredFrameProcessor::~vkDeferredFrameProcessor()
{

}

void vkDeferredFrameProcessor::Render(vkNode *node, IRenderTarget *target)
{

}