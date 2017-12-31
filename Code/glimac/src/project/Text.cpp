#include <ft2build.h>
#include <iostream>
#include FT_FREETYPE_H


void initFreetype2()
{
  FT_Library  library;

  int error = FT_Init_FreeType( &library );

  if (error == 0)
  {
    std::cout << "erreur" << std::endl;
  }
}
