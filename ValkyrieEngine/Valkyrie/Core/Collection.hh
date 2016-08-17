#pragma once

#include <Valkyrie/Types.hh>

template<typename T>
class vkCollection
{
public:
  vkCollection(vkSize initialSize = 64, vkSize resizeStep = 16)
  {
    data = new T[initialSize];
    length = 0;
    capacity = initialSize;
    this->resizeStep = resizeStep;
  }

  void Initialize(vkSize initialSize = 64, vkSize resizeStep = 16)
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

  ~vkCollection()
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

  void Release(vkSize capacity = 64)
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

  T& operator[](vkSize idx)
  {
    return data[idx];
  }

  const T& operator[](vkSize idx) const
  {
    return data[idx];
  }

public:
  T* data;
  vkSize length;
  vkSize capacity;
  vkSize resizeStep;

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
