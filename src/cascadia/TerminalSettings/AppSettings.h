/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- AppSettings.h

Abstract:
- This class acts as the container for all of Windows Terminal's settings.
- Contains the logic for serializing and deserializing this object.

Author(s):
- Mike Griese - March 2019

Revision History:
- Renamed from CascadiaSettings.h and moved to TerminalSettings project
   from TerminalAppLib (Carlos Zamora, July 2020)

--*/
#pragma once

#include "AppSettings.g.h"
#include "IDynamicProfileGenerator.h"
#include "../cascadia/inc/cppwinrt_utils.h"
#include "JsonUtils.h"

namespace winrt::Microsoft::Terminal::Settings::Model::implementation
{
    struct AppSettings : public AppSettingsT<AppSettings>
    {
    public:
        AppSettings();
        explicit AppSettings(const bool addDynamicProfiles);

        static std::unique_ptr<AppSettings> LoadDefaults();
        static std::unique_ptr<AppSettings> LoadAll();
        static std::unique_ptr<AppSettings> LoadUniversal();

        static std::unique_ptr<AppSettings> FromJson(const Json::Value& json);
        void LayerJson(const Json::Value& json);

        static std::filesystem::path GetSettingsPath();
        static std::filesystem::path GetDefaultSettingsPath();

        const Profile* FindProfile(GUID profileGuid) const noexcept;
        const ColorScheme* GetColorSchemeForProfile(const GUID profileGuid) const;

        GlobalSettings Globals();
        Windows::Foundation::Collections::IObservableVector<Profile> Profiles();
        // TODO CARLOS
        //Windows::Foundation::Collections::IObservableMap<KeyChord, Action> Keybindings();
        //Windows::Foundation::Collections::IObservableMap<hstring, Action> Commands();
        Windows::Foundation::Collections::IObservableMap<hstring, ColorScheme> Schemes();
        Windows::Foundation::Collections::IVector<SerializationWarning> Warnings();

    private:
        GlobalSettings _globals;
        Windows::Foundation::Collections::IObservableVector<Profile> _profiles;
        // TODO CARLOS
        //Windows::Foundation::Collections::IObservableMap<KeyChord, Action> _keybindings;
        //Windows::Foundation::Collections::IObservableMap<hstring, Action> _commands;
        Windows::Foundation::Collections::IObservableMap<hstring, ColorScheme> _schemes;
        Windows::Foundation::Collections::IVector<SerializationWarning> _warnings;

        std::vector<std::unique_ptr<IDynamicProfileGenerator>> _profileGenerators;

        std::string _userSettingsString;
        Json::Value _userSettings;
        Json::Value _defaultSettings;
        Json::Value _userDefaultProfileSettings{ Json::Value::null };

        void _LayerOrCreateProfile(const Json::Value& profileJson);
        Profile* _FindMatchingProfile(const Json::Value& profileJson);
        void _LayerOrCreateColorScheme(const Json::Value& schemeJson);
        ColorScheme* _FindMatchingColorScheme(const Json::Value& schemeJson);
        void _ParseJsonString(std::string_view fileData, const bool isDefaultSettings);
        static const Json::Value& _GetProfilesJsonObject(const Json::Value& json);
        static const Json::Value& _GetDisabledProfileSourcesJsonObject(const Json::Value& json);
        bool _PrependSchemaDirective();
        bool _AppendDynamicProfilesToUserSettings();
        std::string _ApplyFirstRunChangesToSettingsTemplate(std::string_view settingsTemplate) const;

        void _ApplyDefaultsFromUserSettings();

        void _LoadDynamicProfiles();

        static bool _IsPackaged();
        static void _WriteSettings(const std::string_view content);
        static std::optional<std::string> _ReadUserSettings();
        static std::optional<std::string> _ReadFile(HANDLE hFile);

        std::optional<GUID> _GetProfileGuidByName(const std::wstring_view) const;
        std::optional<GUID> _GetProfileGuidByIndex(std::optional<int> index) const;
        //GUID _GetProfileForArgs(const winrt::TerminalApp::NewTerminalArgs& newTerminalArgs) const;

        void _ValidateSettings();
        void _ValidateProfilesExist();
        void _ValidateProfilesHaveGuid();
        void _ValidateDefaultProfileExists();
        void _ValidateNoDuplicateProfiles();
        void _ResolveDefaultProfile();
        void _ReorderProfilesToMatchUserSettingsOrder();
        void _RemoveHiddenProfiles();
        void _ValidateAllSchemesExist();
        void _ValidateMediaResources();
        void _ValidateKeybindings();
        void _ValidateNoGlobalsKey();

        // TODO CARLOS: Add tests
        //friend class TerminalAppLocalTests::SettingsTests;
        //friend class TerminalAppLocalTests::ProfileTests;
        //friend class TerminalAppLocalTests::ColorSchemeTests;
        //friend class TerminalAppLocalTests::KeyBindingsTests;
        //friend class TerminalAppLocalTests::TabTests;
        //friend class TerminalAppUnitTests::DynamicProfileTests;
        //friend class TerminalAppUnitTests::JsonTests;
    };
}

namespace winrt::Microsoft::Terminal::Settings::Model::factory_implementation
{
    BASIC_FACTORY(AppSettings);
}
