using System.Collections.ObjectModel;
using System.Xml;
using System.Xml.Linq;
using System.Xml.XPath;

namespace EpubReader.Models;

internal class NavPoint 
{
    public string Label { get; private set; } = string.Empty;
    public string ContentSrc { get; private set; } = string.Empty; 
    public int PlayOrder { get; private set; }

    public ObservableCollection<NavPoint> Children { get; } = []; 

    public static NavPoint ParseXElement(XElement element, XmlNamespaceManager ns)
    {
        var navPoint = new NavPoint();

        var playOrder = element.Attribute("playOrder")?.Value;
        if (!string.IsNullOrEmpty(playOrder))
            navPoint.PlayOrder = int.Parse(playOrder);

        var labelText = element.XPathSelectElement("./ncx:navLabel/ncx:text", ns);
        if (labelText != null)
            navPoint.Label = labelText.Value;

        var content = element.XPathSelectElement("./ncx:content", ns); 
        if (content != null)
        {
            navPoint.ContentSrc = content.Attribute("src")?.Value ?? string.Empty; 
        }

        var childrenItems = element.XPathSelectElements("./ncx:navPoint", ns);
        foreach (var item in childrenItems)
        {
            if (item != null)
            {
                var subNavPoint = ParseXElement(item, ns); 
                navPoint.Children.Add(subNavPoint); 
            }
        }

        return navPoint; 
    }
}
