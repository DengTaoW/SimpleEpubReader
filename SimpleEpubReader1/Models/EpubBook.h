#pragma once
#include "EpubBook.g.h"


namespace winrt::SimpleEpubReader::implementation
{
    struct EpubBook : EpubBookT<EpubBook>
    {
        EpubBook() {}; 

        EpubBook(winrt::hstring const& filePath);
        winrt::Windows::Foundation::IAsyncAction InitializeAsync();
        winrt::hstring FilePath();
        winrt::hstring DocTitle();
        winrt::hstring DocCreator();
        winrt::Microsoft::UI::Xaml::Media::ImageSource CoverImageSource();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::SimpleEpubReader::Chapter> Chapters();

    private: 
        winrt::hstring m_filePath; 
        winrt::hstring m_docTitle; 
        winrt::hstring m_docCreator; 
        winrt::Microsoft::UI::Xaml::Media::ImageSource m_coverImageSource{ nullptr }; 
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::SimpleEpubReader::Chapter> m_chapters{ nullptr }; 
    };
}
namespace winrt::SimpleEpubReader::factory_implementation
{
    struct EpubBook : EpubBookT<EpubBook, implementation::EpubBook>
    {
    };
}
