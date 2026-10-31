using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Media.Imaging;
using System;
using System.IO;
using System.IO.Compression;
using System.Threading.Tasks;

namespace EpubReader.Helper;

/// <summary>
/// 该类型用于提取 zip 压缩包内部的文件内容。
/// 使用后，需要清理资源。 
/// </summary>
internal partial class ZipHelper: IDisposable
{
    private readonly ZipArchive? _zip;
    private readonly FileStream? _fileStream; 

    public ZipHelper(string filePath)
    {
        _fileStream = File.OpenRead(filePath);
        _zip = new ZipArchive(_fileStream, ZipArchiveMode.Read); 
    }
    
    public void Dispose()
    {
        _zip?.Dispose();
        _fileStream?.Dispose(); 
    }

    /// <summary>
    /// 从 zip 内部获取指定 UTF-8 编码的文本文件的内容。
    /// </summary>
    /// <param name="entryName">文本文件的名称，该文件的编码格式应该是 UTF-8。</param>
    /// <returns>文件的内容，如果失败返回空字符串。</returns>
    public string ExtractFileText(string entryName)
    {
        var entry = _zip?.GetEntry(entryName); 
        if (entry != null)
        {
            using var stream = entry.Open();
            using var reader = new StreamReader(stream, System.Text.Encoding.UTF8);
            return reader.ReadToEnd(); 
        }
        else
        {
            return string.Empty; 
        }
    }

    /// <summary>
    /// 异步从 zip 内部获取指定 UTF-8 编码的文本文件的内容。
    /// </summary>
    /// <param name="entryName">文本文件的名称，该文件的编码格式应该是 UTF-8。</param>
    /// <returns>文件的内容。如果失败，返回空字符串。</returns>
    public async Task<string> ExtractFileTextAsync(string entryName)
    {
        var entry = _zip?.GetEntry(entryName); 
        if (entry != null)
        {
            using var stream = entry.Open();
            using var reader = new StreamReader(stream, System.Text.Encoding.UTF8);
            string text = await reader.ReadToEndAsync();
            return text; 
        }
        else
        {
            return string.Empty; 
        }
    }

    /// <summary>
    /// 异步获取 zip 文件内部的图片数据，并转换成 ImageSource 对象。
    /// </summary>
    /// <param name="entryName">图片文件的名称。</param>
    /// <returns>如果成功，返回一个有效的 ImageSource 对象。如果失败返回 null.</returns>
    public async Task<ImageSource?> ExtractImageSourceAsync(string entryName)
    {
        var entry = _zip?.GetEntry(entryName);

        if (entry == null) return null;

        using var zipStream = entry.Open();
        using var memoryStream = new MemoryStream();
        await zipStream.CopyToAsync(memoryStream);
        await memoryStream.FlushAsync(); 
        memoryStream.Position = 0;

        var bitmap = new BitmapImage(); 

        await bitmap.SetSourceAsync(memoryStream.AsRandomAccessStream());
        return bitmap; 
    }
}
