#include "PNGTests.h"


void
PNGTests::testLoader() {
  static const string base = "images/pngsuite/";

  static const char* good[] = {
    "basi0g01.png",
    "basi0g02.png",
    "basi0g04.png",
    "basi0g08.png",
    "basi0g16.png",
    "basi2c08.png",
    "basi2c16.png",
    "basi3p01.png",
    "basi3p02.png",
    "basi3p04.png",
    "basi3p08.png",
    "basi4a08.png",
    "basi4a16.png",
    "basi6a08.png",
    "basi6a16.png",
    "basn0g01.png",
    "basn0g02.png",
    "basn0g04.png",
    "basn0g08.png",
    "basn0g16.png",
    "basn2c08.png",
    "basn2c16.png",
    "basn3p01.png",
    "basn3p02.png",
    "basn3p04.png",
    "basn3p08.png",
    "basn4a08.png",
    "basn4a16.png",
    "basn6a08.png",
    "basn6a16.png",
    "bgai4a08.png",
    "bgai4a16.png",
    "bgan6a08.png",
    "bgan6a16.png",
    "bgbn4a08.png",
    "bggn4a16.png",
    "bgwn6a08.png",
    "bgyn6a16.png",
    "ccwn2c08.png",
    "ccwn3p08.png",
    "cdfn2c08.png",
    "cdhn2c08.png",
    "cdsn2c08.png",
    "cdun2c08.png",
    "ch1n3p04.png",
    "ch2n3p08.png",
    "cm0n0g04.png",
    "cm7n0g04.png",
    "cm9n0g04.png",
    "cs3n2c16.png",
    "cs3n3p08.png",
    "cs5n2c08.png",
    "cs5n3p08.png",
    "cs8n2c08.png",
    "cs8n3p08.png",
    "ct0n0g04.png",
    "ct1n0g04.png",
    "ctzn0g04.png",
    "f00n0g08.png",
    "f00n2c08.png",
    "f01n0g08.png",
    "f01n2c08.png",
    "f02n0g08.png",
    "f02n2c08.png",
    "f03n0g08.png",
    "f03n2c08.png",
    "f04n0g08.png",
    "f04n2c08.png",
    "g03n0g16.png",
    "g03n2c08.png",
    "g03n3p04.png",
    "g04n0g16.png",
    "g04n2c08.png",
    "g04n3p04.png",
    "g05n0g16.png",
    "g05n2c08.png",
    "g05n3p04.png",
    "g07n0g16.png",
    "g07n2c08.png",
    "g07n3p04.png",
    "g10n0g16.png",
    "g10n2c08.png",
    "g10n3p04.png",
    "g25n0g16.png",
    "g25n2c08.png",
    "g25n3p04.png",
    "oi1n0g16.png",
    "oi1n2c16.png",
    "oi2n0g16.png",
    "oi2n2c16.png",
    "oi4n0g16.png",
    "oi4n2c16.png",
    "oi9n0g16.png",
    "oi9n2c16.png",
    "pngsuite_logo.png",
    "pp0n2c16.png",
    "pp0n6a08.png",
    "ps1n0g08.png",
    "ps1n2c16.png",
    "ps2n0g08.png",
    "ps2n2c16.png",
    "s01i3p01.png",
    "s01n3p01.png",
    "s02i3p01.png",
    "s02n3p01.png",
    "s03i3p01.png",
    "s03n3p01.png",
    "s04i3p01.png",
    "s04n3p01.png",
    "s05i3p02.png",
    "s05n3p02.png",
    "s06i3p02.png",
    "s06n3p02.png",
    "s07i3p02.png",
    "s07n3p02.png",
    "s08i3p02.png",
    "s08n3p02.png",
    "s09i3p02.png",
    "s09n3p02.png",
    "s32i3p04.png",
    "s32n3p04.png",
    "s33i3p04.png",
    "s33n3p04.png",
    "s34i3p04.png",
    "s34n3p04.png",
    "s35i3p04.png",
    "s35n3p04.png",
    "s36i3p04.png",
    "s36n3p04.png",
    "s37i3p04.png",
    "s37n3p04.png",
    "s38i3p04.png",
    "s38n3p04.png",
    "s39i3p04.png",
    "s39n3p04.png",
    "s40i3p04.png",
    "s40n3p04.png",
    "tbbn1g04.png",
    "tbbn2c16.png",
    "tbbn3p08.png",
    "tbgn2c16.png",
    "tbgn3p08.png",
    "tbrn2c08.png",
    "tbwn1g16.png",
    "tbwn3p08.png",
    "tbyn3p08.png",
    "tp0n1g08.png",
    "tp0n2c08.png",
    "tp0n3p08.png",
    "tp1n3p08.png",
    "z00n2c08.png",
    "z03n2c08.png",
    "z06n2c08.png",
    "z09n2c08.png",
  };
  static const int good_length = sizeof(good) / sizeof(*good);

  static const char* bad[] = {
    "x00n0g01.png",
    "xcrn0g04.png",
    "xlfn0g04.png",
  };
  static const int bad_length = sizeof(bad) / sizeof(*bad);

    
  for (int i = 0; i < good_length; ++i) {
    string fn = base + good[i];
    auto_ptr<Image> image(OpenImage(fn.c_str()));
    CPPUNIT_ASSERT_MESSAGE("should work - opening " + fn, image.get() != 0);
  }

  for (int i = 0; i < bad_length; ++i) {
    string fn = base + bad[i];
    auto_ptr<Image> image(OpenImage(fn.c_str()));
    CPPUNIT_ASSERT_MESSAGE("should fail - opening " + fn, image.get() == 0);
  }
}


void
setRandomBytes(byte* b, int size) {
  while (size--) {
    *b++ = byte(rand() % 256);
  }
}


void
PNGTests::testWriter() {
  static const int width  = 256;
  static const int height = 256;

  // create an image and fill it with random data
  auto_ptr<Image> image(CreateImage(width, height, PF_R8G8B8A8));
  setRandomBytes((byte*)image->getPixels(), width * height * 4);

  // generate filename
  char* filename = tmpnam(0);
  CPPUNIT_ASSERT_MESSAGE("opening temporary file", filename != 0);

  // save image
  CPPUNIT_ASSERT(SaveImage(filename, FF_PNG, image.get()) == true);

  // load it back
  auto_ptr<Image> img2(OpenImage(filename, PF_R8G8B8A8));
  CPPUNIT_ASSERT_MESSAGE("reloading image file", img2.get() != 0);

  AssertImagesEqual(
    "comparing saved with loaded",
    image.get(),
    img2.get());

  // force pixel format conversion (don't destroy the old image)
  auto_ptr<Image> img3(OpenImage(filename, PF_R8G8B8));
  CPPUNIT_ASSERT(SaveImage(filename, FF_PNG, img3.get()) == true);

  remove(filename);


  //== PALETTIZED SAVING TEST ==
  // disabled until OpenPNG can load palettized images in the right format
#if 0
  char* plt_filename = tmpnam(0);
  CPPUNIT_ASSERT_MESSAGE("opening temporary file (palette)", plt_filename != 0);

  auto_ptr<Image> plt(CreateImage(256, 256, PF_I8, 256, PF_R8G8B8));
  setRandomBytes((byte*)plt->getPixels(), 256 * 256);
  setRandomBytes((byte*)plt->getPalette(), 256);

  CPPUNIT_ASSERT(SaveImage(plt_filename, FF_PNG, plt.get()) == true);

  auto_ptr<Image> plt2(OpenImage(plt_filename, FF_PNG));
  CPPUNIT_ASSERT_MESSAGE("reloading palettized image", plt2.get() != 0);
  CPPUNIT_ASSERT(plt2->getPaletteSize() == 256);
  CPPUNIT_ASSERT(plt2->getPaletteFormat() == PF_R8G8B8);
  CPPUNIT_ASSERT(plt2->getFormat() == PF_I8);
  AssertImagesEqual("Comparing palettized image", plt.get(), plt2.get());

  remove(plt_filename);
#endif

  // remove the temporary files we made
}


Test*
PNGTests::suite() {
  typedef TestCaller<PNGTests> Caller;

  TestSuite* suite = new TestSuite();
  suite->addTest(new Caller("Test PNG Loader", &PNGTests::testLoader));
  suite->addTest(new Caller("Test PNG Writer", &PNGTests::testWriter));
  return suite;
}
