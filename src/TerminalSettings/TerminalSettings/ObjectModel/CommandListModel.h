#pragma once
#include "ObjectModel.CommandListModel.g.h"
#include "CommandModel.h"

#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::ObjectModel::implementation
{
    struct CommandListModel : CommandListModelT<CommandListModel>
    {
        CommandListModel();

        winrt::Windows::Foundation::Collections::IVector<ObjectModel::CommandModel> CommandList();

    private:
        winrt::Windows::Foundation::Collections::IVector<ObjectModel::CommandModel> m_commandList;
    };
}
