#include <stdio.h>

#include <MLX90640_I2C_Driver.h>
#include <MLX90640_API.h>

#define MLX_I2C_ADDR 0x33
#define EEPROM_ENTRY_COUNT 832

#define PIX_COUNT MLX90640_PIXEL_NUM

void dump_eeprom_to_file(uint16_t *eeprom);
void output_vdd(paramsMLX90640 *m);
void output_cp(paramsMLX90640 *m);
void output_alpha(paramsMLX90640 *m);
void output_offsets(paramsMLX90640 *m);
void output_ktas(paramsMLX90640 *m);
void output_kvs(paramsMLX90640 *m);
void output_cilc(paramsMLX90640 *m);
void output_dodgy_pixels(paramsMLX90640 *m);
void output_resolution(paramsMLX90640 *m);



void uint_array_out(uint16_t* arr, int size);
void int_array_out(int16_t* arr, int size);
void int8_array_out(int8_t* arr, int size);

int main(int argc, char *argv[]) {
  static uint16_t eeMLX90640[EEPROM_ENTRY_COUNT];
  paramsMLX90640 m;

  MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640); 
  MLX90640_ExtractParameters(eeMLX90640, &m);
  dump_eeprom_to_file(eeMLX90640);

  output_vdd(&m);
  output_cp(&m);
  output_alpha(&m);
  output_offsets(&m);
  output_ktas(&m);
  output_kvs(&m);
  output_cilc(&m);
  output_dodgy_pixels(&m);
  output_resolution(&m);

  return 0;
}


void output_vdd(paramsMLX90640 *m) {
  puts("VDD");
  printf("kVdd: %d, vdd25 %d", m->kVdd, m->vdd25);
  puts("");
}



void output_cp(paramsMLX90640 *m) {
  puts("\nCP params");
  printf("cpKv: %f, cpKta: %e\n", m->cpKv, m->cpKta);
  printf("cpAlpha[0]: %e, cpAlpha[1]: %e\n", m->cpAlpha[0], m->cpAlpha[1]);
  printf("offsetSP[0]: %d, offsetSP[1]: %d\n", m->cpOffset[0], m->cpOffset[1]); 
  puts("\n\n");
}



void output_alpha(paramsMLX90640 *m){
  puts("Alpha");
  printf("Alpha scale: %d\n\n", m->alphaScale); 
  const char *sep = "Alphas:\n";
  uint_array_out(m->alpha, PIX_COUNT);
}


void output_offsets(paramsMLX90640 *m){
  puts("\nOffsets:");
  int_array_out(m->offset, PIX_COUNT);
  
}


void output_ktas(paramsMLX90640 *m) {
  puts("\nKTas:");
  printf("ktaScale: %d\n\n", m->ktaScale);
  int8_array_out(m->kta, PIX_COUNT);
}

void output_cilc(paramsMLX90640 *m) {
  puts("\nCILC:");
  printf("CalibrationMode EE: %d\n", m->calibrationModeEE);
  printf("ilChess: ");
  for(int i = 0; i < 3; i++) {
    printf("%f ", m->ilChessC[i]);
  }	  
  puts("\n");
}


void output_kvs(paramsMLX90640 *m){
  puts("\nKvs:");
  printf("kvScale: %d\n\n", m->kvScale);
  int8_array_out(m->kv, PIX_COUNT);
}

void output_dodgy_pixels(paramsMLX90640 *m) {
  puts("\nBroken:");
  for(int i = 0; i < 5; i++){
   printf("%d: %d\n", i, m->brokenPixels[i]);
  }
  puts("\nOutlier::");
  for(int i = 0; i < 5; i++){
   printf("%d: %d\n", i, m->outlierPixels[i]);
  }
}

void output_resolution(paramsMLX90640 *m){
  puts("\nResolution:");
  printf("resolutionEE: %d\n\n", m->resolutionEE);
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


void uint_array_out(uint16_t* arr, int size){
  const char *sep = "[\n";
  for(int i=0; i < size; i++){
    printf(sep);
    printf("%d", arr[i]);
    sep = ",";
  }
  puts("\n]");
}

void int_array_out(int16_t* arr, int size){
  const char *sep = "[\n";
  for(int i=0; i < size; i++){
    printf(sep);
    printf("%d", arr[i]);
    sep = ",";
  }
  puts("\n]");
}

void int8_array_out(int8_t* arr, int size) {
  const char *sep = "[\n";
  for(int i=0; i < size; i++){
    printf(sep);
    printf("%d", arr[i]);
    sep = ",";
  }
  puts("\n]");
}
