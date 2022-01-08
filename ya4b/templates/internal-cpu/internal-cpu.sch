EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp "bobshouse.net"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 2300 6450 1    50   Output ~ 0
GND
Text GLabel 2400 6450 1    50   Output ~ 0
VCC
Wire Wire Line
	2300 6450 2300 6600
Wire Wire Line
	2400 6450 2400 6600
Text GLabel 2700 6450 1    50   Output ~ 0
CLK
Wire Wire Line
	2700 6600 2700 6450
Text GLabel 2600 6450 1    33   Output ~ 0
~RST~
Wire Wire Line
	2600 6450 2600 6600
Text GLabel 3700 6450 1    33   Output ~ 0
~TAOE~
Text GLabel 3800 6450 1    33   Output ~ 0
~TLOE~
Text GLabel 3900 6450 1    33   Output ~ 0
~THOE~
Text GLabel 4000 6450 1    33   Output ~ 0
~A~D
Text GLabel 4100 6450 1    33   Output ~ 0
~LDTL~
Wire Wire Line
	3700 6450 3700 6600
Wire Wire Line
	3800 6450 3800 6600
Wire Wire Line
	3900 6450 3900 6600
Wire Wire Line
	4000 6450 4000 6600
Wire Wire Line
	4100 6450 4100 6600
Text GLabel 4200 6450 1    33   Output ~ 0
~LDTH~
Wire Wire Line
	4200 6450 4200 6600
NoConn ~ 5300 -800
$Comp
L Connector_Generic:Conn_01x38 J1
U 1 1 5E739D56
P 2500 6800
F 0 "J1" V 2600 6500 50  0000 L CNN
F 1 "BusConn" V 2600 6750 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x38_P2.54mm_Vertical" H 2500 6800 50  0001 C CNN
F 3 "~" H 2500 6800 50  0001 C CNN
	1    2500 6800
	0    -1   1    0   
$EndComp
Text GLabel 3300 6450 1    50   BiDi ~ 0
D0
Text GLabel 3400 6450 1    50   BiDi ~ 0
D1
Text GLabel 3500 6450 1    50   BiDi ~ 0
D2
Text GLabel 3600 6450 1    50   BiDi ~ 0
D3
Text GLabel 3100 6450 1    33   Output ~ 0
~OEA~
Text GLabel 3000 6450 1    33   Output ~ 0
~LDB~
Text GLabel 2900 6450 1    33   Output ~ 0
U~D~
Text GLabel 2800 6450 1    33   Output ~ 0
~CE~
NoConn ~ 4300 6600
NoConn ~ 4400 6600
NoConn ~ 2500 6600
Text GLabel 1800 6450 1    50   BiDi ~ 0
A4
Text GLabel 1700 6450 1    50   BiDi ~ 0
A5
Text GLabel 1600 6450 1    50   BiDi ~ 0
A6
Text GLabel 1500 6450 1    50   BiDi ~ 0
A7
NoConn ~ 3200 6600
Text GLabel 1900 6450 1    50   BiDi ~ 0
A3
Text GLabel 2000 6450 1    50   BiDi ~ 0
A2
Text GLabel 2100 6450 1    50   BiDi ~ 0
A1
Text GLabel 2200 6450 1    50   BiDi ~ 0
A0
Text GLabel 700  6450 1    50   Output ~ 0
RA3
Text GLabel 800  6450 1    50   Output ~ 0
RA2
Text GLabel 900  6450 1    50   Output ~ 0
RA1
Text GLabel 1000 6450 1    50   Output ~ 0
RA0
Text GLabel 1100 6450 1    50   Output ~ 0
RB3
Text GLabel 1200 6450 1    50   Output ~ 0
RB2
Text GLabel 1300 6450 1    50   Output ~ 0
RB1
Text GLabel 1400 6450 1    50   Output ~ 0
RB0
Wire Wire Line
	3600 6450 3600 6600
Wire Wire Line
	3500 6450 3500 6600
Wire Wire Line
	3400 6450 3400 6600
Wire Wire Line
	3300 6450 3300 6600
Wire Wire Line
	3100 6450 3100 6600
Wire Wire Line
	3000 6450 3000 6600
Wire Wire Line
	2900 6450 2900 6600
Wire Wire Line
	2800 6450 2800 6600
Wire Wire Line
	2200 6450 2200 6600
Wire Wire Line
	2100 6450 2100 6600
Wire Wire Line
	2000 6450 2000 6600
Wire Wire Line
	1900 6450 1900 6600
Wire Wire Line
	1800 6450 1800 6600
Wire Wire Line
	1700 6450 1700 6600
Wire Wire Line
	1600 6450 1600 6600
Wire Wire Line
	1500 6450 1500 6600
Wire Wire Line
	1400 6450 1400 6600
Wire Wire Line
	1300 6450 1300 6600
Wire Wire Line
	1200 6450 1200 6600
Wire Wire Line
	1100 6450 1100 6600
Wire Wire Line
	1000 6450 1000 6600
Wire Wire Line
	900  6450 900  6600
Wire Wire Line
	800  6450 800  6600
Wire Wire Line
	700  6450 700  6600
$EndSCHEMATC
