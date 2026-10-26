
#include "afrm/fuel_status.h"

#include <cmath>

namespace afrm {

    std::optional<FuelStatus> classifyFuelStatus(
        double fuelMarginKg,
        double cautionThresholdKg)
    {
        // Reject invalid numerical values
        if (!std::isfinite(fuelMarginKg) ||
            !std::isfinite(cautionThresholdKg) ||
            cautionThresholdKg < 0.0) {
            return std::nullopt;
        }

        // Insufficient fuel
        if (fuelMarginKg < 0.0) {
            return FuelStatus::Insufficient;
        }

        // Caution: requirement met but limited margin
        if (fuelMarginKg < cautionThresholdKg) {
            return FuelStatus::Caution;
        }

        // Sufficient fuel
        return FuelStatus::Sufficient;
    }

    std::string_view fuelStatusToString(
        FuelStatus status)
    {
        switch (status) {
        case FuelStatus::Insufficient:
            return "INSUFFICIENT";

        case FuelStatus::Caution:
            return "CAUTION";

        case FuelStatus::Sufficient:
            return "SUFFICIENT";
        }

        return "UNKNOWN";
    }

} // namespace afrm
