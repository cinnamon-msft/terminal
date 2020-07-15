// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "KeyChord.g.h"

namespace winrt::Microsoft::Terminal::Settings::Model::implementation
{
    struct KeyChord : KeyChordT<KeyChord>
    {
        KeyChord() noexcept;
        KeyChord(KeyModifiers const& modifiers, int32_t vkey) noexcept;
        KeyChord(bool ctrl, bool alt, bool shift, int32_t vkey) noexcept;

        KeyModifiers Modifiers() noexcept;
        void Modifiers(KeyModifiers const& value) noexcept;
        int32_t Vkey() noexcept;
        void Vkey(int32_t value) noexcept;

    private:
        KeyModifiers _modifiers;
        int32_t _vkey;
    };
}

namespace winrt::Microsoft::Terminal::Settings::Model::factory_implementation
{
    struct KeyChord : KeyChordT<KeyChord, implementation::KeyChord>
    {
    };
}
