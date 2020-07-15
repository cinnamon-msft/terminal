// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "DefaultProfileUtils.h"
#include "../../types/inc/utils.hpp"

static constexpr std::wstring_view PACKAGED_PROFILE_ICON_PATH{ L"ms-appx:///ProfileIcons/" };
static constexpr std::wstring_view PACKAGED_PROFILE_ICON_EXTENSION{ L".png" };

using namespace winrt::Microsoft::Terminal::Settings::Model;
using namespace ::Microsoft::Console::Utils;

// Method Description:
// - Helper function for creating a skeleton default profile with a pre-populated
//   guid and name.
// Arguments:
// - name: the name of the new profile.
// Return Value:
// - A Profile, ready to be filled in
Profile CreateDefaultProfile(const std::wstring_view name)
{
    const auto profileGuid{ CreateV5Uuid(TERMINAL_PROFILE_NAMESPACE_GUID,
                                         gsl::as_bytes(gsl::make_span(name))) };
    Profile newProfile{ profileGuid };

    newProfile.Name(name);

    std::wstring iconPath{ PACKAGED_PROFILE_ICON_PATH };
    iconPath.append(GuidToString(profileGuid));
    iconPath.append(PACKAGED_PROFILE_ICON_EXTENSION);

    // TODO CARLOS: fix Profile.Icon in Profile.idl
    //newProfile.Icon(winrt::hstring(iconPath));

    return newProfile;
}
