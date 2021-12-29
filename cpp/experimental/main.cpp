#include <iostream>
#include <fstream>
#include <string>
//#include <filesystem>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void remove_bigger_files(std::string s) {
    for (auto& p : fs::directory_iterator(s)) {
        const std::string file_path = std::string(p.path());
        const auto pos = file_path.find_last_of('/') + 1;
        const auto file_name = file_path.substr(pos, file_path.length() - pos);
        const auto file_con = file_path.substr(0, file_path.length() - 4) + "-converted.mp4";
        //std::cout << file_con << "\n";
        std::ifstream in(file_path, std::ifstream::ate | std::ifstream::binary);
        std::ifstream con(file_con, std::ifstream::ate | std::ifstream::binary);
        if (con.good()) {
            if (in.tellg() > con.tellg()) {
                std::cout << in.tellg() << " > " << con.tellg() << "\n";
                std::remove(file_path.c_str());
            } else {
                std::remove(file_con.c_str());
            }
        }
    }
}

void rename_files(std::string s, std::string t) {
    for (fs::directory_entry p : fs::directory_iterator(s)) {

        const std::string file_path = std::string(p.path());

        const auto c = file_path.find(t);

        const auto pos = file_path.find_last_of('/') + 1;
        if (c != std::string::npos) {
            //const auto n = file_path.substr(0, c) + ".mp4";
            const auto n = file_path.substr(c + 7);
            const auto m = file_path.substr(0, pos);
            std::cout << file_path << " >> " << m + n << " " << "\n";
            std::rename(file_path.c_str(), (m + n).c_str());
        }
        //const auto pos = file_path.find_last_of('/') + 1;
        //const auto file_name = file_path.substr(pos, file_path.length() - pos);
        //const auto file_con = file_path.substr(0, file_path.length() - 4) + "-converted.mp4";
        ////std::cout << file_con << "\n";
        //std::ifstream con(file_con, std::ifstream::ate | std::ifstream::binary);
        //if (con.good()) {
        //}
    }

}

int main() {
    const auto s = "/home/Data/_share/audio/detskie/";
    //const auto s = "/home/paaanda/Downloads/photos_Emilia/VIDEO/";
    //remove_bigger_files(s);
    rename_files(s, ".com");
    //rename_files(s, "-converted");
    //rename_files(s, "_converted");
}
