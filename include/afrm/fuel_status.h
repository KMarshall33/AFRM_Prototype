
#pragma once

#include <optional>
#include <string_view>

namespace afrm {

    enum class FuelStatus {
        Insufficient,
        Caution,
        Sufficient
    };

    std::optional<FuelStatus> classifyFuelStatus(
        double fuelMarginKg,
        double cautionThresholdKg
    );

    std::string_view fuelStatusToString(
        FuelStatus status
    );

} // namespace afrm
