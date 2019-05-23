#pragma once

#include <cobalt/cstypes.hh>

namespace cs
{

template<typename T>
class Collection
{
public:
  Collection(csSize initialSize = 64, csSize resizeStep = 16)
  {
    data = new T[initialSize];
    length = 0;
    capacity = initialSize;
    this->resizeStep = resizeStep;
  }

  void Initialize(csSize initialSize = 64, csSize resizeStep = 16)
  {
    if (length != 0 || capacity != initialSize || this->resizeStep != resizeStep)
    {
      delete[] data;
      data = new T[initialSize];
      length = 0;
      capacity = initialSize;
      this->resizeStep = resizeStep;
    }
  }

  ~Collection()
  {
    delete[] data;
    length = 0;
    capacity = 0;
  }


  void Add(const T& t)
  {
    if (length >= capacity)
    {
      Resize();
    }
    data[length] = t;
    length++;
  }

  void Clear()
  {
    length = 0;
  }

  void Release(csSize capacity = 64)
  {
    length = 0;
    delete[] data;
    if (capacity > 0)
    {
      data = new T[capacity];
    }
    else
    {
      data = 0;
    }
  }

  T& operator[](csSize idx)
  {
    return data[idx];
  }

  const T& operator[](csSize idx) const
  {
    return data[idx];
  }

public:
  T* data;
  csSize length;
  csSize capacity;
  csSize resizeStep;

private:
  void Resize()
  {
    T* d = new T[capacity + resizeStep];
    memcpy(d, data, capacity * sizeof(T));
    delete[] data;
    data = d;
    capacity += resizeStep;
  }
};

}
