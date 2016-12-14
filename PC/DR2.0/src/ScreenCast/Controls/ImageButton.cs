using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace ScreenCast.Controls
{
    public class ImageButton : Button
    {
        public ImageSource NormalImage
        {
            get { return (ImageSource)GetValue(NormalImageProperty); }
            set { SetValue(NormalImageProperty, value); }
        }

        public static readonly DependencyProperty NormalImageProperty =
            DependencyProperty.Register("NormalImage", typeof(ImageSource), typeof(ImageButton), new PropertyMetadata(null));

        public ImageSource HoverImage
        {
            get { return (ImageSource)GetValue(HoverImageProperty); }
            set { SetValue(HoverImageProperty, value); }
        }

        public static readonly DependencyProperty HoverImageProperty =
            DependencyProperty.Register("HoverImage", typeof(ImageSource), typeof(ImageButton), new PropertyMetadata(null));

        public ImageSource PushImage
        {
            get { return (ImageSource)GetValue(PushImageProperty); }
            set { SetValue(PushImageProperty, value); }
        }

        public static readonly DependencyProperty PushImageProperty =
            DependencyProperty.Register("PushImage", typeof(ImageSource), typeof(ImageButton), new PropertyMetadata(null));
        
        public ImageSource DisableImage
        {
            get { return (ImageSource)GetValue(DisableImageProperty); }
            set { SetValue(DisableImageProperty, value); }
        }

        public static readonly DependencyProperty DisableImageProperty =
            DependencyProperty.Register("DisableImage", typeof(ImageSource), typeof(ImageButton), new PropertyMetadata(null));



        public string ToolTipText
        {
            get { return (string)GetValue(ToolTipTextProperty); }
            set { SetValue(ToolTipTextProperty, value); }
        }

        public static readonly DependencyProperty ToolTipTextProperty =
            DependencyProperty.Register("ToolTipText", typeof(string), typeof(ImageButton), new PropertyMetadata(null, OnToolTipTextChanged));
        private static void OnToolTipTextChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var s = d as ImageButton;
            if (!string.IsNullOrEmpty(s.ToolTipText))
                ToolTipService.SetToolTip(s, s.ToolTipText);
        }

        static ImageButton()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(ImageButton), new FrameworkPropertyMetadata(typeof(ImageButton)));
        }
    }
}