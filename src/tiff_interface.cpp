// tiff_interface.cpp

#include "tiff_interface.hpp"

tiff::tiff() : 
   rows(0), columns(0), bits_per_sample(0), samples_per_pixel(0), extra_samples(0), photometric(0), 
   samples_info(nullptr), has_alpha(false), raster(nullptr)
{
   return;
}

tiff::tiff(const std::string& filename) :
   tiff()
{
   load(filename);
}

void tiff::load(const std::string& filename)
{
   TIFF* tif = TIFFOpen(filename.c_str(), "r");
   if(tif == NULL) {
      raster = nullptr;
      return;
   }
   TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &columns);
   TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &rows);
   raster = (uint32_t*) _TIFFmalloc(rows * columns * sizeof (uint32_t));
   if(raster == nullptr) goto close;
   TIFFReadRGBAImageOriented(tif, columns, rows, raster, ORIENTATION_TOPLEFT);
close:
   TIFFClose(tif);
}

uint32_t* tiff::get_raster() const
{
   if(raster == nullptr) {
      return nullptr;
   }
   uint32_t *rv = (uint32_t*)malloc(rows * columns * sizeof(uint32_t));
   memcpy(rv, raster, rows * columns * sizeof(uint32_t));
   return rv;
}

uint32_t tiff::get_row_count() const
{
   return rows;
}

uint32_t tiff::get_column_count() const
{
   return columns;
}

uint32_t tiff::get_pixel_count() const
{
   return rows * columns;
}

uint32_t tiff::get_pixel_value(uint32_t row, uint32_t column)
{
   uint32_t pos = row * columns + column;
   return raster[pos];
}


tiff::~tiff()
{
   if(raster != nullptr) {
      _TIFFfree(raster);
   }
}

uint32_t rgb2gray(uint32_t rgb)
{
   uint32_t rv;
   double red = static_cast<double>(TIFFGetR(rgb));
   double green = static_cast<double>(TIFFGetG(rgb));
   double blue = static_cast<double>(TIFFGetB(rgb));
https://en.wikipedia.org/wiki/Grayscale
   double gray = std::round(0.2126 * red + 0.7152 * green + 0.0722 * blue);
   rv = static_cast<uint32_t>(gray);
   return rv;
}