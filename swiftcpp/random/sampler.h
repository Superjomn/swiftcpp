#ifndef SWIFTCPP_RANDOM_SAMPLER_H_
#define SWIFTCPP_RANDOM_SAMPLER_H_
#include <cstdint>
#include <ctime>
#include <unordered_map>
namespace swiftcpp {
/*
 * A helper class to sample a list of random values from a random distribution.
 *
 * Template Arguments:
 *   @R: the class of D's density function which support operator(uint64_t id)
 * to get the probability of id.
 *
 * TODO NOTE Currently, due to the limitation of double 's storage accurancy
 * between
 * [0., 1.], only 1 million ids is supported.
 */
template <typename D, typename R> class IntegarRandomSampler {
public:
  typedef uint64_t id_t;
  typedef double prob_t;

  IntegarRandomSampler(const R *dens, id_t min, id_t max)
      : dens_(dens), min_(min), max_(max) {
    build_random_table_meta_info();
  }

  id_t operator() { return sample(); }

  id_t sample() {
    prob_t prob = static_cast<prob_t>(std::rand()) / std::RAND_MAX;
    uint64_t low = 0, high = rand_table_.size();

    uint64_t mid, last_mid;
    while (low + 1 < high) {
      mid = (low + high) / 2;
      const auto &item = rand_table_[mid];
      if (item.second > prob)
        high = mid;
      else if (item.second < prob)
        low = mid;
      else
        return item.first;
    }
    return mid;
  }

protected:
  void build_random_table_meta_info() {
    prob_t total_prob = 0.;
    for (uint64_t id = min_; id <= max_; id++) {
      prob_t prob = (*dens_)(id);
      total_prob += prob;
      rand_table_.emplace_back({id, total_prob});
    }

    for (auto &item : rand_table_) {
      item.second /= total_prob;
    }
  }

private:
  mutable R *dens_;
  const id_t min_;
  const id_t max_;
  std::unordered_map<id_t, prob_t> rand_table_;
};
}
#endif
