

#include <Valkyrie/Graphics/BinaryGradient.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Engine.hh>

ITexture2DArray *vkBinaryGradient::static_textureArray = 0;

static float assign_pattern(const char* pattern, void *colors)
{
  vkUInt32 *bbuf = reinterpret_cast<vkUInt32*>(colors);
  float factor = 0.0f;
  for (unsigned i = 0; i < 64; ++i)
  {
    if (pattern[i] == ' ')
    {
      bbuf[i] = 0xffffffff;
      factor += 1.0f;
    }
    else if (pattern[i] == '#')
    {
      bbuf[i] = 0x00000000;
    }
  }
  return factor / 64.0f;
}

static void create_pattern0(void *colors)
{
  float factor = assign_pattern(
    "########"
    "########"
    "########"
    "########"
    "########"
    "########"
    "########"
    "########",
    colors
    );
  printf("Pattern0: %f\n", factor);
}

static void create_pattern1(void *colors)
{
  float factor = assign_pattern(
    " ### ###"
    "########"
    "########"
    "########"
    " ### ###"
    "########"
    "########"
    "########",
    colors
    );
  printf("Pattern1: %f\n", factor);
}


static void create_pattern2(void *colors)
{
  float factor = assign_pattern(
    " ### ###"
    "########"
    "## ### #"
    "########"
    " ### ###"
    "########"
    "## ### #"
    "########",
    colors
    );
  printf("Pattern2: %f\n", factor);
}


static void create_pattern3(void *colors)
{
  float factor = assign_pattern(
    " # # # #"
    "########"
    "## ### #"
    "########"
    " # # # #"
    "########"
    "## ### #"
    "########",
    colors
    );
  printf("Pattern3: %f\n", factor);
}


static void create_pattern4(void *colors)
{
  float factor = assign_pattern(
    " # # # #"
    "########"
    " # # # #"
    "########"
    " # # # #"
    "########"
    " # # # #"
    "########",
    colors
    );
  printf("Pattern4: %f\n", factor);
}


static void create_pattern5(void *colors)
{
  float factor = assign_pattern(
    " # # # #"
    "########"
    " # # # #"
    "### ### "
    " # # # #"
    "########"
    " # # # #"
    "### ### ",
    colors
    );
  printf("Pattern5: %f\n", factor);
}

static void create_pattern6(void *colors)
{
  float factor = assign_pattern(
    " # # # #"
    "# ### ##"
    " # # # #"
    "### ### "
    " # # # #"
    "# ### ##"
    " # # # #"
    "### ### ",
    colors
    );
  printf("Pattern6: %f\n", factor);
}

static void create_pattern7(void *colors)
{
  float factor = assign_pattern(
    " # # # #"
    "# ### ##"
    " # # # #"
    "# # # # "
    " # # # #"
    "# ### ##"
    " # # # #"
    "# # # # ",
    colors
    );
  printf("Pattern7: %f\n", factor);
}


static void create_pattern8(void *colors)
{
  float factor = assign_pattern(
    " # # # #"
    "# # # # "
    " # # # #"
    "# # # # "
    " # # # #"
    "# # # # "
    " # # # #"
    "# # # # ",
    colors
    );
  printf("Pattern8: %f\n", factor);
}

static void create_pattern9(void *colors)
{
  float factor = assign_pattern(
    " #   #  "
    "# # # # "
    " # # # #"
    "# # # # "
    " #   #  "
    "# # # # "
    " # # # #"
    "# # # # ",
    colors
    );
  printf("Pattern9: %f\n", factor);
}

static void create_pattern10(void *colors)
{
  float factor = assign_pattern(
    " #   #  "
    "# # # # "
    "   #   #"
    "# # # # "
    " #   #  "
    "# # # # "
    "   #   #"
    "# # # # ",
    colors
    );
  printf("Pattern10: %f\n", factor);
}



static void create_pattern11(void *colors)
{
  float factor = assign_pattern(
    "        "
    "# # # # "
    "   #   #"
    "# # # # "
    "        "
    "# # # # "
    "   #   #"
    "# # # # ",
    colors
    );
  printf("Pattern11: %f\n", factor);
}


static void create_pattern12(void *colors)
{
  float factor = assign_pattern(
    "        "
    "# # # # "
    "        "
    "# # # # "
    "        "
    "# # # # "
    "        "
    "# # # # ",
    colors
    );
  printf("Pattern12: %f\n", factor);
}

static void create_pattern13(void *colors)
{
  float factor = assign_pattern(
    "        "
    "# # # # "
    "        "
    "  #   # "
    "        "
    "# # # # "
    "        "
    "  #   # ",
    colors
    );
  printf("Pattern13: %f\n", factor);
}


static void create_pattern14(void *colors)
{
  float factor = assign_pattern(
    "        "
    "#   #   "
    "        "
    "  #   # "
    "        "
    "#   #   "
    "        "
    "  #   # ",
    colors
    );
  printf("Pattern14: %f\n", factor);
}

static void create_pattern15(void *colors)
{
  float factor = assign_pattern(
    "        "
    "#   #   "
    "        "
    "        "
    "        "
    "#   #   "
    "        "
    "        ",
    colors
    );
  printf("Pattern15: %f\n", factor);
}

static void create_pattern16(void *colors)
{
  float factor = assign_pattern(
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "        ",
    colors
    );
  printf("Pattern16: %f\n", factor);
}

vkBinaryGradient::vkBinaryGradient()
{
}

vkBinaryGradient::~vkBinaryGradient()
{

}

ITexture2D *vkBinaryGradient::GetBinaryGradientLevel1()
{
  if (!static_textureArray)
  {
  }
  return 0;
}


ITexture2DArray *vkBinaryGradient::GetBinaryGradient()
{
  if (!static_textureArray)
  {
    IGraphics *graphics = vkEngine::Get()->GetRenderer();
    if (static_textureArray = graphics->CreateTexture2DArray(ePF_RGBA, 8, 8, 17))
    {
      vkPixelFormat format = ePF_RGBA;
      vkUInt32 colors[8 * 8 * 17];
      create_pattern0(&colors[0 * 64]);
      create_pattern1(&colors[1 * 64]);
      create_pattern2(&colors[2 * 64]);
      create_pattern3(&colors[3 * 64]);
      create_pattern4(&colors[4 * 64]);
      create_pattern5(&colors[5 * 64]);
      create_pattern6(&colors[6 * 64]);
      create_pattern7(&colors[7 * 64]);
      create_pattern8(&colors[8 * 64]);
      create_pattern9(&colors[9 * 64]);
      create_pattern10(&colors[10 * 64]);
      create_pattern11(&colors[11 * 64]);
      create_pattern12(&colors[12 * 64]);
      create_pattern13(&colors[13 * 64]);
      create_pattern14(&colors[14 * 64]);
      create_pattern15(&colors[15 * 64]);
      create_pattern16(&colors[16 * 64]);
      static_textureArray->CopyData(0, format, colors);

      ISampler *sampler = graphics->CreateSampler();
      /*
      sampler->SetAddressU(eTAM_Repeat);
      sampler->SetAddressV(eTAM_Repeat);
      sampler->SetFilter(eFM_MinMagNearest);
      */
      static_textureArray->SetSampler(sampler);
    }
  }
  return static_textureArray;
}
