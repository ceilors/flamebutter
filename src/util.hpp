#ifndef __UTIL_HPP__
#define __UTIL_HPP__
#include <string>
#include <vector>

typedef std::vector<std::wstring::const_iterator> str_index;

str_index split_lines(const std::wstring &text);

#endif