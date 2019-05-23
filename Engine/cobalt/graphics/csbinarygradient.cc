
#include <cobalt/graphics/csbinarygradient.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/math/csvector.hh>

#include <cobalt/csengine.hh>

cs::Texture2DArrayWrapper *cs::BinaryGradient::static_textureArray = 0;

static float assign_pattern(const char* pattern, void *colors)
{
  csUInt32 *bbuf = reinterpret_cast<csUInt32*>(colors);
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
}

static void create_pattern8a(void *colors)
{
  float factor = assign_pattern(
    "# # # # "
    " # # # #"
    "# # # # "
    " # # # #"
    "# # # # "
    " # # # #"
    "# # # # "
    " # # # #",
    colors
    );
}

static void create_pattern9(void *colors)
{
  float factor = assign_pattern(
    "# # # # "
    " #   #  "
    "# # # # "
    " # # # #"
    "# # # # "
    " #   #  "
    "# # # # "
    " # # # #",
    colors
    );
}

static void create_pattern10(void *colors)
{
  float factor = assign_pattern(
    "# # # # "
    " #   #  "
    "# # # # "
    "   #   #"
    "# # # # "
    " #   #  "
    "# # # # "
    "   #   #",
    colors
    );
}



static void create_pattern11(void *colors)
{
  float factor = assign_pattern(
    "# # # # "
    "        "
    "# # # # "
    "   #   #"
    "# # # # "
    "        "
    "# # # # "
    "   #   #",
    colors
    );
}


static void create_pattern12(void *colors)
{
  float factor = assign_pattern(
    "# # # # "
    "        "
    "# # # # "
    "        "
    "# # # # "
    "        "
    "# # # # "
    "        ",
    colors
    );
}

static void create_pattern13(void *colors)
{
  float factor = assign_pattern(
    "# # # # "
    "        "
    "  #   # "
    "        "
    "# # # # "
    "        "
    "  #   # "
    "        ",
    colors
    );
}


static void create_pattern14(void *colors)
{
  float factor = assign_pattern(
    "#   #   "
    "        "
    "  #   # "
    "        "
    "#   #   "
    "        "
    "  #   # "
    "        ",
    colors
    );
}

static void create_pattern15(void *colors)
{
  float factor = assign_pattern(
    "#   #   "
    "        "
    "        "
    "        "
    "#   #   "
    "        "
    "        "
    "        ",
    colors
    );
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
}

cs::BinaryGradient::BinaryGradient()
{
}

cs::BinaryGradient::~BinaryGradient()
{

}

cs::Texture2DWrapper *cs::BinaryGradient::GetBinaryGradientLevel1()
{
  if (!static_textureArray)
  {
  }
  return 0;
}


cs::Texture2DArrayWrapper *cs::BinaryGradient::GetBinaryGradient()
{
  if (!static_textureArray)
  {
    if (cs::iTexture2DArray *textureArray = csEng->CreateTexture2DArray(cs::ePF_RGBA, 8, 8, 18, false))
    {
      cs::ePixelFormat format = cs::ePF_RGBA;
      csUInt32 colors[8 * 8 * 18];
      int i = 0;
      create_pattern0(&colors[i++ * 64]);
      create_pattern1(&colors[i++ * 64]);
      create_pattern2(&colors[i++ * 64]);
      create_pattern3(&colors[i++ * 64]);
      create_pattern4(&colors[i++ * 64]);
      create_pattern5(&colors[i++ * 64]);
      create_pattern6(&colors[i++ * 64]);
      create_pattern7(&colors[i++ * 64]);
      create_pattern8(&colors[i++ * 64]);
      create_pattern8a(&colors[i++ * 64]);
      create_pattern9(&colors[i++ * 64]);
      create_pattern10(&colors[i++ * 64]);
      create_pattern11(&colors[i++ * 64]);
      create_pattern12(&colors[i++ * 64]);
      create_pattern13(&colors[i++ * 64]);
      create_pattern14(&colors[i++ * 64]);
      create_pattern15(&colors[i++ * 64]);
      create_pattern16(&colors[i++ * 64]);
      textureArray->CopyData(0, format, colors);

      cs::iSampler *sampler = csEng->CreateSampler();
      /*
      sampler->SetAddressU(cs::eTAM_Repeat);
      sampler->SetAddressV(cs::eTAM_Repeat);
      sampler->SetFilter(cs::eFM_MinMagNearest);
      */
      textureArray->SetSampler(new cs::SamplerWrapper(sampler));


      static_textureArray = new cs::Texture2DArrayWrapper(textureArray);
    }
  }
  return static_textureArray;
}
