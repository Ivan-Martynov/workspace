#include <fstream>
#include <iostream>
#include <string>

//#include <filesystem>
#include <experimental/filesystem>

#include <chrono>
#include <map>

namespace fs = std::experimental::filesystem;

void remove_bigger_files(std::string s)
{
    for (auto& p : fs::directory_iterator(s))
    {
        const std::string file_path = std::string(p.path());
        const auto pos = file_path.find_last_of('/') + 1;
        const auto file_name = file_path.substr(pos, file_path.length() - pos);
        const auto file_con
            = file_path.substr(0, file_path.length() - 4) + "-converted.mp4";
        // std::cout << file_con << "\n";
        std::ifstream in(file_path, std::ifstream::ate | std::ifstream::binary);
        std::ifstream con(file_con, std::ifstream::ate | std::ifstream::binary);
        if (con.good())
        {
            if (in.tellg() > con.tellg())
            {
                std::cout << in.tellg() << " > " << con.tellg() << "\n";
                std::remove(file_path.c_str());
            }
            else
            {
                std::remove(file_con.c_str());
            }
        }
    }
}

//--- helper function convert timepoint to usable timestamp
template <typename TP>
time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(
        tp - TP::clock::now() + system_clock::now());
    return system_clock::to_time_t(sctp);
}

void rename_files(std::string s)
{
    std::map<time_t, fs::directory_entry> sort_by_time {};

    //--- sort the files in the map by time
    for (auto& entry : fs::directory_iterator(s))
    {
        if (fs::is_regular_file(entry))
        {
            auto time = to_time_t(fs::last_write_time(entry));
            sort_by_time[time] = entry;
        }
    }

    std::vector<fs::directory_entry> entries {};
    //--- print the files sorted by time
    for (auto const& [time, entry] : sort_by_time)
    {
        std::string timestamp {asctime(std::localtime(&time))};
        timestamp.pop_back(); // remove automatic linebreak

        entries.push_back(entry);
    }

    const std::string string_to_replace {"deti-online.com_-"};
    const size_t len {string_to_replace.length()};
    size_t i {0};
    for (auto& entry : entries)
    {
        const std::string file_path {std::string(entry.path())};

        const auto pos = file_path.find_last_of('/') + 1;

        const auto c = file_path.find(string_to_replace);

        if ((pos == std::string::npos) || (c == std::string::npos))
        {
            continue;
        }
        const auto n = file_path.substr(pos + len);
        const auto m = file_path.substr(0, pos);

        std::string num_str {++i < 10 ? std::string {"0"} + std::to_string(i)
                                      : std::to_string(i)};

        std::string new_name {m + num_str + n};
        std::cout << file_path << " >> " << new_name << "\n";

        std::rename(file_path.c_str(), new_name.c_str());
    }

    return;
#if 0
    for (fs::directory_entry p : fs::directory_iterator(s))
    {

        const std::string file_path = std::string(p.path());

        const auto c = file_path.find(t);

        const auto pos = file_path.find_last_of('/') + 1;
        if (c != std::string::npos)
        {
            // const auto n = file_path.substr(0, c) + ".mp4";
            const auto n = file_path.substr(c + 7);
            const auto m = file_path.substr(0, pos);
            std::cout << file_path << " >> " << m + n << " "
                      << "\n";
            // std::rename(file_path.c_str(), (m + n).c_str());
        }
        // const auto pos = file_path.find_last_of('/') + 1;
        // const auto file_name = file_path.substr(pos, file_path.length() -
        // pos); const auto file_con = file_path.substr(0, file_path.length() -
        // 4) +
        // "-converted.mp4";
        ////std::cout << file_con << "\n";
        // std::ifstream con(file_con, std::ifstream::ate |
        // std::ifstream::binary); if (con.good()) {
        //}
    }
#endif
}

int main()
{
    const auto s = "C:/Users/Ivan/Downloads/Fiksiki/";
    // const auto s = "/home/paaanda/Downloads/photos_Emilia/VIDEO/";
    // remove_bigger_files(s);
    rename_files(s);
    // rename_files(s, "-converted");
    // rename_files(s, "_converted");
}
