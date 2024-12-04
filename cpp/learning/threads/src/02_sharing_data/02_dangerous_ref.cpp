#include <iostream>
#include <thread>

namespace
{

class Data
{
  public:
    void do_something()
    {
        std::cout << "Do something... For example, print this - seems to be "
                     "harmless enough to output a line of text.\n";
    }

  private:
    int a {};
    std::string b {};
};

class DataWrapper
{
  public:
    template <typename Function>
    void process_data(Function func)
    {
        std::lock_guard guard {m_mutex};
        func(m_data);
    }

  private:
    Data m_data {};
    std::mutex m_mutex {};
};

Data* g_unprotected_data_ptr {};

void malicious_function(Data& protected_data)
{
    g_unprotected_data_ptr = &protected_data;
}

DataWrapper g_wrapper {};

} // namespace

int main()
{
    g_wrapper.process_data(malicious_function);
    g_unprotected_data_ptr->do_something();

    return 0;
}
