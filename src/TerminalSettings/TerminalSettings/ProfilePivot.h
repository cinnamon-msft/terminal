﻿#pragma once

#include "ProfilePivot.g.h"

namespace winrt::SettingsControl::implementation
{
    struct ProfilePivot : ProfilePivotT<ProfilePivot>
    {
        ProfilePivot();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::SettingsControl::factory_implementation
{
    struct ProfilePivot : ProfilePivotT<ProfilePivot, implementation::ProfilePivot>
    {
    };
}
