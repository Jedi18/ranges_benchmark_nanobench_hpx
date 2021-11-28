#include <range/v3/all.hpp>
#include "hpx/hpx.hpp"
#include "hpx/local/init.hpp"
#include "hpx/algorithm.hpp"
#include "hpx/modules/program_options.hpp"

#include "nanobench.h"

#include <array>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <random>

#define MAX_VAL 10007

enum class ALGORITHM_NAME
{
	UNIQUE_REPLACE_IF_TRANSFORM,
	UNIQUE_REMOVE_IF_TRANSFORM,
	REVERSE_REPLACE_IF_TRANSFORM,
	REVERSE_REMOVE_IF_TRANSFORM,
	TRANSFORM_TRANSFORM,
	REPLACE_IF_TRANSFORM,
	REMOVE_IF_TRANSFORM,
	UNIQUE_TRANSFORM,
	REVERSE_TRANSFORM,
};

std::vector<unsigned int> generateSeedArr(std::size_t maxi) {
	std::vector<unsigned int> seed_arr(maxi);

	srand(time(0));
	std::generate(seed_arr.begin(), seed_arr.end(), []() {
		return rand() % MAX_VAL;
		});

	return seed_arr;
}

template<typename ExPolicy>
void normal_benchmark(ExPolicy policy, std::string const& policy_name, ALGORITHM_NAME algo, std::size_t till, std::ofstream& out, std::vector<unsigned int>& const seed_arr) {
	int j = 5;
	for (int size = 32; size < till; size <<= 1, j++) {
		std::mt19937 mersenne_engine{ seed_arr[j] };
		std::uniform_int_distribution<int> dist{ 1, MAX_VAL };

		std::vector<int> arr(size);
		std::generate(arr.begin(), arr.end(),
			[&]() { return dist(mersenne_engine); });

		std::vector<int> res(size);

		ankerl::nanobench::Bench().output(nullptr).run(policy_name.c_str(), [&]() {

			switch (algo)
			{
			case ALGORITHM_NAME::UNIQUE_REPLACE_IF_TRANSFORM:
			{
				auto iter = hpx::unique(policy, arr.begin(), arr.end());
				hpx::replace_if(policy, arr.begin(), iter, [](auto const& elem) {return elem == 2; }, 3);
				hpx::transform(policy, arr.begin(), iter, res.begin(), [](auto const& elem) { return elem * 2; });
				break;
			}
			case ALGORITHM_NAME::UNIQUE_REMOVE_IF_TRANSFORM:
			{
				auto iter = hpx::unique(policy, arr.begin(), arr.end());
				hpx::remove_if(policy, arr.begin(), iter, [](auto const& elem) {return elem == 2; });
				hpx::transform(policy, arr.begin(), iter, res.begin(), [](auto const& elem) { return elem * 2; });
				break;
			}
			case ALGORITHM_NAME::REVERSE_REPLACE_IF_TRANSFORM:
			{
				hpx::reverse(policy, arr.begin(), arr.end());
				hpx::replace_if(policy, arr.begin(), arr.end(), [](auto const& elem) {return elem == 2; }, 3);
				hpx::transform(policy, arr.begin(), arr.end(), res.begin(), [](auto const& elem) { return elem * 2; });
				break;
			}
			case ALGORITHM_NAME::REVERSE_REMOVE_IF_TRANSFORM:
			{
				hpx::reverse(policy, arr.begin(), arr.end());
				hpx::remove_if(policy, arr.begin(), arr.end(), [](auto const& elem) {return elem == 2; });
				hpx::transform(policy, arr.begin(), arr.end(), res.begin(), [](auto const& elem) { return elem * 2; });
				break;
			}
			case ALGORITHM_NAME::TRANSFORM_TRANSFORM:
			{
				hpx::transform(policy, arr.begin(), arr.end(), res.begin(), [](auto const& elem) {return elem * 2; });
				hpx::transform(policy, arr.begin(), arr.end(), res.begin(), [](auto const& elem) {return elem * 2; });
				break;
			}
			case ALGORITHM_NAME::REPLACE_IF_TRANSFORM:
			{
				hpx::replace_if(policy, arr.begin(), arr.end(), [](auto const& elem) {return elem == 2; }, 3);
				hpx::transform(policy, arr.begin(), arr.end(), res.begin(), [](auto const& elem) { return elem * 2; });
				break;
			}
			case ALGORITHM_NAME::REMOVE_IF_TRANSFORM:
			{
				hpx::remove_if(policy, arr.begin(), arr.end(), [](auto const& elem) {return elem == 2; });
				hpx::transform(policy, arr.begin(), arr.end(), res.begin(), [](auto const& elem) { return elem * 2; });
				break;
			}
			case ALGORITHM_NAME::UNIQUE_TRANSFORM:
			{
				auto iter = hpx::unique(policy, arr.begin(), arr.end());
				hpx::transform(policy, arr.begin(), iter, res.begin(), [](auto const& elem) { return elem * 2; });
				break;
			}
			case ALGORITHM_NAME::REVERSE_TRANSFORM:
			{
				hpx::reverse(policy, arr.begin(), arr.end());
				hpx::transform(policy, arr.begin(), arr.end(), res.begin(), [](auto const& elem) { return elem * 2; });
				break;
			}
			}

			ankerl::nanobench::doNotOptimizeAway(res);
			}).render(ankerl::nanobench::templates::csv(), out);
	}
}

void range_benchmark(std::string const& name, ALGORITHM_NAME algo, std::size_t till, std::ofstream& out, std::vector<unsigned int>& const seed_arr) {
	int j = 5;
	for (int size = 32; size < till; size <<= 1, j++) {
		std::mt19937 mersenne_engine{ seed_arr[j] };
		std::uniform_int_distribution<int> dist{ 1, MAX_VAL };

		std::vector<int> arr(size);
		std::generate(arr.begin(), arr.end(),
			[&]() { return dist(mersenne_engine); });

		std::vector<int> res(size);

		ankerl::nanobench::Bench().output(nullptr).run(name.c_str(), [&]() {

			switch (algo)
			{
			case ALGORITHM_NAME::UNIQUE_REPLACE_IF_TRANSFORM:
			{
				auto rng1 = ranges::views::unique(arr);
				auto rng2 = rng1 | ranges::views::replace_if([](auto const& elem) {return elem == 2; }, 3);
				hpx::ranges::transform(hpx::execution::par, rng2, res.begin(), [](auto const& elem) { return elem * 2; });
			}
			case ALGORITHM_NAME::UNIQUE_REMOVE_IF_TRANSFORM:
			{
				auto rng1 = ranges::views::unique(arr);
				auto rng2 = rng1 | ranges::views::remove_if([](auto const& elem) {return elem == 2; });
				hpx::ranges::transform(hpx::execution::par, rng2, res.begin(), [](auto const& elem) { return elem * 2; });
			}
			case ALGORITHM_NAME::REVERSE_REPLACE_IF_TRANSFORM:
			{
				auto rng1 = arr | ranges::views::reverse;
				auto rng2 = rng1 | ranges::views::replace_if([](auto const& elem) {return elem == 2; }, 3);
				hpx::ranges::transform(hpx::execution::par, rng2, res.begin(), [](auto const& elem) { return elem * 2; });
			}
			case ALGORITHM_NAME::REVERSE_REMOVE_IF_TRANSFORM:
			{
				auto rng1 = arr | ranges::views::reverse;
				auto rng2 = rng1 | ranges::views::remove_if([](auto const& elem) {return elem == 2; });
				hpx::ranges::transform(hpx::execution::par, rng2, res.begin(), [](auto const& elem) { return elem * 2; });
			}
			case ALGORITHM_NAME::TRANSFORM_TRANSFORM:
			{
				auto rng2 = arr | ranges::views::transform([](auto const& elem) {return elem * 2; });
				hpx::ranges::transform(hpx::execution::par, rng2, res.begin(), [](auto const& elem) {return elem * 2; });
			}
			case ALGORITHM_NAME::REPLACE_IF_TRANSFORM:
			{
				auto rng2 = arr | ranges::views::replace_if([](auto const& elem) {return elem == 2; }, 3);
				hpx::ranges::transform(hpx::execution::par, rng2, res.begin(), [](auto const& elem) { return elem * 2; });
			}
			case ALGORITHM_NAME::REMOVE_IF_TRANSFORM:
			{
				auto rng2 = arr | ranges::views::remove_if([](auto const& elem) {return elem == 2; });
				hpx::ranges::transform(hpx::execution::par, rng2, res.begin(), [](auto const& elem) { return elem * 2; });
			}
			case ALGORITHM_NAME::UNIQUE_TRANSFORM:
			{
				auto rng2 = ranges::views::unique(arr);
				hpx::ranges::transform(hpx::execution::par, rng2, res.begin(), [](auto const& elem) { return elem * 2; });
			}
			case ALGORITHM_NAME::REVERSE_TRANSFORM:
			{
				auto rng2 = arr | ranges::views::reverse;
				hpx::ranges::transform(hpx::execution::par, rng2, res.begin(), [](auto const& elem) { return elem * 2; });
			}
			}

			ankerl::nanobench::doNotOptimizeAway(res);
			}).render(ankerl::nanobench::templates::csv(), out);
	}
}

int hpx_main(hpx::program_options::variables_map& vm) {
	std::string algorithm_name = vm["algorithm_name"].as<std::string>();
	int max_i = vm["max_i"].as<int>();
	std::string algorithm_filename = vm["output"].as<std::string>();

	std::map<std::string, ALGORITHM_NAME> nameToAlgo = {
		{"UNIQUE_REPLACE_IF_TRANSFORM", ALGORITHM_NAME::UNIQUE_REPLACE_IF_TRANSFORM },
		{"UNIQUE_REMOVE_IF_TRANSFORM", ALGORITHM_NAME::UNIQUE_REMOVE_IF_TRANSFORM },
		{"REVERSE_REPLACE_IF_TRANSFORM", ALGORITHM_NAME::REVERSE_REPLACE_IF_TRANSFORM },
		{"REVERSE_REMOVE_IF_TRANSFORM", ALGORITHM_NAME::REVERSE_REMOVE_IF_TRANSFORM },
		{"TRANSFORM_TRANSFORM", ALGORITHM_NAME::TRANSFORM_TRANSFORM },
		{"REPLACE_IF_TRANSFORM", ALGORITHM_NAME::REPLACE_IF_TRANSFORM },
		{"REMOVE_IF_TRANSFORM", ALGORITHM_NAME::REMOVE_IF_TRANSFORM },
		{"UNIQUE_TRANSFORM", ALGORITHM_NAME::UNIQUE_TRANSFORM },
		{"REVERSE_TRANSFORM", ALGORITHM_NAME::REVERSE_TRANSFORM }
	};

	ALGORITHM_NAME algo = nameToAlgo[algorithm_name];
	std::size_t start = 32;
	std::size_t till = 1 << max_i;

	std::vector<unsigned int> seed_arr = generateSeedArr(max_i);

	std::ofstream out(algorithm_filename);

	normal_benchmark(hpx::execution::seq, "seq_normal", algo, till, out, seed_arr);
	normal_benchmark(hpx::execution::par, "par_normal", algo, till, out, seed_arr);
	range_benchmark("range", algo, till, out, seed_arr);

	return hpx::local::finalize();
}

int main(int argc, char* argv[])
{
	using namespace hpx::program_options;
	options_description desc_commandline("usage" HPX_APPLICATION_STRING " [options]");

	desc_commandline.add_options()("algorithm_name", hpx::program_options::value<std::string>()->default_value("UNIQUE_REPLACE_IF_TRANSFORM"), "algorithm name (default: UNIQUE_REPLACE_IF_TRANSFORM)")
		("max_i", hpx::program_options::value<int>()->default_value(28), "max size 2^i (default: 28)")
		("output", hpx::program_options::value<std::string>()->default_value("unique_replace_if_transform.csv"), "algorithm output filename");

	// Initialize and run HPX
	hpx::local::init_params init_args;
	init_args.desc_cmdline = desc_commandline;

	return hpx::local::init(hpx_main, argc, argv, init_args);
}
