# Assignment 2 - Group 13

## COMPSYS 303 - Microcomputers and Embedded Systems

*September 25, 2017 - Sakayan Sitsabesan & Sylvain Bechet*

## IMPORTANT NOTE:

Make sure there are NO space characters for all files used. There are known software issues caused by these spaces, so please extract this file in place with no such thing.

## SETUP INSTRUCTIONS:
1. After extracting the contents of this file, open Quartus II 13.0
2. Open the Programmer window and click on 'Add File'
3. Select 'cs303.sof' and make sure the Altera DE2-115 is connected to the computer
4. Press the 'Start' button making sure 'Program/Configure' is ticked
5. Open Nios II 13.0 and select an appropriate workspace
6. Add the two folders to this workspace ('DDPacemaker' and 'DDPacemaker_bsp')
7. Select the 'Build All' option (Ctrl + B)
8. After successfully building the project, right click 'DDPacemaker'
9. Under 'Run as' select '3 Nios II Hardware'

## USAGE INSTRUCTIONS:
* The Buttons on the DE2-115 can be used to simulate the inputs VSense and ASense
    * Key 0 is VSense
    * Key 1 is ASense
    * NOTE: These signals are debounced and each press is only processed as a single pulse.
* The LEDs on the DE2-115 are used to show the outputs VPace and APace
    * LEDG0 is VPace
    * LEDG1 is APace
    * NOTE: The LEDs have been buffered and illuminate for 200ms after the pulse occured. This is to make sure the pulses are visible to the human eye.

## UART CONFIGURATION:
1. Run CS303-Heart.exe and select the 'Session' tab
2. Select the appropriate port in the 'COM Port' setting
3. Set the 'Baud Rate' setting to 115200
4. Click the 'Update' button to establish connection between the pacemaker and virtual heart

## QUESTION & QUERIES:

For any questions or queries please contact Sakayan Sitsabesan and/or Sylvain Bechet.
The presentation has been included for clarity of understanding.