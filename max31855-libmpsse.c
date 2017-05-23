#include <stdio.h>
#include <inttypes.h>
#include <mpsse.h>
#include <endian.h>

int main(void)
{
	int retval = EXIT_FAILURE;
	struct mpsse_context *ctx = NULL;
	char *data = NULL;

	if((ctx = MPSSE(SPI0, 10000, MSB)) != NULL && ctx->open) {
		Start(ctx);
		data = Read(ctx, 4);
		Stop(ctx);
		if (data) {
#ifdef DEBUG
			printf("%02hhx%02hhx%02hhx%02hhx\n", data[0], data[1], data[2], data[3]);
#endif
			uint32_t idata = be32toh(*((uint32_t *)data));
#ifdef DEBUG
			printf("%08" PRIx32 "\n", idata);
			uint32_t shifty = idata;
			for (int i = 32; i > 0; i--) {
				if (i % 8 == 0) putchar(' ');
				if (shifty & 0x80000000) printf("1");
				else printf("0");
				shifty <<= 1;
			}
			printf("\n");
#endif
			if (idata & (1 << 0)) fprintf(stderr, "open circuit\n");
			if (idata & (1 << 1)) fprintf(stderr, "short to GND\n");
			if (idata & (1 << 2)) fprintf(stderr, "short to Vcc\n");
			if (idata & (1 << 16)) fprintf(stderr, "fault\n");
			uint16_t itemp = *((uint16_t *)(&idata)) & 0xfff0;
#ifdef DEBUG
			printf("%04x\n", itemp);
#endif
			printf("%.4f\n", 0.0625 / 16 * (int16_t)itemp);
			uint16_t otemp = idata >> 16 & 0xfffc;
#ifdef DEBUG
			printf("%04x\n", otemp);
#endif
			printf("%.2f\n", 0.25 * (int16_t)otemp / 4);
			free(data);
			retval = EXIT_SUCCESS;
		} else {
			fprintf(stderr, "Failed to read from the chip.\n");
		}
	} else {
		fprintf(stderr, "Failed to initialize MPSSE: %s\n", ErrorString(ctx));
	}

	Close(ctx);

	return retval;
}
