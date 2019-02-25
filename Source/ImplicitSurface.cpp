////
////  ImplicitSurface.cpp
////  CSE_168
////
////  Created by Clayton Anderson on 4/11/17.
////  Copyright © 2017 Clayton Anderson. All rights reserved.
////
//
//#include "ImplicitSurface.hpp"
//
//void ImplicitSurface::SetFandG()
//{
//    
//    auto tempFun = [this](float x, float y, float z)
//    {
//        float val = 0;
//        for (int i = 0; i < 10; i++)
//        {
//            val += (sin(this->freqs[i] * x) + sin(this->freqs[i] * y) + sin(this->freqs[i] * z));
//        }
//        return val - pow((fabs(z)),1.4f);
//    };
//    
//    auto tempFun2 = [](float x, float y, float z) {return 1.0f;};
//    
//    this->fun = tempFun2;
//    
//    this->SetF([](float x, float y, float z) {
//        return pow(x*x+2.25f*z*z+(y-2)*(y-2)-1,3.0f) -(x*x + 0.1125f*z*z)*(y-2)*(y-2)*(y-2); });
//    this->SetGrad( [](float x, float y, float z) {
//        return std::make_tuple(6.0f*x*pow(x*x+2.25f*z*z+(y-2)*(y-2)-1,2.0f)-2.0f*x*(y-2)*(y-2)*(y-2),
//                               13.5f*z*pow(x*x+2.25f*z*z+(y-2)*(y-2)-1,2.0f)-0.225f*z*(y-2)*(y-2)*(y-2),
//                               6.0f*(y-2)*pow(x*x+2.25f*z*z+(y-2)*(y-2)-1,2.0f)-(3.0f*x*x+0.3375f*z*z)*(y-2)*(y-2)); });
//}
//
//bool ImplicitSurface::Intersect(const Ray &ray, Intersection &hit)
//{
//    vec3 p = ray.Origin;
//    vec3 d = ray.Direction;
//    
//    //    p = vec3(0,0,0);
//    //    d = vec3(1,1,1);
//    //    SHOWVEC(d);
//    
//    // Evaluation of the function along the ray (g:R->R)
//    auto g = [p, d, this](float t) { return fun(p.x + t * d.x, p.y + t * d.y, p.z + t * d.z); };
//    
//    // Should be g'[t]:R->R given by dot(GradF(l(t)), l'[t]) where l[t] is the ray
//    //    auto gGrad = [p, d, this](float t) {
//    //        vec3 point = vec3(p.x+t*d.x, p.y+t*d.y, p.z+t*d.z);
//    //        float value = std::get<0>(grad(point.x, point.y, point.z)) * d.x + \
//    //        std::get<1>(grad(point.x, point.y, point.z)) * d.y + \
//    //        std::get<2>(grad(point.x, point.y, point.z)) * d.z;
//    //        return value;
//    //    };
//    //
//    //    auto F = [g, gGrad](float t) {return std::make_tuple(g(t),gGrad(t));};
//    
//    boost::uintmax_t maxIter = 25;
//    float minGuess = 5.0f;
//    float maxGuess = 12.5f;
//    //    float initialGuess = 0.1f;
//    
//    int numDivisions = 5000;
//    
//    std::vector<float> functionVals(numDivisions, 0);
//    std::vector<float> vals(numDivisions, 0);
//    std::vector<float> decentGuesses;
//    std::vector<float> roots;
//    
//    for (int i = 0; i < numDivisions; i++)
//    {
//        float val = (maxGuess - minGuess) / numDivisions * i + minGuess;
//        functionVals[i] = g(val);
//        vals[i] = val;
//    }
//    
//    for (int i = 0; i < numDivisions - 1; i++)
//    {
//        if (functionVals[i]*functionVals[i+1] < 0)
//        {
//            decentGuesses.push_back(vals[i]);
//            decentGuesses.push_back(vals[i+1]);
//        }
//    }
//    
//    
//    //    for (int i = 0; i < decentGuesses.size(); i++)
//    //    {
//    //        float guess = decentGuesses[i];
//    //        //        float guess = glm::linearRand(minGuess, maxGuess);
//    //        float estimatedRoot = newton_raphson_iterate<typeof(F), float>(F, guess, minGuess, maxGuess, 5, maxIter);
//    //        float error = g(estimatedRoot);
//    //        if (error*error < 0.001) roots.push_back(estimatedRoot);
//    //    }
//    
//    auto tolFun = [g](float min, float max) {
//        float avg = (min + max) / 2.0f;
//        float avg_g = g(avg);
//        if (avg_g * avg_g < 0.0001) return true;
//        return false;};
//    
//    for (int i = 0; i < decentGuesses.size(); i+=2)
//    {
//        auto estimatedRoot2 = bisect<typeof(g), float, typeof(tolFun)>(g, decentGuesses[i], decentGuesses[i+1], tolFun, maxIter);
//        float root = (std::get<0>(estimatedRoot2) + std::get<1>(estimatedRoot2))/2.0f;
//        float error = g(root);
//        if (error*error < 0.0001) roots.push_back(root);
//    }
//    
//    if (roots.size() == 0) return false;
//    float bestGuess = *std::min_element(roots.begin(), roots.end());
//    float error = g(bestGuess);
//    
//    if (error*error < 0.001)
//    {
//        //        std::cout << "Found a point" << std::endl;
//        float hitDist = bestGuess;
//        if (hitDist < hit.HitDistance)
//        {
//            
//            vec3 hitPos = p + bestGuess * d;
//            hit.HitDistance = hitDist;
//            hit.Position = hitPos;
//            auto normal = EvaluateGrad(hitPos.x, hitPos.y, hitPos.z);
//            hit.Normal = glm::normalize(vec3(std::get<0>(normal), std::get<1>(normal), std::get<2>(normal)));
//            return true;
//        }
//    }
//    //    std::cout << "g(estimatedRoot): " << g(estimatedRoot) << std::endl;
//    //    std::cout << "gGrad(1): " << gGrad(1) << std::endl;
//    //    std::cout << "estimation: " << estimatedRoot << std::endl;
//    
//    return false;
//    
//}
