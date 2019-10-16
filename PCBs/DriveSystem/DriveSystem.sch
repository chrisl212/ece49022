EESchema Schematic File Version 4
LIBS:DriveSystem-cache
EELAYER 30 0
EELAYER END
$Descr A1 33110 23386
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_ST_STM32F0:STM32F051R8Tx U?
U 1 1 5DA62A92
P 6300 5700
F 0 "U?" H 5700 3950 50  0000 C CNN
F 1 "STM32F051R8Tx" H 5750 3850 50  0000 C CNN
F 2 "Package_QFP:LQFP-64_10x10mm_P0.5mm" H 5800 4000 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00039193.pdf" H 6300 5700 50  0001 C CNN
	1    6300 5700
	1    0    0    -1  
$EndComp
$Comp
L Driver_Motor:DRV8848 U?
U 1 1 5DA66C23
P 9250 4700
F 0 "U?" H 8900 5350 50  0000 C CNN
F 1 "DRV8848" H 8900 5250 50  0000 C CNN
F 2 "Package_SO:TSSOP-16-1EP_4.4x5mm_P0.65mm" H 9250 4000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/drv8848.pdf" H 8100 6050 50  0001 C CNN
	1    9250 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5DA69C3C
P 10100 4700
F 0 "R?" V 10100 4700 50  0000 C CNN
F 1 "0.25" V 10200 4700 50  0000 C CNN
F 2 "" V 10030 4700 50  0001 C CNN
F 3 "~" H 10100 4700 50  0001 C CNN
	1    10100 4700
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5DA6A557
P 10100 5100
F 0 "R?" V 10100 5100 50  0000 C CNN
F 1 "0.25" V 10200 5100 50  0000 C CNN
F 2 "" V 10030 5100 50  0001 C CNN
F 3 "~" H 10100 5100 50  0001 C CNN
	1    10100 5100
	0    1    1    0   
$EndComp
Text Notes 10650 3000 0    50   ~ 0
-Spade Connectors for motors
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5DA6E047
P 10450 4500
F 0 "J?" H 10530 4492 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10530 4401 50  0000 L CNN
F 2 "" H 10450 4500 50  0001 C CNN
F 3 "~" H 10450 4500 50  0001 C CNN
	1    10450 4500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5DA7004C
P 10450 4900
F 0 "J?" H 10530 4892 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10530 4801 50  0000 L CNN
F 2 "" H 10450 4900 50  0001 C CNN
F 3 "~" H 10450 4900 50  0001 C CNN
	1    10450 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	12000 4650 12000 4700
Wire Wire Line
	12300 5200 12300 5400
Wire Wire Line
	12300 4900 12300 4650
Text Notes 10650 3100 0    50   ~ 0
-Sense resistor values\n
Wire Wire Line
	9850 4600 10250 4600
Wire Wire Line
	9850 4500 10250 4500
Wire Wire Line
	9850 4900 10250 4900
Wire Wire Line
	9850 5000 10250 5000
Wire Wire Line
	9850 4700 9950 4700
Wire Wire Line
	9850 5100 9950 5100
$Comp
L power:GND #PWR?
U 1 1 5DA753B4
P 9300 5400
F 0 "#PWR?" H 9300 5150 50  0001 C CNN
F 1 "GND" H 9305 5227 50  0000 C CNN
F 2 "" H 9300 5400 50  0001 C CNN
F 3 "" H 9300 5400 50  0001 C CNN
	1    9300 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 5300 9350 5350
Wire Wire Line
	9350 5350 9300 5350
Wire Wire Line
	9250 5350 9250 5300
Wire Wire Line
	9300 5400 9300 5350
Connection ~ 9300 5350
Wire Wire Line
	9300 5350 9250 5350
$Comp
L power:GND #PWR?
U 1 1 5DA76542
P 10350 5150
F 0 "#PWR?" H 10350 4900 50  0001 C CNN
F 1 "GND" H 10450 5150 50  0000 C CNN
F 2 "" H 10350 5150 50  0001 C CNN
F 3 "" H 10350 5150 50  0001 C CNN
	1    10350 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5DA76B06
P 10350 4750
F 0 "#PWR?" H 10350 4500 50  0001 C CNN
F 1 "GND" H 10450 4750 50  0000 C CNN
F 2 "" H 10350 4750 50  0001 C CNN
F 3 "" H 10350 4750 50  0001 C CNN
	1    10350 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 4700 10350 4700
Wire Wire Line
	10350 4700 10350 4750
Wire Wire Line
	10250 5100 10350 5100
Wire Wire Line
	10350 5100 10350 5150
$Comp
L power:GND #PWR?
U 1 1 5DA7AC84
P 11650 4200
F 0 "#PWR?" H 11650 3950 50  0001 C CNN
F 1 "GND" H 11750 4200 50  0000 C CNN
F 2 "" H 11650 4200 50  0001 C CNN
F 3 "" H 11650 4200 50  0001 C CNN
	1    11650 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 4300 8400 4300
Text Label 8400 4300 0    50   ~ 0
VINT
Wire Wire Line
	12000 5050 12000 5000
$Comp
L Device:R R?
U 1 1 5DA6CB47
P 12000 4850
F 0 "R?" H 12070 4896 50  0000 L CNN
F 1 "510" H 12070 4805 50  0000 L CNN
F 2 "" V 11930 4850 50  0001 C CNN
F 3 "~" H 12000 4850 50  0001 C CNN
	1    12000 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	12000 5350 12000 5400
$Comp
L Device:LED D?
U 1 1 5DA6BEEC
P 12000 5200
F 0 "D?" V 12039 5083 50  0000 R CNN
F 1 "LED" V 11948 5083 50  0000 R CNN
F 2 "" H 12000 5200 50  0001 C CNN
F 3 "~" H 12000 5200 50  0001 C CNN
	1    12000 5200
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5DA6B503
P 12300 5050
F 0 "R?" H 12370 5096 50  0000 L CNN
F 1 "3.3k" H 12370 5005 50  0000 L CNN
F 2 "" V 12230 5050 50  0001 C CNN
F 3 "~" H 12300 5050 50  0001 C CNN
	1    12300 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	12300 4650 12150 4650
$Comp
L power:+3V3 #PWR?
U 1 1 5DA84D3C
P 12150 4550
F 0 "#PWR?" H 12150 4400 50  0001 C CNN
F 1 "+3V3" H 12165 4723 50  0000 C CNN
F 2 "" H 12150 4550 50  0001 C CNN
F 3 "" H 12150 4550 50  0001 C CNN
	1    12150 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	12150 4650 12150 4550
Connection ~ 12150 4650
Wire Wire Line
	12150 4650 12000 4650
Wire Wire Line
	8650 4700 8400 4700
Wire Wire Line
	8650 4800 8400 4800
Wire Wire Line
	8650 5000 8400 5000
Wire Wire Line
	8650 5100 8400 5100
Text Label 8400 4700 0    50   ~ 0
AIN1
Text Label 8400 4800 0    50   ~ 0
AIN2
Text Label 8400 5000 0    50   ~ 0
BIN1
Text Label 8400 5100 0    50   ~ 0
BIN2
Wire Wire Line
	9850 4300 10100 4300
Text Label 10100 4300 2    50   ~ 0
VINT
Wire Wire Line
	11400 3700 11650 3700
Text Label 11400 3700 0    50   ~ 0
VINT
Wire Wire Line
	12600 4200 12850 4200
$Comp
L power:GND #PWR?
U 1 1 5DA904C8
P 12850 4250
F 0 "#PWR?" H 12850 4000 50  0001 C CNN
F 1 "GND" H 12950 4250 50  0000 C CNN
F 2 "" H 12850 4250 50  0001 C CNN
F 3 "" H 12850 4250 50  0001 C CNN
	1    12850 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	12850 4250 12850 4200
Connection ~ 12850 4200
Wire Wire Line
	12850 4200 13100 4200
Wire Wire Line
	9250 4100 9250 4000
Wire Wire Line
	9250 4000 9500 4000
Text Label 9500 4000 2    50   ~ 0
VM
Text Label 12350 3700 0    50   ~ 0
VM
Wire Wire Line
	12000 5400 12300 5400
Wire Wire Line
	12300 5400 12600 5400
Connection ~ 12300 5400
Text Label 12600 5400 2    50   ~ 0
nFAULT
Wire Wire Line
	8650 4400 8400 4400
Wire Wire Line
	8650 4500 8400 4500
Text Label 8400 4400 0    50   ~ 0
nFAULT
Text Label 8400 4500 0    50   ~ 0
nSLEEP
Wire Wire Line
	6900 4900 7200 4900
Wire Wire Line
	6900 5000 7200 5000
Text Label 7200 4900 2    50   ~ 0
AIN1
Text Label 7200 5000 2    50   ~ 0
AIN2
Wire Wire Line
	6900 5100 7200 5100
Wire Wire Line
	6900 5200 7200 5200
Wire Wire Line
	5700 6600 5400 6600
Text Label 7200 5100 2    50   ~ 0
BIN1
Text Label 7200 5200 2    50   ~ 0
BIN2
Text Label 5400 6600 0    50   ~ 0
nSLEEP
Wire Wire Line
	6900 6700 7200 6700
Wire Wire Line
	6900 4700 7200 4700
Wire Wire Line
	6900 4800 7200 4800
Text Label 7200 4700 2    50   ~ 0
ECHOr
Text Label 7200 4800 2    50   ~ 0
ECHOl
Text Label 7200 6700 2    50   ~ 0
TRIG
$Comp
L Regulator_Linear:LM7805_TO220 U?
U 1 1 5DA79D3F
P 8300 6000
F 0 "U?" H 8300 6242 50  0000 C CNN
F 1 "LM7805_TO220" H 8300 6151 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 8300 6225 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM7805.pdf" H 8300 5950 50  0001 C CNN
	1    8300 6000
	1    0    0    -1  
$EndComp
Text Notes 10650 3200 0    50   ~ 0
-Verify package on the 5V regulator\n
$Comp
L Device:C C?
U 1 1 5DA7B8E8
P 7800 6150
F 0 "C?" H 7915 6196 50  0000 L CNN
F 1 "0.22u" H 7915 6105 50  0000 L CNN
F 2 "" H 7838 6000 50  0001 C CNN
F 3 "~" H 7800 6150 50  0001 C CNN
	1    7800 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 6000 8000 6000
Wire Wire Line
	7800 6300 8300 6300
Wire Wire Line
	8300 6300 8800 6300
Connection ~ 8300 6300
Wire Wire Line
	8800 6000 8600 6000
$Comp
L power:GND #PWR?
U 1 1 5DA81605
P 8300 6400
F 0 "#PWR?" H 8300 6150 50  0001 C CNN
F 1 "GND" H 8305 6227 50  0000 C CNN
F 2 "" H 8300 6400 50  0001 C CNN
F 3 "" H 8300 6400 50  0001 C CNN
	1    8300 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 6400 8300 6300
Wire Wire Line
	12850 3650 12850 3700
Connection ~ 12850 3700
Wire Wire Line
	12850 3700 13100 3700
$Comp
L power:+12V #PWR?
U 1 1 5DA84CA6
P 12850 3650
F 0 "#PWR?" H 12850 3500 50  0001 C CNN
F 1 "+12V" H 12865 3823 50  0000 C CNN
F 2 "" H 12850 3650 50  0001 C CNN
F 3 "" H 12850 3650 50  0001 C CNN
	1    12850 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 6000 7800 6000
Connection ~ 7800 6000
$Comp
L power:+12V #PWR?
U 1 1 5DA86D50
P 7600 6000
F 0 "#PWR?" H 7600 5850 50  0001 C CNN
F 1 "+12V" H 7615 6173 50  0000 C CNN
F 2 "" H 7600 6000 50  0001 C CNN
F 3 "" H 7600 6000 50  0001 C CNN
	1    7600 6000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5DA7C877
P 8800 6150
F 0 "C?" H 8915 6196 50  0000 L CNN
F 1 "0.1u" H 8915 6105 50  0000 L CNN
F 2 "" H 8838 6000 50  0001 C CNN
F 3 "~" H 8800 6150 50  0001 C CNN
	1    8800 6150
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5DA8909C
P 8950 6000
F 0 "#PWR?" H 8950 5850 50  0001 C CNN
F 1 "+5V" H 8965 6173 50  0000 C CNN
F 2 "" H 8950 6000 50  0001 C CNN
F 3 "" H 8950 6000 50  0001 C CNN
	1    8950 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 6000 8800 6000
Connection ~ 8800 6000
$Comp
L Connector:Conn_01x04_Male J?
U 1 1 5DA8B5A2
P 10050 6050
F 0 "J?" H 9900 6350 50  0000 C CNN
F 1 "Conn_01x04_Male" H 9900 6250 50  0000 C CNN
F 2 "" H 10050 6050 50  0001 C CNN
F 3 "~" H 10050 6050 50  0001 C CNN
	1    10050 6050
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J?
U 1 1 5DA8BAA6
P 10050 6800
F 0 "J?" H 9900 7100 50  0000 C CNN
F 1 "Conn_01x04_Male" H 9900 7000 50  0000 C CNN
F 2 "" H 10050 6800 50  0001 C CNN
F 3 "~" H 10050 6800 50  0001 C CNN
	1    10050 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 6050 10500 6050
Wire Wire Line
	10250 6150 10500 6150
Text Label 10500 6150 2    50   ~ 0
ECHOr
Text Label 10500 6050 2    50   ~ 0
TRIG
Wire Wire Line
	10250 5950 10500 5950
Wire Wire Line
	10250 6250 10500 6250
$Comp
L power:GND #PWR?
U 1 1 5DA9B381
P 10500 6250
F 0 "#PWR?" H 10500 6000 50  0001 C CNN
F 1 "GND" H 10505 6077 50  0000 C CNN
F 2 "" H 10500 6250 50  0001 C CNN
F 3 "" H 10500 6250 50  0001 C CNN
	1    10500 6250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5DA9B73B
P 10500 5950
F 0 "#PWR?" H 10500 5800 50  0001 C CNN
F 1 "+5V" H 10515 6123 50  0000 C CNN
F 2 "" H 10500 5950 50  0001 C CNN
F 3 "" H 10500 5950 50  0001 C CNN
	1    10500 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 6700 10500 6700
Wire Wire Line
	10250 6800 10500 6800
Wire Wire Line
	10250 6900 10500 6900
Wire Wire Line
	10250 7000 10500 7000
$Comp
L power:GND #PWR?
U 1 1 5DAA2D4E
P 10500 7000
F 0 "#PWR?" H 10500 6750 50  0001 C CNN
F 1 "GND" H 10505 6827 50  0000 C CNN
F 2 "" H 10500 7000 50  0001 C CNN
F 3 "" H 10500 7000 50  0001 C CNN
	1    10500 7000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5DAA2F7C
P 10500 6700
F 0 "#PWR?" H 10500 6550 50  0001 C CNN
F 1 "+5V" H 10515 6873 50  0000 C CNN
F 2 "" H 10500 6700 50  0001 C CNN
F 3 "" H 10500 6700 50  0001 C CNN
	1    10500 6700
	1    0    0    -1  
$EndComp
Text Label 10500 6800 2    50   ~ 0
TRIG
Text Label 10500 6900 2    50   ~ 0
ECHOl
Text Notes 10650 3300 0    50   ~ 0
-Make sure ultrasonic pins oriented correctly
$Comp
L Device:C C?
U 1 1 5DAA68B0
P 13100 3950
F 0 "C?" H 13215 3996 50  0000 L CNN
F 1 "0.1u" H 13215 3905 50  0000 L CNN
F 2 "" H 13138 3800 50  0001 C CNN
F 3 "~" H 13100 3950 50  0001 C CNN
	1    13100 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	13100 4100 13100 4200
Wire Wire Line
	13100 3800 13100 3700
Wire Wire Line
	12350 3700 12600 3700
$Comp
L Device:C C?
U 1 1 5DAAD29C
P 12600 3950
F 0 "C?" H 12715 3996 50  0000 L CNN
F 1 "10u" H 12715 3905 50  0000 L CNN
F 2 "" H 12638 3800 50  0001 C CNN
F 3 "~" H 12600 3950 50  0001 C CNN
	1    12600 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	12600 4100 12600 4200
Wire Wire Line
	12600 3800 12600 3700
Connection ~ 12600 3700
Wire Wire Line
	12600 3700 12850 3700
$Comp
L Device:C C?
U 1 1 5DAB180D
P 11650 3950
F 0 "C?" H 11765 3996 50  0000 L CNN
F 1 "2.2u" H 11765 3905 50  0000 L CNN
F 2 "" H 11688 3800 50  0001 C CNN
F 3 "~" H 11650 3950 50  0001 C CNN
	1    11650 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	11650 4100 11650 4200
Wire Wire Line
	11650 3800 11650 3700
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5DAB56F1
P 14000 3850
F 0 "J?" H 14080 3842 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 14080 3751 50  0000 L CNN
F 2 "" H 14000 3850 50  0001 C CNN
F 3 "~" H 14000 3850 50  0001 C CNN
	1    14000 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	13800 3950 13650 3950
Wire Wire Line
	13650 3950 13650 4200
$Comp
L power:GND #PWR?
U 1 1 5DAB8482
P 13650 4200
F 0 "#PWR?" H 13650 3950 50  0001 C CNN
F 1 "GND" H 13750 4200 50  0000 C CNN
F 2 "" H 13650 4200 50  0001 C CNN
F 3 "" H 13650 4200 50  0001 C CNN
	1    13650 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	13800 3850 13650 3850
Wire Wire Line
	13650 3850 13650 3650
$Comp
L power:+12V #PWR?
U 1 1 5DABA6A3
P 13650 3650
F 0 "#PWR?" H 13650 3500 50  0001 C CNN
F 1 "+12V" H 13665 3823 50  0000 C CNN
F 2 "" H 13650 3650 50  0001 C CNN
F 3 "" H 13650 3650 50  0001 C CNN
	1    13650 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3900 6200 3800
Wire Wire Line
	6200 3800 6300 3800
Wire Wire Line
	6500 3800 6500 3900
Wire Wire Line
	6300 3900 6300 3800
Connection ~ 6300 3800
Wire Wire Line
	6300 3800 6350 3800
Wire Wire Line
	6400 3900 6400 3800
Connection ~ 6400 3800
Wire Wire Line
	6400 3800 6500 3800
Wire Wire Line
	6350 3800 6350 3750
Connection ~ 6350 3800
Wire Wire Line
	6350 3800 6400 3800
$Comp
L power:VDD #PWR?
U 1 1 5DA8196E
P 6350 3750
F 0 "#PWR?" H 6350 3600 50  0001 C CNN
F 1 "VDD" H 6367 3923 50  0000 C CNN
F 2 "" H 6350 3750 50  0001 C CNN
F 3 "" H 6350 3750 50  0001 C CNN
	1    6350 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 7500 6200 7550
Wire Wire Line
	6200 7550 6300 7550
Wire Wire Line
	6400 7550 6400 7500
Wire Wire Line
	6300 7500 6300 7550
Connection ~ 6300 7550
Wire Wire Line
	6300 7550 6400 7550
$Comp
L power:GND #PWR?
U 1 1 5DA875CA
P 6300 7600
F 0 "#PWR?" H 6300 7350 50  0001 C CNN
F 1 "GND" H 5750 7500 50  0000 C CNN
F 2 "" H 6300 7600 50  0001 C CNN
F 3 "" H 6300 7600 50  0001 C CNN
	1    6300 7600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 7600 6300 7550
Text Label 9400 7750 2    50   ~ 0
NRST
$Comp
L Connector:Conn_01x05_Male J?
U 1 1 5DAA0C45
P 8900 7550
F 0 "J?" H 9008 7931 50  0000 C CNN
F 1 "Conn_01x05_Male" H 9008 7840 50  0000 C CNN
F 2 "" H 8900 7550 50  0001 C CNN
F 3 "~" H 8900 7550 50  0001 C CNN
	1    8900 7550
	1    0    0    -1  
$EndComp
Text Label 9400 7450 2    50   ~ 0
SWCLK
Text Label 9400 7650 2    50   ~ 0
SWDIO
$Comp
L power:GND #PWR?
U 1 1 5DAA5383
P 9550 7600
F 0 "#PWR?" H 9550 7350 50  0001 C CNN
F 1 "GND" H 9555 7427 50  0000 C CNN
F 2 "" H 9550 7600 50  0001 C CNN
F 3 "" H 9550 7600 50  0001 C CNN
	1    9550 7600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 7550 9550 7600
Wire Wire Line
	9100 7550 9550 7550
Wire Wire Line
	9100 7750 9400 7750
Wire Wire Line
	9100 7650 9400 7650
Wire Wire Line
	9100 7450 9400 7450
$Comp
L power:VDD #PWR?
U 1 1 5DAB6913
P 9550 7300
F 0 "#PWR?" H 9550 7150 50  0001 C CNN
F 1 "VDD" H 9567 7473 50  0000 C CNN
F 2 "" H 9550 7300 50  0001 C CNN
F 3 "" H 9550 7300 50  0001 C CNN
	1    9550 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 7350 9550 7300
Wire Wire Line
	9100 7350 9550 7350
$Comp
L Device:CP1 C?
U 1 1 5DABEC87
P 11500 6550
F 0 "C?" H 11615 6596 50  0000 L CNN
F 1 "100n" H 11615 6505 50  0000 L CNN
F 2 "" H 11500 6550 50  0001 C CNN
F 3 "~" H 11500 6550 50  0001 C CNN
	1    11500 6550
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5DABFA24
P 11850 6550
F 0 "SW?" V 11804 6698 50  0000 L CNN
F 1 "SW_Push" V 11895 6698 50  0000 L CNN
F 2 "" H 11850 6750 50  0001 C CNN
F 3 "~" H 11850 6750 50  0001 C CNN
	1    11850 6550
	0    1    1    0   
$EndComp
Wire Wire Line
	11500 6700 11500 6900
Wire Wire Line
	11850 6900 11850 6750
Wire Wire Line
	11850 6350 11850 6150
Wire Wire Line
	11850 6150 11500 6150
Wire Wire Line
	11500 6150 11500 6400
Wire Wire Line
	11500 6900 11850 6900
$Comp
L power:GND #PWR?
U 1 1 5DACB9D7
P 11500 7000
F 0 "#PWR?" H 11500 6750 50  0001 C CNN
F 1 "GND" H 11505 6827 50  0000 C CNN
F 2 "" H 11500 7000 50  0001 C CNN
F 3 "" H 11500 7000 50  0001 C CNN
	1    11500 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	11500 7000 11500 6900
Connection ~ 11500 6900
Connection ~ 11500 6150
Text Label 11250 6150 0    50   ~ 0
NRST
$Comp
L Device:R R?
U 1 1 5DAD73A4
P 11100 7800
F 0 "R?" H 11170 7846 50  0000 L CNN
F 1 "510" H 11170 7755 50  0000 L CNN
F 2 "" V 11030 7800 50  0001 C CNN
F 3 "~" H 11100 7800 50  0001 C CNN
	1    11100 7800
	1    0    0    -1  
$EndComp
Wire Wire Line
	11100 7650 11100 7450
$Comp
L power:GND #PWR?
U 1 1 5DADBC2E
P 11100 8100
F 0 "#PWR?" H 11100 7850 50  0001 C CNN
F 1 "GND" H 11250 8050 50  0000 C CNN
F 2 "" H 11100 8100 50  0001 C CNN
F 3 "" H 11100 8100 50  0001 C CNN
	1    11100 8100
	1    0    0    -1  
$EndComp
Wire Wire Line
	11100 8100 11100 7950
Text Label 10800 7450 0    50   ~ 0
BOOT0
Wire Wire Line
	10800 7450 11100 7450
Wire Wire Line
	11250 6150 11500 6150
$Comp
L Device:LED D?
U 1 1 5DAE8082
P 11900 7850
F 0 "D?" V 11939 7733 50  0000 R CNN
F 1 "LED" V 11848 7733 50  0000 R CNN
F 2 "" H 11900 7850 50  0001 C CNN
F 3 "~" H 11900 7850 50  0001 C CNN
	1    11900 7850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5DAE8356
P 11900 7500
F 0 "R?" H 11970 7546 50  0000 L CNN
F 1 "510" H 11970 7455 50  0000 L CNN
F 2 "" V 11830 7500 50  0001 C CNN
F 3 "~" H 11900 7500 50  0001 C CNN
	1    11900 7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	11900 7700 11900 7650
Wire Wire Line
	11900 8000 11900 8100
$Comp
L power:GND #PWR?
U 1 1 5DAE86B9
P 11900 8100
F 0 "#PWR?" H 11900 7850 50  0001 C CNN
F 1 "GND" H 12050 8050 50  0000 C CNN
F 2 "" H 11900 8100 50  0001 C CNN
F 3 "" H 11900 8100 50  0001 C CNN
	1    11900 8100
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR?
U 1 1 5DB04244
P 11900 7300
F 0 "#PWR?" H 11900 7150 50  0001 C CNN
F 1 "VDD" H 11917 7473 50  0000 C CNN
F 2 "" H 11900 7300 50  0001 C CNN
F 3 "" H 11900 7300 50  0001 C CNN
	1    11900 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	11900 7350 11900 7300
$Comp
L Device:CP1 C?
U 1 1 5DB086EC
P 12500 6450
F 0 "C?" H 12615 6496 50  0000 L CNN
F 1 "1u" H 12615 6405 50  0000 L CNN
F 2 "" H 12500 6450 50  0001 C CNN
F 3 "~" H 12500 6450 50  0001 C CNN
	1    12500 6450
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C?
U 1 1 5DB08B53
P 12900 6450
F 0 "C?" H 13015 6496 50  0000 L CNN
F 1 "100n" H 13015 6405 50  0000 L CNN
F 2 "" H 12900 6450 50  0001 C CNN
F 3 "~" H 12900 6450 50  0001 C CNN
	1    12900 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	12500 6600 12500 6750
Wire Wire Line
	12900 6750 12900 6600
Wire Wire Line
	12900 6300 12900 6150
$Comp
L Device:CP1 C?
U 1 1 5DB16670
P 13400 6450
F 0 "C?" H 13515 6496 50  0000 L CNN
F 1 "1u" H 13515 6405 50  0000 L CNN
F 2 "" H 13400 6450 50  0001 C CNN
F 3 "~" H 13400 6450 50  0001 C CNN
	1    13400 6450
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C?
U 1 1 5DB16676
P 13800 6450
F 0 "C?" H 13915 6496 50  0000 L CNN
F 1 "100n" H 13915 6405 50  0000 L CNN
F 2 "" H 13800 6450 50  0001 C CNN
F 3 "~" H 13800 6450 50  0001 C CNN
	1    13800 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	13400 6600 13400 6750
Wire Wire Line
	13800 6750 13800 6600
Wire Wire Line
	13800 6300 13800 6150
Wire Wire Line
	13800 6150 13600 6150
Wire Wire Line
	13400 6150 13400 6300
$Comp
L power:GND #PWR?
U 1 1 5DB1A811
P 12700 6900
F 0 "#PWR?" H 12700 6650 50  0001 C CNN
F 1 "GND" H 12705 6727 50  0000 C CNN
F 2 "" H 12700 6900 50  0001 C CNN
F 3 "" H 12700 6900 50  0001 C CNN
	1    12700 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	12700 6900 12700 6750
$Comp
L power:GND #PWR?
U 1 1 5DB1F223
P 13600 6900
F 0 "#PWR?" H 13600 6650 50  0001 C CNN
F 1 "GND" H 13605 6727 50  0000 C CNN
F 2 "" H 13600 6900 50  0001 C CNN
F 3 "" H 13600 6900 50  0001 C CNN
	1    13600 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	13600 6900 13600 6750
Connection ~ 13600 6750
Wire Wire Line
	13600 6750 13800 6750
Wire Wire Line
	13400 6750 13600 6750
Connection ~ 12700 6750
Wire Wire Line
	12700 6750 12900 6750
Wire Wire Line
	12500 6750 12700 6750
$Comp
L power:VDD #PWR?
U 1 1 5DB3207B
P 13600 6050
F 0 "#PWR?" H 13600 5900 50  0001 C CNN
F 1 "VDD" H 13617 6223 50  0000 C CNN
F 2 "" H 13600 6050 50  0001 C CNN
F 3 "" H 13600 6050 50  0001 C CNN
	1    13600 6050
	1    0    0    -1  
$EndComp
Connection ~ 13600 6150
Wire Wire Line
	13600 6150 13400 6150
Wire Wire Line
	12500 6150 12500 6300
Wire Wire Line
	12700 6150 12500 6150
Connection ~ 12700 6150
Wire Wire Line
	12900 6150 12700 6150
$Comp
L power:VDD #PWR?
U 1 1 5DB31C98
P 12700 6050
F 0 "#PWR?" H 12700 5900 50  0001 C CNN
F 1 "VDD" H 12717 6223 50  0000 C CNN
F 2 "" H 12700 6050 50  0001 C CNN
F 3 "" H 12700 6050 50  0001 C CNN
	1    12700 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	12700 6050 12700 6150
Wire Wire Line
	13600 6050 13600 6150
Wire Wire Line
	6900 5500 7200 5500
Text Label 7200 5500 2    50   ~ 0
SWCLK
Wire Wire Line
	6900 5400 7200 5400
Text Label 7200 5400 2    50   ~ 0
SWDIO
Wire Wire Line
	5700 4100 5400 4100
Text Label 5400 4100 0    50   ~ 0
NRST
Wire Wire Line
	5400 4300 5700 4300
Text Label 5400 4300 0    50   ~ 0
BOOT0
Text Notes 10650 3400 0    50   ~ 0
-Connect nFault to micro?\n
$EndSCHEMATC
