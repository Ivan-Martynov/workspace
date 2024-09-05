#ifndef _H_BATCH_FILE_RENAMER_H_
#define _H_BATCH_FILE_RENAMER_H_

#include <filesystem>
#include <vector>

namespace Marvin
{

class BatchFileRenamer
{
  private:
    // Parameters.
    bool m_recursive {false};
    bool m_verbose {true};
    std::vector<std::filesystem::directory_entry> entries {};

  public:
    explicit BatchFileRenamer() = default;
    explicit BatchFileRenamer(
        const std::vector<std::string>&, const bool = false, const bool = true);

    explicit BatchFileRenamer(const BatchFileRenamer& other) = default;
    BatchFileRenamer& operator=(const BatchFileRenamer& other) = default;

    explicit BatchFileRenamer(BatchFileRenamer&& other) = default;
    BatchFileRenamer& operator=(BatchFileRenamer&& other) = default;

    // Functions to modify the paths.
    void replace_using_regex(const std::wstring&, const std::wstring&);
    void replace_spaces_with_underscores();
};

} // namespace Marvin

#endif // _H_BATCH_FILE_RENAMER_H_
