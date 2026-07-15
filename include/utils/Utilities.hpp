#pragma once
#include <string>
#include <algorithm>
#include <cctype>

namespace sfi::utils {

class StringUtils {
public:
    static std::string toLower(const std::string& input) {
        std::string result = input;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c){ return std::tolower(c); });
        return result;
    }

    static bool containsIgnoreCase(const std::string& str, const std::string& substr) {
        if (substr.empty()) return true;
        auto it = std::search(
            str.begin(), str.end(),
            substr.begin(), substr.end(),
            [](char ch1, char ch2) { return std::tolower((unsigned char)ch1) == std::tolower((unsigned char)ch2); }
        );
        return it != str.end();
    }
};

} // namespace sfi::utils
