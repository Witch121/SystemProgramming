using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace Lab5
{
    public sealed partial class ConverterPage : Page
    {
        public ConverterPage()
        {
            this.InitializeComponent();
        }
        // Length
        private void ConvertLengthButton_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            double meters;
            if (double.TryParse(LengthInput.Text, out meters))
            {
                LengthOutput.Text = $"{meters * 3.28084} feet, {meters * 39.3701} inches, " +
                                    $"{meters * 1.09361} yards, {meters * 0.000621371} miles";
            }
            else
            {
                LengthOutput.Text = "Invalid input!";
            }
        }

        // Weight
        private void ConvertWeightButton_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            double kilograms;
            if (double.TryParse(WeightInput.Text, out kilograms))
            {
                WeightOutput.Text = $"{kilograms * 2.20462} pounds, {kilograms * 35.274} ounces";
            }
            else
            {
                WeightOutput.Text = "Invalid input!";
            }
        }

        // Volume
        private void ConvertVolumeButton_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            double liters;
            if (double.TryParse(VolumeInput.Text, out liters))
            {
                VolumeOutput.Text = $"{liters * 2.11338} US pints, {liters * 0.264172} US gallons, " +
                                    $" {liters * 1.75975} imperial pints, {liters * 0.219969} imperial gallons";
            }
            else
            {
                VolumeOutput.Text = "Invalid input!";
            }
        }

        // Navigate to Developer Info Page
        private void DeveloperInfoButton_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            Frame.Navigate(typeof(DeveloperInfoPage));
        }

        // Navigate to Main Page 
        private void MainPageButton_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            Frame.Navigate (typeof(MainPage));  
        }
    }
}
