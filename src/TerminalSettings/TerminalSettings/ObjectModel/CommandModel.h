#pragma once
#include "ObjectModel.CommandModel.g.h"
#include "Command.h"

namespace winrt::ObjectModel::implementation
{
    struct CommandModel : CommandModelT<CommandModel>
    {
        CommandModel();

        ObjectModel::Command Command();

    private:
        ObjectModel::Command m_command{ nullptr };
    };
}
