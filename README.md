If this code runs at 200mhz CPU and we need 25mhz clock timer, divide 200mhz with 8 and we get 25mhz.

200mhz ÷ 8 = 25mhz


float div = clock_get_hz(clk_sys) / (freq * cycles_per_bit); sm_config_clkdiv(&c, div);


      Help from https://scanlines.xyz/t/tutorials-for-generating-video-sync-signals-with-arduino/104
      & 
     http://tinyvga.com/vga-timing
     https://martin.hinner.info/vga/timing.html

VGA Signal 640 x 480 @ 60 Hz Industry standard timing


Screen refresh rate	60 Hz
Vertical refresh	31.46875 kHz
Pixel freq.	25.175 MHz


Horizontal timing (line)

Polarity of horizontal sync pulse is negative.
Scanline part	Pixels	Time [µs]
Visible area	640	25.422045680238
Front porch	16	0.63555114200596
Sync pulse	96	3.8133068520357
Back porch	48	1.9066534260179
Whole line	800	31.777557100298

Vertical timing (frame)

Polarity of vertical sync pulse is negative.
Frame part	Lines	Time [ms]
Visible area	480	15.253227408143
Front porch	10	0.31777557100298
Sync pulse	2	0.063555114200596
Back porch	33	1.0486593843098
Whole frame	525	16.683217477656