/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- GlobalSettings.h

Abstract:
- This class encapsulates all of the settings that are global to the app, and
    not a part of any particular profile.

Author(s):
- Mike Griese - March 2019

Revision History:
- Renamed from GlobalAppSettings.h and moved to TerminalSettings project
   from TerminalAppLib (Carlos Zamora, July 2020)

--*/
#pragma once

#include "GlobalSettings.g.h"
#include "..\inc\cppwinrt_utils.h"
#include "JsonUtils.h"

namespace winrt::Microsoft::Terminal::Settings::Model::implementation
{
    struct GlobalSettings : public GlobalSettingsT<GlobalSettings>
    {
    public:
        GlobalSettings();
        ~GlobalSettings();

        static com_ptr<GlobalSettings> FromJson(const Json::Value& json);
        void LayerJson(const Json::Value& json);

        // These are implemented manually to handle the string/GUID exchange
        // by higher layers in the app.
        void DefaultProfile(const GUID defaultProfile) noexcept;
        GUID DefaultProfile() const;
        std::wstring UnparsedDefaultProfile() const;

        GETSET_PROPERTY(int32_t, InitialRows); // default value set in constructor
        GETSET_PROPERTY(int32_t, InitialCols); // default value set in constructor
        GETSET_PROPERTY(bool, AlwaysShowTabs, true);
        GETSET_PROPERTY(bool, ShowTitleInTitlebar, true);
        GETSET_PROPERTY(bool, ConfirmCloseAllTabs, true);
        GETSET_PROPERTY(winrt::Windows::UI::Xaml::ElementTheme, Theme, winrt::Windows::UI::Xaml::ElementTheme::Default);
        GETSET_PROPERTY(TabWidthMode, TabWidth, TabWidthMode::Equal);
        GETSET_PROPERTY(bool, ShowTabsInTitlebar, true);
        GETSET_PROPERTY(hstring, WordDelimiters); // default value set in constructor
        GETSET_PROPERTY(bool, CopyOnSelect, false);
        GETSET_PROPERTY(bool, CopyFormatting, false);
        GETSET_PROPERTY(bool, WarnAboutLargePaste, true);
        GETSET_PROPERTY(bool, WarnAboutMultiLinePaste, true);
        GETSET_PROPERTY(LaunchPosition, InitialPosition);
        GETSET_PROPERTY(AppLaunchMode, LaunchMode, AppLaunchMode::DefaultMode);
        GETSET_PROPERTY(bool, SnapToGridOnResize, true);
        GETSET_PROPERTY(bool, ForceFullRepaintRendering, false);
        GETSET_PROPERTY(bool, SoftwareRendering, false);
        GETSET_PROPERTY(bool, ForceVTInput, false);
        GETSET_PROPERTY(bool, DebugFeaturesEnabled); // default value set in constructor
        GETSET_PROPERTY(bool, StartOnUserLogin, false);
        GETSET_PROPERTY(bool, AlwaysOnTop, false);

    private:
        std::optional<std::wstring> _unparsedDefaultProfile;
        GUID _defaultProfile;
    };
}

namespace winrt::Microsoft::Terminal::Settings::Model::factory_implementation
{
    BASIC_FACTORY(GlobalSettings);
}
