#include <iostream>

#include <gtest/gtest.h>

#include "tiff_interface.hpp"

// problem 2-2(b)
TEST(tiff_interface, load)
{
   tiff image("../images/girl.tif");
   uint32_t* raster = image.get_raster();
   ASSERT_NE(raster, nullptr);

   // origin in ARGB
   uint32_t origin = image.get_pixel_value(0,0);
   uint32_t gray = rgb2gray(origin);
   EXPECT_EQ(gray, 128);
   uint32_t rows = image.get_row_count();
   uint32_t columns = image.get_column_count();
   uint32_t center = image.get_pixel_value(rows/2, columns/2);
   gray = rgb2gray(center);
   EXPECT_EQ(gray, 111);
   free(raster);
	return;
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}