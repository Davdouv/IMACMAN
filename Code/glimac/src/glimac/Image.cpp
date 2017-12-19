#include "glimac/Image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

namespace glimac {

std::unique_ptr<Image> loadImage(const FilePath& filepath) {
    int x, y, n;
    unsigned char *data = stbi_load(filepath.c_str(), &x, &y, &n, 4);
    if(!data) {
        std::cerr << "loading image " << filepath << " error: " << stbi_failure_reason() << std::endl;
        return std::unique_ptr<Image>();
    }
    std::unique_ptr<Image> pImage(new Image(x, y));
    unsigned int size = x * y;
    auto scale = 1.f / 255;
    auto ptr = pImage->getPixels();
    for(auto i = 0u; i < size; ++i) {
        auto offset = 4 * i;
        ptr->r = data[offset] * scale;
        ptr->g = data[offset + 1] * scale;
        ptr->b = data[offset + 2] * scale;
        ptr->a = data[offset + 3] * scale;
        ++ptr;
    }
    stbi_image_free(data);
    return pImage;
}

std::unordered_map<FilePath, std::unique_ptr<Image>> ImageManager::m_ImageMap;

const Image* ImageManager::loadImage(const FilePath& filepath) {
    auto it = m_ImageMap.find(filepath);
    if(it != std::end(m_ImageMap)) {
        return (*it).second.get();
    }
    auto pImage = glimac::loadImage(filepath);
    if(!pImage) {
        return nullptr;
    }
    auto& img = m_ImageMap[filepath] = std::move(pImage);
    return img.get();
}

}
