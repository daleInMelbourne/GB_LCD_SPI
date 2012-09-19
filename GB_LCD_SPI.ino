// inslude the SPI library:
#include <SPI.h>

//#include <string.h> // Required for strlen()
// SCI16IS750 Registers 
#define THR        0x00 << 3
#define RHR        0x00 << 3
#define IER        0x01 << 3
#define FCR        0x02 << 3
#define IIR        0x02 << 3
#define LCR        0x03 << 3
#define MCR        0x04 << 3
#define LSR        0x05 << 3
#define MSR        0x06 << 3
#define SPR        0x07 << 3
#define TXFIFO     0x08 << 3
#define RXFIFO     0x09 << 3
#define DLAB       0x80 << 3
#define IODIR      0x0A << 3
#define IOSTATE    0x0B << 3
#define IOINTMSK   0x0C << 3
#define IOCTRL     0x0E << 3
#define EFCR       0x0F << 3

#define DLL        0x00 << 3
#define DLM        0x01 << 3
#define EFR        0x02 << 3
#define XON1       0x04 << 3  
#define XON2       0x05 << 3
#define XOFF1      0x06 << 3
#define XOFF2      0x07 << 3

#define SELECT     digitalWrite(CS,LOW)
#define DESELECT   digitalWrite(CS,HIGH)


//opcodes
#define WREN 6
#define WRDI 4
#define RDSR 5
#define WRSR 1
#define READ 3
#define WRITE 2

#define DATAOUT 11 //MOSI
#define DATAIN 12 //MISO
#define SPICLOCK 13 //sck
#define SLAVESELECT 2 //ss
// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 2;
char data;
void setup() {
  // set the slaveSelectPin as an output:
pinMode (SLAVESELECT, OUTPUT);
digitalWrite (SLAVESELECT, HIGH); //disable device
  // initialize SPI:
  Serial.begin(9600);
    Serial.println("\n\r\n\rGB-LCD Routine");
  SPI.begin(); 

  SPI_write(LCR,0x80);  // 0x80 to program baudrate
  SPI_write(DLL,0x60);  // 0x60=9600, 0x30=19.2K, 0x08 =115.2K with X1=14.7456 MHz
  SPI_write(DLM,0x00);  // divisor = 0x0008 for 115200 bps
  SPI_write(LCR, 0xBF); // access EFR register
  SPI_write(EFR, 0x10); // enable enhanced registers
  SPI_write(LCR, 0x03); // 8 data bit, 1 stop bit, no parity
  SPI_write(FCR, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
//  SPI_write(FCR, 0x01); // enable FIFO mode
 // Perform read/write test to check if UART is working
  SPI_write(SPR,'H');
  char data = SPI_read(SPR);
  if(data == 'H') Serial.println("Bridge initialized successfully!"); 
  else                Serial.println("Could not initialize bridge, locking up.\n\r"); 

  delay(100);                  
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);

}

void loop() {
  digitalWrite(7, HIGH);
	delay(100); 
//	delay(5000); 

SPI_write(THR, 0x55);
delay(20); 
SPI_write(THR, 0x56); 
delay(20); 
SPI_write(THR, 0x01); 
delay(2000); 
//Turn on BL
//SPI_write(THR, 0x59); 
//SPI_write(THR, 0x00); 
//SPI_write(THR, 0x01); 

}

char SPI_read (char SPIregister) {
digitalWrite (SLAVESELECT, LOW);
SPI.transfer(SPIregister | 0x80); // register address is sent
SPIregister = SPI.transfer(0); // dummy data is sent for spi read
digitalWrite (SLAVESELECT, HIGH); //release chip, signal end transfer
return SPIregister; // receive the read data
}
void SPI_write (char address, char data) { // mcu writes data to SC16IS750
digitalWrite (SLAVESELECT, LOW);
SPI.transfer (address); // address is sent
SPI.transfer (data); // data is sent
digitalWrite (SLAVESELECT, HIGH); //release chip, signal end transfer
}

