#pragma once
#include <string>

#include "beatsaber-hook/shared/utils/typedefs.h"

namespace Nya::Utils {
    List<StringW>* vectorToList(std::vector<StringW> values);
    std::vector<StringW> listWToVector(List<StringW>* values);
    int findStrIndexInList(List<StringW>* values, StringW string );
}

