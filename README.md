# Woke - Utility for preventing a Windows Computer from Sleeping

I use this application to prevent my computer from going to sleep. Though 
you could accomplish something similar by changing the power settings on 
your computer, I prefer to use this utility since its effects do not persist
past termination of the program. If the program is run with no arguments, it 
will keep the computer awake indefinitely. Pressing `CTRL-C` while the 
application has focus will terminate the program and cancel its effects. 

![Alarm Clock Icon](./gfx/alarmclock.png, "Woke")

# Arguments (optional)

* `-w <seconds>` - Sets the number of seconds that the computer should be forced to stay awake. 
* `-i` explicitly sets the program to keep the computer awake indefinitely
* `'-v' enables verbose mode
* `-s` enables silent mode (no console output)
* `-a` shows the help screen

# About

This program was created by J2i.Net, LLC (2026). 