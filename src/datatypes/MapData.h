// Copyright 2017 UBC Sailbot

#ifndef DATATYPES_MAP_DATA_H
#define DATATYPES_MAP_DATA_H

#include <cstdio>
#include <stdint.h>
#include <vector>

template<typename T>
class MapData {
 protected:
  std::vector <T> data_;

 public:
  void init(const T &val, size_t num);

  // return 0 on success
  int read(FILE *f);

  int write(FILE *f) const;

  const T &operator[](size_t i) const;
  T &operator[](size_t i);
};

template<typename T>
void MapData<T>::init(const T &val, size_t num) {
  data_.clear();
  data_.insert(data_.end(), num, val);
}

template<typename T>
int MapData<T>::read(FILE *f) {
  uint32_t num_elements = 0;
  size_t ret = fread(&num_elements, 4, 1, f);
  if (ret != 1)
    return 1;

  data_.reserve(num_elements);

  T element;
  for (uint32_t i = 0; !feof(f) && i < num_elements; ++i) {
    ret = fread(&element, sizeof(element), 1, f);
    if (ret != 1)
      return 2;

    data_.push_back(element);
  }

  return data_.size() == num_elements ? 0 : 3;
}

template<typename T>
int MapData<T>::write(FILE *f) const {
  uint32_t size = static_cast<uint32_t>(data_.size());
  size_t ret = fwrite(&size, 4, 1, f);
  if (ret != 1)
    return 1;

  for (typename std::vector<T>::const_iterator i = data_.begin(); i != data_.end(); ++i) {
    ret = fwrite(&*i, sizeof(T), 1, f);
    if (ret != 1)
      return 2;
  }

  return 0;
}

template<typename T>
const T &MapData<T>::operator[](size_t i) const {
  return data_[i];
}

template<typename T>
T &MapData<T>::operator[](size_t i) {
  return data_[i];
}

#endif  // DATATYPES_MAP_DATA_H

