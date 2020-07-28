#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "Home.h"
#include "Globals.h"
#include "Launch.h"
#include "Interaction.h"
#include "Rendering.h"
#include "Profiles.h"
#include "GlobalAppearance.h"
#include "ColorSchemes.h"
#include "Keybindings.h"
#include "Setting.h"

#include <winrt/Windows.Foundation.h>
#include <string_view>

namespace winrt
{
    namespace MUX = Microsoft::UI::Xaml;
}

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::TerminalSettings::implementation;

namespace winrt::SettingsControl::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();

        // TODO: When we actually connect this to Windows Terminal,
        //       this section will clone the active AppSettings
        _settingsSource = AppSettings();
        _settingsClone = _settingsSource.Clone();
        _allSettings = winrt::single_threaded_vector<winrt::SettingsControl::Setting>();
        _filteredSettings = winrt::single_threaded_observable_vector<winrt::SettingsControl::Setting>();
    }

    void MainPage::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
    }

    void MainPage::SettingsNav_SelectionChanged(const MUX::Controls::NavigationView, const MUX::Controls::NavigationViewSelectionChangedEventArgs)
    {
    }

    void MainPage::SettingsNav_Loaded(IInspectable const&, RoutedEventArgs const&)
    {
        //// set the initial selectedItem
        for (uint32_t i = 0; i < SettingsNav().MenuItems().Size(); i++)
        {
            const auto item = SettingsNav().MenuItems().GetAt(i).as<Controls::ContentControl>();
            const hstring homeNav = L"Home_Nav";
            const hstring itemTag = unbox_value<hstring>(item.Tag());

            if (itemTag == homeNav)
            {
                // item.IsSelected(true); // have to investigate how to replace this
                SettingsNav().Header() = item.Tag();
                break;
            }
        }

        contentFrame().Navigate(xaml_typename<SettingsControl::Home>());
    }

    void MainPage::SettingsNav_ItemInvoked(MUX::Controls::NavigationView const&, MUX::Controls::NavigationViewItemInvokedEventArgs const& args)
    {
        auto clickedItemContainer = args.InvokedItemContainer();

        if (clickedItemContainer != NULL)
        {
            const hstring homePage = L"Home_Nav";
            const hstring generalPage = L"General_Nav";
            const hstring launchSubpage = L"Launch_Nav";
            const hstring interactionSubpage = L"Interaction_Nav";
            const hstring renderingSubpage = L"Rendering_Nav";

            const hstring profilesPage = L"Profiles_Nav";
            const hstring globalprofileSubpage = L"GlobalProfile_Nav";
            const hstring addnewSubpage = L"AddNew_Nav";

            const hstring appearancePage = L"Appearance_Nav";
            const hstring colorSchemesPage = L"ColorSchemes_Nav";
            const hstring globalAppearancePage = L"GlobalAppearance_Nav";

            const hstring keybindingsPage = L"Keyboard_Nav";
            

            hstring clickedItemTag = unbox_value<hstring>(clickedItemContainer.Tag());

            if (clickedItemTag == homePage)
            {
                contentFrame().Navigate(xaml_typename<SettingsControl::Home>());
            }
            else if (clickedItemTag == launchSubpage)
            {
                contentFrame().Navigate(xaml_typename<SettingsControl::Launch>());
            }
            else if (clickedItemTag == interactionSubpage)
            {
                contentFrame().Navigate(xaml_typename<SettingsControl::Interaction>());
            }
            else if (clickedItemTag == renderingSubpage)
            {
                contentFrame().Navigate(xaml_typename<SettingsControl::Rendering>());
            }
            else if (clickedItemTag == globalprofileSubpage)
            {
                contentFrame().Navigate(xaml_typename<SettingsControl::Profiles>());
            }
            else if (clickedItemTag == addnewSubpage)
            {
                contentFrame().Navigate(xaml_typename<SettingsControl::Profiles>());
            }
            else if (clickedItemTag == colorSchemesPage)
            {
                contentFrame().Navigate(xaml_typename<SettingsControl::ColorSchemes>());
            }
            else if (clickedItemTag == globalAppearancePage)
            {
                contentFrame().Navigate(xaml_typename<SettingsControl::GlobalAppearance>());
            }
            else if (clickedItemTag == keybindingsPage)
            {
                contentFrame().Navigate(xaml_typename<SettingsControl::Keybindings>());
            }
        }
    }

    void MainPage::_filterTextChanged(IInspectable const& /*sender*/,
                                      winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs const& /*args*/)
    {
        _updateFilteredSettings();
    }

    void MainPage::SetSettings(Windows::Foundation::Collections::IVector<SettingsControl::Setting> const& settings)
    {
        _allSettings = settings;
        _updateFilteredSettings();
    }

    //static bool _compareSettings(const SettingsControl::MainPage& lhs, const SettingsControl::MainPage& rhs)
    //{
    //    /*std::wstring_view leftName{ lhs.Name() };
    //    std::wstring_view rightName{ rhs.Name() };
    //    return leftName.compare(rightName) < 0;*/
    //}

    void MainPage::_updateFilteredSettings()
    {
        _filteredSettings.Clear();
        auto searchText = _searchBox().Text();
        /*const bool addAll = _searchBox().empty();*/

        if (searchText != L"")
        {
            std::vector<SettingsControl::Setting> sortedSettings;
            sortedSettings.reserve(_allSettings.Size());

            for (auto setting : _allSettings)
            {
                sortedSettings.push_back(setting);
            }

            /*std::sort(sortedSettings.begin(),
                      sortedSettings.end(),
                      _compareSettings);*/

            for (auto setting : sortedSettings)
            {
                _filteredSettings.Append(setting);
            }

            return;
        }
    }

    //void MainPage::AutoSuggestBox_TextChanged(const winrt::Windows::UI::Xaml::Controls::AutoSuggestBox sender, const winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs args)
    //{
    //    //We only want to get results when it was a user typing,
    //    //otherwise we assume the value got filled in by TextMemberPath
    //    //or the handler for SuggestionChosen
    //    if (args.Reason() == winrt::Windows::UI::Xaml::Controls::AutoSuggestionBoxTextChangeReason(0))
    //    {
    //        Windows::Foundation::Collections::IVector<winrt::hstring> suggestions = SearchSettings(sender.Text());

    //        int tmp = suggestions.Size();
    //        

    //        Windows::Foundation::Collections::IVector<winrt::hstring> empty{ winrt::single_threaded_vector<winrt::hstring>() };
    //        empty.Append(L"No results found");

    //        if (suggestions.Size() > 0)
    //        {
    //            sender.ItemsSource(suggestions);
    //        }
    //        else
    //            sender.ItemsSource(empty);
    //    }
    //}

    /// <summary>
    /// This event gets fired when:
    ///     * a user presses Enter while focus is in the TextBox
    ///     * a user clicks or tabs to and invokes the query button (defined using the QueryIcon API)
    ///     * a user presses selects (clicks/taps/presses Enter) a suggestion
    /// </summary>
    /// <param name="sender">The AutoSuggestBox that fired the event.</param>
    /// <param name="args">The args contain the QueryText, which is the text in the TextBox,
    /// and also ChosenSuggestion, which is only non-null when a user selects an item in the list.</param>

    //void MainPage::AutoSuggestBox_QuerySubmitted(const winrt::Windows::UI::Xaml::Controls::AutoSuggestBox sender, const winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs args)
    //{
    //    //if (args.ChosenSuggestion != null && args.ChosenSuggestion is ControlInfoDataItem)
    //    //{
    //    //    //User selected an item, take an action
    //    //    SelectControl(args.ChosenSuggestion as ControlInfoDataItem);
    //    //}
    //    //else if (!string.IsNullOrEmpty(args.QueryText))
    //    //{
    //    //    //Do a fuzzy search based on the text
    //    //    var suggestions = SearchControls(sender.Text);
    //    //    if (suggestions.Count > 0)
    //    //    {
    //    //        SelectControl(suggestions.FirstOrDefault());
    //    //    }
    //    //}
    //}

    /// <summary>
    /// This event gets fired as the user keys through the list, or taps on a suggestion.
    /// This allows you to change the text in the TextBox to reflect the item in the list.
    /// Alternatively you can use TextMemberPath.
    /// </summary>
    /// <param name="sender">The AutoSuggestBox that fired the event.</param>
    /// <param name="args">The args contain SelectedItem, which contains the data item of the item that is currently highlighted.</param>
    //void MainPage::AutoSuggestBox_SuggestionChosen(const winrt::Windows::UI::Xaml::Controls::AutoSuggestBox sender, const winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxSuggestionChosenEventArgs args)
    //{
    //    //Don't autocomplete the TextBox when we are showing "no results"
    //    //if (args.SelectedItem is ControlInfoDataItem control)
    //    //{
    //    //    sender.Text = control.Title;
    //    //}
    //}

    //Windows::Foundation::Collections::IVector<winrt::hstring> MainPage::SearchSettings(winrt::hstring query)
    //{
    //    std::vector<std::string> settings;

    //    settings.push_back("Default profile");

    //    Windows::Foundation::Collections::IVector<winrt::hstring> suggestions{ winrt::single_threaded_vector<winrt::hstring>() };
    //    std::vector<std::string> querySplit;

    //    std::string tmp = "";
    //    for (int i = 0; i < query.size(); ++i)
    //    {
    //        if (query[i] == ' ')
    //        {
    //            querySplit.push_back(tmp);
    //            tmp = "";
    //        }
    //        else
    //        {
    //            tmp.push_back(query[i]);
    //        }
    //    }
    //    querySplit.push_back(tmp);

    //    
    //    for (int i = 0; i < settings.size(); ++i)
    //    {
    //        for (int j = 0; j < querySplit.size(); ++j)
    //        {
    //            if (settings[i].find(querySplit[j]))
    //            {
    //                suggestions.Append(to_hstring(settings[i]));
    //                break;
    //            }
    //        }
    //    }

    //    for (int i = 0; i < suggestions.Size(); ++i)
    //    {
    //        winrt::hstring tmp = suggestions.GetAt(i);
    //    }

    //    return suggestions;
    //}
}
