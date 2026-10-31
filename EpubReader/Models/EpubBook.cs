using System.IO;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;
using System.Xml.XPath;
using EpubReader.Helper;
using Microsoft.UI.Xaml.Media;

namespace EpubReader.Models;

internal partial class EpubBook(string filePath) : BindableBase
{
    private string _docTitle = string.Empty;
    private string _docCreator = string.Empty;
    private ImageSource? _coverImageSource;


    public string DocTitle
    {
        get => _docTitle;
        set => SetProperty2(ref _docTitle, value); 
    }
    
    public string DocCreator
    {
        get => _docCreator;
        set => SetProperty2(ref _docCreator, value); 
    }

    public ImageSource? CoverImageSource
    {
        get => _coverImageSource;
        set => SetProperty(ref _coverImageSource, value); 
    }

    public async Task InitializeAsync()
    {
        using ZipHelper zipHelper = new(filePath);
        var opfEntryName = ParseContainerXml(zipHelper);
        await ParseContentOpfXml(zipHelper, opfEntryName); 
    }


    /// <summary>
    /// 解析 META-INF/container.xml 文件，返回 content.opf 文件名称。
    /// </summary>
    /// <param name="zip"></param>
    /// <returns>返回 content.opf 文件名称。</returns>
    /// <exception cref="InvalidDataException"></exception>
    private static string ParseContainerXml(ZipHelper zip)
    {
        string xmlContent = zip.ExtractFileText("META-INF/container.xml");
        XDocument doc = XDocument.Parse(xmlContent);  

        var reader = doc.CreateReader();
        var nsManager = new XmlNamespaceManager(reader.NameTable);
        nsManager.AddNamespace("ns", "urn:oasis:names:tc:opendocument:xmlns:container"); 

        var rootfile = doc.Root?.XPathSelectElement("./ns:rootfiles/ns:rootfile", nsManager);
        return rootfile?.Attribute("full-path")?.Value ?? throw new InvalidDataException(); 
    }


    private async Task ParseContentOpfXml(ZipHelper zip, string entryName)
    {
        string xmlContent = await zip.ExtractFileTextAsync(entryName);
        XDocument doc = XDocument.Parse(xmlContent);

        var reader = doc.CreateReader();
        var nsManager = new XmlNamespaceManager(reader.NameTable);
        nsManager.AddNamespace("opf", "http://www.idpf.org/2007/opf");
        nsManager.AddNamespace("dc", "http://purl.org/dc/elements/1.1/");

        var titleElement = doc.Root?.XPathSelectElement("./opf:metadata/dc:title", nsManager);
        if (titleElement != null)
            DocTitle = titleElement.Value;

        var creatorElement = doc.Root?.XPathSelectElement("./opf:metadata/dc:creator", nsManager);
        if (creatorElement != null)
            DocCreator = creatorElement.Value;

        // cover image 
        string entryDirectory = Path.GetDirectoryName(entryName) ?? string.Empty;  
        var coverMeta = doc.Root?.XPathSelectElement("./opf:metadata/opf:meta[@name='cover']", nsManager);
        XElement? coverItem; 
        if (coverMeta != null)
        {
            string coverItemId = coverMeta.Attribute("content")?.Value ?? string.Empty;
            coverItem = doc.Root?.XPathSelectElement($"./opf:manifest/opf:item[@id='{coverItemId}']", nsManager); 
        }
        else
        {
            coverItem = doc.Root?.XPathSelectElement("./opf:manifest/opf:item[@properties='cover-image']", nsManager); 
        }

        if (coverItem != null)
        {
            var coverEntryName = Path.Combine(entryDirectory, coverItem.Attribute("href")?.Value ?? string.Empty);
            coverEntryName = coverEntryName.Replace('\\', '/');
            CoverImageSource = await zip.ExtractImageSourceAsync(coverEntryName); 
        }

        // Spine Items
    }
}
