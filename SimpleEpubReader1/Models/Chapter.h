#pragma once
#include "Chapter.g.h"

namespace winrt::SimpleEpubReader::implementation
{
    struct Chapter : ChapterT<Chapter>
    {
        Chapter() = default;

        Chapter(winrt::hstring const& content);
        winrt::hstring Title();
        winrt::hstring Text();

    private: 
        winrt::hstring m_title; 
        winrt::hstring m_text; 
    };
}
namespace winrt::SimpleEpubReader::factory_implementation
{
    struct Chapter : ChapterT<Chapter, implementation::Chapter>
    {
    };
}
