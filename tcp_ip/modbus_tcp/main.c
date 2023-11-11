#include <stdio.h>
#include <string.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <unistd.h>

#define i08 int8_t
#define u08 uint8_t
#define u16 uint16_t

#define REG_COIL_START		0
#define	REG_COIL_NREGS 		16
u08 us_coil_buf[(REG_COIL_NREGS/8)+1];

#define REG_DESCRETES_START	0
#define REG_DESCRETES_NREGS	16
u08 us_descretes_buf[(REG_DESCRETES_NREGS/8)+1] = {0};

#define REG_INPUT_START 	0
#define REG_INPUT_NREGS 	8
u16 us_reg_input_buf[REG_INPUT_NREGS] = {0};

#define REG_HOLDING_START 	0
#define REG_HOLDING_NREGS 	8
u16 us_reg_holding_buf[REG_HOLDING_NREGS] = {1,2,3,4,5,6,7,8};

int main(int argc, char *argv[]){

	printf("IP	: %s\n", argv[1]);
	printf("Port	: %s\n", argv[2]);

	modbus_t *ctx;
	i08 status = 0;

	ctx = modbus_new_tcp(argv[1], atoi((char*)argv[2]));
	modbus_set_debug(ctx, TRUE);
	sleep(1);

	if (ctx == NULL) {
		printf("Unable to allocate libmodbus context\n");
		return -1;
	}

	status = modbus_set_slave(ctx, 1);
	if (status == -1) {
		printf("Invalid slave ID\n");
		modbus_close(ctx);
		modbus_free(ctx);
		return -1;
	}

	if (modbus_connect(ctx) == -1){
		printf("Error : %s\n", modbus_strerror(errno));
		modbus_close(ctx);
		modbus_free(ctx);
		return -1;
	}

	while(1){
		/* ----------------- read coil ----------------- */
		status = modbus_read_bits(ctx, REG_COIL_START, REG_COIL_NREGS, us_coil_buf);
		if(status == -1){
			printf("Error : %s\n", modbus_strerror(errno));
			break;
		}
		printf("-----------------------\r\n");
		printf("Coil ");
		for(int i= REG_COIL_START;i< REG_COIL_NREGS; i++){
			printf("[%d] = %d ", i, us_coil_buf[i]);
		}
		printf("\r\n");
		sleep(1);
		/* ----------------- read descrete ----------------- */
		status = modbus_read_input_registers(ctx, REG_INPUT_START, REG_INPUT_NREGS, us_reg_input_buf);
		if(status == -1){
			printf("Error : %s\n", modbus_strerror(errno));
			break;
		}
		printf("-----------------------\r\n");
		printf("Input reg ");
		for(int i=REG_INPUT_START; i<REG_INPUT_NREGS; i++){
			printf("[%d] = %5d ", i, us_descretes_buf[i]);
		}
		printf("\r\n");
		sleep(1);
	}
	modbus_close(ctx);
	modbus_free(ctx);
	return 0;
}

// #include <stdio.h>
// #include <modbus/modbus.h>
// #include <errno.h>
// #include <unistd.h>

// int main(void){
// 	modbus_t *ctx;
// 	int rc_read = 0;
// 	int rc_write = 0;
// 	uint16_t tab_dest[64];
// 	uint16_t tab_src[10] = {1};

// 	// create context
// 	ctx = modbus_new_tcp("192.168.7.102", 502);
// 	// try connection
// 	while(modbus_connect(ctx) == -1){
// 		fprintf(stderr, "Connection failed : %s\n", modbus_strerror(errno));
// 	}

// 	// write to holding registers and read from input register
// 	while(1){
// 		for(int i=0;i<10;i++){
// 			tab_src[i] += i;
// 		}
// 		// write to holding registes
// 		rc_write = modbus_write_registers(ctx,0,10,tab_src);
// 		// read from input registers
// 		rc_read = modbus_read_input_registers(ctx,0,10,tab_dest);
// 		// check if read and write operation where successful
// 		if(rc_write == -1 || rc_read == -1){
// 			fprintf(stderr, "%s\n", modbus_strerror(errno));
// 			break;
// 		}
// 		uint8_t rc = 12;
// 		printf("write %d registers\n", rc);
// 		// print out read data
// 		for(int i=0;i<rc;i++){
// 			printf("reg[%d}=%d (0x%X)\n", i, tab_dest[i], tab_dest[i]);
// 		}
// 		printf("--------------------\n");
// 		sleep(2);
// 	}
// 	// free up resource and close connection
// 	printf("freeing up source\n");
// 	modbus_close(ctx);
// 	modbus_free(ctx);
// 	return 0;
// }
