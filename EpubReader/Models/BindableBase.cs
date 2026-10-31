using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace EpubReader.Models;

internal abstract class BindableBase : INotifyPropertyChanged
{
    public event PropertyChangedEventHandler? PropertyChanged;

    protected void SetProperty<T>(ref T oldValue, T newValue, [CallerMemberName] string? propertyName = null)
    {
        if (oldValue?.Equals(newValue) != true )
        {
            oldValue = newValue;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    protected void SetProperty2<T>(ref T oldValue, T newValue, [CallerMemberName] string? propertyName = null)
        where T : IEquatable<T>
    {
        if (oldValue.Equals(newValue) != true)
        {
            oldValue = newValue;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }



}
