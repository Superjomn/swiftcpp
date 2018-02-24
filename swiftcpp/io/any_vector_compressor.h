#ifndef SWIFTCPP_IO_ANY_VECTOR_COMPRESSOR_H_
#define SWIFTCPP_IO_ANY_VECTOR_COMPRESSOR_H_

#include <vector>

namespace swiftcpp {

/*
 * Compressing a float/double type vector by storing the element in short/char
 * type, which will save 1 to 3 times storege.
 */
template <typename F, typename T> class AnyVectorCompressor {
public:
  template <typename type> struct DimScope {
    type min;
    type max;
    type interval;
  };

  typedef F from_type;
  typedef T to_type;
  typedef T unsigned_t;
  typedef DimScope<from_type> scope_t;
  typedef unsigned char byte_t;

  int process_init(int dim = 0);

  int process_destroy(){};

  int statis_one(from_type *vec, int dim);

  int compress_one(from_type *vec, unsigned_t *cvec, int dim);

  int uncompress_one(unsigned_t *cvec, from_type *vec, int dim);

  int dump(unsigned char *buffer, size_t space, size_t *size);

  int load(const unsigned char *buffer, size_t size);

  size_t meta_info_size();

  std::string debug_info();

  size_t dim() const { return dim_; }

  const std::vector<scope_t> &scopes() const { return _scopes; }

private:
  int dim_;
  unsigned long _interval_num;
  std::vector<scope_t> _scopes;
};

} // namespace swiftcpp

#endif
