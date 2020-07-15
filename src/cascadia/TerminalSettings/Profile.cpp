// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "Profile.h"
#include "Utils.h"
#include "JsonUtils.h"
#include "../types/inc/utils.hpp"

#include "LegacyProfileGeneratorNamespaces.h"
#include "TerminalSettingsSerializationHelpers.h"

#include "Profile.g.cpp"

using namespace ::Microsoft::Console;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Microsoft::Terminal::Settings::Model::implementation;

static constexpr std::string_view NameKey{ "name" };
static constexpr std::string_view GuidKey{ "guid" };
static constexpr std::string_view SourceKey{ "source" };
static constexpr std::string_view ColorSchemeKey{ "colorScheme" };
static constexpr std::string_view HiddenKey{ "hidden" };

static constexpr std::string_view ForegroundKey{ "foreground" };
static constexpr std::string_view BackgroundKey{ "background" };
static constexpr std::string_view SelectionBackgroundKey{ "selectionBackground" };
static constexpr std::string_view TabTitleKey{ "tabTitle" };
static constexpr std::string_view SuppressApplicationTitleKey{ "suppressApplicationTitle" };
static constexpr std::string_view HistorySizeKey{ "historySize" };
static constexpr std::string_view SnapOnInputKey{ "snapOnInput" };
static constexpr std::string_view AltGrAliasingKey{ "altGrAliasing" };
static constexpr std::string_view CursorColorKey{ "cursorColor" };
static constexpr std::string_view CursorShapeKey{ "cursorShape" };
static constexpr std::string_view CursorHeightKey{ "cursorHeight" };

static constexpr std::string_view ConnectionTypeKey{ "connectionType" };
static constexpr std::string_view CommandlineKey{ "commandline" };
static constexpr std::string_view FontFaceKey{ "fontFace" };
static constexpr std::string_view FontSizeKey{ "fontSize" };
static constexpr std::string_view FontWeightKey{ "fontWeight" };
static constexpr std::string_view AcrylicOpacityKey{ "acrylicOpacity" };
static constexpr std::string_view UseAcrylicKey{ "useAcrylic" };
static constexpr std::string_view ScrollbarStateKey{ "scrollbarState" };
static constexpr std::string_view CloseOnExitKey{ "closeOnExit" };
static constexpr std::string_view PaddingKey{ "padding" };
static constexpr std::string_view StartingDirectoryKey{ "startingDirectory" };
static constexpr std::string_view IconKey{ "icon" };
static constexpr std::string_view BackgroundImageKey{ "backgroundImage" };
static constexpr std::string_view BackgroundImageOpacityKey{ "backgroundImageOpacity" };
static constexpr std::string_view BackgroundImageStretchModeKey{ "backgroundImageStretchMode" };
static constexpr std::string_view BackgroundImageAlignmentKey{ "backgroundImageAlignment" };
static constexpr std::string_view RetroTerminalEffectKey{ "experimental.retroTerminalEffect" };
static constexpr std::string_view AntialiasingModeKey{ "antialiasingMode" };

Profile::Profile()
{
}

Profile::Profile(const GUID& guid) :
    _Guid(guid)
{
}

Profile::~Profile()
{
}

// Method Description:
// - Generates a Json::Value which is a "stub" of this profile. This stub will
//   have enough information that it could be layered with this profile.
// - This method is used during dynamic profile generation - if a profile is
//   ever generated that didn't already exist in the user's settings, we'll add
//   this stub to the user's settings file, so the user has an easy point to
//   modify the generated profile.
// Arguments:
// - <none>
// Return Value:
// - A json::Value with a guid, name and source (if applicable).
Json::Value Profile::GenerateStub() const
{
    Json::Value stub;

    ///// Profile-specific settings /////
    if (_Guid == nullptr)
    {
        stub[JsonKey(GuidKey)] = winrt::to_string(Utils::GuidToString(_Guid.Value()));
    }

    stub[JsonKey(NameKey)] = winrt::to_string(_Name);

    if (_Source != nullptr)
    {
        stub[JsonKey(SourceKey)] = winrt::to_string(_Source.Value());
    }

    stub[JsonKey(HiddenKey)] = _Hidden;

    return stub;
}

// Method Description:
// - Create a new instance of this class from a serialized JsonObject.
// Arguments:
// - json: an object which should be a serialization of a Profile object.
// Return Value:
// - a new Profile instance created from the values in `json`
winrt::com_ptr<Profile> Profile::FromJson(const Json::Value& json)
{
    auto result = winrt::make_self<Profile>();
    result->LayerJson(json);
    return result;
}

// Method Description:
// - Returns true if we think the provided json object represents an instance of
//   the same object as this object. If true, we should layer that json object
//   on us, instead of creating a new object.
// Arguments:
// - json: The json object to query to see if it's the same
// Return Value:
// - true iff the json object has the same `GUID` as we do.
bool Profile::ShouldBeLayered(const Json::Value& json) const
{
    if (_Guid == nullptr)
    {
        return false;
    }

    // First, check that GUIDs match. This is easy. If they don't match, they
    // should _definitely_ not layer.
    if (const auto otherGuid{ JsonUtils::GetValueForKey<std::optional<GUID>>(json, GuidKey) })
    {
        if (otherGuid.has_value() && IsEqualGUID(otherGuid.value(), _Guid.Value()) == 0) // optional compare takes care of this
        {
            return false;
        }
    }
    else
    {
        // If the other json object didn't have a GUID, we definitely don't want
        // to layer. We technically might have the same name, and would
        // auto-generate the same guid, but they should be treated as different
        // profiles.
        return false;
    }

    std::optional<std::wstring> otherSource;
    bool otherHadSource = JsonUtils::GetValueForKey(json, SourceKey, otherSource);

    // For profiles with a `source`, also check the `source` property.
    bool sourceMatches = false;
    if (_Source != nullptr)
    {
        if (otherHadSource)
        {
            // If we have a source and the other has a source, compare them!
            sourceMatches = otherSource == _Source.Value();
        }
        else
        {
            // Special case the legacy dynamic profiles here. In this case,
            // `this` is a dynamic profile with a source, and our _source is one
            // of the legacy DPG namespaces. We're looking to see if the other
            // json object has the same guid, but _no_ "source"
            if (_Source.Value() == WslGeneratorNamespace ||
                _Source.Value() == AzureGeneratorNamespace ||
                _Source.Value() == PowershellCoreGeneratorNamespace)
            {
                sourceMatches = true;
            }
        }
    }
    else
    {
        // We do not have a source. The only way we match is if source is unset or set to "".
        sourceMatches = (!otherSource.has_value() || otherSource.value() == L"");
    }

    return sourceMatches;
}

// Method Description:
// - Layer values from the given json object on top of the existing properties
//   of this object. For any keys we're expecting to be able to parse in the
//   given object, we'll parse them and replace our settings with values from
//   the new json object. Properties that _aren't_ in the json object will _not_
//   be replaced.
// - Optional values in the profile that are set to `null` in the json object
//   will be set to nullopt.
// Arguments:
// - json: an object which should be a partial serialization of a Profile object.
// Return Value:
// <none>
void Profile::LayerJson(const Json::Value& json)
{
    // Profile-specific Settings
    JsonUtils::GetValueForKey(json, NameKey, _Name);
    // TODO CARLOS: no '=' operator for guid
    //JsonUtils::GetValueForKey(json, GuidKey, _Guid);
    JsonUtils::GetValueForKey(json, HiddenKey, _Hidden);

    // Core Settings
    JsonUtils::GetValueForKey(json, ForegroundKey, _DefaultForeground);
    JsonUtils::GetValueForKey(json, BackgroundKey, _DefaultBackground);
    JsonUtils::GetValueForKey(json, SelectionBackgroundKey, _SelectionBackground);
    JsonUtils::GetValueForKey(json, CursorColorKey, _CursorColor);
    JsonUtils::GetValueForKey(json, ColorSchemeKey, _ColorScheme);

    // TODO:MSFT:20642297 - Use a sentinel value (-1) for "Infinite scrollback"
    JsonUtils::GetValueForKey(json, HistorySizeKey, _HistorySize);
    JsonUtils::GetValueForKey(json, SnapOnInputKey, _SnapOnInput);
    JsonUtils::GetValueForKey(json, AltGrAliasingKey, _AltGrAliasing);
    JsonUtils::GetValueForKey(json, CursorHeightKey, _CursorHeight);
    JsonUtils::GetValueForKey(json, CursorShapeKey, _CursorShape);
    JsonUtils::GetValueForKey(json, TabTitleKey, _TabTitle);

    // Control Settings
    JsonUtils::GetValueForKey(json, FontWeightKey, _FontWeight);
    // TODO CARLOS: no '=' operator for guid
    //JsonUtils::GetValueForKey(json, ConnectionTypeKey, _ConnectionType);
    JsonUtils::GetValueForKey(json, CommandlineKey, _Commandline);
    JsonUtils::GetValueForKey(json, FontFaceKey, _FontFace);
    JsonUtils::GetValueForKey(json, FontSizeKey, _FontSize);
    JsonUtils::GetValueForKey(json, AcrylicOpacityKey, _AcrylicOpacity);
    JsonUtils::GetValueForKey(json, UseAcrylicKey, _UseAcrylic);
    JsonUtils::GetValueForKey(json, SuppressApplicationTitleKey, _SuppressApplicationTitle);
    JsonUtils::GetValueForKey(json, CloseOnExitKey, _CloseOnExit);
    JsonUtils::GetValueForKey(json, PaddingKey, _Padding);
    JsonUtils::GetValueForKey(json, ScrollbarStateKey, _ScrollState);
    JsonUtils::GetValueForKey(json, StartingDirectoryKey, _StartingDirectory);
    JsonUtils::GetValueForKey(json, IconKey, _Icon);
    JsonUtils::GetValueForKey(json, BackgroundImageKey, _BackgroundImage);
    JsonUtils::GetValueForKey(json, BackgroundImageOpacityKey, _BackgroundImageOpacity);
    JsonUtils::GetValueForKey(json, BackgroundImageStretchModeKey, _BackgroundImageStretchMode);
    JsonUtils::GetValueForKey(json, BackgroundImageAlignmentKey, _BackgroundImageAlignment);
    JsonUtils::GetValueForKey(json, RetroTerminalEffectKey, _RetroTerminalEffect);
    JsonUtils::GetValueForKey(json, AntialiasingModeKey, _AntialiasingMode);
}

HorizontalAlignment Profile::BackgroundImageHorizontalAlignment() const noexcept
{
    return std::get<0>(_BackgroundImageAlignment);
}

void Profile::BackgroundImageHorizontalAlignment(const HorizontalAlignment& value) noexcept
{
    std::get<0>(_BackgroundImageAlignment) = value;
}

VerticalAlignment Profile::BackgroundImageVerticalAlignment() const noexcept
{
    return std::get<1>(_BackgroundImageAlignment);
}

void Profile::BackgroundImageVerticalAlignment(const VerticalAlignment& value) noexcept
{
    std::get<1>(_BackgroundImageAlignment) = value;
}

// Function Description:
// - Returns true if the given JSON object represents a dynamic profile object.
//   If it is a dynamic profile object, we should make sure to only layer the
//   object on a matching profile from a dynamic source.
// Arguments:
// - json: the partial serialization of a profile object to check
// Return Value:
// - true iff the object has a non-null `source` property
bool Profile::IsDynamicProfileObject(const Json::Value& json)
{
    const auto& source = json.isMember(JsonKey(SourceKey)) ? json[JsonKey(SourceKey)] : Json::Value::null;
    return !source.isNull();
}

// Method Description:
// - If this profile never had a GUID set for it, generate a runtime GUID for
//   the profile. If a profile had their guid manually set to {0}, this method
//   will _not_ change the profile's GUID.
void Profile::GenerateGuidIfNecessary() noexcept
{
    if (_Guid == nullptr)
    {
        // Always use the name to generate the temporary GUID. That way, across
        // reloads, we'll generate the same static GUID.
        _Guid = winrt::guid{ Profile::_GenerateGuidForProfile(_Name, _Source) };

        // TODO CARLOS: figure out how to move telemetry provider to Terminal Settings (or introduce a new one)
        //TraceLoggingWrite(
        //    g_hTerminalAppProvider,
        //    "SynthesizedGuidForProfile",
        //    TraceLoggingDescription("Event emitted when a profile is deserialized without a GUID"),
        //    TraceLoggingKeyword(MICROSOFT_KEYWORD_MEASURES),
        //    TelemetryPrivacyDataTag(PDT_ProductAndServicePerformance));
    }
}

// Function Description:
// - Generates a unique guid for a profile, given the name. For an given name, will always return the same GUID.
// Arguments:
// - name: The name to generate a unique GUID from
// Return Value:
// - a uuidv5 GUID generated from the given name.
GUID Profile::_GenerateGuidForProfile(const hstring& name, const IReference<hstring>& source) noexcept
{
    // If we have a _source, then we can from a dynamic profile generator. Use
    // our source to build the namespace guid, instead of using the default GUID.

    const GUID namespaceGuid = source != nullptr ?
                                   Utils::CreateV5Uuid(RUNTIME_GENERATED_PROFILE_NAMESPACE_GUID, gsl::as_bytes(gsl::make_span(source.Value()))) :
                                   RUNTIME_GENERATED_PROFILE_NAMESPACE_GUID;

    // Always use the name to generate the temporary GUID. That way, across
    // reloads, we'll generate the same static GUID.
    return Utils::CreateV5Uuid(namespaceGuid, gsl::as_bytes(gsl::make_span(name)));
}
