#include "pch.h"
#include "CommandModel.h"
#include "ObjectModel.CommandListModel.g.cpp"
#include "CommandListModel.h"


namespace winrt::ObjectModel::implementation
{
    CommandListModel::CommandListModel()
    {
        m_commandList = winrt::single_threaded_vector<ObjectModel::CommandModel>();
    }

    winrt::Windows::Foundation::Collections::IVector<ObjectModel::CommandModel> CommandListModel::CommandList()
    {
        return m_commandList;
    }
}
