#include <stdio.h>

#include <MLX90640_I2C_Driver.h>
#include <MLX90640_API.h>

#define MLX_I2C_ADDR 0x33
#define EEPROM_ENTRY_COUNT 832

#define PIX_COUNT MLX90640_PIXEL_NUM

void dump_to_file(uint16_t *bin, int count, const char* filename);
void dump_frame(const char* filename);

int main(int argc, char* argv[]) {
  printf("Hello frames\n");
  dump_frame("all_frame.bin");
}

void dump_frame(const char *filename) {
  uint16_t frame[834];
  MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
  printf("%x %x %x %x\n", frame[0], frame[1], frame[2], frame[3]);
  printf("%x %x %x %x\n", frame[830], frame[831], frame[832], frame[833]);
  dump_to_file(frame, 834, filename);
}


void dump_to_file(uint16_t *bin, int count, const char* filename){
  FILE *out_file = fopen(filename, "wb");
  /*for(int i = 0; i < count; i++) {
    fwrite(&bin[i], sizeof(bin[i]), 1, out_file);
  }*/
  fwrite(bin, sizeof(bin[0]), count, out_file);
  fclose(out_file);
}
