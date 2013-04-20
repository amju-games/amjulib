// From http://stackoverflow.com/questions/10402499/mkdir-c-function

#include <sstream>
#include <sys/stat.h>

// for windows mkdir
#ifdef _WIN32
#include <direct.h>
#endif

#include "Directory.h"

namespace Amju
{
   /**
     * Checks if a folder exists
     * @param foldername path to the folder to check.
     * @return true if the folder exists, false otherwise.
     */
    bool folder_exists(std::string foldername)
    {
        struct stat st;
        stat(foldername.c_str(), &st);
        return st.st_mode & S_IFDIR;
    }

    /**
     * Portable wrapper for mkdir. Internally used by mkdir()
     * @param[in] path the full path of the directory to create.
     * @return zero on success, otherwise -1.
     */
    int _mkdir(const char *path)
    {
    #ifdef _WIN32
        return ::_mkdir(path);
    #else
    #if _POSIX_C_SOURCE
        return ::mkdir(path);
    #else
        return ::mkdir(path, 0755); // not sure if this works on mac
    #endif
    #endif
    }

    /**
     * Recursive, portable wrapper for mkdir.
     * @param[in] path the full path of the directory to create.
     * @return zero on success, otherwise -1.
     */
    int mkdir(const char *path)
    {
        std::string current_level = "";
        std::string level;
        std::stringstream ss(path);

        // split path using slash as a separator
        while (std::getline(ss, level, '/'))
        {
            current_level += level; // append folder to the current level

            // create current level
            if (!folder_exists(current_level) && _mkdir(current_level.c_str()) != 0)
                return -1;

            current_level += "/"; // don't forget to append a slash
        }

        return 0;
    }

    bool MkDir(const std::string& dir)
    {
      return (mkdir(dir.c_str()) == 0);
    }
} // namespace Amju