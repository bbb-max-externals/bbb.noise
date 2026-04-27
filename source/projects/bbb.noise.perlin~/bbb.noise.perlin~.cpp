#include "c74_min.h"
#include <bbb/noise.hpp>

class bbb_noise_perlin_tilde : public c74::min::object<bbb_noise_perlin_tilde>, public c74::min::mc_operator<> {
public:
	MIN_DESCRIPTION{"Perlin noise signal generator"};
	MIN_TAGS{"noise, perlin, procedural, audio"};
	MIN_AUTHOR{"2bit"};

	c74::min::inlet<> inlet1{this, "(signal) x coordinate", "signal"};
	c74::min::inlet<> inlet2{this, "(signal) y coordinate", "signal"};
	c74::min::inlet<> inlet3{this, "(signal) z coordinate", "signal"};
	c74::min::inlet<> inlet4{this, "(signal) w coordinate", "signal"};
	c74::min::outlet<> output{this, "(signal) noise value", "signal"};

	c74::min::attribute<int> input_dim{this, "input_dim", 2,
		c74::min::description{"Number of input dimensions (1-4)"},
		c74::min::range{1, 4}
	};

	c74::min::attribute<int> signed_out{this, "signed", 0,
		c74::min::description{"Bipolar output (1) or unipolar (0)"},
		c74::min::range{0, 1}
	};

	c74::min::attribute<int> seed_attr{this, "seed", 0,
		c74::min::description{"Random seed (deterministic when fixed)"},
		c74::min::setter{[this](const c74::min::atoms &args, int) -> c74::min::atoms {
			int new_seed = static_cast<int>(args[0]);
			bbb::noise::config cfg = eval_.get_config();
			cfg.seed = new_seed;
			eval_.configure(cfg);
			return args;
		}}
	};

	c74::min::attribute<double> scale{this, "scale", 1.0,
		c74::min::description{"Coordinate scale"}
	};

	c74::min::attribute<double> offset{this, "offset", 0.0,
		c74::min::description{"Coordinate offset"}
	};

	void operator()(c74::min::audio_bundle input, c74::min::audio_bundle output) {
		auto& in = input;
		auto& out = output;
		double s = scale;
		double o = offset;
		int dim = input_dim;
		auto nframes = in.frame_count();
		auto nc_in = in.channel_count();
		auto nc_out = out.channel_count();

		double y_default{0.0}, z_default{0.0}, w_default{0.0};

		for (auto i = 0; i < nframes; ++i) {
			double x = (nc_in > 0 && in.samples(0)) ? in.samples(0)[i] : 0.0;
			double y = (nc_in > 1 && in.samples(1)) ? in.samples(1)[i] : y_default;
			double z = (nc_in > 2 && in.samples(2)) ? in.samples(2)[i] : z_default;
			double w = (nc_in > 3 && in.samples(3)) ? in.samples(3)[i] : w_default;

			double sx = x * s + o;
			double sy = y * s + o;
			double sz = z * s + o;
			double sw = w * s + o;

			double raw = 0.0;
			switch(dim) {
				case 1: raw = eval_.eval(sx); break;
				case 2: raw = eval_.eval(sx, sy); break;
				case 3: raw = eval_.eval(sx, sy, sz); break;
				case 4: raw = eval_.eval(sx, sy, sz, sw); break;
			}

			double result = signed_out ? raw : (raw + 1.0) * 0.5;
			for (auto ch = 0; ch < nc_out; ++ch) {
				if (out.samples(ch)) {
					out.samples(ch)[i] = result;
				}
			}
		}
	}

	c74::min::message<> seed_msg{this, "seed", "Set seed",
		MIN_FUNCTION {
			seed_attr = static_cast<int>(args[0]);
			return {};
		}
	};

	c74::min::message<> randomize_msg{this, "randomize", "Randomize seed",
		MIN_FUNCTION {
			bbb::noise::config cfg = eval_.get_config();
			cfg.seed = static_cast<int>(std::rand());
			eval_.configure(cfg);
			return {};
		}
	};

private:
	bbb::noise::evaluator eval_{bbb::noise::config{bbb::noise::type::perlin, 0}};
};

MIN_EXTERNAL(bbb_noise_perlin_tilde);
