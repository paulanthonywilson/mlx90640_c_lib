#include <stdio.h>

#include <MLX90640_I2C_Driver.h>
#include <MLX90640_API.h>

#define MLX_I2C_ADDR 0x33
#define EEPROM_ENTRY_COUNT 832

void dump_eeprom_to_file(uint16_t *eeprom);

int main(int argc, char *argv[]) {
  static uint16_t eeMLX90640[EEPROM_ENTRY_COUNT];
  paramsMLX90640 m;

  MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640); 
  MLX90640_ExtractParameters(eeMLX90640, &m);
  dump_eeprom_to_file(eeMLX90640);

  puts("VDD");
  printf("kVdd: %d, vdd25 %d", m.kVdd, m.vdd25);
  puts("");
  puts("\nCP params");
  printf("cpKv: %f, cpKta: %e\n", m.cpKv, m.cpKta);
  printf("cpAlpha[0]: %e, cpAlpha[1]: %e\n", m.cpAlpha[0], m.cpAlpha[1]);
  printf("offsetSP[0]: %d, offsetSP[1]: %d\n", m.cpOffset[0], m.cpOffset[1]); 
  puts("\n\n");

  puts("Alpha");
  printf("Alpha scale: %d\n\n", m.alphaScale); 
  const char *sep = "Alphas:\n[\n";
  for(int i = 0; i < MLX90640_PIXEL_NUM; i++) {
    printf(sep);
    printf("%d", m.alpha[i]);
    sep = ",";
  }
  puts("\n]");


  return 0;
}

void dump_eeprom_to_file(uint16_t *eeprom){
  uint16_t reverse_end;
  FILE *out_file = fopen("eeprom.bin", "wb");
  for(int i = 0; i < EEPROM_ENTRY_COUNT; i++) {
    // Need to reverse endiannes for the file to match what comes from
    // the sensor over i2c
    reverse_end = (eeprom[i] >> 8) + (eeprom[i] & 0xFF) * 256;
    fwrite(&reverse_end, sizeof(reverse_end), 1, out_file);
  }
  fclose(out_file);
}
