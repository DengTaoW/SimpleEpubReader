#include "pch.h"
#include "Chapter.h"
#include "Chapter.g.cpp"

namespace winrt::SimpleEpubReader::implementation
{
    Chapter::Chapter(winrt::hstring const& content)
        : m_text {content}
    {
    }

    winrt::hstring Chapter::Title()
    {
        uint32_t end = 0; 
        uint32_t start = 0; 
        while (m_title.empty()) {
            for (uint32_t i = start; i < m_text.size(); ++i) {
                if (m_text[i] == L'\n') {
                    end = i; 
                    break; 
                }
            }
            
            if (end != start) {
                winrt::impl::hstring_builder builder(end - start);
                std::copy(m_text.begin() + start, m_text.begin() + end, builder.data());
                m_title = builder.to_hstring(); 
            }

            start = end + 1; 
            if (start >= m_text.size()) {
                m_title = L"Title"; 
                break; 
            }

            if (m_text[start] == L'\r') {
                ++start; 
            }
        }
        return m_title; 
    }

    winrt::hstring Chapter::Text()
    {
        return m_text; 
    }
}
