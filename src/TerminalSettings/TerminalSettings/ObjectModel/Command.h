#pragma once
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include "ObjectModel.Command.g.h"

// Use this macro to quick implement both the getter and setter for a property.
// This should only be used for simple types where there's no logic in the
// getter/setter beyond just accessing/updating the value.
#define GETSET_PROPERTY(type, name, ...)                                                              \
public:                                                                                               \
    type name() const noexcept { return _##name; }                                                    \
    void name(const type& value) noexcept                                                             \
    {                                                                                                 \
        if (value != _##name)                                                                         \
        {                                                                                             \
            _##name = value;                                                                          \
            m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"##name" }); \
        }                                                                                             \
    }                                                                                                 \
                                                                                                      \
private:                                                                                              \
    type _##name{ __VA_ARGS__ };

#define DEFINE_PROPERTYCHANGED()                                                                     \
public:                                                                                              \
    event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler) \
    {                                                                                                \
        return m_propertyChanged.add(handler);                                                       \
    }                                                                                                \
                                                                                                     \
    void PropertyChanged(event_token const& token)                                                   \
    {                                                                                                \
        m_propertyChanged.remove(token);                                                             \
    }                                                                                                \
                                                                                                     \
private:                                                                                             \
    winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

namespace winrt::ObjectModel::implementation
{
    struct Command : CommandT<Command>
    {
    public:
        Command() = default;

        GETSET_PROPERTY(hstring, CommandString, L"");
        GETSET_PROPERTY(hstring, Args, L"");
        GETSET_PROPERTY(hstring, KeybindString, L"");
        DEFINE_PROPERTYCHANGED();
    };
}
