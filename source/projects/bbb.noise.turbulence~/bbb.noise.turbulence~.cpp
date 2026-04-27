#include "c74_min.h"
#include <bbb/noise.hpp>

class bbb_noise_turbulence_tilde : public c74::min::object<bbb_noise_turbulence_tilde>, public c74::min::mc_operator<> {
public:
	MIN_DESCRIPTION{"Turbulence noise signal generator"};
	MIN_TAGS{"noise, turbulence, fractal, procedural, audio"};
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
		c74::min::description{"Random seed"},
		c74::min::setter{[this](const c74::min::atoms &args, int) -> c74::min::atoms {
			bbb::noise::config cfg = eval_.get_config();
			cfg.seed = static_cast<int>(args[0]);
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

	c74::min::attribute<c74::min::symbol> source_attr{this, "source", "simplex",
		c74::min::description{"Basis noise type"},
		c74::min::setter{[this](const c74::min::atoms &args, int) -> c74::min::atoms {
			c74::min::symbol src = static_cast<c74::min::symbol>(args[0]);
			bbb::noise::config cfg = eval_.get_config();
			if(src == "perlin") { cfg.source = bbb::noise::type::perlin; }
			else if(src == "simplex") { cfg.source = bbb::noise::type::simplex; }
			else if(src == "value") { cfg.source = bbb::noise::type::value; }
			eval_.configure(cfg);
			return args;
		}}
	};

	c74::min::attribute<int> octaves_attr{this, "octaves", 4,
		c74::min::description{"Number of octaves (1-8)"},
		c74::min::range{1, 8},
		c74::min::setter{[this](const c74::min::atoms &args, int) -> c74::min::atoms {
			bbb::noise::config cfg = eval_.get_config();
			cfg.octaves = static_cast<int>(args[0]);
			eval_.configure(cfg);
			return args;
		}}
	};

	c74::min::attribute<double> gain_attr{this, "gain", 0.5,
		c74::min::description{"Fractal gain"},
		c74::min::range{0.0, 1.0},
		c74::min::setter{[this](const c74::min::atoms &args, int) -> c74::min::atoms {
			bbb::noise::config cfg = eval_.get_config();
			cfg.gain = static_cast<double>(args[0]);
			eval_.configure(cfg);
			return args;
		}}
	};

	c74::min::attribute<double> lacunarity_attr{this, "lacunarity", 2.0,
		c74::min::description{"Fractal lacunarity"},
		c74::min::range{0.01, 10.0},
		c74::min::setter{[this](const c74::min::atoms &args, int) -> c74::min::atoms {
			bbb::noise::config cfg = eval_.get_config();
			cfg.lacunarity = static_cast<double>(args[0]);
			eval_.configure(cfg);
			return args;
		}}
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

		for (auto i = 0; i < nframes; ++i) {
			double x = (nc_in > 0 && in.samples(0)) ? in.samples(0)[i] : 0.0;
			double y = (nc_in > 1 && in.samples(1)) ? in.samples(1)[i] : 0.0;
			double z = (nc_in > 2 && in.samples(2)) ? in.samples(2)[i] : 0.0;
			double w = (nc_in > 3 && in.samples(3)) ? in.samples(3)[i] : 0.0;

			double raw = 0.0;
			switch(dim) {
				case 1: raw = eval_.eval(x * s + o); break;
				case 2: raw = eval_.eval(x * s + o, y * s + o); break;
				case 3: raw = eval_.eval(x * s + o, y * s + o, z * s + o); break;
				case 4: raw = eval_.eval(x * s + o, y * s + o, z * s + o, w * s + o); break;
			}

			double result = signed_out ? raw : (raw + 1.0) * 0.5;
			for (auto ch = 0; ch < nc_out; ++ch) {
				if (out.samples(ch)) { out.samples(ch)[i] = result; }
			}
		}
	}

	c74::min::message<> randomize_msg{this, "randomize", "Randomize seed",
		MIN_FUNCTION {
			bbb::noise::config cfg = eval_.get_config();
			cfg.seed = static_cast<int>(std::rand());
			eval_.configure(cfg);
			return {};
		}
	};

private:
	bbb::noise::evaluator eval_{
		bbb::noise::config{bbb::noise::type::simplex, 0, bbb::noise::cellular_mode::f1,
		                   bbb::noise::fractal_type::turbulence, bbb::noise::type::simplex, 4, 0.5, 2.0}
	};
};

MIN_EXTERNAL(bbb_noise_turbulence_tilde);
