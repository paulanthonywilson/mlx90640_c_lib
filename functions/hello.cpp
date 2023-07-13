#include <stdio.h>

#include <MLX90640_I2C_Driver.h>
#include <MLX90640_API.h>

#define MLX_I2C_ADDR 0x33

int main(int argc, char *argv[]) {
  static uint16_t eeMLX90640[832];
  paramsMLX90640 m;

  MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640); 
  MLX90640_ExtractParameters(eeMLX90640, &m);


  puts("VDD");
  printf("kVdd: %d, vdd25 %d", m.kVdd, m.vdd25);
  puts("");
  puts("\nCPP params");
  printf("cpKv: %f, cpAlpha[0]: %e, cpAlpha[1]: %e\n", m.cpKv, m.cpAlpha[0], m.cpAlpha[1]);
  printf("offsetSP[0]: %d, offsetSP[1]: %d\n", m.cpOffset[0], m.cpOffset[1]); 
  puts("");
  return 0;
}
