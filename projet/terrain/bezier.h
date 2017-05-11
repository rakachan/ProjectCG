#pragma once
#include "icg_helper.h"
#include "glm/glm.hpp"
#include <vector>
using namespace std;
using namespace glm;

class Bezier {
private:
    vector<vec3> points;

public:
    void Init(vector<vec3> array) {
        for(size_t i = 0; i<array.size(); i++){
            points.push_back(array.at(i));
        }
    }

    vec3 getPos(float t) {
        return drawCurve(points, t);
    }

    vec3 drawCurve(vector<vec3> array, float t){
      if(array.size()==1){
        return (array.back());
      } else {
        vector<vec3> newpoints;
        for(unsigned int i=0; i<array.size()-1; i++){
          newpoints.push_back((1-t) * array.at(i) + t * array.at(i+1));
        }
        return drawCurve(newpoints, t);
      }
    }
};
