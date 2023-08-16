#include <stdio.h>
#include <math.h>

#include <MLX90640_I2C_Driver.h>
#include <MLX90640_API.h>

#define MLX_I2C_ADDR 0x33
#define EEPROM_ENTRY_COUNT 832

#define PIX_COUNT MLX90640_PIXEL_NUM

void dump_to_file(uint16_t *bin, int count, const char* filename);
void dump_frame(const char* filename, paramsMLX90640 *m);
void read_eeprom(paramsMLX90640 *m);
void float_out(float* arr, int size); 
void just_read_frame(paramsMLX90640 *m);

int main(int argc, char* argv[]) {
  printf("Hello frames\n");
  paramsMLX90640 m;
  read_eeprom(&m);
  
  just_read_frame(&m);
  dump_frame("all_frame_a.bin", &m);
  dump_frame("all_frame_b.bin", &m);
}

void just_read_frame(paramsMLX90640 *m) {
  uint16_t frame[834];
  float to[PIX_COUNT];

  int subpage = MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
  float eta = MLX90640_GetTa(frame, m);
  MLX90640_CalculateTo(frame, m, 1, eta, to);

}

void dump_frame(const char *filename, paramsMLX90640 *m) {
  uint16_t frame[834];
  float to[PIX_COUNT];

  int subpage = MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
  printf("%x %x %x %x\n", frame[0], frame[1], frame[2], frame[3]);
  printf("%x %x %x %x\n", frame[830], frame[831], frame[832], frame[833]);
  printf("\n\n");
  float eta = MLX90640_GetTa(frame, m);
  MLX90640_CalculateTo(frame, m, 1, eta, to);

  printf("\nSubpage:  %d\n\n", subpage);
  float_out(to, PIX_COUNT);
  printf("\n\nETA: %f\n", eta);
  
  dump_to_file(frame, 834, filename);
}


void dump_to_file(uint16_t *bin, int count, const char* filename){
  FILE *out_file = fopen(filename, "wb");
  fwrite(bin, sizeof(bin[0]), count, out_file);
  fclose(out_file);
}

void read_eeprom(paramsMLX90640 *m) {
  static uint16_t eeMLX90640[EEPROM_ENTRY_COUNT];
  MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640); 
  MLX90640_ExtractParameters(eeMLX90640, m);
}

void float_out(float* arr, int size) {
  const char *sep = "[\n";
  for(int i=0; i < size; i++){
    printf(sep);
    printf("%F", arr[i]);
    sep = ",";
  }
  puts("\n]");
}
