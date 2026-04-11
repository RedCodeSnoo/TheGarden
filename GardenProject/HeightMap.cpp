#include "HeightMap.hpp"
#include <iostream>

HeightMap::HeightMap() : m_width(0), m_height(0), m_isLoaded(false) {}

void HeightMap::skipComments(std::ifstream& file) {
    char ch;
    while (file.get(ch) && (isspace(ch) || ch == '#')) {
        if (ch == '#') {
            file.ignore(1000, '\n');
        }
    }
    file.unget();
}

bool HeightMap::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[HeightMap] Impossible d'ouvrir : " << filename << std::endl;
        return false;
    }

    std::string magic;
    file >> magic;
    if (magic != "P5") {
        std::cerr << "[HeightMap] Format non supporte (P5 uniquement)" << std::endl;
        return false;
    }

    skipComments(file);
    file >> m_width;
    
    skipComments(file);
    file >> m_height;
    
    skipComments(file);
    int maxVal;
    file >> maxVal;

    file.ignore(1, '\n');

    if (m_width > 0 && m_height > 0) {
        m_data.resize(m_width * m_height);
        file.read((char*)(m_data.data()), m_data.size());
        m_isLoaded = true;
        
        m_minPixel = 0.0f;
        bool first_found = false;
        
        for (auto val : m_data) {
            if (val > 0) {
                if (!first_found) {
                    m_minPixel = val;
                    first_found = true;
                } else if (val < m_minPixel) {
                    m_minPixel = val;
                }
            }
        }

        return true;
    }

    return false;
}

float HeightMap::getPixel(int i, int j) const {
    auto idx = j * m_width + i;
    auto value = m_data[idx];

    if (value > 0) {
        return value;
    }

    auto sum = 0.0f;
    auto count = 0;

    // on verifie les 4 voisins
    // gauche
    if (i > 0 && m_data[idx - 1] > 0) {
        sum += m_data[idx - 1];
        count++;
    }
    // droite
    if (i < m_width - 1 && m_data[idx + 1] > 0) {
        sum += m_data[idx + 1];
        count++;
    }
    // haut
    if (j > 0 && m_data[idx - m_width] > 0) {
        sum += m_data[idx - m_width];
        count++;
    }
    // bas
    if (j < m_height - 1 && m_data[idx + m_width] > 0) {
        sum += m_data[idx + m_width];
        count++;
    }

    if (count > 0) {
        return sum / (float)count;
    }
    
    return 0.0f;
}


// Vérifie si un pixel spécifique correspond à l'emplacement d'un arbre (valeur 0)
bool HeightMap::isTreeLocation(int i, int j) const {
    if (!m_isLoaded) return false;
    auto idx = j * m_width + i;
    return m_data[idx] == 0;
}