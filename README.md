# NixieClock
For nixie tube control (Made in 2017/Mar)

![image](https://github.com/QuntinMKII/NixieClock/assets/50934318/80d9c474-d890-474b-a242-8e7597a50517)


## Electrical design concepts
![概念圖](https://github.com/QuntinMKII/NixieClock/assets/50934318/ae31dc7e-94f8-4a3c-97a9-9b87c3f5fd19)

- Arduino : controlling the display and switch of the Nixie Tube. With an RTC module to obtain the time.
- Nixie Tube : Useing Soviet IN14 [datasheet](https://tubehobby.com/datasheets/in14.pdf)
- 74HC595_digit : Due to the unavailability of the dedicated control IC K155ID1 for Nixie Tubes, the 74HC595 shift register is used along with several digital outputs of Arduino to control the display digits of the tubes. Also need an logic inverter if using 74HC595, which can achieved by a simple BJT inverter.
- 74HC595_tube :  Used to control individual tube illumination for achieving scanning display. It is used in conjunction with TLP627 to control the anode of the Nixie Tube, achieving individual switching effects.
- 12V-180V DC/DC converter : Originally, a circuit using NE555 was used, but later it was replaced with a circuit using MC34063.

## About program

- Includes four modes that can be switched using buttons:
1. World Line Divergence Meter mode : Ref from Steins Gate, randomly display a series of numbers.
2. Time mode
3. Date mode
4. Test mode : Loop display each digit on all tubes to check for any malfunctions.

- nixie_ctrl(int nixie_num, int digit_num, int dot)
  - Used to control the scanning behavior of the tubes. It specifies the {digit_num} to be displayed on the {nixie_num} tube and whether to display the dot.
  - Current switch ratio is set to 2ms on and 1ms off, but it need to be adjusted according to the hardware conditions.
- loop
  - Retrieves the numbers that each tube should display (calculated based on the selected mode).
  - Displays the corresponding numbers on tubes 1-8 in sequence (only one tube is lit at a time).


ref : https://atifans.net/nixie-tube-clock/
