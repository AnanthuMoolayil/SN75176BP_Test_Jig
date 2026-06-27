## **SN75176BP 10-DUT Test Jig** 

## **1. Components** 

|**1. Components**||
|---|---|
|Component|Qty|
|Arduino UNO|1|
|SN75176BP|10|
|74HC595|3|
|CD74HC4067|1|
|LED|10|
|330Ω resistor|10|
|10kΩ pull resistor on RO|10|
|120Ω terminator resistor for A & B pins|1|
|680Ω pull resistor on A & B pins|2|
|100nF ceramic capacitor|13 (1 per IC)|
|10µF electrolytic capacitor|1|
|DIP-8 socket for DUT|10|

---

## **2. Arduino Pin Assignment** 

|Arduino Pin|Destination|Notes|
|---|---|---|
|D2|All SN75176 DI (Pin4)|Common data input|
|D3|CD74HC4067 SIG|Read selected RO|
|D4|MUX S0|Address bit0|
|D5|MUX S1|Address bit1|
|D6|MUX S2|Address bit2|
|D7|MUX S3|Address bit3|
|D8|74HC595 #1 SER|Serial data|
|D9|All 74HC595 SRCLK|Shared|
|D10|All 74HC595 RCLK|Shared|
|5V|+5V rail|Power|
|GND|Common GND|Ground|

---

## **3. 74HC595 Wiring** 

- Pin14 (SER): Arduino D8 for first IC; subsequent ICs from previous Q7'. 

- Pin11 (SRCLK): Arduino D9 (common). 

- Pin12 (RCLK): Arduino D10 (common). 

- Pin13 (OE): Tie permanently to GND. 

- Pin10 (MR): Tie permanently to +5V. 

- Pin16: +5V. Pin8: GND. 

- Pin9 (Q7'): Daisy-chain to next SER. 

- Daisy chaining: 

   - Q7’ pin of 1[st] 74HC595 is connected to SER pin of 2[nd] 74HC595. 

   - Q7’ pin of 2[nd] 74HC595 is connected to SER pin of 3[rd] 74HC595. 

## **4. Output Assignment** 

- 74HC595 #1 Q0-Q7 -> DUT0 to DUT7 DE (Pin3). 

- 74HC595 #2 Q0-Q1 -> DUT8 to DUT9 DE (Pin3). 

- 74HC595 #2 Q2-Q7 -> LED0 to LED5 anodes through 330Ω. 

- 74HC595 #3 Q0-Q3 -> LED6 to LED9 anodes through 330Ω. 

## **5. SN75176 Wiring** 

- Pin1 RO -> Dedicated CD74HC4067 input (Y0-Y9). 

- Pin2 RE̅ -> GND (receiver always enabled). 

- Pin3 DE -> Individual output from 74HC595 #1. 

- Pin4 DI -> Arduino D2 (common to all DUTs). 

- Pin5 GND -> Ground. 

- Pin6 A -> Common A bus. 

- Pin7 B -> Common B bus. 

- Pin8 VCC -> +5V. 

- For common A & B bus, consider the below resistor congiguration: 
```text
   +5V
     |
    680Ω  (pull-up)
     |
     |---->  A ----<---- RS-485 bus ----> to DUT A pins
     |                |
     |               120Ω
     |                |
     |---->  B ----<---- RS-485 bus ----> to DUT B pins
    680Ω  (pull-down)
     |
    GND
```


## **6. CD74HC4067 Wiring** 

- SIG -> Arduino D3. 

- S0-S3 -> Arduino D4-D7. 

- Y0-Y9 -> RO of DUT0-DUT9. 

- Y10-Y15 -> Not Connected. 

- EN -> GND. 

- VCC -> +5V. GND -> GND.

---

## **7. Passive Components** 

- 330Ω resistor in series with every LED. 

- 10kΩ pull-up OR pull-down on every RO line. 

- 100nF ceramic capacitor between VCC and GND of EVERY IC placed close to pins. 

- 10µF electrolytic capacitor across +5V and GND near power entry.

---

## **8. Design Rules** 

- All grounds common. 

- All A pins connected together. 

- All B pins connected together. 

- Only one DE active at a time. 

- LED ON = PASS, LED OFF = FAIL. 

