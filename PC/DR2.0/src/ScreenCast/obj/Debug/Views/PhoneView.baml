   M S B A M L   `   `   `  ����E  AScreenCast, Version=2.0.2.8, Culture=neutral, PublicKeyToken=null9!clr-namespace:ScreenCast.ControlsScreenCast.Controls  5   	   3clr-namespace:ScreenCast.ViewsScreenCast.Views  5   	        ScreenCast.Views.PhoneView   5   	   R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   6controls!clr-namespace:ScreenCast.Controls     5   	   0viewsclr-namespace:ScreenCast.Views     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   -   5          � Title$  
MainWindow��5   	   $	��396��6   $	��656��6)    � WindowStyle$
 None=�65    � WindowStartupLocation$ CenterScreen=�6H    � 
ResizeMode$ NoResize=�6m    � Icon 5
   k   �� *	AppIcon+.��5      �� -   5        ._�5   
   � .+�5      �  � RowDefinitions 5      �� 5      $
��Auto �6$   �� 5      $��* �6$   �� 5      $
��Auto �6$    � ColumnDefinitions 5      �� 5      $
��Auto �6'   �� 5      $��* �6'   �� 5      $
��Auto �6'   .+�5%      �� $��/Images/phone_lefttop.png��6   $��0��6;   $��0��6I   $
��Fill=�6Y   $��30��6i   �� 5&      $��/Images/phone_top.png��6   $��0��67   $��1��6E   $
��Fill=�6U   $��30��6e   �� 5'      $ ��/Images/phone_righttop.png��6   $��0��6<   $��2��6J   $
��Fill=�6Z   $��30��6j   �� 5*      $��/Images/phone_left.png��6   $��1��68   $��0��6F   $
��Fill=�6V   $��30��6f   �� 5+      $��/Images/phone_right.png��6   $��1��69   $��2��6G   $
��Fill=�6W   $��30��6g   �� 5.      $"��/Images/phone_leftbottom.png��6   $��2��6>   $��0��6L   $
��Fill=�6\   $��30��6l   �� 5/      $��/Images/phone_top.png��6   $��2��67   $��1��6E   $
��Fill=�6U   $��30��6e   �� 50      $#��/Images/phone_rightbottom.png��6   $��2��6?   $��2��6M   $
��Fill=�6]   $��30��6m   53      &   ScreenCast.Views.VideoPlayerHost  54      -   5          Name$ Player��54   7   
���   �$��30q�6G   �� 56      -   5         ��Name$ 
resizeGrip��56      $y�Visible=�6-   	 � Cursor$	 SizeNWSE��6B   $��Right=�6T   $��Bottom=�57      ��.  $��0,0,5,5q�58   [   5B      5C      5F      