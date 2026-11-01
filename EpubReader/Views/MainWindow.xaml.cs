using Microsoft.UI.Xaml;


namespace EpubReader.Views;

/// <summary>
/// An empty window that can be used on its own or navigated to within a Frame.
/// </summary>
public sealed partial class MainWindow : Window
{
    public MainWindow()
    {
        this.ExtendsContentIntoTitleBar = true; 
        InitializeComponent();
        this.SetTitleBar(MainStripFooter); 
    }
}
