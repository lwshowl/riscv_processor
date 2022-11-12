#pragma once
#include <stdio.h>
#include <vector>
#include <string>

#define ASNI_FG_BLACK "\33[1;30m"
#define ASNI_FG_RED "\33[1;31m"
#define ASNI_FG_GREEN "\33[1;32m"
#define ASNI_FG_YELLOW "\33[1;33m"
#define ASNI_FG_BLUE "\33[1;34m"
#define ASNI_FG_MAGENTA "\33[1;35m"
#define ASNI_FG_CYAN "\33[1;36m"
#define ASNI_FG_WHITE "\33[1;37m"
#define ASNI_BG_BLACK "\33[1;40m"
#define ASNI_BG_RED "\33[1;41m"
#define ASNI_BG_GREEN "\33[1;42m"
#define ASNI_BG_YELLOW "\33[1;43m"
#define ASNI_BG_BLUE "\33[1;44m"
#define ASNI_BG_MAGENTA "\33[1;35m"
#define ASNI_BG_CYAN "\33[1;46m"
#define ASNI_BG_WHITE "\33[1;47m"
#define ASNI_NONE "\33[0m"

#define _Log(...)        \
  do                     \
  {                      \
    printf(__VA_ARGS__); \
  } while (0)

#define ASNI_FMT(str, fmt) fmt str ASNI_NONE

#define Log(format, ...)                                  \
  _Log(ASNI_FMT("[%s:%d %s] " format, ASNI_FG_BLUE) "\n", \
       __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define FMT_WORD "0x%016lx"

namespace std
{
  static vector<string> split(string s, string delimiter)
  {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
      token = s.substr(pos_start, pos_end - pos_start);
      pos_start = pos_end + delim_len;
      res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
  }
}
