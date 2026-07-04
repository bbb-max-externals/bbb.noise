#include <bbb/noise.hpp>

#include <cmath>
#include <cstdint>
#include <iostream>

namespace {

int failure_count{0};

void expect_true(bool condition, const char *message) {
    if(condition) {
        return;
    }
    ++failure_count;
    std::cerr << "FAIL: " << message << "\n";
}

void expect_near(double actual, double expected, double tolerance, const char *message) {
    if(std::abs(actual - expected) <= tolerance) {
        return;
    }
    ++failure_count;
    std::cerr << "FAIL: " << message
        << " expected " << expected
        << " got " << actual
        << " tolerance " << tolerance << "\n";
}

void expect_not_near(double first_value, double second_value, double tolerance, const char *message) {
    if(tolerance < std::abs(first_value - second_value)) {
        return;
    }
    ++failure_count;
    std::cerr << "FAIL: " << message
        << " first " << first_value
        << " second " << second_value
        << " tolerance " << tolerance << "\n";
}

bbb::noise::evaluator make_evaluator(bbb::noise::type noise_type, int seed) {
    bbb::noise::config config;
    config.noise_type = noise_type;
    config.seed = seed;
    return bbb::noise::evaluator{config};
}

bbb::noise::evaluator make_fractal_evaluator(bbb::noise::fractal_type fractal_type) {
    bbb::noise::config config;
    config.fractal = fractal_type;
    config.source = bbb::noise::type::simplex;
    config.seed = 777;
    config.octaves = 4;
    config.gain = 0.5;
    config.lacunarity = 2.0;
    return bbb::noise::evaluator{config};
}

void expect_snapshot(
    const bbb::noise::evaluator &evaluator,
    double expected_1d,
    double expected_2d,
    double expected_3d,
    double expected_4d,
    const char *label
) {
    constexpr double tolerance{0.000000000001};
    expect_near(evaluator.eval(0.125), expected_1d, tolerance, label);
    expect_near(evaluator.eval(0.125, -1.5), expected_2d, tolerance, label);
    expect_near(evaluator.eval(0.125, -1.5, 2.25), expected_3d, tolerance, label);
    expect_near(evaluator.eval(0.125, -1.5, 2.25, -0.75), expected_4d, tolerance, label);
}

void test_base_algorithm_snapshots() {
    expect_snapshot(
        make_evaluator(bbb::noise::type::perlin, 12345),
        0.42174530029296875,
        0.84189224243164062,
        0.12899617105722427,
        0.29398792554275133,
        "perlin seed 12345 snapshot"
    );
    expect_snapshot(
        make_evaluator(bbb::noise::type::simplex, 12345),
        1.9266149401664734,
        0.90808981114338938,
        0.4368871577274655,
        0.59588267272333795,
        "simplex seed 12345 snapshot"
    );
    expect_snapshot(
        make_evaluator(bbb::noise::type::value, 12345),
        -0.99999999719108246,
        -0.99999993821128896,
        -0.99999997427164877,
        -0.99999997465062562,
        "value seed 12345 snapshot"
    );
    expect_snapshot(
        make_evaluator(bbb::noise::type::cellular, 12345),
        0.11328125,
        0.51267206048463621,
        0.55901699437494745,
        0.61237243569579447,
        "cellular seed 12345 snapshot"
    );
}

void test_fractal_snapshots() {
    expect_snapshot(
        make_fractal_evaluator(bbb::noise::fractal_type::fbm),
        1.3564715782801311,
        -0.73400166739489547,
        0.16803284412106703,
        0.044347353401108201,
        "fbm snapshot"
    );
    expect_snapshot(
        make_fractal_evaluator(bbb::noise::fractal_type::turbulence),
        1.3564715782801311,
        0.92985565085636945,
        0.26937945591804913,
        0.12723686308962814,
        "turbulence snapshot"
    );
    expect_snapshot(
        make_fractal_evaluator(bbb::noise::fractal_type::ridged),
        -0.12772010157522029,
        -0.99382979846121955,
        1.0786420398736705,
        1.8647692306752233,
        "ridged snapshot"
    );
}

void test_cellular_modes() {
    constexpr double tolerance{0.000000000001};
    const double x{0.125};
    const double y{-1.5};
    const double z{2.25};

    bbb::noise::config config;
    config.noise_type = bbb::noise::type::cellular;
    config.seed = 2468;

    config.cell_mode = bbb::noise::cellular_mode::f1;
    expect_near(bbb::noise::evaluator{config}.eval(x, y, z), 0.56342155412821227, tolerance, "cellular f1 snapshot");

    config.cell_mode = bbb::noise::cellular_mode::f2;
    expect_near(bbb::noise::evaluator{config}.eval(x, y, z), 0.56444974588088881, tolerance, "cellular f2 snapshot");

    config.cell_mode = bbb::noise::cellular_mode::f2_minus_f1;
    expect_near(bbb::noise::evaluator{config}.eval(x, y, z), 0.0010281917526765438, tolerance, "cellular f2-f1 snapshot");

    config.cell_mode = bbb::noise::cellular_mode::id;
    expect_near(bbb::noise::evaluator{config}.eval(x, y, z), -0.90625, tolerance, "cellular id snapshot");
}

void test_seed_determinism() {
    const bbb::noise::evaluator first_evaluator{make_evaluator(bbb::noise::type::perlin, 4242)};
    const bbb::noise::evaluator second_evaluator{make_evaluator(bbb::noise::type::perlin, 4242)};
    const bbb::noise::evaluator third_evaluator{make_evaluator(bbb::noise::type::perlin, 4243)};

    const double first_value{first_evaluator.eval(0.25, 0.5, 0.75)};
    const double second_value{second_evaluator.eval(0.25, 0.5, 0.75)};
    const double third_value{third_evaluator.eval(0.25, 0.5, 0.75)};

    expect_near(first_value, second_value, 0.0, "same seed and same coordinates are deterministic");
    expect_not_near(first_value, third_value, 0.000000001, "different seed changes perlin output at sampled coordinate");
}

void test_all_snapshots_are_finite() {
    const bbb::noise::evaluator evaluators[]{
        make_evaluator(bbb::noise::type::perlin, 12345),
        make_evaluator(bbb::noise::type::simplex, 12345),
        make_evaluator(bbb::noise::type::value, 12345),
        make_evaluator(bbb::noise::type::cellular, 12345),
        make_fractal_evaluator(bbb::noise::fractal_type::fbm),
        make_fractal_evaluator(bbb::noise::fractal_type::turbulence),
        make_fractal_evaluator(bbb::noise::fractal_type::ridged),
    };

    for(const auto &evaluator : evaluators) {
        expect_true(std::isfinite(evaluator.eval(0.125)), "finite 1D noise value");
        expect_true(std::isfinite(evaluator.eval(0.125, -1.5)), "finite 2D noise value");
        expect_true(std::isfinite(evaluator.eval(0.125, -1.5, 2.25)), "finite 3D noise value");
        expect_true(std::isfinite(evaluator.eval(0.125, -1.5, 2.25, -0.75)), "finite 4D noise value");
    }
}

} // namespace

int main() {
    test_base_algorithm_snapshots();
    test_fractal_snapshots();
    test_cellular_modes();
    test_seed_determinism();
    test_all_snapshots_are_finite();

    if(failure_count != 0) {
        std::cerr << failure_count << " bbb.noise evaluator test(s) failed\n";
        return 1;
    }

    std::cout << "bbb.noise evaluator tests passed\n";
    return 0;
}
