#include <iostream>
#include <vector>
#include <span>
#include <cassert>

namespace Items
{

enum Type : int
{
    health_potion,
    torch,
    arrow,
    max_items,
};

} // namespace Items

static std::string_view get_type_text(int index, int count)
{
    switch (index)
    {
        case Items::health_potion:
            return (count == 1) ? "health potion" : "health potions";
        case Items::torch:
            return (count == 1) ? "torch" : "torches";
        case Items::arrow:
            return (count == 1) ? "arrow" : "arrows";
        case Items::max_items:
            return (count == 1) ? "total item" : "total items";
        default:
            return "???";
    }
}

static void print_info(int i, std::string_view text)
{
    std::cout << "You have " << i << " " << text << "\n";
}

int main()
{
    std::vector inventory {1, 5, 10};
    const auto n {std::ssize(inventory)};

    assert(n == Items::max_items);

    int sum {0};
    for (int i {0}; i < n; ++i)
    {
        print_info(inventory[i], get_type_text(i, inventory[i]));
        // std::cout << "You have " << i << " total items\n";
        sum += inventory[i];
    }

    print_info(sum, get_type_text(n, sum));
    // std::cout << "You have " << count_items(items) << " total items\n";

    return 0;
}