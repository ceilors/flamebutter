#include "util.hpp"

str_index split_lines(const std::wstring &text) {
    str_index indexes;
    indexes.push_back(text.begin());
    // собираем указатели на блоки данных
    for (std::wstring::const_iterator p = text.begin(); p != text.end(); ++p) {
        if ((*p) == '\n') {
            indexes.push_back(p);
            indexes.push_back(p + 1);
        }
    }
    // добавляем индекс на конец
    indexes.push_back(text.end());
    return indexes;
}