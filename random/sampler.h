#ifndef SWIFTCPP_RANDOM_SAMPLER_H_
#define SWIFTCPP_RANDOM_SAMPLER_H_
#include <cstdint>
#include <unordered_map>
namespace swiftcpp {
/*
 * A helper class to sample a list of random values from a random distribution.
 *
 * Template Arguments:
 *   @R: the class of D's density function which support operator(uint64_t id)
 * to get the probability of id.
 *   @T: random sample's data type
 *
 * TODO NOTE Currently, due to the limitation of double 's storage accurancy between
 * [0., 1.], only 1 million ids is supported.
 */
template <typename D, typename R, typename T> class IntegarRandomSampler {
public:
  RandomSampler(const R *dens, uint64_t min, uint64_t max)
      : dens_(dens), min_(min), max_(max) {
    build_random_table_meta_info();
  }
  T random_sample() { return (*dist_)(); }

protected:
  void build_random_table_meta_info() {
    double total_prob = 0.;
    for (uint64_t id = min_; id <= max_; id++) {
      double prob = (*dens_)(id);
      total_prob += prob;
      rand_table_.push_back({id, total_prob});
    }

    for (auto &item : rand_table_) {
      item.second /= total_prob;
    }
  }

private:
  mutable D *dist_;
  mutable R *dens_;
  const uint64_t min_;
  const uint64_t max_;
  std::unordered_map<uint64_t, double> rand_table_;
};
}
#endif
