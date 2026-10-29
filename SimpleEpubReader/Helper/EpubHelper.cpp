#include "pch.h"
#include "EpubHelper.h"
#include <filesystem>

namespace Helper
{
	EpubHelper::EpubHelper(winrt::hstring const& filePath)
		: m_zipHelper{ filePath }
	{

	}

	winrt::hstring EpubHelper::GetContentOpfName()
	{
		if (!m_zipHelper.IsReady()) {
			return winrt::hstring{};
		}

		winrt::hstring containerXml = m_zipHelper.ExtractFileText(L"META-INF/container.xml");
		if (containerXml.empty()) {
			return winrt::hstring{}; 
		}

		winrt::hstring ns{ L"xmlns:ns='urn:oasis:names:tc:opendocument:xmlns:container'" };
		winrt::Windows::Foundation::IInspectable nsObject = winrt::box_value(ns);

		winrt::Windows::Data::Xml::Dom::XmlDocument doc; 
		doc.LoadXml(containerXml); 

		auto fullPathNode = doc.DocumentElement().SelectSingleNodeNS(
			L"./ns:rootfiles/ns:rootfile/@full-path",
			nsObject
		); 

		if (!fullPathNode) {
			return winrt::hstring{}; 
		}

		return fullPathNode.InnerText(); 
	}

	winrt::Windows::Foundation::IAsyncAction EpubHelper::ParseContentOpfAsync(winrt::hstring const& contentOpfName)
	{
		if (!m_zipHelper.IsReady()) {
			co_return; 
		}

		std::filesystem::path opfPath{ contentOpfName.c_str() }; 
		std::filesystem::path oebpsPath = opfPath.parent_path(); 

		// 1. get content.opf text 
		winrt::hstring contentOpfXml = co_await m_zipHelper.ExtractFileTextAsync(contentOpfName); 
		if (contentOpfXml.empty()) {
			co_return; 
		}

		// 2. load xml
		winrt::Windows::Data::Xml::Dom::XmlDocument doc; 
		doc.LoadXml(contentOpfXml); 

		winrt::hstring ns =
			L"xmlns:opf='http://www.idpf.org/2007/opf' "
			L"xmlns:dc='http://purl.org/dc/elements/1.1/"; 
		winrt::Windows::Foundation::IInspectable nsObject = winrt::box_value(ns); 

		// 3. metadata
		auto metadata = doc.DocumentElement().SelectSingleNodeNS(L"./opf:metadata", nsObject); 
		winrt::hstring coverId; 

		if (metadata) {
			auto docTitle = metadata.SelectSingleNodeNS(L"./dc:title", nsObject); 
			if (docTitle) {
				m_docTitle = docTitle.InnerText(); 
			}

			auto docCreator = metadata.SelectSingleNodeNS(L"./dc:creator", nsObject); 
			if (docCreator) {
				m_docCreator = docCreator.InnerText(); 
			}

			auto coverContent = metadata.SelectSingleNodeNS(L"./opf:meta[@name='cover']", nsObject); 
			if (coverContent) {
				coverId = coverContent.InnerText(); 
			}
		}

		// 4. manifest
		auto manifest = doc.DocumentElement().SelectSingleNodeNS(L"./opf:manifest", nsObject); 
		if (!manifest) {
			co_return;
		}

		winrt::Windows::Data::Xml::Dom::IXmlNode coverHref{ nullptr };
		if (!coverId.empty()) {
			coverHref = manifest.SelectSingleNodeNS(L"./opf:item[@id='" + coverId + L"']/@href", nsObject); 
		}
		else {
			coverHref = manifest.SelectSingleNodeNS(L"./opf:item[@properties='cover-image']/@href", nsObject); 
		}
		
		if (coverHref) {
			std::filesystem::path coverNamePath = oebpsPath / coverHref.InnerText().c_str(); 
			m_coverName = coverNamePath.generic_wstring(); 
		}

		// 5. spine



	}



}