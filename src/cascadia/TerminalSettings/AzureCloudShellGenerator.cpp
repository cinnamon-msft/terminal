// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"

//#include <winrt/Microsoft.Terminal.TerminalConnection.h>

#include "AzureCloudShellGenerator.h"
#include "LegacyProfileGeneratorNamespaces.h"

#include "../../types/inc/utils.hpp"
#include "../../inc/DefaultSettings.h"
#include "Utils.h"
#include "DefaultProfileUtils.h"

using namespace winrt::Microsoft::Terminal::Settings::Model;

std::wstring_view AzureCloudShellGenerator::GetNamespace()
{
    return AzureGeneratorNamespace;
}

// Method Description:
// - Checks if the Azure Cloud shell is available on this platform, and if it
//   is, creates a profile to be able to launch it.
// Arguments:
// - <none>
// Return Value:
// - a vector with the Azure Cloud Shell connection profile, if available.
std::vector<Profile> AzureCloudShellGenerator::GenerateProfiles()
{
    std::vector<Profile> profiles;

    // TODO CARLOS: TerminalConnection: figure out how to actually include TerminalConnection
    //if (winrt::Microsoft::Terminal::TerminalConnection::AzureConnection::IsAzureConnectionAvailable())
    if (false)
    {
        auto azureCloudShellProfile{ CreateDefaultProfile(L"Azure Cloud Shell") };
        azureCloudShellProfile.Commandline(L"Azure");
        azureCloudShellProfile.StartingDirectory(DEFAULT_STARTING_DIRECTORY);
        azureCloudShellProfile.ColorScheme({ L"Vintage" });
        azureCloudShellProfile.AcrylicOpacity(0.6);
        azureCloudShellProfile.UseAcrylic(true);
        //azureCloudShellProfile.ConnectionType(AzureConnectionType);
        profiles.emplace_back(azureCloudShellProfile);
    }

    return profiles;
}
