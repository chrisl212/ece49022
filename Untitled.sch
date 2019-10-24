EESchema Schematic File Version 4
LIBS:Untitled-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
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
L INA126P:INA126P U?
U 1 1 5DB1C9EE
P 5700 4000
F 0 "U?" H 5700 4770 50  0000 C CNN
F 1 "INA126P" H 5700 4679 50  0000 C CNN
F 2 "DIP254P762X508-8" H 5700 4000 50  0001 L BNN
F 3 "INA126P" H 5700 4000 50  0001 L BNN
F 4 "None" H 5700 4000 50  0001 L BNN "Field4"
F 5 "8pin Dip/Pkg Micropower Instrumentation Amplifier Single And Dual Versions" H 5700 4000 50  0001 L BNN "Field5"
F 6 "Unavailable" H 5700 4000 50  0001 L BNN "Field6"
F 7 "DIP-8 Texas Instruments" H 5700 4000 50  0001 L BNN "Field7"
F 8 "Texas Instruments" H 5700 4000 50  0001 L BNN "Field8"
	1    5700 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5DB1CD8A
P 4700 4350
F 0 "R?" H 4770 4396 50  0000 L CNN
F 1 "R" H 4770 4305 50  0000 L CNN
F 2 "" V 4630 4350 50  0001 C CNN
F 3 "~" H 4700 4350 50  0001 C CNN
	1    4700 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 4300 5000 4200
Wire Wire Line
	5000 4200 4700 4200
Wire Wire Line
	4700 4500 5000 4500
Wire Wire Line
	5000 4500 5000 4400
Wire Wire Line
	5000 3900 4600 3900
Wire Wire Line
	5000 3800 4600 3800
Wire Wire Line
	4600 3800 4600 3750
Wire Wire Line
	4600 3900 4600 3950
Wire Wire Line
	5000 3600 4800 3600
Wire Wire Line
	4800 3600 4800 3400
Wire Wire Line
	5000 4600 4850 4600
Wire Wire Line
	4850 4600 4850 4800
$Comp
L power:GND #PWR?
U 1 1 5DB1D9AD
P 4850 4800
F 0 "#PWR?" H 4850 4550 50  0001 C CNN
F 1 "GND" H 4855 4627 50  0000 C CNN
F 2 "" H 4850 4800 50  0001 C CNN
F 3 "" H 4850 4800 50  0001 C CNN
	1    4850 4800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5DB1DF4A
P 4800 3400
F 0 "#PWR?" H 4800 3250 50  0001 C CNN
F 1 "+5V" H 4815 3573 50  0000 C CNN
F 2 "" H 4800 3400 50  0001 C CNN
F 3 "" H 4800 3400 50  0001 C CNN
	1    4800 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 4100 4450 4100
Wire Wire Line
	4450 4100 4450 4600
Wire Wire Line
	4450 4600 4850 4600
Connection ~ 4850 4600
Wire Wire Line
	6400 3600 6650 3600
Text Notes 6600 3550 0    50   ~ 0
Vadc
Text Notes 4700 4000 0    50   ~ 0
Vgreen
Text Notes 4700 3750 0    50   ~ 0
Vred\n
$EndSCHEMATC
