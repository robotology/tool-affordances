% initialize YARP
LoadYarp;
import yarp.BufferedPortImageRgb
import yarp.BufferedPortBottle
import yarp.Port
import yarp.Bottle
import yarp.Time
import yarp.ImageRgb
import yarp.Image
import yarp.PixelRgb
done=0;


portIn=BufferedPortBottle;        %port for reading "quit" signal
portOut=Port;               %port for sending image

%first close the port just in case
%(this is to try to prevent matlab from being unresponsive)
portIn.close;
portOut.close;

disp('opening ports...');
portIn.open('/matlab/read');
disp('opened port /matlab/read');
pause(0.5);
portOut.open('/matlab/data:o');
disp('opened port /matlab/data:o');
pause(0.5);
disp('done.');