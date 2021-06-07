#include "hzpch.h"
#include "ASSInterface/Utils/Math.h"

namespace ASSInterface {
	float Math::Round(float value) {        
        char str[40];
        sprintf(str, "%.2f", value);
        sscanf(str, "%f", &value);

        return value;
	}
}