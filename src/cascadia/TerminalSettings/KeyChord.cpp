// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "KeyChord.h"

#include "KeyChord.g.cpp"

namespace winrt::Microsoft::Terminal::Settings::Model::implementation
{
    KeyChord::KeyChord() noexcept :
        _modifiers{ 0 },
        _vkey{ 0 }
    {
    }

    KeyChord::KeyChord(bool ctrl, bool alt, bool shift, int32_t vkey) noexcept :
        _modifiers{ (ctrl ? KeyModifiers::Ctrl : KeyModifiers::None) |
                    (alt ? KeyModifiers::Alt : KeyModifiers::None) |
                    (shift ? KeyModifiers::Shift : KeyModifiers::None) },
        _vkey{ vkey }
    {
    }

    KeyChord::KeyChord(KeyModifiers const& modifiers, int32_t vkey) noexcept :
        _modifiers{ modifiers },
        _vkey{ vkey }
    {
    }

    KeyModifiers KeyChord::Modifiers() noexcept
    {
        return _modifiers;
    }

    void KeyChord::Modifiers(KeyModifiers const& value) noexcept
    {
        _modifiers = value;
    }

    int32_t KeyChord::Vkey() noexcept
    {
        return _vkey;
    }

    void KeyChord::Vkey(int32_t value) noexcept
    {
        _vkey = value;
    }
}
