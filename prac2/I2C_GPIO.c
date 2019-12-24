#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define GPIO_BASE_ADDRESS 0x01C20800
#define PCA9632_I2C_ADDR 0x70

/*i2c setting*/
#define MODE 0x00 //mode1

#define LEDG_PWM0 0x02 //led0
#define LEDB_PWM1 0x03 //led1 
#define LEDR_PWM2 0x04 //led2
#define LEDOUT 0x80 //led output state
#define RESET 0x6

#define LED_ON 255
#define LED_OFF 0

/*gpio setting*/
#define GPIO_MAX_PORT_COUNT 6
#define MEM_AREA_SIZE (GPIO_MAX_PORT_COUNT*sizeof(port_type))

#define GPIO_IN 0x00
#define GPIO_OUT 0x01
#define GPIO_DISABLE 0x07



int fd_i2c, fd_gpio;


void I2C_SLAVE_CHECK()
{
	if (ioctl(fd_i2c, I2C_SLAVE, PCA9632_I2C_ADDR) < 0)
	{
		printf("fail!\n");
		exit(1);
	}
	if (write(fd_i2c, NULL, 0) < 0)
	{
		printf("sensor nono\n");
		exit(1);
	}
	else
	{
		printf("Check OK!!\n");
	}
}

int WriteData(uint8_t reg_addr, uint8_t *data, int size)
{
	uint8_t* buf;

	buf = malloc(size + 1);
	buf[0] = reg_addr;
	memcpy(buf + 1, data, size);
	write(fd_i2c, buf, size + 1);
	free(buf);

	return 1;
}

int ReadData(uint8_t reg_addr, uint8_t* data, int size)
{
	write(fd_i2c, &reg_addr, 1);
	read(fd_i2c, data, size);

	return 1;
}

void Reset()
{
	uint8_t data[3] = { 0xa5,0x5a };

	WriteData(RESET, data, 2);
}

void Init()
{
	uint8_t data = 0x0f;

	WriteData(MODE, &data, 1); 
	/* register auto-increment disable, auto-increment bit1=0,bit0=0,normal mode, 
	  pca9632 responds to i2c--1, 2, 3, led all call i2c address */
}

void LEDOUT_Setting()
{
	uint8_t data = 0b11111111;

	WriteData(LEDOUT, &data, 1);
}

void First_reg_Setting()
{
	uint8_t data = 0;

	WriteData(LEDG_PWM0, &data, 1); //green
	WriteData(LEDB_PWM1, &data, 1); //blue
	WriteData(LEDR_PWM2, &data, 1); //red
}


typedef struct
{
	uint32_t cfg[4];
	uint32_t dat;
	uint32_t no_used[4];
}port_type;

static port_type* ports = 0;

int gpio_init()
{
	if (ports != 0)
	{
		printf("inited already!\n");
		
		return -1;
	}
	
	int fd_gpio = open("/dev/mem", O_RDWR);
	
	if (fd_gpio == -1)
	{
		perror("open '/dev/mem nono!\n");
		return -1;
	}

	uint32_t page_size = sysconf(_SC_PAGESIZE);
	uint32_t offset = GPIO_BASE_ADDRESS % page_size;
	void* map_addr = mmap(0, MEM_AREA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_gpio, GPIO_BASE_ADDRESS - offset);

	if (map_addr == MAP_FAILED)
	{
		perror("mmap fail!!\n");
		close(fd_gpio);
		return 1;
	}
	close(fd_gpio);
	printf("map_addr: 0x%x, offset: 0x%x\n", map_addr, offset);
	ports = (port_type*)(map_addr + offset);

	return 0;
}

static port_type* CheckAndGet_GPIO(char port, int pin)
{
	if (ports == 0)
	{
		printf("not inited yet!\n");
		return 0;
	}
	int port_index = port - 'A';
	if (port_index < 0 || port_index >= GPIO_MAX_PORT_COUNT)
	{
		printf("no such port %c", port);
		return 0;
	}
	if (pin < 0 || pin >= 32)
	{
		printf("no such pin %d", pin);
		return 0;
	}
	return ports + port_index;
}

void gpio_set_mode(char port, int pin, int mode)
{
	port_type* p_port = CheckAndGet_GPIO(port, pin);
	if (p_port == 0)
	{
		return;
	}

	uint32_t* cfg = &(p_port->cfg[pin / 8]);
	int shift = (pin % 8) * 4;
	*cfg &= ~((uint32_t)0x0f << shift);
	*cfg |= ((uint32_t)(mode & 0x0f)) << shift;
}

int gpio_get_value(char port, int pin)
{
	port_type* p_port = CheckAndGet_GPIO(port, pin);

	if (p_port == 0) return -1;

	return (p_port->dat & ((uint32_t)1 << pin)) > 0;
}

int gpio_set_value(char port, int pin, int value)
{
	port_type* p_port = CheckAndGet_GPIO(port, pin);

	if (p_port == 0) return;

	if (value == 1) p_port->dat |= ((uint32_t)1) << pin;
	else p_port->dat &= ~(((uint32_t)1) << pin);
}

void Set_Switch()
{
	gpio_set_mode('E', 3, 0);
	gpio_set_mode('E', 2, 0);
	gpio_set_mode('E', 1, 0);
	gpio_set_mode('E', 0, 0);
	printf("Switch 6,5,3,4 on/off\n");
}

/*if() problem  -> switch oo*/
void Switch_on_LED()
{
	uint8_t on=255, off=0;

	if (gpio_get_value('E', 3) == 0)
	{
		WriteData(LEDR_PWM2, &on, 1);
	}
	if (gpio_get_value('E', 2) == 0)
	{
		WriteData(LEDG_PWM0, &on, 1);
	}
	if (gpio_get_value('E', 1) == 0)
	{
		WriteData(LEDB_PWM1, &on, 1);
	}
	if (gpio_get_value('E', 0) == 0)
	{
		WriteData(LEDR_PWM2, &off, 1);
		WriteData(LEDG_PWM0, &off, 1);
		WriteData(LEDB_PWM1, &off, 1);
	}

}

int main()
{
	fd_i2c = open("/dev/i2c-0", O_RDWR);
	if (fd_i2c < 0)
	{
		perror("i2c open nono\n");
		exit(1);
	}
	I2C_SLAVE_CHECK();
	Reset();
	Init();
	LEDOUT_Setting();
	gpio_init();
	Set_Switch();

	uint8_t val=255;
	while (1)
	{
		//Switch_on_LED();
		WriteData(LEDR_PWM2, &val, 1);
		

	}

	return 0;
}
