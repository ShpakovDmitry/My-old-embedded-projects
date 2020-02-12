EESchema Schematic File Version 4
LIBS:radio_morse_panel-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Morse panel"
Date "2020-02-11"
Rev "r1"
Comp ""
Comment1 ""
Comment2 "ShpakovDmitry"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATtiny:ATtiny2313-20PU U2
U 1 1 5E42D6AB
P 4450 6000
F 0 "U2" H 4450 7278 50  0000 C CNN
F 1 "ATtiny2313-20PU" H 4450 7187 50  0000 C CNN
F 2 "Package_DIP:DIP-20_W7.62mm" H 4450 6000 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2543-AVR-ATtiny2313_Datasheet.pdf" H 4450 6000 50  0001 C CNN
	1    4450 6000
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:uA7805 U1
U 1 1 5E42D8D6
P 2100 2600
F 0 "U1" H 2100 2842 50  0000 C CNN
F 1 "uA7805" H 2100 2751 50  0000 C CNN
F 2 "" H 2125 2450 50  0001 L CIN
F 3 "http://www.ti.com/lit/ds/symlink/ua78.pdf" H 2100 2550 50  0001 C CNN
	1    2100 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5E42DA15
P 2100 3150
F 0 "#PWR05" H 2100 2900 50  0001 C CNN
F 1 "GND" H 2105 2977 50  0000 C CNN
F 2 "" H 2100 3150 50  0001 C CNN
F 3 "" H 2100 3150 50  0001 C CNN
	1    2100 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C1
U 1 1 5E42DAED
P 1650 2900
F 0 "C1" H 1768 2946 50  0000 L CNN
F 1 "0.33uF" H 1768 2855 50  0000 L CNN
F 2 "" H 1688 2750 50  0001 C CNN
F 3 "~" H 1650 2900 50  0001 C CNN
	1    1650 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C3
U 1 1 5E42DB42
P 2550 2900
F 0 "C3" H 2668 2946 50  0000 L CNN
F 1 "0.1uF" H 2668 2855 50  0000 L CNN
F 2 "" H 2588 2750 50  0001 C CNN
F 3 "~" H 2550 2900 50  0001 C CNN
	1    2550 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 2900 2100 3050
Wire Wire Line
	2550 3050 2100 3050
Connection ~ 2100 3050
Wire Wire Line
	2100 3050 2100 3150
Wire Wire Line
	1650 3050 2100 3050
Wire Wire Line
	2400 2600 2550 2600
Wire Wire Line
	2550 2750 2550 2600
Connection ~ 2550 2600
Wire Wire Line
	1800 2600 1650 2600
Wire Wire Line
	1650 2750 1650 2600
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 5E42E16D
P 900 1200
F 0 "J1" H 820 875 50  0000 C CNN
F 1 "Conn_01x02" H 820 966 50  0000 C CNN
F 2 "" H 900 1200 50  0001 C CNN
F 3 "~" H 900 1200 50  0001 C CNN
	1    900  1200
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5E42E229
P 1100 1650
F 0 "#PWR03" H 1100 1400 50  0001 C CNN
F 1 "GND" H 1105 1477 50  0000 C CNN
F 2 "" H 1100 1650 50  0001 C CNN
F 3 "" H 1100 1650 50  0001 C CNN
	1    1100 1650
	1    0    0    -1  
$EndComp
Text Notes 750  750  0    50   ~ 0
DC input power connector\n      range 7-25 V
$Comp
L power:+5V #PWR07
U 1 1 5E42E9C6
P 3050 2300
F 0 "#PWR07" H 3050 2150 50  0001 C CNN
F 1 "+5V" H 3065 2473 50  0000 C CNN
F 2 "" H 3050 2300 50  0001 C CNN
F 3 "" H 3050 2300 50  0001 C CNN
	1    3050 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 2300 3050 2600
Wire Wire Line
	2550 2600 3050 2600
$Comp
L Device:Fuse F1
U 1 1 5E42F06D
P 2950 1100
F 0 "F1" V 2753 1100 50  0000 C CNN
F 1 "Fuse" V 2844 1100 50  0000 C CNN
F 2 "" V 2880 1100 50  0001 C CNN
F 3 "~" H 2950 1100 50  0001 C CNN
	1    2950 1100
	0    1    1    0   
$EndComp
$Comp
L Device:D D1
U 1 1 5E42F4D9
P 3200 1300
F 0 "D1" V 3154 1379 50  0000 L CNN
F 1 "D" V 3245 1379 50  0000 L CNN
F 2 "" H 3200 1300 50  0001 C CNN
F 3 "~" H 3200 1300 50  0001 C CNN
	1    3200 1300
	0    1    1    0   
$EndComp
Wire Wire Line
	3100 1100 3200 1100
Wire Wire Line
	3200 1100 3200 1150
Wire Wire Line
	1100 1200 1100 1650
Wire Wire Line
	1100 1100 1650 1100
Text Label 1150 1100 0    50   ~ 0
DC_input
$Comp
L power:GND #PWR08
U 1 1 5E43127C
P 3200 1550
F 0 "#PWR08" H 3200 1300 50  0001 C CNN
F 1 "GND" H 3205 1377 50  0000 C CNN
F 2 "" H 3200 1550 50  0001 C CNN
F 3 "" H 3200 1550 50  0001 C CNN
	1    3200 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 1450 3200 1550
Wire Wire Line
	3200 1100 3700 1100
Connection ~ 3200 1100
Wire Wire Line
	2800 1100 2300 1100
Text Label 2400 1100 0    50   ~ 0
DC_input
Text Label 3300 1100 0    50   ~ 0
circuit_protection
Text Notes 2600 750  0    50   ~ 0
Simple circuit protection from\npower reversal
Wire Wire Line
	1650 2600 700  2600
Connection ~ 1650 2600
Text Label 850  2600 0    50   ~ 0
circuit_protection
Text Notes 1500 2250 0    50   ~ 0
Voltage regulator Vout = +5V\n                  Imax = 1.5A
$Comp
L Device:LED D2
U 1 1 5E433D5B
P 5200 1600
F 0 "D2" V 5238 1482 50  0000 R CNN
F 1 "RED" V 5147 1482 50  0000 R CNN
F 2 "" H 5200 1600 50  0001 C CNN
F 3 "~" H 5200 1600 50  0001 C CNN
	1    5200 1600
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR09
U 1 1 5E4342F2
P 4450 4250
F 0 "#PWR09" H 4450 4100 50  0001 C CNN
F 1 "+5V" H 4465 4423 50  0000 C CNN
F 2 "" H 4450 4250 50  0001 C CNN
F 3 "" H 4450 4250 50  0001 C CNN
	1    4450 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5E434572
P 4450 7350
F 0 "#PWR010" H 4450 7100 50  0001 C CNN
F 1 "GND" H 4455 7177 50  0000 C CNN
F 2 "" H 4450 7350 50  0001 C CNN
F 3 "" H 4450 7350 50  0001 C CNN
	1    4450 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 7100 4450 7350
$Comp
L Device:Crystal Y1
U 1 1 5E4348DD
P 3350 5500
F 0 "Y1" V 3304 5631 50  0000 L CNN
F 1 "16MHz" V 3395 5631 50  0000 L CNN
F 2 "" H 3350 5500 50  0001 C CNN
F 3 "~" H 3350 5500 50  0001 C CNN
	1    3350 5500
	0    1    1    0   
$EndComp
$Comp
L Device:C C4
U 1 1 5E43502F
P 2900 5250
F 0 "C4" V 2648 5250 50  0000 C CNN
F 1 "15pF" V 2739 5250 50  0000 C CNN
F 2 "" H 2938 5100 50  0001 C CNN
F 3 "~" H 2900 5250 50  0001 C CNN
	1    2900 5250
	0    1    1    0   
$EndComp
$Comp
L Device:C C5
U 1 1 5E4350A2
P 2900 5750
F 0 "C5" V 2648 5750 50  0000 C CNN
F 1 "15pF" V 2739 5750 50  0000 C CNN
F 2 "" H 2938 5600 50  0001 C CNN
F 3 "~" H 2900 5750 50  0001 C CNN
	1    2900 5750
	0    1    1    0   
$EndComp
Wire Wire Line
	3850 5400 3700 5400
Wire Wire Line
	3700 5400 3700 5250
Wire Wire Line
	3700 5250 3350 5250
Wire Wire Line
	3350 5250 3350 5350
Wire Wire Line
	3850 5600 3700 5600
Wire Wire Line
	3700 5600 3700 5750
Wire Wire Line
	3700 5750 3350 5750
Wire Wire Line
	3350 5750 3350 5650
Wire Wire Line
	3050 5250 3350 5250
Connection ~ 3350 5250
Wire Wire Line
	3050 5750 3350 5750
Connection ~ 3350 5750
Wire Wire Line
	2750 5250 2600 5250
Wire Wire Line
	2600 5250 2600 5750
Wire Wire Line
	2600 5750 2750 5750
$Comp
L power:GND #PWR06
U 1 1 5E436191
P 2600 6050
F 0 "#PWR06" H 2600 5800 50  0001 C CNN
F 1 "GND" H 2605 5877 50  0000 C CNN
F 2 "" H 2600 6050 50  0001 C CNN
F 3 "" H 2600 6050 50  0001 C CNN
	1    2600 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 6050 2600 5750
Connection ~ 2600 5750
Text Notes 2800 6050 0    50   ~ 0
Keep those components\nas close as possible to uC\n
Wire Wire Line
	4450 4250 4450 4450
$Comp
L Switch:SW_Push SW1
U 1 1 5E438B4A
P 950 4600
F 0 "SW1" V 996 4552 50  0000 R CNN
F 1 "SW_Push" V 905 4552 50  0000 R CNN
F 2 "" H 950 4800 50  0001 C CNN
F 3 "" H 950 4800 50  0001 C CNN
	1    950  4600
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5E439418
P 950 5000
F 0 "#PWR02" H 950 4750 50  0001 C CNN
F 1 "GND" H 955 4827 50  0000 C CNN
F 2 "" H 950 5000 50  0001 C CNN
F 3 "" H 950 5000 50  0001 C CNN
	1    950  5000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5E43A26E
P 950 4050
F 0 "R1" H 1020 4096 50  0000 L CNN
F 1 "56k" H 1020 4005 50  0000 L CNN
F 2 "" V 880 4050 50  0001 C CNN
F 3 "~" H 950 4050 50  0001 C CNN
	1    950  4050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5E43A9A2
P 1350 4300
F 0 "R2" V 1143 4300 50  0000 C CNN
F 1 "5.6k" V 1234 4300 50  0000 C CNN
F 2 "" V 1280 4300 50  0001 C CNN
F 3 "~" H 1350 4300 50  0001 C CNN
	1    1350 4300
	0    1    1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 5E43AAE3
P 1650 4600
F 0 "C2" H 1765 4646 50  0000 L CNN
F 1 "0.1uF" H 1765 4555 50  0000 L CNN
F 2 "" H 1688 4450 50  0001 C CNN
F 3 "~" H 1650 4600 50  0001 C CNN
	1    1650 4600
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 5E43C33E
P 950 3700
F 0 "#PWR01" H 950 3550 50  0001 C CNN
F 1 "+5V" H 965 3873 50  0000 C CNN
F 2 "" H 950 3700 50  0001 C CNN
F 3 "" H 950 3700 50  0001 C CNN
	1    950  3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  3700 950  3900
Wire Wire Line
	950  4200 950  4300
Wire Wire Line
	1200 4300 950  4300
Connection ~ 950  4300
Wire Wire Line
	950  4300 950  4400
Wire Wire Line
	950  4800 950  5000
$Comp
L power:GND #PWR04
U 1 1 5E43DA98
P 1650 5000
F 0 "#PWR04" H 1650 4750 50  0001 C CNN
F 1 "GND" H 1655 4827 50  0000 C CNN
F 2 "" H 1650 5000 50  0001 C CNN
F 3 "" H 1650 5000 50  0001 C CNN
	1    1650 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 4750 1650 5000
Wire Wire Line
	1500 4300 1650 4300
Wire Wire Line
	1650 4450 1650 4300
Connection ~ 1650 4300
Wire Wire Line
	1650 4300 2050 4300
Text Label 1800 4300 0    50   ~ 0
RESET
Wire Wire Line
	3850 5200 3850 5000
Wire Wire Line
	3850 5000 3200 5000
Text Label 3350 5000 0    50   ~ 0
RESET
Text Notes 1250 3800 0    50   ~ 0
HW reset circuit
$Comp
L Transistor_BJT:PN2222A Q1
U 1 1 5E441B5A
P 5100 2050
F 0 "Q1" H 5291 2096 50  0000 L CNN
F 1 "PN2222A" H 5291 2005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 5300 1975 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/PN/PN2222A.pdf" H 5100 2050 50  0001 L CNN
	1    5100 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5E442E57
P 5200 1200
F 0 "R4" H 5130 1154 50  0000 R CNN
F 1 "330R" H 5130 1245 50  0000 R CNN
F 2 "" V 5130 1200 50  0001 C CNN
F 3 "~" H 5200 1200 50  0001 C CNN
	1    5200 1200
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR011
U 1 1 5E44469D
P 5200 950
F 0 "#PWR011" H 5200 800 50  0001 C CNN
F 1 "+5V" H 5215 1123 50  0000 C CNN
F 2 "" H 5200 950 50  0001 C CNN
F 3 "" H 5200 950 50  0001 C CNN
	1    5200 950 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5E445327
P 5200 2350
F 0 "#PWR012" H 5200 2100 50  0001 C CNN
F 1 "GND" H 5205 2177 50  0000 C CNN
F 2 "" H 5200 2350 50  0001 C CNN
F 3 "" H 5200 2350 50  0001 C CNN
	1    5200 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5E445F88
P 4650 2050
F 0 "R3" V 4857 2050 50  0000 C CNN
F 1 "1k" V 4766 2050 50  0000 C CNN
F 2 "" V 4580 2050 50  0001 C CNN
F 3 "~" H 4650 2050 50  0001 C CNN
	1    4650 2050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5200 1750 5200 1850
Wire Wire Line
	5200 1350 5200 1450
Wire Wire Line
	5200 950  5200 1050
Wire Wire Line
	4800 2050 4900 2050
Wire Wire Line
	5200 2250 5200 2350
$Comp
L Device:LED D3
U 1 1 5E44BEB7
P 7050 1600
F 0 "D3" V 7088 1482 50  0000 R CNN
F 1 "GREEN" V 6997 1482 50  0000 R CNN
F 2 "" H 7050 1600 50  0001 C CNN
F 3 "~" H 7050 1600 50  0001 C CNN
	1    7050 1600
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_BJT:PN2222A Q2
U 1 1 5E44BEBE
P 6950 2050
F 0 "Q2" H 7141 2096 50  0000 L CNN
F 1 "PN2222A" H 7141 2005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 7150 1975 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/PN/PN2222A.pdf" H 6950 2050 50  0001 L CNN
	1    6950 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5E44BEC5
P 7050 1200
F 0 "R7" H 6980 1154 50  0000 R CNN
F 1 "330R" H 6980 1245 50  0000 R CNN
F 2 "" V 6980 1200 50  0001 C CNN
F 3 "~" H 7050 1200 50  0001 C CNN
	1    7050 1200
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR014
U 1 1 5E44BECC
P 7050 950
F 0 "#PWR014" H 7050 800 50  0001 C CNN
F 1 "+5V" H 7065 1123 50  0000 C CNN
F 2 "" H 7050 950 50  0001 C CNN
F 3 "" H 7050 950 50  0001 C CNN
	1    7050 950 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5E44BED2
P 7050 2350
F 0 "#PWR015" H 7050 2100 50  0001 C CNN
F 1 "GND" H 7055 2177 50  0000 C CNN
F 2 "" H 7050 2350 50  0001 C CNN
F 3 "" H 7050 2350 50  0001 C CNN
	1    7050 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5E44BED8
P 6500 2050
F 0 "R5" V 6707 2050 50  0000 C CNN
F 1 "1k" V 6616 2050 50  0000 C CNN
F 2 "" V 6430 2050 50  0001 C CNN
F 3 "~" H 6500 2050 50  0001 C CNN
	1    6500 2050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7050 1750 7050 1850
Wire Wire Line
	7050 1350 7050 1450
Wire Wire Line
	7050 950  7050 1050
Wire Wire Line
	6650 2050 6750 2050
Wire Wire Line
	7050 2250 7050 2350
Wire Wire Line
	4500 2050 4100 2050
Wire Wire Line
	6350 2050 5950 2050
Wire Wire Line
	5050 6400 5800 6400
Wire Wire Line
	5050 6500 5800 6500
Text Label 5250 6400 0    50   ~ 0
RED_LED
Text Label 5150 6500 0    50   ~ 0
GREEN_LED
Text Label 4150 2050 0    50   ~ 0
RED_LED
Text Label 6300 2050 2    50   ~ 0
GREEN_LED
$Comp
L Device:Speaker LS1
U 1 1 5E456CB7
P 9050 1550
F 0 "LS1" H 9220 1546 50  0000 L CNN
F 1 "Speaker" H 9220 1455 50  0000 L CNN
F 2 "" H 9050 1350 50  0001 C CNN
F 3 "~" H 9040 1500 50  0001 C CNN
	1    9050 1550
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:PN2222A Q3
U 1 1 5E4576FE
P 8600 2050
F 0 "Q3" H 8791 2096 50  0000 L CNN
F 1 "PN2222A" H 8791 2005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8800 1975 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/PN/PN2222A.pdf" H 8600 2050 50  0001 L CNN
	1    8600 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 5E457704
P 8700 1200
F 0 "R10" H 8630 1154 50  0000 R CNN
F 1 "330R" H 8630 1245 50  0000 R CNN
F 2 "" V 8630 1200 50  0001 C CNN
F 3 "~" H 8700 1200 50  0001 C CNN
	1    8700 1200
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR020
U 1 1 5E45770A
P 8700 950
F 0 "#PWR020" H 8700 800 50  0001 C CNN
F 1 "+5V" H 8715 1123 50  0000 C CNN
F 2 "" H 8700 950 50  0001 C CNN
F 3 "" H 8700 950 50  0001 C CNN
	1    8700 950 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5E457710
P 8700 2350
F 0 "#PWR021" H 8700 2100 50  0001 C CNN
F 1 "GND" H 8705 2177 50  0000 C CNN
F 2 "" H 8700 2350 50  0001 C CNN
F 3 "" H 8700 2350 50  0001 C CNN
	1    8700 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5E457716
P 8150 2050
F 0 "R8" V 8357 2050 50  0000 C CNN
F 1 "1k" V 8266 2050 50  0000 C CNN
F 2 "" V 8080 2050 50  0001 C CNN
F 3 "~" H 8150 2050 50  0001 C CNN
	1    8150 2050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8700 950  8700 1050
Wire Wire Line
	8300 2050 8400 2050
Wire Wire Line
	8700 2250 8700 2350
Wire Wire Line
	8000 2050 7600 2050
Wire Wire Line
	8700 1550 8850 1550
Wire Wire Line
	8700 1350 8700 1550
Wire Wire Line
	8850 1650 8700 1650
Wire Wire Line
	8700 1650 8700 1850
Wire Wire Line
	5050 6600 5800 6600
Text Label 5250 6600 0    50   ~ 0
SPEAKER
Text Label 7950 2050 2    50   ~ 0
SPEAKER
$Comp
L Switch:SW_Push SW2
U 1 1 5E461018
P 7200 3700
F 0 "SW2" V 7246 3848 50  0000 L CNN
F 1 "BUTTON_1" V 7155 3848 50  0000 L CNN
F 2 "" H 7200 3900 50  0001 C CNN
F 3 "" H 7200 3900 50  0001 C CNN
	1    7200 3700
	0    1    -1   0   
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5E46101F
P 7200 4100
F 0 "#PWR016" H 7200 3850 50  0001 C CNN
F 1 "GND" H 7205 3927 50  0000 C CNN
F 2 "" H 7200 4100 50  0001 C CNN
F 3 "" H 7200 4100 50  0001 C CNN
	1    7200 4100
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5E46102C
P 6800 3400
F 0 "R6" V 6593 3400 50  0000 C CNN
F 1 "5.6k" V 6684 3400 50  0000 C CNN
F 2 "" V 6730 3400 50  0001 C CNN
F 3 "~" H 6800 3400 50  0001 C CNN
	1    6800 3400
	0    -1   1    0   
$EndComp
$Comp
L Device:C C6
U 1 1 5E461033
P 6500 3700
F 0 "C6" H 6615 3746 50  0000 L CNN
F 1 "DNP" H 6615 3655 50  0000 L CNN
F 2 "" H 6538 3550 50  0001 C CNN
F 3 "~" H 6500 3700 50  0001 C CNN
	1    6500 3700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6950 3400 7200 3400
Wire Wire Line
	7200 3400 7200 3500
Wire Wire Line
	7200 3900 7200 4100
$Comp
L power:GND #PWR013
U 1 1 5E461046
P 6500 4100
F 0 "#PWR013" H 6500 3850 50  0001 C CNN
F 1 "GND" H 6505 3927 50  0000 C CNN
F 2 "" H 6500 4100 50  0001 C CNN
F 3 "" H 6500 4100 50  0001 C CNN
	1    6500 4100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6500 3850 6500 4100
Wire Wire Line
	6650 3400 6500 3400
Wire Wire Line
	6500 3550 6500 3400
Connection ~ 6500 3400
Wire Wire Line
	6500 3400 6100 3400
Text Notes 6050 1550 0    50   ~ 0
Is ON if pressed button\nsequence is correct,\notherwise is OFF
Text Notes 4200 1550 0    50   ~ 0
Is ON if pressed button\nsequence is wrong,\notherwise is OFF
$Comp
L Switch:SW_Push SW3
U 1 1 5E46FC79
P 8900 3700
F 0 "SW3" V 8946 3848 50  0000 L CNN
F 1 "BUTTON_2" V 8855 3848 50  0000 L CNN
F 2 "" H 8900 3900 50  0001 C CNN
F 3 "" H 8900 3900 50  0001 C CNN
	1    8900 3700
	0    1    -1   0   
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5E46FC7F
P 8900 4100
F 0 "#PWR022" H 8900 3850 50  0001 C CNN
F 1 "GND" H 8905 3927 50  0000 C CNN
F 2 "" H 8900 4100 50  0001 C CNN
F 3 "" H 8900 4100 50  0001 C CNN
	1    8900 4100
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 5E46FC85
P 8500 3400
F 0 "R9" V 8293 3400 50  0000 C CNN
F 1 "5.6k" V 8384 3400 50  0000 C CNN
F 2 "" V 8430 3400 50  0001 C CNN
F 3 "~" H 8500 3400 50  0001 C CNN
	1    8500 3400
	0    -1   1    0   
$EndComp
$Comp
L Device:C C7
U 1 1 5E46FC8B
P 8200 3700
F 0 "C7" H 8315 3746 50  0000 L CNN
F 1 "DNP" H 8315 3655 50  0000 L CNN
F 2 "" H 8238 3550 50  0001 C CNN
F 3 "~" H 8200 3700 50  0001 C CNN
	1    8200 3700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8650 3400 8900 3400
Wire Wire Line
	8900 3400 8900 3500
Wire Wire Line
	8900 3900 8900 4100
$Comp
L power:GND #PWR019
U 1 1 5E46FC94
P 8200 4100
F 0 "#PWR019" H 8200 3850 50  0001 C CNN
F 1 "GND" H 8205 3927 50  0000 C CNN
F 2 "" H 8200 4100 50  0001 C CNN
F 3 "" H 8200 4100 50  0001 C CNN
	1    8200 4100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8200 3850 8200 4100
Wire Wire Line
	8350 3400 8200 3400
Wire Wire Line
	8200 3550 8200 3400
Connection ~ 8200 3400
Wire Wire Line
	8200 3400 7800 3400
$Comp
L Switch:SW_Push SW4
U 1 1 5E4714F3
P 10600 3700
F 0 "SW4" V 10646 3848 50  0000 L CNN
F 1 "BUTTON_2" V 10555 3848 50  0000 L CNN
F 2 "" H 10600 3900 50  0001 C CNN
F 3 "" H 10600 3900 50  0001 C CNN
	1    10600 3700
	0    1    -1   0   
$EndComp
$Comp
L power:GND #PWR024
U 1 1 5E4714F9
P 10600 4100
F 0 "#PWR024" H 10600 3850 50  0001 C CNN
F 1 "GND" H 10605 3927 50  0000 C CNN
F 2 "" H 10600 4100 50  0001 C CNN
F 3 "" H 10600 4100 50  0001 C CNN
	1    10600 4100
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R11
U 1 1 5E4714FF
P 10200 3400
F 0 "R11" V 9993 3400 50  0000 C CNN
F 1 "5.6k" V 10084 3400 50  0000 C CNN
F 2 "" V 10130 3400 50  0001 C CNN
F 3 "~" H 10200 3400 50  0001 C CNN
	1    10200 3400
	0    -1   1    0   
$EndComp
$Comp
L Device:C C8
U 1 1 5E471505
P 9900 3700
F 0 "C8" H 10015 3746 50  0000 L CNN
F 1 "DNP" H 10015 3655 50  0000 L CNN
F 2 "" H 9938 3550 50  0001 C CNN
F 3 "~" H 9900 3700 50  0001 C CNN
	1    9900 3700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10350 3400 10600 3400
Wire Wire Line
	10600 3400 10600 3500
Wire Wire Line
	10600 3900 10600 4100
$Comp
L power:GND #PWR023
U 1 1 5E47150E
P 9900 4100
F 0 "#PWR023" H 9900 3850 50  0001 C CNN
F 1 "GND" H 9905 3927 50  0000 C CNN
F 2 "" H 9900 4100 50  0001 C CNN
F 3 "" H 9900 4100 50  0001 C CNN
	1    9900 4100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9900 3850 9900 4100
Wire Wire Line
	10050 3400 9900 3400
Wire Wire Line
	9900 3550 9900 3400
Connection ~ 9900 3400
Wire Wire Line
	9900 3400 9500 3400
Text Label 5150 5800 0    50   ~ 0
BUTTON_1
Text Label 5150 5600 0    50   ~ 0
BUTTON_3
Text Label 5150 5300 0    50   ~ 0
BUTTON_2
Text Label 6450 3400 2    50   ~ 0
BUTTON_1
Text Label 8150 3400 2    50   ~ 0
BUTTON_2
Text Label 9850 3400 2    50   ~ 0
BUTTON_3
Text Notes 6000 3150 0    50   ~ 0
Pull-up is uC internal 56k pullup
Text Notes 7700 3150 0    50   ~ 0
Pull-up is uC internal 56k pullup
Text Notes 9400 3150 0    50   ~ 0
Pull-up is uC internal 56k pullup
$Comp
L Connector:AVR-ISP-6 J2
U 1 1 5E484428
P 7450 5400
F 0 "J2" H 7170 5496 50  0000 R CNN
F 1 "AVR-ISP-6" H 7170 5405 50  0000 R CNN
F 2 "" V 7200 5450 50  0001 C CNN
F 3 " ~" H 6175 4850 50  0001 C CNN
	1    7450 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5E484C7C
P 7350 5900
F 0 "#PWR018" H 7350 5650 50  0001 C CNN
F 1 "GND" H 7355 5727 50  0000 C CNN
F 2 "" H 7350 5900 50  0001 C CNN
F 3 "" H 7350 5900 50  0001 C CNN
	1    7350 5900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR017
U 1 1 5E4850E3
P 7350 4800
F 0 "#PWR017" H 7350 4650 50  0001 C CNN
F 1 "+5V" H 7365 4973 50  0000 C CNN
F 2 "" H 7350 4800 50  0001 C CNN
F 3 "" H 7350 4800 50  0001 C CNN
	1    7350 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 4800 7350 4900
Wire Wire Line
	7350 5800 7350 5900
Wire Wire Line
	7850 5200 8600 5200
Wire Wire Line
	7850 5300 8600 5300
Wire Wire Line
	7850 5400 8600 5400
Wire Wire Line
	7850 5500 8600 5500
Text Label 8050 5500 0    50   ~ 0
RESET
Text Label 8050 5400 0    50   ~ 0
SCK
Text Label 8050 5300 0    50   ~ 0
MOSI
Text Label 8050 5200 0    50   ~ 0
MISO
Text Label 5600 5900 0    50   ~ 0
SCK
Text Label 5600 5800 0    50   ~ 0
MISO
Text Label 5600 5700 0    50   ~ 0
MOSI
Text Notes 7650 4850 0    100  ~ 0
ISP connector
Text Notes 9100 5450 0    100  ~ 0
uC is configured that\ninternal pull-up resistors\nare connected to PORTB\nafter init
Wire Notes Line
	9000 4800 9000 5600
Wire Notes Line
	9000 5600 11050 5600
Wire Notes Line
	11050 5600 11050 4800
Wire Notes Line
	11050 4800 9000 4800
Wire Wire Line
	5050 5700 5800 5700
Wire Wire Line
	5050 5800 5800 5800
Wire Wire Line
	5050 5900 5800 5900
$Comp
L Device:CP C9
U 1 1 5E4C1AA4
P 3950 4450
F 0 "C9" V 3695 4450 50  0000 C CNN
F 1 "1uF" V 3786 4450 50  0000 C CNN
F 2 "" H 3988 4300 50  0001 C CNN
F 3 "~" H 3950 4450 50  0001 C CNN
	1    3950 4450
	0    1    1    0   
$EndComp
Wire Wire Line
	4100 4450 4450 4450
Connection ~ 4450 4450
Wire Wire Line
	4450 4450 4450 4900
$Comp
L power:GND #PWR025
U 1 1 5E4C433B
P 3650 4650
F 0 "#PWR025" H 3650 4400 50  0001 C CNN
F 1 "GND" H 3655 4477 50  0000 C CNN
F 2 "" H 3650 4650 50  0001 C CNN
F 3 "" H 3650 4650 50  0001 C CNN
	1    3650 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 4450 3650 4450
Wire Wire Line
	3650 4450 3650 4650
Wire Wire Line
	5050 5300 5800 5300
Wire Wire Line
	5050 5600 5800 5600
NoConn ~ 5050 5200
NoConn ~ 5050 5400
NoConn ~ 5050 5500
NoConn ~ 5050 6100
NoConn ~ 5050 6200
NoConn ~ 5050 6300
NoConn ~ 5050 6700
$EndSCHEMATC
