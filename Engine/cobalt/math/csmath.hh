
#pragma once

#include <math.h>

namespace cs
{

static double cos(double v)
{
  return ::cos(v);
}


static float cos(float v)
{
  return ::cosf(v);
}


static double sin(double v)
{
  return ::sin(v);
}

static float sin(float v)
{
  return ::sinf(v);
}

static double abs(double v)
{
  return ::abs(v);
}


static float abs(float v)
{
  return ::fabsf(v);
}

static double mod(double x, double y)
{
  return ::fmod(x, y);
}


static float mod(float x, float y)
{
  return ::fmodf(x, y);
}


static double min(double x, double y)
{
  return ::fmin(x, y);
}

static float min(float x, float y)
{
  return ::fminf(x, y);
}

static int min(int x, int y)
{
  return x < y ? x : y;
}


static double max(double x, double y)
{
  return ::fmax(x, y);
}

static float max(float x, float y)
{
  return ::fminf(x, y);
}

static int max(int x, int y)
{
  return x > y ? x : y;
}

static double sqrt(double v)
{
  return ::sqrt(v);
}

static float sqrt(float v)
{
  return ::sqrtf(v);
}

}