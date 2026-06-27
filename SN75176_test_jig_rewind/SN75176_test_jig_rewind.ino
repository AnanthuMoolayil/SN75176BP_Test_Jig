//Shift register 74HC595 pin setup
const int SER=8;
const int SRCLK=9;
const int RCLK=10;

//Multiplxer CD74HC4067 pin setup
const int MUX_SIG=3;
const int MUX[4]={4,5,6,7};

//SN75176BP 
const int DI=2;       //Common for DI pins of all DUTs. 

#define out_count 24  //Total no. of Shift register outputs 


uint32_t buffer=0;    //Used to push values into shift registers

inline int LED(int dut){ return 10+dut; } //To associate LEDs with corresponding DUTs.

//To select input channel 'ch' on multiplxer board.
void mux_ch_select(int ch)
{
  for(int i=3; i>=0; i--)
    digitalWrite( MUX[i], (ch>>i)&1 );
}

//To output input 'num' onto the output pins of shift registers.
void update_buffer(uint32_t num)
{
  
  //Send bits one by one into SER pin. 
  for(int i=out_count-1; i>=0; i--)
  {
    digitalWrite( SER, (num>>i)&1 );

    //Load bits into the shift register by sending a digital pulse to SRCLK pin.
    digitalWrite(SRCLK, HIGH);
    digitalWrite(SRCLK, LOW);
  }   
  //Output result bits onto the output pins by sending a digital pulse to RCLK pin. 
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}

//To enable DE pin of IC no. 'dut'.
void DE_enable(int dut)
{
  buffer |= (1UL<<dut);
  update_buffer(buffer);
}

//To disable DE pin of IC no. 'dut'.
void DE_disable(int dut)
{
  buffer &= ~(1UL<<dut);
  update_buffer(buffer);
}

//To turn ON LED corresponding to IC no. 'dut'.
void LED_ON(int dut)
{
  buffer |= ( 1UL<<LED(dut) );
  update_buffer(buffer);
}

//To display result on IC test pass.
void pass_mssg(int dut)
{
  LED_ON(dut);
  Serial.print("IC no.");
  Serial.print(dut+1);
  Serial.println(" TEST PASS!");
}

//To display result on IC test fail.
void fail_mssg(int dut)
{
  Serial.print("IC no.");
  Serial.print(dut+1);
  Serial.println(" TEST FAIL!");
}

//To test IC no. 'dut' 
void test_DUT(int dut)
{
  digitalWrite(DI, HIGH);

  mux_ch_select(dut);

  delayMicroseconds(10);
  
  DE_enable(dut);

  if( digitalRead(MUX_SIG) )
  {
    digitalWrite(DI, LOW);

    delayMicroseconds(10);
    
    if( !digitalRead(MUX_SIG) )
      pass_mssg(dut);
    else
      fail_mssg(dut);
  }
  else
    fail_mssg(dut);

  DE_disable(dut);
}


void setup() 
{
  Serial.begin(115200);
  
  //74HC595
  pinMode(SER, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);

  //CD74HC4067
  pinMode(MUX_SIG, INPUT);
  for(int i=0; i<4; i++)
    pinMode(MUX[i], OUTPUT);

  //SN75176BP
  pinMode(DI, OUTPUT);

  //Clear all outputs.
  update_buffer(buffer);

  //Test all DUTs one by one.
  for(int dut=0; dut<10; dut++)
    test_DUT(dut);

}

void loop() 
{
  
}
