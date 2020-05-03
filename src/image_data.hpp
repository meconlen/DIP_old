// image_data.hpp

// represent bitmap or image data in a form that's useful for computation 
// the idea is to first define a underlying type
// uint8_t, uint16_t, uint32_t, uint64_t (probably not), float and double would be typical
// std::complex<float> and std::complex<double> might be used to represent transforms of 
// images of the above types 

// then we define a number of channels
// 1 -> grayscale
// 3 -> rgb
// 4 -> cmyk 
// 4 -> rgb+alpha
// 5 -> cmyk+alpha

// then a size (rows, columns) that might be resizable or do we want std::array in which case a resize produces a new image 

// if all of these parameters are going to be types then we want a base class which we can use as a pointer type 


#include <cstdint>

class dip_image {
   virtual ~dip_image();
};

template<typename T, uint8_t L, uint8_t R, uint8_t C>
class image : virtual dip_image
{
private:
   std::array< std::array< std::array<T, C>, R>, L> image_data;
};