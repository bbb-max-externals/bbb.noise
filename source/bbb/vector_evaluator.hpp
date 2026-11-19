#pragma once

#include <bbb/noise.hpp>
#include <atomic>
#include <mutex>
#include <type_traits>

namespace bbb::noise {

// Control threads prepare complete tables. The DSP reader adopts them only
// between vectors and keeps its previous tables if an update is in flight.
class vector_evaluator {
public:
	explicit vector_evaluator(config initial) : pending_(initial), active_(pending_) {}

	template<class Edit>
	void update(Edit edit) {
		std::lock_guard<std::mutex> lock(mutex_);
		auto next = pending_.get_config();
		edit(next);
		pending_.configure(next);
		changed_.store(true, std::memory_order_release);
	}

	const evaluator& begin_vector() {
		if (changed_.load(std::memory_order_acquire)) {
			std::unique_lock<std::mutex> lock(mutex_, std::try_to_lock);
			if (lock) {
				active_ = pending_;
				changed_.store(false, std::memory_order_relaxed);
			}
		}
		return active_;
	}

private:
	static_assert(std::is_trivially_copyable_v<evaluator>, "DSP snapshot must be a fixed-size value");
	std::mutex mutex_;
	evaluator pending_;
	evaluator active_;
	std::atomic<bool> changed_{false};
};

}
