#include "pch.h"
#include "Launch.h"
#include "Launch.g.cpp"

using namespace winrt;
using namespace winrt::Windows::UI::Xaml;

namespace winrt::SettingsControl::implementation
{
    Launch::Launch()
    {
        InitializeComponent();
    }

    void Launch::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
    }
}
