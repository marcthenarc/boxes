#ifndef UTIL_HPP__
#define UTIL_HPP__

#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <cstdarg>
#include <cstring>

namespace Template
{
   template<typename T, typename U>
   inline auto find_or_throw(T& t, const U& u) -> decltype(*std::begin(t))
   {
      auto itr = std::find(std::begin(t), std::end(t), u);
      if (itr == std::end(t))
         throw std::runtime_error("Couldn't find element in iterator.");
      return *itr;
   }

   template<typename T, typename U>
   inline void erase_all(T& t, const U& u)
   {
      t.erase(std::remove_if(std::begin(t), std::end(t), [&u](const U& other) { return other == u; }), std::end(t));
   }
}

namespace Log
{
   inline void log(const char *fmt, ...)
   {
      char message[4096];
      va_list list;
      va_start(list, fmt);
      std::vsnprintf(message, sizeof(message), fmt, list);
      std::cerr << "[libretro GL]: " << message << std::endl;
      va_end(list);
   }
}

namespace Path
{
   inline std::string basedir(const std::string& path)
   {
      size_t last = path.find_last_of("/\\");
      if (last != std::string::npos)
         return path.substr(0, last);
      else
         return ".";
   }

   inline std::string join(const std::string& dir, const std::string& path)
   {
      char last = dir.size() ? dir.back() : '\0';
      std::string sep;
      if (last != '/' && last != '\\')
         sep = "/";
      return dir + sep + path;
   }

   inline std::string ext(const std::string& path)
   {
      size_t last = path.find_last_of('.');
      if (last == std::string::npos)
         return "";

      last++;
      if (last == path.size())
         return "";

      return path.substr(last);
   }
}

namespace String
{
   inline std::vector<std::string> split(const std::string& str, const std::string& splitter, bool keep_empty = false)
   {
      std::vector<std::string> list;

      for (std::size_t pos = 0, endpos = 0;
            endpos != std::string::npos; pos = endpos + 1)
      {
         endpos = str.find_first_of(splitter, pos);

         if (endpos - pos)
            list.push_back(str.substr(pos, endpos - pos));
         else if (keep_empty)
            list.push_back("");
      }

      return list;
   }

   inline std::string strip(const std::string& str)
   {
      size_t first_non_space = str.find_first_not_of(" \t\r");
      if (first_non_space == std::string::npos)
         return "";

      return str.substr(first_non_space,
            str.find_last_not_of(" \t\r") - first_non_space + 1);
   }

   inline std::string to_string(std::string str)
   {
      return std::move(str);
   }

   inline std::string to_string(const char *str)
   {
      return str;
   }

   template<typename T>
   inline std::string to_string(T&& t)
   {
      return std::to_string(std::forward<T>(t));
   }

   template<typename T>
   inline std::string cat(T&& t)
   {
      return to_string(std::forward<T>(t));
   }

   template<typename T, typename... P>
   inline std::string cat(T&& t, P&&... p)
   {
      return to_string(std::forward<T>(t)) + cat(std::forward<P>(p)...);
   }
}

namespace File
{
   inline std::string read_string(const std::string& path)
   {
      std::ifstream file{path};
      if (!file.is_open())
         throw std::runtime_error(String::cat("Failed to open file: ", path));

      std::stringstream ss;
      ss << file.rdbuf();
      return ss.str();
   }
}

#endif
