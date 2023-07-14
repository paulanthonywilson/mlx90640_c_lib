



hello: functions/hello.cpp functions/MLX90640_LINUX_I2C_Driver.cpp functions/MLX90640_API.cpp 
	gcc -o hello functions/hello.cpp functions/MLX90640_LINUX_I2C_Driver.cpp functions/MLX90640_API.cpp -I ./headers/ -lstdc++ -lm
