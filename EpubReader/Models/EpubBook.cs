using System.Xml;
using System.Xml.XPath;
using EpubReader.Helper;

namespace EpubReader.Models;

internal partial class EpubBook(string filePath) : BindableBase
{
    private readonly ZipHelper _zipHelper = new(filePath);
    

    


}
