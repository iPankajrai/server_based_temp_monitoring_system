#include<avr/io.h>
#define baud_rate 9600UL

#define F_CPU 16000000UL
#define ubrr_value ((F_CPU/(16UL*baud_rate))-1)
#include<util/delay.h>

//#define lcd PORTC
//#include</lcd.h>
#include<ilibrary/adc.h>

void uart_init()
{
  UBRRL=ubrr_value;
  UBRRH=(ubrr_value>>8);
  UCSRB=(1<<RXEN)|(1<<TXEN);
  UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);//UMSEL=0 IS BY DEFAULT ..NO NEED TO DECLARE THAT
}
void uart_string_send(unsigned char data)
{
  while(!(UCSRA&(1<<UDRE)));
  UDR=data;
}
unsigned char uart_rec()
{
  while(!(UCSRA&(1<<RXC)));
  return UDR;
}
void gsm_string(unsigned char *str)
{ unsigned char i=0;
  while(str[i]!='\0')
  {
    uart_string_send(str[i]);
	i++;
  }
}
/*void main()
{   DDRB=0xff;
	DDRA=0xf0;
	//lcd_init();	adc_init();
	usart_init();
	unsigned int x=0;
	while(1)
	{ 
	    //PORTA=0x00;
	   lcd_command(0x80);
	   lcd_string("message sending       ");
		x=adc_read(0);

    	x=x*4;
		x=x/10;

		 usart_string("AT+CIPCSGP=1\",\"www\"\r\n");
		 _delay_ms(1000);
          PORTB=0x01;
	     usart_string("AT+CIPMUX=0\r\n");
		 _delay_ms(1000);
         PORTB=0x02;
		 usart_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"\r\n");
		 _delay_ms(1000);
		  PORTB=0x04;

		usart_string("AT+CIPSEND\r\n");
		 _delay_ms(400);
         PORTB=0x08;
		 usart_string("GET https://api.thingspeak.com/update?api_key=UN9IFOZH5WMDP3I2&field1=200\r\n");
		   PORTB=0x10;	
//usart_num(x);
		 //usart_string("*C");
		_delay_ms(400);
		usart_send(0x1A);
		_delay_ms(10000);
		 PORTB=0x80;
	//_delay_ms(10000);
	//_delay_ms(10000);
	usart_string("AT+CIPSHUT\r\n");
	_delay_ms(1000);
		//lcd_command(0x80);
	   //lcd_string("message delivered   ");
}}	   	x=adc_read(0);

    	x=x*4;
		x=x/10;

	  while(x>=50)
	   {
	     	//lcd_command(0x80);
	        //lcd_string("your phone temperature has exceeded stay away from phone");
	   
	     PORTA=0b10000000;
	     _delay_ms(2000);   
		 usart_string("AT+CMGF=1\r\n");
		 _delay_ms(400);

	     usart_string("AT+CMGS=\"8601067669\"\r\n");
		 _delay_ms(400);

		 usart_string("Your Current Temperature is: ");
		 usart_num(x);
		 usart_string("*C");
		//_delay_ms(200);
		usart_string("\r");
		//_delay_ms(200);
		//usart_string("You are in danger");
		//_delay_ms(300);
		usart_string("your phone temperature has exceeded Please stay away from you cell phone");
		_delay_ms(300);
		usart_string("You are in danger");
		_delay_ms(300);
		usart_send(0x1A);
		_delay_ms(10000);
		PORTD=0b00000100;
		_delay_ms(2000);
		x=adc_read(0);
		 x=x*4;
         x=x/10;	   

	  }
      }		
   }
*/


void uart_number(unsigned long long int p)
{
    if(p>9) 
	{
	uart_number(p/10);
	}
	uart_string_send(p%10+48);
}

/*void usart_number(unsigned int val)
{
	int a=0;
	unsigned char value[5]={'\0'};
	for(int i=3;i>=0;i--)
	{	
		a=val%10;
		value[i]=a+48;
		val=val/10;
		}
		usart_string(value);
}
*/

void sms(int i)
{
        gsm_string("AT+CMGF=1\r\n");
            _delay_ms(1000);
	    gsm_string("AT+CMGS=\"8802767106\"\r\n");
	        _delay_ms(1000);
		gsm_string(" ALERT!!!...");
        gsm_string("\r\n");
        gsm_string("Current Temperature at your home is: ");
		uart_number(i);
        gsm_string("*Celcius");
    	    _delay_ms(300);
    	uart_string_send(0x1A);
		    _delay_ms(300);

}

/*void delay()
{ 
  unsigned char time[]="ok";
  unsigned char rec[4]={'\0'};
  start:
  for(int i=0;i<4;i++)
  {
    rec[i]=UART_rec();
  }
  int j=strcmp(time,rec);
  if(j==0)
  return 0;
  else
  goto start;
}*/

void main()
{   	
	DDRA=0xf0;
	uart_init();
	adc_init();

    unsigned int x=0;
    while(1)
   	{
	    PORTA=0X00;
		x=adc_value(0);  // 0 indicates port A0
			x=(x*4)/10;  // coz 4.8mA * 1023 will give 5v...and for 1 celsius 10 mA 
			
	    //sms(x);
		  
	      gsm_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
		  _delay_ms(5000);
          //delay();    

     	  gsm_string("AT+CIPSEND\r\n");
	      _delay_ms(5000);//wait for 5sec
		
		  gsm_string("GET https://api.thingspeak.com/update?api_key=CIWJBG1GB3O9Q53C&field1=0");
			//uart_int();
		
	   	  	uart_number(x);
          
		  gsm_string("\r\n");
		  _delay_ms(300);

		  uart_string_send(0x1A);
		  _delay_ms(5000);

		  gsm_string("AT+CIPSHUT\r\n");
		  _delay_ms(5000);
		  _delay_ms(5000);
			//sms(x);

			////////////////////////////ALERT//////////////////////////////////
			while(x>35)
		  {
		    PORTA=0x80;
			_delay_ms(500);
             sms(x);
			 gsm_string("AT+CIPMUX=0\r\n");
			 _delay_ms(5000);
			 gsm_string("AT+CIPCSGP=1,\"www\"\r\n");
			 _delay_ms(5000);

			gsm_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
		  _delay_ms(5000);

     	  gsm_string("AT+CIPSEND\r\n");
	      _delay_ms(5000);//wait for 5sec
		
		  gsm_string("GET https://api.thingspeak.com/update?api_key=CIWJBG1GB3O9Q53C&field1=0");
			uart_number(x);
          
		  gsm_string("\r\n");
		  _delay_ms(300);

		  uart_string_send(0x1A);
		  _delay_ms(5000);

		  gsm_string("AT+CIPSHUT\r\n");
		  _delay_ms(5000);
		  _delay_ms(5000);
		  	x=adc_value(1);
			x=x*4;
			x=x/10;
		  }
}
}
