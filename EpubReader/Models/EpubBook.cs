using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;
using System.Xml.XPath;
using EpubReader.Helper;
using Microsoft.UI.Xaml.Media;
using Microsoft.Windows.Storage;

namespace EpubReader.Models;

internal partial class EpubBook(string filePath)
{
    private string _oebpsDirectoryName = string.Empty;
    
    public string FilePath { get; init; } = filePath;

    public string DocTitle { get; private set; } = string.Empty;

    public string DocCreator { get; private set; } = string.Empty;

    public ImageSource? CoverImageSource { get; private set; }

    public ObservableCollection<NavPoint> NavPoints { get; } = []; 


    public async Task InitializeAsync()
    {
        // FilePath -> OpfEntryName -> TocNcxEntryName
        using ZipHelper zipHelper = new(FilePath);
        var opfEntryName = ParseContainerXml(zipHelper);
        var tocNcxEntryName = await ParseContentOpfXml(zipHelper, opfEntryName);
        await ParseTocNcxAsync(zipHelper, tocNcxEntryName);
    }

    public async Task<string> ExtractFilesAsync()
    {
        var tempFolder = ApplicationData.GetDefault().TemporaryFolder;
        var dataFolderName = string.Format("{0:X}", FilePath.GetHashCode());
        var dataFolder = await tempFolder.CreateFolderAsync(dataFolderName, Windows.Storage.CreationCollisionOption.ReplaceExisting);

        using var zipHelper = new ZipHelper(FilePath);
        await zipHelper.ExtractAllFilesAsync(dataFolder.Path);

        return Path.Combine(dataFolder.Path, _oebpsDirectoryName); 
    }


    private async Task ParseTocNcxAsync(ZipHelper zip, string tocNcxEntryName)
    {
        string content = await zip.ExtractFileTextAsync(tocNcxEntryName);
        if (string.IsNullOrEmpty(content)) return;

        XDocument doc = XDocument.Parse(content);
        XElement root = doc.Root ?? throw new InvalidDataException(); 

        var reader = doc.CreateReader();
        var nsManager = new XmlNamespaceManager(reader.NameTable);
        nsManager.AddNamespace("ncx", "http://www.daisy.org/z3986/2005/ncx/");

        var navPointElements = root.XPathSelectElements("./ncx:navMap/ncx:navPoint", nsManager);

        foreach (XElement item in navPointElements)
        {
            var navPoint = NavPoint.ParseXElement(item, nsManager); 
            this.NavPoints.Add(navPoint);
        }
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
        XElement root = doc.Root ?? throw new InvalidDataException(); 

        var reader = doc.CreateReader();
        var nsManager = new XmlNamespaceManager(reader.NameTable);
        nsManager.AddNamespace("ns", "urn:oasis:names:tc:opendocument:xmlns:container"); 

        var rootfile = root.XPathSelectElement("./ns:rootfiles/ns:rootfile", nsManager);
        return rootfile?.Attribute("full-path")?.Value ?? throw new InvalidDataException(); 
    }


    private async Task<string> ParseContentOpfXml(ZipHelper zip, string entryName)
    {
        string xmlContent = await zip.ExtractFileTextAsync(entryName);
        XDocument doc = XDocument.Parse(xmlContent);
        XElement root = doc.Root ?? throw new InvalidDataException();

        var reader = doc.CreateReader();
        var nsManager = new XmlNamespaceManager(reader.NameTable);
        nsManager.AddNamespace("opf", "http://www.idpf.org/2007/opf");
        nsManager.AddNamespace("dc", "http://purl.org/dc/elements/1.1/");

        var titleElement = root.XPathSelectElement("./opf:metadata/dc:title", nsManager);
        if (titleElement != null)
            DocTitle = titleElement.Value;

        var creatorElement = root.XPathSelectElement("./opf:metadata/dc:creator", nsManager);
        if (creatorElement != null)
            DocCreator = creatorElement.Value;

        // cover image 
        _oebpsDirectoryName = Path.GetDirectoryName(entryName) ?? string.Empty;  
        var coverMeta = root.XPathSelectElement("./opf:metadata/opf:meta[@name='cover']", nsManager);
        XElement? coverItem; 
        if (coverMeta != null)
        {
            string coverItemId = coverMeta.Attribute("content")?.Value ?? string.Empty;
            coverItem = root.XPathSelectElement($"./opf:manifest/opf:item[@id='{coverItemId}']", nsManager); 
        }
        else
        {
            coverItem = root.XPathSelectElement("./opf:manifest/opf:item[@properties='cover-image']", nsManager); 
        }

        if (coverItem != null)
        {
            var coverEntryName = Path.Combine(_oebpsDirectoryName, coverItem.Attribute("href")?.Value ?? string.Empty);
            coverEntryName = coverEntryName.Replace('\\', '/');
            CoverImageSource = await zip.ExtractImageSourceAsync(coverEntryName); 
        }

        // ncx file
        var ncxItem = root.XPathSelectElement("./opf:manifest/opf:item[@media-type='application/x-dtbncx+xml']", nsManager); 
        if (ncxItem != null)
        {
            var tocNcxEntryName = Path.Combine(_oebpsDirectoryName, ncxItem.Attribute("href")?.Value ?? string.Empty);
            tocNcxEntryName = tocNcxEntryName.Replace('\\', '/');
            return tocNcxEntryName;

        }
        return string.Empty; 
    }
}
