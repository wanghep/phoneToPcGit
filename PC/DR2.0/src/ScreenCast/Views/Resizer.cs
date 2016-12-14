using System.Windows;

namespace ScreenCast.Views
{
    public enum ResizeDirection
    {
        /// <summary>
        ///     Size is increased by dragging up and to the right, and decreased by dragging down and to the left.
        /// </summary>
        NorthEast,

        /// <summary>
        ///     Size is increased by dragging up and to the left, and decreased by dragging down and to the right.
        /// </summary>
        NorthWest,

        /// <summary>
        ///     Size is increased by dragging down and to the right, and decreased by dragging up and to the left.
        /// </summary>
        SouthEast,

        /// <summary>
        ///     Size is increased by dragging down and to the left, and decreased by dragging up and to the right.
        /// </summary>
        SouthWest
    }

    public class Resizer
    {
        public Point ResizeOrigin { get; set; }
        public Point ResizeTo { get; set; }
        public double ResizeToWidth { get; set; }
        public double ResizeToHeight { get; set; }
        public double OriginalWidth { get; set; }
        public double OriginalHeight { get; set; }
        public ResizeDirection ResizeDirection { get; set; }
    }
}