#pragma once

#include <iostream>
#include <cstdlib>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <thread>
#include <string>
#include <iomanip>
#include <stdint.h>
#include <stdexcept>
#include <sstream>
#include <array>
#include <vector>
#include <any>
#include <unordered_map>
#include <unordered_set>
#include "ASSInterface/Core/Log.h"
#include "ASSInterface/Debug/Instrumentor.h"

#ifdef ASS_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // ASS_PLATFORM_WINDOWS
