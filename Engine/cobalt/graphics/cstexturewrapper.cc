
#include <cobalt/graphics/cstexturewrapper.hh>


csTextureWrapper::csTextureWrapper(iTexture *texture)
  : csResourceWrapper(texture)
  , m_texture(texture)
{
}

csTextureWrapper::~csTextureWrapper()
{

}

csTexture2DWrapper::csTexture2DWrapper(iTexture2D *texture)
  : csTextureWrapper(texture)
  , m_texture2D(texture)
{
}

csTexture2DWrapper::~csTexture2DWrapper()
{

}

csTexture2DArrayWrapper::csTexture2DArrayWrapper(iTexture2DArray *texture)
  : csTextureWrapper(texture)
  , m_texture2DArray(texture)
{
}

csTexture2DArrayWrapper::~csTexture2DArrayWrapper()
{

}

csTextureCubeWrapper::csTextureCubeWrapper(iTextureCube *texture)
  : csTextureWrapper(texture)
  , m_textureCube(texture)
{
}

csTextureCubeWrapper::~csTextureCubeWrapper()
{

}

