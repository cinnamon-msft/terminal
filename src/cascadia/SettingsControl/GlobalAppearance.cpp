#include "pch.h"
#include "GlobalAppearance.h"
#include "GlobalAppearance.g.cpp"

using namespace winrt;
using namespace winrt::Windows::UI::Xaml;

namespace winrt::SettingsControl::implementation
{
    GlobalAppearance::GlobalAppearance()
    {
        InitializeComponent();
    }

    void GlobalAppearance::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
    }
}
