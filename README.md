# HX711-Load-Cell-x4

This code is written in collaboration with Junson Park, Jake Morgan, and B Lee
------------------------------------------------------------------------------

Using an arduino uno you can send out the serial data of 4 individual load cells seperately using this code.
Typically, code and circuits containing the Hx711 Load Cell use 2 or 4 cells together and summed. 
Our goal was to be able to read each value independently from one another so that we could use positional weight to manipulate midi values in ableton.

you DO need to have the Hx711 library file included in this repo.
you DO NOT need to use the SURF.maxpat file to interpret the serial data. It is just what we used at the time of our project. It measures the flux between the load cells.
you can use whatever you desire to interpret serial data.

-
Notable issues and whatnot:
1. At the time of publishing this code we only have 3 load cells classes declared at once. This is because one of our loadcells broke in the process of creating this project
2. Goodluck getting a stable value without precise soldering.(the load cells are extremely unstable)
3. When you encounter the inevitable "timeout" message, the problem is 99% always on electronics side.
4. The serial print lines are a little messy but that's because of an error in the class we could not debug.
