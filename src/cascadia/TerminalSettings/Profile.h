/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- Profile.h

Abstract:
- A profile acts as a single set of terminal settings. Many tabs or panes could
     exist side-by-side with different profiles simultaneously.

Author(s):
- Mike Griese - March 2019

--*/

#pragma once

#include "Profile.g.h"
#include "DefaultSettings.h"
#include "../inc/cppwinrt_utils.h"
#include "JsonUtils.h"

// GUID used for generating GUIDs at runtime, for profiles that did not have a
// GUID specified manually.
constexpr GUID RUNTIME_GENERATED_PROFILE_NAMESPACE_GUID = { 0xf65ddb7e, 0x706b, 0x4499, { 0x8a, 0x50, 0x40, 0x31, 0x3c, 0xaf, 0x51, 0x0a } };

namespace winrt::Microsoft::Terminal::Settings::Model::implementation
{
    struct Profile : public ProfileT<Profile>
    {
    public:
        Profile();
        explicit Profile(const GUID& guid);

        ~Profile();

        Json::Value GenerateStub() const;
        static com_ptr<Profile> FromJson(const Json::Value& json);
        bool ShouldBeLayered(const Json::Value& json) const;
        void LayerJson(const Json::Value& json);
        static bool IsDynamicProfileObject(const Json::Value& json);

        void GenerateGuidIfNecessary() noexcept;

        static GUID GetGuidOrGenerateForJson(const Json::Value& json) noexcept;

        void SetRetroTerminalEffect(bool value) noexcept;

        GETSET_PROPERTY(Windows::Foundation::IReference<guid>, Guid, nullptr);
        GETSET_PROPERTY(hstring, Name, L"Default");
        GETSET_PROPERTY(Windows::Foundation::IReference<hstring>, Source, nullptr);
        GETSET_PROPERTY(Windows::Foundation::IReference<guid>, ConnectionType, nullptr);
        GETSET_PROPERTY(Windows::Foundation::IReference<hstring>, Icon, nullptr);
        GETSET_PROPERTY(bool, Hidden, false);
        GETSET_PROPERTY(CloseOnExitMode, CloseOnExit, CloseOnExitMode::Graceful);
        GETSET_PROPERTY(hstring, TabTitle);

        // Terminal Control Settings
        GETSET_PROPERTY(bool, UseAcrylic, false);
        GETSET_PROPERTY(double, AcrylicOpacity, 0.5);
        GETSET_PROPERTY(ScrollbarState, ScrollState);
        GETSET_PROPERTY(hstring, FontFace, DEFAULT_FONT_FACE);
        GETSET_PROPERTY(int32_t, FontSize, DEFAULT_FONT_SIZE);
        GETSET_PROPERTY(Windows::UI::Text::FontWeight, FontWeight, DEFAULT_FONT_WEIGHT);
        GETSET_PROPERTY(hstring, Padding, DEFAULT_PADDING);
        GETSET_PROPERTY(bool, CopyOnSelect);
        GETSET_PROPERTY(hstring, Commandline, L"cmd.exe");
        GETSET_PROPERTY(hstring, StartingDirectory);
        GETSET_PROPERTY(hstring, EnvironmentVariables);
        GETSET_PROPERTY(Windows::Foundation::IReference<hstring>, BackgroundImage);
        GETSET_PROPERTY(Windows::Foundation::IReference<double>, BackgroundImageOpacity);
        GETSET_PROPERTY(Windows::Foundation::IReference<Windows::UI::Xaml::Media::Stretch>, BackgroundImageStretchMode, Windows::UI::Xaml::Media::Stretch::Fill);

    public:
        // BackgroundImageAlignment is 1 setting saved as 2 separate values
        Windows::UI::Xaml::HorizontalAlignment BackgroundImageHorizontalAlignment() const noexcept;
        void BackgroundImageHorizontalAlignment(const Windows::UI::Xaml::HorizontalAlignment& value) noexcept;
        Windows::UI::Xaml::VerticalAlignment BackgroundImageVerticalAlignment() const noexcept;
        void BackgroundImageVerticalAlignment(const Windows::UI::Xaml::VerticalAlignment& value) noexcept;

        GETSET_PROPERTY(uint32_t, SelectionBackground);
        GETSET_PROPERTY(TextAntialiasingMode, AntialiasingMode, TextAntialiasingMode::Grayscale);
        GETSET_PROPERTY(bool, RetroTerminalEffect);
        GETSET_PROPERTY(bool, ForceFullRepaintRendering);
        GETSET_PROPERTY(bool, SoftwareRendering);

        // Terminal Core Settings
        GETSET_PROPERTY(uint32_t, DefaultForeground);
        GETSET_PROPERTY(uint32_t, DefaultBackground);
        GETSET_PROPERTY(hstring, ColorScheme, L"Campbell");
        GETSET_PROPERTY(int32_t, HistorySize, DEFAULT_HISTORY_SIZE);
        GETSET_PROPERTY(int32_t, InitialRows);
        GETSET_PROPERTY(int32_t, InitialCols);
        GETSET_PROPERTY(bool, SnapOnInput, true);
        GETSET_PROPERTY(bool, AltGrAliasing, true);
        GETSET_PROPERTY(uint32_t, CursorColor);
        GETSET_PROPERTY(CursorStyle, CursorShape, CursorStyle::Bar);
        GETSET_PROPERTY(uint32_t, CursorHeight, DEFAULT_CURSOR_HEIGHT);
        GETSET_PROPERTY(hstring, StartingTitle);
        GETSET_PROPERTY(bool, SuppressApplicationTitle);
        GETSET_PROPERTY(bool, ForceVTInput);

    private:
        // TODO CARLOS:
        // A few things were originally in Profile, but now belong in TerminalApp
        // or ICore/ControlSettings
        // - CreateTerminalSettings()
        // - EvaluateStartingDirectory()

        static GUID _GenerateGuidForProfile(const hstring& name, const Windows::Foundation::IReference<hstring>& source) noexcept;

        std::tuple<Windows::UI::Xaml::HorizontalAlignment, Windows::UI::Xaml::VerticalAlignment> _BackgroundImageAlignment;
    };
}

namespace winrt::Microsoft::Terminal::Settings::Model::factory_implementation
{
    BASIC_FACTORY(Profile);
}
