using System;
using EpubReader.Helper;
using Microsoft.UI.Xaml;
using Microsoft.Windows.Storage.Pickers;


namespace EpubReader;

/// <summary>
/// An empty window that can be used on its own or navigated to within a Frame.
/// </summary>
public sealed partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
    }

    private async void OpenFile_ButtonClick(object sender, RoutedEventArgs e)
    {
        var picker = new FileOpenPicker(AppWindow.Id);
        picker.FileTypeFilter.Add(".epub");
        picker.SuggestedStartLocation = PickerLocationId.Downloads;

        var result = await picker.PickSingleFileAsync(); 
        if (result != null)
        {
            using var zip = new ZipHelper(result.Path);
            ContainerXml.Text = await zip.ExtractFileTextAsync("META-INF/container.xml"); ;

            CoverImage.Source = await zip.ExtractImageSourceAsync("OEBPS/Images/cover00374.jpeg"); 
        }
       
    }
}
