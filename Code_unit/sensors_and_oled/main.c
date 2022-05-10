//#include "delay.h"
//#include "sys.h"
//#include "usart.h"
//#include "lcd.h"
//#include "adc.h"
//#include "math.h"

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "math.h"

 int main(void)
  {	u8 t;
	u8 k=0;			    
	u8 temperature;  	    
	u8 humidity;    	   
    u16 adcx;
	u8 peo;
	float V,R,F,temp;
	delay_init();
	Adc_Init();
	SystemInit();	     
	uart_init(9600);
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化
    delay_ms(8000);
	OLED_Init();			//初始化OLED  
	OLED_Clear()  	; 
	
	t=' ';
	while(DHT11_Init())	
	{
		LCD_ShowString(60,130,"DHT11 Error");
		delay_ms(200);
		//LCD_Fill(60,130,239,130+16,WHITE);
 		delay_ms(200);
	}	
	
	while(1) 
	{				
		if(k%10==0)
		{									  
			DHT11_Read_Data(&temperature,&humidity);
		}				   
	 	delay_ms(10);
		k++;
		if(k==20)
		{
			k=0;
			//LED0=!LED0;
		}

	adcx=Get_Adc_Average(ADC_Channel_1,10);//ADC数字量
	V=(float)adcx*(3.3/4096);
	R=510*(5-V)/V;
	F=1008*exp(-((R-5.211)/0.3419)*((R-5.211)/0.3419))-593.3*exp(-((R-6.205)/0.7494)*((R-6.205)/0.7494))-2999*exp(-((R-6.981)/1.842)*((R-6.981)/1.842))+132400*exp(-((R+1605)/697.3)*((R+1605)/697.3))-418.5*exp(-((R-13.61)/6.125)*((R-13.62)/6.125))+66.13*exp(-((R-13.95)/1.247)*((R-13.95)/1.247))+6815*exp(-((R-5.532)/2.929)*((R-5.532)/2.929))+3225*exp(-((R-1.876)/13.36)*((R-1.876)/13.36));
	if(F>50)
		OLED_ShowString(110,6,"Y",16);//显示各种字符的函数	门口的人   
	else
		OLED_ShowString(110,6,"N",16);//显示各种字符的函数	门口的人 
        OLED_Clear();
		OLED_ShowString(0,0,"Temp:",16);//温度
		OLED_ShowChar(48,0,'0'+temperature/10,16);
		OLED_ShowChar(56,0,'0'+temperature%10,16);
		OLED_ShowString(0,3,"Hum:",16); //湿度
		OLED_ShowString(0,6,"Smog:",16);//烟雾 
		OLED_ShowString(48,6,"1",16);
		OLED_ShowChar(56,6,'0'+temperature%10,16);
	    OLED_ShowString(70,6,"Sta:",16);//压力--->	门口有没有人	
		OLED_ShowChar(48,3,'0'+humidity/10,16);
		OLED_ShowChar(56,3,'0'+humidity%10,16);	
		delay_ms(2000);
		//printf ()各种数据的蓝牙输出

	}	  
}
	
