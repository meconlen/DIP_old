// tiff_interface.hpp

#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "tiffio.h"

class tiff
{
private:
   // these are fixed to 32 bit by the interface
   // files are limited to 4 GB 
   uint32_t rows, columns;
   uint16_t bits_per_sample, samples_per_pixel, extra_samples, photometric;
   uint16_t* samples_info;
   bool has_alpha;
   // BGRA 16 bit packed into 32 bit integers 
   uint64_t* raster;
public:
   tiff();
   tiff(const std::string& filename);
   void load(const std::string& filename);
   // caller owns the return value
   uint32_t* get_raster() const;
   uint32_t get_row_count() const;
   uint32_t get_column_count() const;
   uint32_t get_pixel_count() const;
   // 2.2(a)
   uint32_t get_pixel_value(uint32_t row, uint32_t column);
   ~tiff();
private:

};

uint32_t rgb2gray(uint32_t rgb);
