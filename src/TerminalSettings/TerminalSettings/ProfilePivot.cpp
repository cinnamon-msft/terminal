#include "pch.h"
#include "ProfilePivot.h"
#include "ProfilePivot.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::SettingsControl::implementation
{
    ProfilePivot::ProfilePivot()
    {
        InitializeComponent();
    }

    int32_t ProfilePivot::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void ProfilePivot::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void ProfilePivot::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {

    }
}
