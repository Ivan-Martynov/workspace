#include <iostream>
#include <vector>
#include <algorithm>

struct StudentGrade
{
    std::string name;
    char grade;
};

class GradeMap
{
  private:
    std::vector<StudentGrade> m_map {};

  public:
    char& operator[](std::string_view name)
    {
        if (const auto res {std::find_if(m_map.begin(), m_map.end(),
                [name](const auto& item) { return item.name == name; })};
            res == m_map.cend())
        {
            return m_map.emplace_back(std::string {name}).grade;
        }
        else
        {
            return res->grade;
        }
    }
};

int main()
{
    GradeMap grades{};

	grades["Joe"] = 'A';
	grades["Frank"] = 'B';

	std::cout << "Joe has a grade of " << grades["Joe"] << '\n';
	std::cout << "Frank has a grade of " << grades["Frank"] << '\n';

    return 0;
}
