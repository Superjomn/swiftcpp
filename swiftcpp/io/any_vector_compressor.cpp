#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>

#include "any_vector_compressor.h"

namespace swiftcpp {

#define ANY_VECTOR_COMP_DEF(ret, func, args...)                                \
  template <typename F, typename T> ret AnyVectorCompressor<F, T>::func(args)

ANY_VECTOR_COMP_DEF(int, process_init, int dim) {
  // VCHECK(dim >= 0, "dim should be greater than zero");
  dim_ = dim;

  if (dim == 0) {
    return 0;
  }

  const from_type type_min = std::numeric_limits<from_type>::max();
  const from_type type_max = std::numeric_limits<from_type>::min();

  for (int i = 0; i < dim; i++) {
    _scopes.emplace_back(scope_t{type_min, type_max, 0});
  }

  _interval_num = std::numeric_limits<unsigned_t>::max();

  return 0;
}

ANY_VECTOR_COMP_DEF(int, statis_one, from_type *vec, int dim) {
  // VCHECK(dim == dim_,
  //        "error in compressing vector, dims are not compatable, %d != %d",
  //        dim,
  //        dim_);

  for (int i = 0; i < dim_; i++) {
    auto &scope = _scopes[i];
    auto d = vec[i];

    if (d < scope.min) {
      scope.min = d;
    }
    if (d > scope.max) {
      scope.max = d;
    }
    scope.interval = (scope.max - scope.min) / _interval_num;
  }

  return 0;
}

ANY_VECTOR_COMP_DEF(int, compress_one, from_type *vec, unsigned_t *cvec,
                    int dim) {
  // VCHECK(dim == dim_,
  //        "error in compressing vector, dims are not compatable, %d != %d",
  //        dim,
  //        dim_);
  // VCHECK_F(vec != NULL, "an invalid pointer is passed to vec");
  // VCHECK_F(cvec != NULL, "an invalid pointer is passed to cvec");

  for (int i = 0; i < dim_; i++) {
    auto d = vec[i];
    const auto &scope = _scopes[i];
    if (d > scope.max)
      d = scope.max;
    if (d < scope.min)
      d = scope.min;
    cvec[i] = (d - scope.min) / scope.interval;
  }
  return 0;
}

ANY_VECTOR_COMP_DEF(int, uncompress_one, unsigned_t *cvec, from_type *vec,
                    int dim) {
  for (int i = 0; i < dim_; i++) {
    // vec[i] = _scopes[i].min + float(cvec[i] - 1) * _scopes[i].interval;
    vec[i] = _scopes[i].min + cvec[i] * _scopes[i].interval;
  }
  return 0;
}

ANY_VECTOR_COMP_DEF(int, dump, unsigned char *buffer, size_t space,
                    size_t *size) {
  // VCHECK_F(buffer != NULL, "an invalid pointer is passed to buffer");
  // VCHECK_F(size != NULL, "an invalid pointer is passed to size");
  // *size = meta_info_size();
  // VCHECK(space >= *size, "no enough space for AnyVectorCompressor to "
  //                        "dump meta infomation. need [%d] while offered "
  //                        "space size [%d]",
  //        *size, space);

  memcpy((byte_t *)buffer, (byte_t *)(&dim_), sizeof(int));
  memcpy((byte_t *)(buffer + sizeof(int)), (byte_t *)_scopes.data(),
         dim_ * sizeof(scope_t));
  return 0;
}

ANY_VECTOR_COMP_DEF(int, load, const byte_t *buffer, size_t size) {
  const auto meta_size = meta_info_size();
  // VCHECK(meta_size == size,
  //        "AnyVectorCompressor's meta size[%ld] doesn't matches buffer[%ld], "
  //        "load failed!",
  //        meta_size, size);
  memcpy((byte_t *)&dim_, (const byte_t *)buffer, sizeof(int));
  // VCHECK(dim_ > 0, "loaded dim value < 0: %d, load failed", dim_);
  _scopes.resize(dim_);
  memcpy((byte_t *)(_scopes.data()), (const byte_t *)(buffer + sizeof(int)),
         size - sizeof(int));
  return 0;
}

ANY_VECTOR_COMP_DEF(size_t, meta_info_size) {
  // size of dim_
  size_t size = sizeof(int);
  size += sizeof(scope_t) * dim_;
  return size;
}

ANY_VECTOR_COMP_DEF(std::string, debug_info) {
  std::stringstream ss;
  ss << "dim: " << dim_ << std::endl;
  ss << "scope_num: " << _interval_num << std::endl;
  for (int i = 0; i < dim_; i++) {
    ss << i << "-th min:" << _scopes[i].min << " max: " << _scopes[i].max
       << " interval: " << _scopes[i].interval << std::endl;
  }
  return ss.str();
}

template class AnyVectorCompressor<float, unsigned short>;
template class AnyVectorCompressor<float, unsigned int>;
template class AnyVectorCompressor<double, unsigned short>;
template class AnyVectorCompressor<double, unsigned int>;
template class AnyVectorCompressor<long long, unsigned int>;
template class AnyVectorCompressor<long long, unsigned short>;

} // namespace swiftcpp

#ifdef ANY_VECTOR_COMP_DEF
#undef ANY_VECTOR_COMP_DEF
#endif
