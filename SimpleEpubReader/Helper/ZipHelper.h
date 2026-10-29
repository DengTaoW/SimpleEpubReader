#pragma once
#include <miniz/miniz.h>

namespace Helper
{
	class ZipHelper
	{
	public: 
		explicit ZipHelper(winrt::hstring const& filePath);
		~ZipHelper(); 
		bool IsReady() const; 

		/// <summary>
		/// 提取 zip 压缩包内部文本文件（UTF-8编码）的内容。
		/// </summary>
		/// <param name="entryName">内部文件名称（分隔符 '/' ）</param>
		/// <returns>文件内容。如果失败，返回空字符串。</returns>
		winrt::hstring ExtractFileText(winrt::hstring const& entryName); 
		winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> ExtractFileTextAsync(winrt::hstring const& entryName); 

		/// <summary>
		/// 提取 zip 压缩包内部文件的字节流。
		/// </summary>
		/// <param name="entryName">内部文件名称（分隔符 '/'）</param>
		/// <returns>文件内容的字节流。如果失败返回 nullptr。</returns>
		winrt::Windows::Storage::Streams::Buffer ExtractFileData(winrt::hstring const& entryName); 
		winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::Streams::Buffer> ExtractFileDataAsync(winrt::hstring const& entryName); 


		/// <summary>
		///  提取 zip 内部所有文件到 folderPath。
		/// </summary>
		/// <param name="folderPath">输出目录。</param>
		/// <returns>是否成功。成功返回 true，失败返回 false。 </returns>
		bool ExtractFiles(winrt::hstring const& folderPath); 
		winrt::Windows::Foundation::IAsyncOperation<bool> ExtractFilesAsync(winrt::hstring const& folderPath); 

	private:
		mz_zip_archive m_zip;
		bool m_isReady; 

	};
}
