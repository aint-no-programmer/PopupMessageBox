### PopupMessageBox
Simple cross-platform Qt popup messagebox based on Evileg PopUp notification, link: https://evileg.com/en/post/146/  

What has been developed:
- added multiple animated messages with expire timer
- added a separate thread event loop for messages

### How does it look
![Demostration video](./screenshots/example.gif?raw=true)  
The example application demonstrates popup messages

### How to use it
Simple use:
```
#include "PopMsgBox.h"

/*
 * some code
 */
 
/*
 * place where need to show message
 */
pmb::PopMsgBox::instance().enqueueMessage("some title", "some message", QColor(0, 0, 0, 180));
```
### Build
Qt5 needs to be installed

Windows
```
>mkdir build
>cd build
>cmake ..
>cmake --build .
```

Linux
```
>mkdir build
>cd build
>cmake ..
>make
```
### Platforms supported
You can use it on Windows and Linux
