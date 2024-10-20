#include "SampleStatisticsData.h"

#include <iostream>
#include <numeric>
#include <vector>

void test_statistics()
{
    std::vector<double> vec {1.0, 3.0, 5.0, 9.0};
    Marvin::SampleStatisticsData sample_data {vec};
    std::cout << "Sum = " << sample_data.sum() << "\n";
    std::cout << "Mean = " << sample_data.mean() << "\n";
}

int main(int, char**)
{
    test_statistics();

    return 0;
}