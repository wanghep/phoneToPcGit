   M S B A M L   `   `   `  ����E  AScreenCast, Version=2.0.2.8, Culture=neutral, PublicKeyToken=null9!clr-namespace:ScreenCast.ControlsScreenCast.Controls  5   	   #    ScreenCast.Views.SplashWindow   5   	   R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   6controls!clr-namespace:ScreenCast.Controls     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     6F     � Title$  SplashWindow��5   	   $	��416��6   $	��696��6+    � AllowsTransparency .  � WindowStyle$
 None=�5	   	    � WindowStartupLocation$ CenterScreen=�6    � ShowInTaskbar .  ��5   	   Z�  � Icon 5   #   �� *	AppIcon+.��5      �� -   5        ._�5   
   � .+�5      �� -   5        $��
PCConnetfl��5      .+�5      �� -   5        $��	flushImg2��5   4   $��/Images/splash.png��6   $��Uniform=�6F   $	��696��6X   $	��416��6d   r� 5      -   5         r�Name$ element��5       r� LoadedBehavior$
 Play=�60    r� Stretch$
 Fill=�6F   	 r� Source$	 ydy.gif?�5      
 J� Opacity$	
 1.0Q�5   *   $	��447��68   $	��171��6D   $��130,96,110,0q�6Q   �� 5      $��82,35,58,219q�6      AppFullName#���   62   $��16(�6W   
�������$
��Bold"�6w   �� 5       �� Left$ 0��6   
 �� Top$	 282��6)   $	��696��6:   $��20��6F   ._�5      �� $��14(�6    
�������  Instructment#���  6@   $��Center=�6f   5       %   ScreenCast.Controls.ImageButton  5"      
�������  BlackImageButton#��[ 6:     NowShow#��[ 6�   $��2��6�   $��Right=�6�   $	 494��6�   $	 338��6�   ��6f   �� *ExperienceCommand+6�   �� 5#      $��10(�6   
�������$��2��6a   $ 35��6n   $	 353��6    d� 	IsChecked 6=   �� *ShowFirstPage+6   �� 5$      $��10(�6   
������� 
 NoShow#���  6<   $
��Left=�6\   $��2��6w   $	��200��6�   $ 54��6�   $	 355��6�   �� 5%      $��/Images/titleLine.png��6   $	��593��67   $��2��6C   $ 81��6N   $ 62��6_   �� 5&      $��/Images/appicon.png��6   $��47��65   $��47��6@   $ 21��6L   $ 26��6]   5'      5(      5)      5*      