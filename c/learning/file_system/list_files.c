#include <stdio.h>
#include <dirent.h>

const char* type_to_string(const unsigned char type)
{
    const char* result;

    switch (type)
    {
        case DT_REG:
            result = "Regular file";
            break;

        case DT_DIR:
            result = "Directory";
            break;

        case DT_FIFO:
            result = "Name pipe (FIFO)";
            break;

        case DT_SOCK:
            result = "Local-domain socket";
            break;

        case DT_CHR:
            result = "Character device";
            break;

        case DT_BLK:
            result = "Block device";
            break;

        case DT_LNK:
            result = "Symbolic link";
            break;

        case DT_UNKNOWN:
            result = "Unknown type";
            break;

        default:
            result = "Unknown type";
            break;
    }

    return result;
}

void default_listing(const char* const path)
{
    DIR* directory = opendir(path);
    if (directory)
    {
        struct dirent* dir;
        while ((dir = readdir(directory)) != NULL)
        {
            const char* type = type_to_string(dir->d_type);
            printf("%s: %s\n", type, dir->d_name);
        }
        closedir(directory);
    }
    else
    {
        printf("Cannot open directory\n");
    }
}

int main(const int argc, const char* argv[static argc])
{
    const char* const directory_path = (argc > 1) ? argv[1] : ".";

    default_listing(directory_path);

    return 0;
}
