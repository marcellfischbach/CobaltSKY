

#include <cobalt/graphics/csbinarygradient.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/csengine.hh>

iTexture2DArray *csBinaryGradient::static_textureArray = 0;

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

csBinaryGradient::csBinaryGradient()
{
}

csBinaryGradient::~csBinaryGradient()
{

}

iTexture2D *csBinaryGradient::GetBinaryGradientLevel1()
{
  if (!static_textureArray)
  {
  }
  return 0;
}


iTexture2DArray *csBinaryGradient::GetBinaryGradient()
{
  if (!static_textureArray)
  {
    if (static_textureArray = csEng->CreateTexture2DArray(ePF_RGBA, 8, 8, 18, false))
    {
      csPixelFormat format = ePF_RGBA;
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
      static_textureArray->CopyData(0, format, colors);

      iSampler *sampler = csEng->CreateSampler();
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