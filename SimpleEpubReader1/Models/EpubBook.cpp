#include "pch.h"
#include "EpubBook.h"

namespace winrt::SimpleEpubReader::implementation
{
	EpubBook::EpubBook(winrt::hstring const& filePath)
		: m_chapters{ winrt::single_threaded_observable_vector<winrt::SimpleEpubReader::Chapter>() }
		, m_filePath{ filePath }
	{

	}

	winrt::Windows::Foundation::IAsyncAction EpubBook::InitializeAsync()
	{
		co_return; 
	}

	winrt::hstring EpubBook::FilePath()
	{
		return m_filePath; 
	}

	winrt::hstring EpubBook::DocTitle()
	{
		return m_docTitle; 
	}

	winrt::hstring EpubBook::DocCreator()
	{
		return m_docCreator; 
	}

	winrt::Microsoft::UI::Xaml::Media::ImageSource EpubBook::CoverImageSource()
	{
		return m_coverImageSource; 
	}

	winrt::Windows::Foundation::Collections::IObservableVector<winrt::SimpleEpubReader::Chapter> EpubBook::Chapters()
	{
		return m_chapters; 
	}
}
