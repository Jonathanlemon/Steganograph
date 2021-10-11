DEPS= ppm.h ppm.c EncodeDecode.h EncodeDecode.c utils.h utils.c driver.c

run: $(DEPS)
	@gcc $(DEPS)
	@./a.out input.ppm output.ppm
