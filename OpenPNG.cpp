#include <png.h>
#include "Debug.h"
#include "Open.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  typedef unsigned char byte;


  //////////////////////////////////////////////////////////////////////////////

  void PNG_read_function(png_structp png_ptr,
                         png_bytep data,
                         png_size_t length) {
    File* file = (File*)png_get_io_ptr(png_ptr);
    if (file->read(data, length) != int(length)) {
      png_error(png_ptr, "Read error");
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  void fill_palette(png_structp png, png_infop info, png_color palette[256]) {

   COR_GUARD("fill_palette");

    // by default, the palette is greyscale
    for (int i = 0; i < 256; ++i) {
      palette[i].red   = i;
      palette[i].green = i;
      palette[i].blue  = i;
    }

    // do we have a palette and is it big enough?
    png_colorp png_palette;
    int num_palette = 0;
    png_get_PLTE(png, info, &png_palette, &num_palette);

    COR_IF_DEBUG {
      char str[80];
      sprintf(str, "palette size: %d", num_palette);
      COR_LOG(str);
    }

    if (num_palette >= 256) {

      COR_IF_DEBUG {
        for (int i = 0; i < 256; ++i) {
          char str[80];
          sprintf(str, "r(%d) g(%d) b(%d)",
            int(palette[i].red),
            int(palette[i].green),
            int(palette[i].blue));
          COR_LOG(str);
        }
      }

      memcpy(palette, png_palette, 256 * sizeof(png_color));
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  Image* OpenPNG(File* file) {

    COR_GUARD("OpenPNG");

    // verify PNG signature
    byte sig[8];
    file->read(sig, 8);
    if (png_sig_cmp(sig, 0, 8)) {
      return 0;
    }

    COR_LOG("Signature verified");

    // read struct
    png_structp png_ptr = png_create_read_struct(
      PNG_LIBPNG_VER_STRING,
      NULL, NULL, NULL);
    if (!png_ptr) {
      return 0;
    }

    COR_LOG("png struct created");

    // info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return 0;
    }

    COR_LOG("info struct created");

    // the PNG error function calls longjmp(png_ptr->jmpbuf)
    if (setjmp(png_jmpbuf(png_ptr))) {
      COR_LOG("Error loading PNG");
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return 0;
    }

    COR_LOG("setjmp() succeeded");

    // read the image
    png_set_read_fn(png_ptr, file, PNG_read_function);
    png_set_sig_bytes(png_ptr, 8);  // we already read 8 bytes for the sig
    // always give us 8-bit samples (strip 16-bit and expand <8-bit)
    int png_transform = PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_EXPAND;
    png_read_png(png_ptr, info_ptr, png_transform, NULL);

    COR_LOG("PNG read");

    if (!png_get_rows(png_ptr, info_ptr)) {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return 0;
    }

    int width  = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    byte* pixels = 0;  // allocate when we know the format
    PixelFormat format;

    // decode based on pixel format
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int num_channels = png_get_channels(png_ptr, info_ptr);
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    // 32-bit RGBA
    if (bit_depth == 8 && num_channels == 4) {

      format = PF_R8G8B8A8;
      pixels = new byte[width * height * 4];
      for (int i = 0; i < height; ++i) {
        memcpy(pixels + i * width * 4, row_pointers[i], width * 4);
      }

    // 24-bit RGB
    } else if (bit_depth == 8 && num_channels == 3) {

      format = PF_R8G8B8;
      pixels = new byte[width * height * 3];
      for (int i = 0; i < height; ++i) {
        memcpy(pixels + i * width * 3, row_pointers[i], width * 3);
      }

    // palettized or greyscale with alpha
    } else if (bit_depth == 8 && (num_channels == 2 || num_channels == 1)) {

      format = PF_R8G8B8A8;
      pixels = new byte[width * height * 4];
      byte* out = pixels;

      png_color palette[256];
      fill_palette(png_ptr, info_ptr, palette);

      if (num_channels == 2) {

        for (int i = 0; i < height; ++i) {
          byte* in = row_pointers[i];
          for (int j = 0; j < width; ++j) {
            byte c = *in++;
            *out++ = palette[c].red;
            *out++ = palette[c].green;
            *out++ = palette[c].blue;
            *out++ = *in++;  // alpha
          }
        }

      } else { // (num_channels == 1)

        byte alpha[256];
        memset(alpha, 255, 256);  // by default, everything is opaque

        // get the transparent palette flags
        png_bytep trans;
        int num_trans = 0;
        png_color_16p trans_values; // XXX not used - should be?
        png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);

        // update the alpha table
        for (int i = 0; i < num_trans; ++i) {
          alpha[trans[i]] = 0;
        }

        for (int i = 0; i < height; ++i) {
          byte* row = row_pointers[i];
          for (int j = 0; j < width; j++) {
            byte c = *row++;
            *out++ = palette[c].red;
            *out++ = palette[c].green;
            *out++ = palette[c].blue;
            *out++ = alpha[c];

            COR_IF_DEBUG {
              char str[100];
              sprintf(str, "%d %d %d %d %d",
                int(palette[c].red),
                int(palette[c].green),
                int(palette[c].blue),
                int(alpha[c]),
                int(c));
              COR_LOG(str);
            }
          }
        }
      }

    } else {  // unknown format
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return 0;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return new SimpleImage(width, height, format, pixels);
  }

  //////////////////////////////////////////////////////////////////////////////

}
