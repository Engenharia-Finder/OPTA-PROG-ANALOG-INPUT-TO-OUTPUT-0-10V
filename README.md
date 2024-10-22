# OPTA-PROG-ANALOG-INPUT-TO-OUTPUT-0-10V
This program aims to read an analog signal from 0 to 10V at an Arduino OPTA input and then reproduce that same value at an analog output. The program continuously reads the value present at the analog input, converts it according to the range required by the DAC (Digital-to-Analog Converter), and adjusts the analog output to reflect the same voltage value read at the input.

This type of application is useful in systems where it is necessary to monitor and replicate analog signals, such as in industrial controllers, where a sensor may provide a variable output voltage that needs to be transmitted to another device or control system.

The programming flow is simple:

* Read the voltage value at the analog input.

* Convert the value to the appropriate range.

* Apply the converted value to the analog output, ensuring that the same input signal is reflected at the output.

With this, the Arduino OPTA acts as a signal amplifier or replicator, maintaining the integrity of the analog signal within the specified range.
