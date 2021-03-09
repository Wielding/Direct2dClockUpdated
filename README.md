# Direct2dClock Enhanced

This is my updated version of the [Direct2dClock](https://github.com/microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/begin/LearnWin32/Direct2DClock) sample found in the [Windows classic samples](https://github.com/microsoft/Windows-classic-samples) github repository.

I have separated out and renamed some of the classes. I have also added a resource file with a menu, accelerators and about box that gets version information from the internal resources.

I have changed the application to use WM_TIMER events for drawing the clock so that it uses a traditional message loop.

The code has also been cleaned up a little to match my coding preferences as well as ReSharper's.

I use this as a starting point for my Direct2D Win32 applications since it nicely contains the main window logic in a class and prevents the need for using global variables to store state information.