#pragma once

#include <algorithm>
#include <atomic>
#include <optional>
#include <span>

namespace iomisc {

  template<typename T, std::size_t capacity>
    requires(std::atomic_ref<T>::is_always_lock_free)
  class StealingQueue {
  public:
    bool TryPush(std::convertible_to<T> auto&& t) noexcept {
      std::size_t cur_prod = prod_idx_.load(std::memory_order_relaxed);
      if (cur_prod - cons_idx_.load(std::memory_order_relaxed) >= capacity) {
        return false;
      }

      std::atomic_ref<T>{items_[cur_prod % capacity]}.store(std::forward<decltype(t)>(t),
                                                            std::memory_order_relaxed);
      prod_idx_.store(cur_prod + 1, std::memory_order_release);
      return true;
    }

    std::optional<T> TryPop() noexcept {
      std::optional<T> ret;
      std::size_t cur_cons = cons_idx_.load(std::memory_order_relaxed);
      std::size_t cur_prod = cur_cons;
      while (true) {
        if (cur_cons == cur_prod) {
          cur_prod = prod_idx_.load(std::memory_order_relaxed);
          if (cur_cons == cur_prod) {
            return nullptr;
          }
        }
        ret.emplace(
            std::atomic_ref<T>{items_[cur_cons % capacity]}.load(std::memory_order_relaxed));
        if (cons_idx_.compare_exchange_weak(cur_cons, cur_cons + 1, std::memory_order_relaxed)) {
          return ret;
        }
      }
    }

    template<std::size_t size>
    std::size_t Grab(std::span<T, size> out_buffer) noexcept {
      std::size_t cur_cons = cons_idx_.load(std::memory_order_relaxed);
      while (true) {
        std::size_t cur_prod = prod_idx_.load(std::memory_order_acquire);
        std::size_t to_steal = std::min(out_buffer.size(), cur_prod - cur_cons);
        if (to_steal == 0) {
          return 0;
        }
      }
    }

  private:
    alignas(std::atomic_ref<T>::required_alignment) T items_[capacity] = {};
    std::atomic<std::size_t> prod_idx_ = 0;
    std::atomic<std::size_t> cons_idx_ = 0;
  };

} // namespace iomisc
