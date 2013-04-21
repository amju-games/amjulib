// From http://stackoverflow.com/questions/10402499/mkdir-c-function

#include <iostream>
#include <sstream>
#include <sys/stat.h>

// for windows mkdir
#ifdef _WIN32
#include <direct.h>
#endif

#include "Directory.h"
//#define MKDIR_DEBUG
#include <StringUtils.h>

namespace Amju
{
    // TODO Have "DirExists" function in Directory. Don't think this works 
    //  for Windows. 
   /**
     * Checks if a folder exists
     * @param foldername path to the folder to check.
     * @return true if the folder exists, false otherwise.
     */
    bool folder_exists(std::string foldername)
    {
        struct stat st;
#ifdef IPHONE
        // j.c. fix for iphone
        if (stat(foldername.c_str(), &st) == -1)
        {
            return false;
        }
        bool exists = (st.st_mode & S_IFDIR) != 0;
        return exists;
#else
        if (stat(foldername.c_str(), &st) != 0)
        {
            return false;
        }
        bool exists = ((st.st_mode & S_IFDIR) != 0);
        return exists;
#endif
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

#ifdef WIN32
        // Get drive letter & colon (windows only)
        std::string drive;
        if (std::getline(ss, drive, ':'))
        {
          std::cout << "Mkdir: Drive: " << drive << "\n";
          current_level = drive + ":/";
        }
#endif
        // split path using slash as a separator
        while (std::getline(ss, level, '/'))
        {
            if (level.empty()) // j.c. fix
            {
                current_level += "/"; // don't forget to append a slash
                continue;
            }
            current_level += level; // append folder to the current level
#ifdef MKDIR_DEBUG_2
            std::cout << "mkdir: current_level: \"" << current_level << "\"\n";
#endif
            // create current level
            if (folder_exists(current_level))
            {
#ifdef MKDIR_DEBUG_2
                std::cout << "Folder DOES exist: " << current_level << "\n";
#endif
            }
            else
            {
#ifdef MKDIR_DEBUG
                std::cout << "Folder does not exist: " << current_level << " - creating...\n";
#endif
                if (_mkdir(current_level.c_str()) != 0)
                {
                    std::cout << "FAILED to create folder " << current_level << "!!\n";
                    return -1;
                }
            }

            current_level += "/"; // don't forget to append a slash
        }

        return 0;
    }

    bool MkDir(const std::string& dir)
    {
      std::string s = Replace(dir, "\\", "/");
      return (mkdir(s.c_str()) == 0);
    }
} // namespace Amju
