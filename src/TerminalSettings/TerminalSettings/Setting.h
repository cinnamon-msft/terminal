#pragma once

#include "Setting.g.h"

namespace winrt::SettingsControl::implementation
{
    struct Setting : SettingT<Setting>
    {
        Setting() = default;

    };
}
