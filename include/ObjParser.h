//
// Created by shoham on 8/14/22.
//

#ifndef RENDERER_OBJPARSER_H
#define RENDERER_OBJPARSER_H

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>


struct face{
    std::vector<unsigned int> vertices;
    explicit face(std::vector<unsigned int>  fac) : vertices(std::move(fac)){
    }
};

Geometries parse(const std::string& fname) {
    srand( (unsigned)time( NULL ) );
    Geometries mesh{};
    std::vector<Point> points;
    std::vector<face> faces;
    std::string s;
    std::ifstream fin(fname);

    if(!fin) {std::cout<<"Error"; return mesh;}

    while(std::getline(fin, s))
    {
        if(!s.empty()) {
            if (s.at(0) == 'v' && s.at(1) == ' ') {
                size_t pos = 0;
                s = s.substr(1, s.length());
                std::vector<double> ver;
                while ((pos = s.find(' ')) != std::string::npos) {
                    if (s.substr(0, pos).length()) {

                        if (s.substr(0, pos).find(' ') != std::string::npos) s.erase(0, 1);
                        else {
                            ver.push_back(std::stod(s.substr(0, pos)));
                            s.erase(0, pos + 1);
                        }
                    } else { s = s.substr(1, s.length() - 1); }
                }
                points.emplace_back(ver.at(0), ver.at(1), std::stod(s));
            }
            else if (s.at(0) == 'f') {
                size_t pos = 0;
                s = s.substr(1, s.length());
                std::vector<unsigned int> fac;
                while ((pos = s.find(' ')) != std::string::npos) {
                    if (s.substr(0, pos).length()) {

                        if (s.substr(0, pos).find(' ') != std::string::npos) s.erase(0, 1);
                        else {
                            fac.push_back(std::stoi(s.substr(0, pos)));
                            s.erase(0, pos + 1);
                        }
                    } else { s = s.substr(1, s.length() - 1); }
                }
                faces.emplace_back(fac);
            }
        }
    }
    Point poly[4];
    int index=0;
    for (const auto& face: faces){
        for(auto ver: face.vertices){
            poly[index%4] = points[ver-1];
            index++;
            if (index%4==0) mesh.addShared(std::make_shared<Polygon>(
                    Polygon(poly[0], poly[1], poly[2], poly[3])
                    .setMaterial(Material().setDiffusive(0.5).setSpecular(0.5).setShininess(30))
                    .setEmission(Color::green())));
                    //.setEmission(Color(rand()/double_t(RAND_MAX) + 0.5,rand()/double_t(RAND_MAX) + 0.5,rand()/double_t(RAND_MAX) + 0.5))));
        }
    }
    poly[3] = points[points.size()-1];
    mesh.addShared(std::make_shared<Polygon>(
            Polygon(poly[0], poly[1], poly[2], poly[3])
                    .setMaterial(Material().setDiffusive(0.5).setSpecular(0.5).setShininess(30))
                    .setEmission(Color::green())));
    return mesh;
}


#endif //RENDERER_OBJPARSER_H
