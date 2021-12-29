#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>

std::vector<bool> num_to_binvec(size_t n, const size_t num_digits) {
    std::vector<bool> res(num_digits, false);
    size_t i {num_digits};
    while (n) {
        res[--i] = (n % 2 == 1);
        n >>= 1;
    }
    return res;
}

int sign_sum(const std::vector<int>& values, const std::vector<bool> signs) {
    int res = values.front();
    for (size_t i {1}; i < values.size(); ++i)
        (signs[i - 1]) ? res += values[i] : res -= values[i];
    return res;
}

int sign_sum(const std::vector<int>& values, const std::bitset<25>& signs) {
    int res = values.front();
    for (size_t i {1}; i < values.size(); ++i)
        (signs[i - 1]) ? res += values[i] : res -= values[i];
    return res;
}

void sol1(std::vector<int>& values, const int check_sum) {
    size_t num_values = values.size();
    size_t num_signs {num_values - 1};
    bool no_solution {true};
    for (size_t i {0}; i < std::pow(2, num_signs); ++i) {
        const std::bitset<25> signs {i};
        //std::cout << t << '\n';
        //for (size_t j {0}; j < 8; ++j)
        //    std::cout << t[j] << ' ';
        //std::cout << '\n';

        //const auto signs = num_to_binvec(i, num_signs);
        if (sign_sum(values, signs) == check_sum) {
            std::cout << values.front();
            for (size_t j {1}; j < num_values; ++j)
                std::cout << (signs[j - 1] ? '+' : '-') << values[j];
            std::cout << '=' << check_sum << '\n';
            no_solution = false;
            break;
        }
    }
    if (no_solution)
        std::cout << "No solution\n";
}

void recursive_sol(const std::vector<int>& values, std::string& signs,
        bool& found,
        const size_t k, const int curr_sum, const int check_sum) {
    if (k == 0) {
        if (curr_sum + values.front() == check_sum) {
            std::cout << values.front();
            for (size_t j {1}; j < values.size(); ++j)
                std::cout << signs[j] << values[j];
            std::cout << '=' << check_sum << '\n';
            found = true;
            std::exit(0);
            //return true;
        } else {
            //return false;
        }
    } else {
        signs[k] = '-';
        recursive_sol(values, signs, found, k - 1, curr_sum - values[k],
                check_sum);
        signs[k] = '+';
        recursive_sol(values, signs, found, k - 1, curr_sum + values[k],
                check_sum);
    }
}

void sol2(std::vector<int>& values, const int check_sum) {
    size_t num_values = values.size();
    std::string signs(num_values, '+');
    bool found {false};
    recursive_sol(values, signs, found, num_values, 0, check_sum);
    //if (!recursive_sol(values, signs, found, num_values, 0, check_sum))
    if (!found)
        std::cout << "No solution\n";
}

int main() {

    size_t num_values;
    int check_sum;

    std::ifstream ifs {"input.txt"};
    if (!(ifs >> num_values >> check_sum)) {
        throw std::runtime_error{"cannot open file"};
        return 1;
    }

    std::vector<int> values {};
    for (int i {0}; i < num_values; ++i) {
        int x;
        ifs >> x;
        if (x != 0) {
            values.push_back(x);
        }
    }
    //for (auto &x: values)
    //    ifs >> x;

    sol1(values, check_sum);

    //sol2(values, check_sum);

    return 0;
}

