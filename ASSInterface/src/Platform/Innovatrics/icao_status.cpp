#include "hzpch.h"
#include "icao_status.h"

namespace ASSInterface {
    std::string FaceAttributeDependenciesStatusToString(IFACE_FaceAttributeDependenciesStatus faceAttributeDependenciesStatus)
    {
        switch (faceAttributeDependenciesStatus)
        {
        case IFACE_FACE_ATTRIBUTE_DEPENDENCIES_STATUS_OK:
            return "dependencies fulfilled";
        case IFACE_FACE_ATTRIBUTE_DEPENDENCIES_STATUS_NOT_OK:
            return "dependencies not fulfilled";
        default:
            return "unknown";
        }
    }

    std::string FaceAttributeRangeStatusToString(IFACE_FaceAttributeRangeStatus faceAttributeRangeStatus)
    {
        switch (faceAttributeRangeStatus)
        {
        case IFACE_FACE_ATTRIBUTE_RANGE_STATUS_TOO_LOW:
            return "range not compliant (too low)";
        case IFACE_FACE_ATTRIBUTE_RANGE_STATUS_IN_RANGE:
            return "range compliant";
        case IFACE_FACE_ATTRIBUTE_RANGE_STATUS_TOO_HIGH:
            return "range not compliant (too high)";
        default:
            return "unknown";
        }
    }

    std::string IcaoFinalComplianceToString(bool IcaoFinalCompliance)
    {
        if (IcaoFinalCompliance)
        {
            return "ICAO compliant";
        }
        else
        {
            return "ICAO not compliant";
        }
    }
}

