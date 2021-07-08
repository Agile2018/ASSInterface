#pragma once

#include "ASSInterface.h"
#include "iface.h"
#include <string>

namespace ASSInterface {
	std::string FaceAttributeDependenciesStatusToString(IFACE_FaceAttributeDependenciesStatus faceAttributeDependenciesStatus);
	std::string FaceAttributeRangeStatusToString(IFACE_FaceAttributeRangeStatus icaoCompliance);
	std::string IcaoFinalComplianceToString(bool IcaoFinalCompliance);
}
