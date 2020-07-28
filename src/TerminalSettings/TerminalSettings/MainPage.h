#pragma once

#include "MainPage.g.h"
#include "winrt/Microsoft.UI.Xaml.Controls.h"
#include "ObjectModel/AppSettings.h"

namespace winrt::SettingsControl::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
        void SettingsNav_Loaded(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
        void SettingsNav_SelectionChanged(const Microsoft::UI::Xaml::Controls::NavigationView sender, const Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs args);
        void SettingsNav_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView const& sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args);

        Windows::Foundation::Collections::IObservableVector<SettingsControl::Setting> FilteredSettings();
        void SetSettings(Windows::Foundation::Collections::IVector<SettingsControl::Setting> const& settings);

        /*void AutoSuggestBox_TextChanged(const winrt::Windows::UI::Xaml::Controls::AutoSuggestBox sender, const winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs args);
        void AutoSuggestBox_QuerySubmitted(const winrt::Windows::UI::Xaml::Controls::AutoSuggestBox sender, const winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs args);
        void AutoSuggestBox_SuggestionChosen(const winrt::Windows::UI::Xaml::Controls::AutoSuggestBox sender, const winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxSuggestionChosenEventArgs args);

        Windows::Foundation::Collections::IVector<winrt::hstring> SearchSettings(winrt::hstring query);*/

    private:
        // XAML should data-bind to the _settingsClone
        // When "save" is pressed, _settingsSource = _settingsClone
        winrt::TerminalSettings::implementation::AppSettings _settingsSource;
        winrt::TerminalSettings::implementation::AppSettings _settingsClone;
        Windows::Foundation::Collections::IVector<SettingsControl::Setting> _allSettings{ nullptr };
        Windows::Foundation::Collections::IObservableVector<SettingsControl::Setting> _filteredSettings{ nullptr };

        void _filterTextChanged(Windows::Foundation::IInspectable const& sender,
                                Windows::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs const& args);
        void _updateFilteredSettings();
    };
}

namespace winrt::SettingsControl::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
