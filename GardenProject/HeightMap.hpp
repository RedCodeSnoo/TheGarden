#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <string>
#include <vector>
#include <fstream>

class HeightMap {

public:
    HeightMap();
    bool load(const std::string& filename);
    
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    float getPixel(int i, int j) const;

private:
    int m_width;
    int m_height;
    std::vector<unsigned char> m_data;
    bool m_isLoaded;

    void skipComments(std::ifstream& file);

};

#endif