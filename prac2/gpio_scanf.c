#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef struct
{
	uint32_t cfg[4];
	uint32_t dat;
	uint32_t no_used[4];
}port_type;

#define BASE_ADDR 0x01C20800
#define GPIO_MAX_PORT_COUNT 8
#define MEM_AREA_SIZE (GPIO_MAX_PORT_COUNT*sizeof(port_type))

#define GPIO_IN 0x00
#define GPIO_OUT 0x01
#define GPIO_DISABLE 0x07

int gpio_init();
static port_type* CheckAndGet_GPIO(char port, int pin);
void gpio_set_mode(char port, int pin, int mode);
int gpio_get_value(char port, int pin);
int gpio_set_value(char port, int pin, int value);

static port_type* ports = 0;

int main()
{

	int port, pin, mode, value;
	printf("port pin mode value");
	scanf("%c %d %d %d", &port, &pin, &mode, &value);
	gpio_init();
	gpio_set_mode(port, pin, mode);
	gpio_set_value(port, pin, value);
	
	while(1)
	{
		sleep(5);
		value = (value + 1) % 2;
		gpio_set_value(port, pin, value);
	}

	return 0;
}

/*GPIO 사용 설정*/
int gpio_init()
{
	if (ports != 0)
	{
		printf("inited already\n");
		return -1;
	}
	int fd = open("/dev/mem", O_RDWR);
	if (fd == -1)
	{
		perror("open '/dev/mem/ failed\n");
		return -1;
	}
	uint32_t page_size = sysconf(_SC_PAGESIZE); //시스템 하드웨어 페이지 크기를 나타냄
	uint32_t offset = BASE_ADDR % page_size;
	void* map_addr = mmap(0, MEM_AREA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BASE_ADDR - offset);
	if (map_addr == MAP_FAILED)
	{
		perror("mmap failed!\n");
		close(fd);
		return 1;
	}
	close(fd);
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
