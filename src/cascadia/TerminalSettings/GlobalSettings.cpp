// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "GlobalSettings.h"
#include "../types/inc/utils.hpp"
#include "../inc/DefaultSettings.h"
#include "Utils.h"
#include "JsonUtils.h"

#include "GlobalSettings.g.cpp"

using namespace winrt::Microsoft::Terminal::Settings::Model::implementation;

static constexpr std::string_view LegacyKeybindingsKey{ "keybindings" };
static constexpr std::string_view BindingsKey{ "bindings" };
static constexpr std::string_view DefaultProfileKey{ "defaultProfile" };
static constexpr std::string_view AlwaysShowTabsKey{ "alwaysShowTabs" };
static constexpr std::string_view InitialRowsKey{ "initialRows" };
static constexpr std::string_view InitialColsKey{ "initialCols" };
static constexpr std::string_view InitialPositionKey{ "initialPosition" };
static constexpr std::string_view ShowTitleInTitlebarKey{ "showTerminalTitleInTitlebar" };
static constexpr std::string_view ThemeKey{ "theme" };
static constexpr std::string_view TabWidthModeKey{ "tabWidthMode" };
static constexpr std::string_view ShowTabsInTitlebarKey{ "showTabsInTitlebar" };
static constexpr std::string_view WordDelimitersKey{ "wordDelimiters" };
static constexpr std::string_view CopyOnSelectKey{ "copyOnSelect" };
static constexpr std::string_view CopyFormattingKey{ "copyFormatting" };
static constexpr std::string_view WarnAboutLargePasteKey{ "largePasteWarning" };
static constexpr std::string_view WarnAboutMultiLinePasteKey{ "multiLinePasteWarning" };
static constexpr std::string_view LaunchModeKey{ "launchMode" };
static constexpr std::string_view ConfirmCloseAllKey{ "confirmCloseAllTabs" };
static constexpr std::string_view SnapToGridOnResizeKey{ "snapToGridOnResize" };
static constexpr std::string_view EnableStartupTaskKey{ "startOnUserLogin" };
static constexpr std::string_view AlwaysOnTopKey{ "alwaysOnTop" };

static constexpr std::string_view DebugFeaturesKey{ "debugFeatures" };

static constexpr std::string_view ForceFullRepaintRenderingKey{ "experimental.rendering.forceFullRepaint" };
static constexpr std::string_view SoftwareRenderingKey{ "experimental.rendering.software" };
static constexpr std::string_view ForceVTInputKey{ "experimental.input.forceVT" };

#ifdef _DEBUG
static constexpr bool debugFeaturesDefault{ true };
#else
static constexpr bool debugFeaturesDefault{ false };
#endif

GlobalSettings::GlobalSettings() :
    _unparsedDefaultProfile{ std::nullopt },
    _defaultProfile{},
    _InitialRows{ DEFAULT_ROWS },
    _InitialCols{ DEFAULT_COLS },
    _WordDelimiters{ DEFAULT_WORD_DELIMITERS },
    _DebugFeaturesEnabled{ debugFeaturesDefault }
{
}

GlobalSettings::~GlobalSettings()
{
}

void GlobalSettings::DefaultProfile(const GUID defaultProfile) noexcept
{
    _unparsedDefaultProfile.reset();
    _defaultProfile = defaultProfile;
}

GUID GlobalSettings::DefaultProfile() const
{
    // If we have an unresolved default profile, we should likely explode.
    THROW_HR_IF(E_INVALIDARG, _unparsedDefaultProfile.has_value());
    return _defaultProfile;
}

std::wstring GlobalSettings::UnparsedDefaultProfile() const
{
    return _unparsedDefaultProfile.value();
}

// Method Description:
// - Create a new instance of this class from a serialized JsonObject.
// Arguments:
// - json: an object which should be a serialization of a GlobalAppSettings object.
// Return Value:
// - a new GlobalAppSettings instance created from the values in `json`
winrt::com_ptr<GlobalSettings> GlobalSettings::FromJson(const Json::Value& json)
{
    auto result = winrt::make_self<GlobalSettings>();
    result->LayerJson(json);
    return result;
}

void GlobalSettings::LayerJson(const Json::Value& json)
{
    JsonUtils::GetValueForKey(json, DefaultProfileKey, _unparsedDefaultProfile);

    JsonUtils::GetValueForKey(json, AlwaysShowTabsKey, _AlwaysShowTabs);

    JsonUtils::GetValueForKey(json, ConfirmCloseAllKey, _ConfirmCloseAllTabs);

    JsonUtils::GetValueForKey(json, InitialRowsKey, _InitialRows);

    JsonUtils::GetValueForKey(json, InitialColsKey, _InitialCols);

    // TODO CARLOS: JsonConversion: Figure out Json ConversionTrait
    //JsonUtils::GetValueForKey(json, InitialPositionKey, _InitialPosition);

    JsonUtils::GetValueForKey(json, ShowTitleInTitlebarKey, _ShowTitleInTitlebar);

    JsonUtils::GetValueForKey(json, ShowTabsInTitlebarKey, _ShowTabsInTitlebar);

    JsonUtils::GetValueForKey(json, WordDelimitersKey, _WordDelimiters);

    JsonUtils::GetValueForKey(json, CopyOnSelectKey, _CopyOnSelect);

    JsonUtils::GetValueForKey(json, CopyFormattingKey, _CopyFormatting);

    JsonUtils::GetValueForKey(json, WarnAboutLargePasteKey, _WarnAboutLargePaste);

    JsonUtils::GetValueForKey(json, WarnAboutMultiLinePasteKey, _WarnAboutMultiLinePaste);

    // TODO CARLOS: JsonConversion: Figure out Json ConversionTrait
    //JsonUtils::GetValueForKey(json, LaunchModeKey, _LaunchMode);

    //JsonUtils::GetValueForKey(json, ThemeKey, _Theme);

    //JsonUtils::GetValueForKey(json, TabWidthModeKey, _TabWidth);

    JsonUtils::GetValueForKey(json, SnapToGridOnResizeKey, _SnapToGridOnResize);

    // GetValueForKey will only override the current value if the key exists
    JsonUtils::GetValueForKey(json, DebugFeaturesKey, _DebugFeaturesEnabled);

    JsonUtils::GetValueForKey(json, ForceFullRepaintRenderingKey, _ForceFullRepaintRendering);

    JsonUtils::GetValueForKey(json, SoftwareRenderingKey, _SoftwareRendering);
    JsonUtils::GetValueForKey(json, ForceVTInputKey, _ForceVTInput);

    JsonUtils::GetValueForKey(json, EnableStartupTaskKey, _StartOnUserLogin);

    JsonUtils::GetValueForKey(json, AlwaysOnTopKey, _AlwaysOnTop);
}
