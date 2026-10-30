#pragma once 
#include "ZipHelper.h"
#include <memory>

namespace Helper
{
	class EpubHelper
	{
	public: 
		explicit EpubHelper(winrt::hstring const& filePath); 
		~EpubHelper(); 
		
		/// <summary>
		/// 获取 content.opf 文件在 zip 内部的名称。
		/// </summary>
		/// <returns>如果失败，返回空字符串。</returns>
		winrt::hstring GetOpfEntryName(); 
		
		/// <summary>
		/// 解析 content.opf 文件，获取需要的数据。
		/// </summary>
		/// <param name="contentOpfName"></param>
		/// <returns></returns>
		winrt::Windows::Foundation::IAsyncAction ParseContentOpfAsync(winrt::hstring const& contentOpfName); 

		winrt::hstring DocTitle() const; 
		winrt::hstring DocCreator() const; 
		uint32_t SpineItemsCount() const; 
		winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Xaml::Media::ImageSource> GetCoverImageSourceAsync(); 
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> GetSpineItemContentAsync(uint32_t index); 

	private: 
		std::unique_ptr<ZipHelper> m_zipHelper{ nullptr }; 
		winrt::hstring m_docTitle; 
		winrt::hstring m_docCreator; 
		winrt::hstring m_coverEntryName; 
		winrt::Windows::Foundation::Collections::IVector<winrt::hstring> m_spineEntryNames{ nullptr }; 
	};
}