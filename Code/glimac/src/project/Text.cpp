#include "project/Text.hpp"

void Text::init_resources() {
  m_fontfilename = "../Code/assets/fonts/DejaVuSans.ttf";

	/* Initialize the FreeType2 library */
	if (FT_Init_FreeType(&m_ft)) {
		std::cout << "Could not init freetype library" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* Load a font */
	if (FT_New_Face(m_ft, m_fontfilename.data(), 0, &m_face)) {
		std::cout << "Could not open font" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Create the vertex buffer object
	glGenBuffers(1, &m_vbo);
}
