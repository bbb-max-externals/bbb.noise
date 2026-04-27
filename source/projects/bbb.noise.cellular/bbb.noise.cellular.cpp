#include "c74_min.h"
#include <bbb/noise.hpp>

class bbb_noise_cellular : public c74::min::object<bbb_noise_cellular> {
public:
	MIN_DESCRIPTION{"Cellular (Worley) noise generator"};
	MIN_TAGS{"noise, cellular, worley, procedural"};
	MIN_AUTHOR{"2bit"};

	c74::min::inlet<> inlet1{this, "(float/list/bang) x coordinate or bang"};
	c74::min::inlet<> inlet2{this, "(float) y coordinate"};
	c74::min::inlet<> inlet3{this, "(float) z coordinate"};
	c74::min::inlet<> inlet4{this, "(float) w coordinate"};
	c74::min::outlet<> output{this, "(float) noise value"};

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

	c74::min::attribute<c74::min::symbol> cell_mode_attr{this, "cellular_mode", "f1",
		c74::min::description{"Cellular distance mode: f1, f2, f2minusf1, id"},
		c74::min::setter{[this](const c74::min::atoms &args, int) -> c74::min::atoms {
			c74::min::symbol mode_sym = static_cast<c74::min::symbol>(args[0]);
			bbb::noise::config cfg = eval_.get_config();
			if(mode_sym == "f1") { cfg.cell_mode = bbb::noise::cellular_mode::f1; }
			else if(mode_sym == "f2") { cfg.cell_mode = bbb::noise::cellular_mode::f2; }
			else if(mode_sym == "f2minusf1") { cfg.cell_mode = bbb::noise::cellular_mode::f2_minus_f1; }
			else if(mode_sym == "id") { cfg.cell_mode = bbb::noise::cellular_mode::id; }
			eval_.configure(cfg);
			return args;
		}}
	};

	c74::min::message<> float_msg{this, "float", "Set x and output",
		MIN_FUNCTION {
			x_ = args[0];
			output_noise();
			return {};
		}
	};

	c74::min::message<> list_msg{this, "list", "Set coordinates from list and output",
		MIN_FUNCTION {
			if(args.size() > 0) { x_ = args[0]; }
			if(args.size() > 1) { y_ = args[1]; }
			if(args.size() > 2) { z_ = args[2]; }
			if(args.size() > 3) { w_ = args[3]; }
			output_noise();
			return {};
		}
	};

	c74::min::message<> bang_msg{this, "bang", "Output using stored coordinates",
		MIN_FUNCTION {
			output_noise();
			return {};
		}
	};

	c74::min::message<> x_msg{this, "x", "Set x coordinate",
		MIN_FUNCTION {
			x_ = args[0];
			return {};
		}
	};

	c74::min::message<> y_msg{this, "y", "Set y coordinate",
		MIN_FUNCTION {
			y_ = args[0];
			return {};
		}
	};

	c74::min::message<> z_msg{this, "z", "Set z coordinate",
		MIN_FUNCTION {
			z_ = args[0];
			return {};
		}
	};

	c74::min::message<> w_msg{this, "w", "Set w coordinate",
		MIN_FUNCTION {
			w_ = args[0];
			return {};
		}
	};

	c74::min::message<> coord_msg{this, "coord", "Set coordinates from list and output",
		MIN_FUNCTION {
			if(args.size() > 0) { x_ = args[0]; }
			if(args.size() > 1) { y_ = args[1]; }
			if(args.size() > 2) { z_ = args[2]; }
			if(args.size() > 3) { w_ = args[3]; }
			output_noise();
			return {};
		}
	};

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
	double x_{0.0}, y_{0.0}, z_{0.0}, w_{0.0};
	bbb::noise::evaluator eval_{bbb::noise::config{bbb::noise::type::cellular, 0}};

	void output_noise() {
		double s = scale;
		double o = offset;
		double raw = 0.0;
		int dim = input_dim;

		switch(dim) {
			case 1: raw = eval_.eval(x_ * s + o); break;
			case 2: raw = eval_.eval(x_ * s + o, y_ * s + o); break;
			case 3: raw = eval_.eval(x_ * s + o, y_ * s + o, z_ * s + o); break;
			case 4: raw = eval_.eval(x_ * s + o, y_ * s + o, z_ * s + o, w_ * s + o); break;
		}

		double result;
		if(signed_out) {
			result = raw;
		} else {
			result = (raw + 1.0) * 0.5;
		}

		output.send(result);
	}
};

MIN_EXTERNAL(bbb_noise_cellular);
