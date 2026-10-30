#include "pch.h"
#include "ZipHelper.h"
#include <string>
#include <filesystem>
#include <fstream>

namespace Helper
{
	ZipHelper::ZipHelper(winrt::hstring const& filePath)
	{
		std::string path = winrt::to_string(filePath); 
		m_isReady = mz_zip_reader_init_file(&m_zip, path.c_str(), 0); 
	}

	ZipHelper::~ZipHelper()
	{
		if (m_isReady) {
			mz_zip_reader_end(&m_zip);
		}
	}

	bool ZipHelper::IsReady() const
	{
		return m_isReady;
	}

	winrt::hstring ZipHelper::ExtractFileText(winrt::hstring const& entryName)
	{
		if (!m_isReady || entryName.empty()) {
			return winrt::hstring{};
		}

		std::string entryNameStr = winrt::to_string(entryName);
		uint32_t index{};
		if (!mz_zip_reader_locate_file_v2(&m_zip, entryNameStr.c_str(), nullptr, 0, &index)) {
			return winrt::hstring{};
		}

		mz_zip_archive_file_stat st{};
		if (!mz_zip_reader_file_stat(&m_zip, index, &st)) {
			return winrt::hstring{};
		}

		std::string buffer(st.m_uncomp_size, '\0');
		if (!mz_zip_reader_extract_to_mem(&m_zip, index, buffer.data(), buffer.size(), 0)) {
			return winrt::hstring{};
		}

		return winrt::to_hstring(buffer);
	}

	winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> ZipHelper::ExtractFileTextAsync(winrt::hstring const& entryName)
	{
		winrt::apartment_context context; 
		co_await winrt::resume_background(); 

		winrt::hstring result = ExtractFileText(entryName); 
		co_await context; 
		co_return result; 
	}

	winrt::Windows::Storage::Streams::Buffer ZipHelper::ExtractFileData(winrt::hstring const& entryName)
	{
		using Buffer = winrt::Windows::Storage::Streams::Buffer;

		if (!m_isReady || entryName.empty()) {
			return Buffer{ nullptr };
		}

		std::string entryNameStr = winrt::to_string(entryName);
		uint32_t index{};

		if (!mz_zip_reader_locate_file_v2(&m_zip, entryNameStr.c_str(), nullptr, 0, &index)) {
			return Buffer{ nullptr };
		}

		mz_zip_archive_file_stat st{};
		if (!mz_zip_reader_file_stat(&m_zip, index, &st)) {
			return Buffer{ nullptr };
		}

		uint32_t bufferSize = static_cast<uint32_t>(st.m_uncomp_size); 
		Buffer result{ bufferSize };
		if (!mz_zip_reader_extract_to_mem(&m_zip, index, result.data(), st.m_uncomp_size, 0)) {
			return Buffer{ nullptr }; 
		}

		result.Length(bufferSize);
		return result; 
	}

	winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::Streams::Buffer> ZipHelper::ExtractFileDataAsync(winrt::hstring const& entryName)
	{
		winrt::apartment_context context; 
		co_await winrt::resume_background(); 

		winrt::Windows::Storage::Streams::Buffer buffer = ExtractFileData(entryName); 
		co_await context; 
		co_return buffer; 
	}

	bool ZipHelper::ExtractFiles(winrt::hstring const& folderPath)
	{
		if (!m_isReady || folderPath.empty()) {
			return false; 
		}

		std::filesystem::path outputPath{ folderPath.c_str() }; 
		if (!std::filesystem::exists(outputPath)) {
			std::filesystem::create_directories(outputPath); 
		}

		mz_uint numFiles = mz_zip_reader_get_num_files(&m_zip); 
		mz_uint errFilesCount = 0u; 
		
		for (mz_uint i = 0; i < numFiles; ++i) {
			mz_zip_archive_file_stat st{}; 
			if (!mz_zip_reader_file_stat(&m_zip, i, &st)) {
				continue; 
			}

			winrt::hstring entryName = winrt::to_hstring(std::string{ st.m_filename });
			std::filesystem::path itemPath = outputPath / entryName.c_str(); 
			itemPath.make_preferred(); 

			if (st.m_is_directory) {
				std::filesystem::create_directories(itemPath); 
				continue; 
			}
			
			if (!std::filesystem::exists(itemPath.parent_path())) {
				std::filesystem::create_directories(itemPath.parent_path()); 
			}

			std::string destFileName = itemPath.string(); 
			if (!mz_zip_reader_extract_to_file(&m_zip, i, destFileName.c_str(), 0)) {
				++errFilesCount; 
			}
		}

		return errFilesCount == 0u; 
	}

	winrt::Windows::Foundation::IAsyncOperation<bool> ZipHelper::ExtractFilesAsync(winrt::hstring const& folderPath)
	{
		winrt::apartment_context context; 
		co_await winrt::resume_background(); 

		bool result = ExtractFiles(folderPath); 
		co_await context; 
		co_return result; 
	}

	std::vector<uint8_t> ZipHelper::ReadBinaryFile(const std::string& filePath)
	{
		std::ifstream file{ filePath, std::ios::binary | std::ios::ate }; 

		if (!file.is_open()) {
			return {}; 
		}

		std::streamsize fileSize = file.tellg(); 
		file.seekg(0, std::ios::beg); 

		std::vector<uint8_t> buffer(fileSize); 
		if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
			return {}; 
		}

		return buffer; 
	}
}