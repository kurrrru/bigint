#include <sstream>
#include <string>

#include <toolbox/string.hpp>

namespace toolbox {

std::string to_string(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

}  // namespace toolbox