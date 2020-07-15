// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "AppSettings.h"

#include "PowershellCoreProfileGenerator.h"
#include "WslDistroGenerator.h"
#include "AzureCloudShellGenerator.h"

#include "AppSettings.g.cpp"

using namespace winrt::Windows::Foundation::Collections;

static constexpr std::wstring_view PACKAGED_PROFILE_ICON_PATH{ L"ms-appx:///ProfileIcons/" };

static constexpr std::wstring_view PACKAGED_PROFILE_ICON_EXTENSION{ L".png" };
static constexpr std::wstring_view DEFAULT_LINUX_ICON_GUID{ L"{9acb9455-ca41-5af7-950f-6bca1bc9722f}" };

// make sure this matches defaults.json.
static constexpr std::wstring_view DEFAULT_WINDOWS_POWERSHELL_GUID{ L"{61c54bbd-c2c6-5271-96e7-009a87ff44bf}" };

namespace winrt::Microsoft::Terminal::Settings::Model::implementation
{
    AppSettings::AppSettings() :
        AppSettings(true)
    {
    }

    // Constructor Description:
    // - Creates a new settings object. If addDynamicProfiles is true, we'll
    //   automatically add the built-in profile generators to our list of profile
    //   generators. Set this to `false` for unit testing.
    // Arguments:
    // - addDynamicProfiles: if true, we'll add the built-in DPGs.
    AppSettings::AppSettings(const bool addDynamicProfiles)
    {
        if (addDynamicProfiles)
        {
            _profileGenerators.emplace_back(std::make_unique<PowershellCoreProfileGenerator>());
            _profileGenerators.emplace_back(std::make_unique<WslDistroGenerator>());
            _profileGenerators.emplace_back(std::make_unique<AzureCloudShellGenerator>());
        }
    }

    // Method Description:
    // - Finds a profile that matches the given GUID. If there is no profile in this
    //      settings object that matches, returns nullptr.
    // Arguments:
    // - profileGuid: the GUID of the profile to return.
    // Return Value:
    // - a non-ownership pointer to the profile matching the given guid, or nullptr
    //      if there is no match.
    const Profile* AppSettings::FindProfile(GUID profileGuid) const noexcept
    {
        for (auto profile : _profiles)
        {
            try
            {
                if (profile.Guid() != nullptr && IsEqualGUID(profile.Guid().Value(), profileGuid) == 0)
                {
                    return &profile;
                }
            }
            CATCH_LOG();
        }
        return nullptr;
    }

    GlobalSettings AppSettings::Globals()
    {
        return _globals;
    }

    IObservableVector<Profile> AppSettings::Profiles()
    {
        return _profiles;
    }

    // TODO CARLOS
    //IObservableMap<KeyChord, Action> AppSettings::Keybindings()
    //{
    //    return _keybindings;
    //}
    //
    //IObservableMap<hstring, Action> AppSettings::Commands()
    //{
    //    return _commands;
    //}

    IObservableMap<hstring, ColorScheme> AppSettings::Schemes()
    {
        return _schemes;
    }

    IVector<SerializationWarning> AppSettings::Warnings()
    {
        return _warnings;
    }
}
