#pragma once 
#include "ZipHelper.h"

namespace Helper
{
	class EpubHelper
	{
	public: 
		explicit EpubHelper(winrt::hstring const& filePath); 
		
		/// <summary>
		/// 获取 content.opf 文件在 zip 内部的名称。
		/// </summary>
		/// <returns>如果失败，返回空字符串。</returns>
		winrt::hstring GetContentOpfName(); 
		
		/// <summary>
		/// 解析 content.opf 文件，获取需要的数据。
		/// </summary>
		/// <param name="contentOpfName"></param>
		/// <returns></returns>
		winrt::Windows::Foundation::IAsyncAction ParseContentOpfAsync(winrt::hstring const& contentOpfName); 
		
		

	private: 
		ZipHelper m_zipHelper; 
		winrt::hstring m_docTitle; 
		winrt::hstring m_docCreator; 
		winrt::hstring m_coverName; 
		winrt::Windows::Foundation::Collections::IVector<winrt::hstring> m_spinItems{ nullptr }; 
	};
}