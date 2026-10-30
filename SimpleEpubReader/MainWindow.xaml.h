#pragma once

#include "MainWindow.g.h"

namespace winrt::SimpleEpubReader::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
        winrt::fire_and_forget OpenFile_ButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

    };
}

namespace winrt::SimpleEpubReader::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
