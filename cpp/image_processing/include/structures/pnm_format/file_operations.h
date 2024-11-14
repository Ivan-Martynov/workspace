#ifndef H_INCLUDE_STRUCTURES_PNM_FORMAT_FILE_OPERATIONS_H_H
#define H_INCLUDE_STRUCTURES_PNM_FORMAT_FILE_OPERATIONS_H_H

#include <fstream>
#include <limits>

namespace Marvin
{

namespace PNM_Format
{

namespace File_Operations
{

void skip_comment(std::ifstream& stream)
{
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

} // namespace File_Operations

    
} // namespace PNM_Format


} // namespace Marvin

#endif // H_INCLUDE_STRUCTURES_PNM_FORMAT_FILE_OPERATIONS_H_H
