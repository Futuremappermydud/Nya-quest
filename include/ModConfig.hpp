#pragma once

#include "config-utils/shared/config-utils.hpp"

// Declare the mod config as "ModConfiguration" and declare all its values and functions.
DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(NSFWEnabled, bool, "NSFW Enabled", false);
    CONFIG_VALUE(API, std::string, "API", "waifu.pics");
    CONFIG_VALUE(SFWEndpoint, std::string, "SFW Endpoint", "neko");
    CONFIG_VALUE(NSFWEndpoint, std::string, "NSFW Endpoint", "neko");

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(NSFWEnabled);
        CONFIG_INIT_VALUE(API);
        CONFIG_INIT_VALUE(SFWEndpoint);
        CONFIG_INIT_VALUE(NSFWEndpoint);
    )
)