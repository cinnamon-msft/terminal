#include "pch.h"
#include "Command.h"
#include "ObjectModel.CommandModel.g.cpp"
#include "CommandModel.h"

namespace winrt::ObjectModel::implementation
{
    CommandModel::CommandModel()
    {
        m_command = winrt::make<ObjectModel::implementation::Command>();
    }

    ObjectModel::Command CommandModel::Command()
    {
        return m_command;
    }
}
