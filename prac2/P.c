#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <string.h>

//PCA9632 Sensor Slave Address
#define PCA9632_I2C_ADDRESS 0x70

//PCA9632 Sensor Mode Set
#define MODE 0x00

//PCA9632 Sensor Register Address
#define LEDR_PWM2 0x04
#define LEDG_PWM0 0x02
#define LEDB_PWM1 0x03
#define LED_OUT 0x08
#define RESET 0x06

#define SUCCESS 1
#define ERROR -1

/*"/dev/i2c-0"'s file descriptor*/
int fd;

//-----------------------------------------------------------

//PCA9632 Sensor Check
void I2C_Slave_Check(void)
{	
    if(ioctl(fd, I2C_SLAVE, PCA9632_I2C_ADDRESS) < 0)
	{
        printf("Failed to acquire bus access and/or talk to slave\n");
        exit(1);
    }
	
	if(write(fd, NULL, 0) < 0)
	{
		printf("[PCA9632(0x70)] I2C Sensor Is Missing\n");
		exit(1);
	}
	else
	{
		printf("Check OK!! [PCA9632(0x70)] I2C Sensor\n");
	}
}

//-----------------------------------------------------------

//PCA9632 Sensor Register Data Write
int WriteData(uint8_t reg_addr, uint8_t *data, int size)
{
	uint8_t *buf;
	
	buf = malloc(size + 1);
	buf[0] = reg_addr;
	memcpy(buf + 1, data, size);
	write(fd, buf, size + 1);
	free(buf);

	return SUCCESS;
}

//-----------------------------------------------------------

//PCA9632 Sensor Register Data Read
int ReadData(uint8_t reg_addr, uint8_t *data, int size) 
{
    write(fd, &reg_addr, 1);
    read(fd, data, size);

    return SUCCESS;
}

//-----------------------------------------------------------

//PCA9632 Sensor Reset
void Reset(void)
{
	uint8_t data[3] = {0xA5, 0x5A};
	
	WriteData(RESET, data, 2);
	
	//printf("PCA9632 Reset Finish\n");
}

//-----------------------------------------------------------

//PCA9632 Sensor Mode Setting
void Init(void) 
{
	uint8_t data = 0x0f;
	
	WriteData(MODE, &data, 1);
	
	//printf("PCA9632 Mode Setting Finish\n");
}

//-----------------------------------------------------------

//PCA9632 LEDOut Setting
void LEDOUT_Setting(void)
{
	uint8_t data = 0xff;
	
	WriteData(LED_OUT, &data, 1);
	//printf("PCA9632 LEDOut Setting Finish\n");
}

//-----------------------------------------------------------

//PCA9632 First_Register_Setting
void First_Register_Setting(void)
{
	uint8_t data=255;
	
	WriteData(LEDR_PWM2, &data, 1); //RED LED
	WriteData(LEDG_PWM0, &data, 1); //GREEN LED
	WriteData(LEDB_PWM1, &data, 1); //BLUE LED
	//printf("PCA9632 First_Register_Setting Finish\n");
}

//-----------------------------------------------------------

//PCA9632 LED PWM Setting
void LED_PWM_Setting(void)
{
	uint8_t data;
	volatile int8_t i;
	
	for(i = 0; i <= 2; i++)
	{
		switch(i)
		{
			case 0 :
				printf("PWM Power Range : 0 ~ 255\n");
				printf("Warning : If it is not in range, it can be treated as 0 or strange\n");
				printf("Input LED Red PWM Power\n: ");
				break;
		
			case 1 :
				printf("Input LED Green PWM Power\n: ");
				break;
		
			case 2 :
				printf("Input LED Blue PWM Power\n: ");
				break;
		}
		scanf("%d", &data);
	
		switch(i)
		{
			case 0 :
				WriteData(LEDR_PWM2, &data, 1);
				break;
		
			case 1 :
				WriteData(LEDG_PWM0, &data, 1);
				break;
		
			case 2 :
				WriteData(LEDB_PWM1, &data, 1);
				break;
		}
	}
}

//-----------------------------------------------------------

//PCA9632 LED PWM Data Print
void Data_Print(void)
{
	uint8_t data[3] = {0,};
	
	ReadData(LEDR_PWM2, &data[0], 1);	
	ReadData(LEDG_PWM0, &data[1], 1);
	ReadData(LEDB_PWM1, &data[2], 1);
	
	printf("│ RED LED PWM : %3d │ GREEN LED PWM : %3d │ BLUE LED PWM : %3d │\n", data[0], data[1], data[2]); 
	printf("\n");
}

//-----------------------------------------------------------

int main(int argc, char *argv[])
{	
    if((fd = open("/dev/i2c-0", O_RDWR)) < 0)
	{
        printf("Failed to open i2c-0");
        exit(1);
    }
	
	printf("--------------------------------------------------------------------------\n");
	I2C_Slave_Check();
	Reset();
	Init();
	LEDOUT_Setting();
	First_Register_Setting();
	
	while(1)
	{
		printf("--------------------------------------------------------------------------\n");
		Data_Print();
		LED_PWM_Setting();
	}
	
	return 0;
}

