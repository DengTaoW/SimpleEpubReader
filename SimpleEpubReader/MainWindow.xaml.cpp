#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "Helper/EpubHelper.h"
#include "Helper/ZipHelper.h"


namespace winrt::SimpleEpubReader::implementation
{
	MainWindow::MainWindow()
	{

	}

	winrt::fire_and_forget MainWindow::OpenFile_ButtonClick(
		winrt::Windows::Foundation::IInspectable const& , 
		winrt::Microsoft::UI::Xaml::RoutedEventArgs const& )
	{
		using FileOpenPicker = winrt::Microsoft::Windows::Storage::Pickers::FileOpenPicker; 
		using PickFileResult = winrt::Microsoft::Windows::Storage::Pickers::PickFileResult;
		using PickerLocationId = winrt::Microsoft::Windows::Storage::Pickers::PickerLocationId;
		using ImageSource = winrt::Microsoft::UI::Xaml::Media::ImageSource; 


		FileOpenPicker picker{ this->AppWindow().Id() }; 
		picker.FileTypeFilter().Append(L".epub"); 
		picker.SuggestedStartLocation(PickerLocationId::Downloads); 

		PickFileResult result = co_await picker.PickSingleFileAsync(); 
		if (!result) {
			co_return; 
		}

		Helper::EpubHelper epubHelper{ result.Path() }; 
		winrt::hstring opfEntryName = epubHelper.GetOpfEntryName(); 
		co_await epubHelper.ParseContentOpfAsync(opfEntryName); 

		this->OpfEntryLabel().Text(opfEntryName); 
		this->FilePathLabel().Text(result.Path()); 
		this->DocTitleLabel().Text(epubHelper.DocTitle()); 
		this->DocCreatorLabel().Text(epubHelper.DocCreator()); 
		this->SpineItemsCountLabel().Text(winrt::to_hstring(epubHelper.SpineItemsCount())); 

		ImageSource coverImageSource = co_await epubHelper.GetCoverImageSourceAsync(); 
		this->CoverImage().Source(coverImageSource); 

	}

}


