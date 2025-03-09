#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <fstream>
#include <cstdio>
#include <cstdlib>

#if defined __linux__ || defined __APPLE__
#else
// #define M_PI 3.14159265358979323846
// #define INFINITY 1e8
#endif

template <typename T>
class Vec3 {
public:
    T x, y, z;
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T xx) : x(xx), y(xx), z(xx) {}
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
    Vec3(const Vec3<T> &v) : x(v.x), y(v.y), z(v.z) {}

    Vec3 &normalize() {
        T nor2 = length2();
        if (nor2 > 0) {
            T invNor = 1 / sqrt(nor2);
            x *= invNor, y *= invNor, z *= invNor;
        }
        return *this;
    }

    Vec3<T> operator * (const T &f) const { return Vec3<T>(x * f, y * f, z * f); }
    Vec3<T> operator * (const Vec3<T> &v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }
    T dot(const Vec3<T> &v) const { return this->x * v.x + this->y * v.y + this->z * v.z; }
    Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
    Vec3<T> operator - (const T &f) const { return Vec3<T>(x - f, y - f, z - f); }
    Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
    Vec3<T>& operator += (const Vec3<T> &v) { x += v.x, y += v.y, z += v.z; return *this; }
    Vec3<T>& operator *= (const Vec3<T> &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
    Vec3<T> operator - () const { return Vec3<T>(-x, -y, -z); }
    T length2() const { return x * x + y * y + z * z; }
    T length() const { return sqrt(length2()); }
    friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v) {
        os << "[" << v.x << " " << v.y << " " << v.z << "]";
        return os;
    }
};

typedef Vec3<float> Vec3f;
class Sphere {
public:
    Vec3f center;
    float radius, radius2;
    Vec3f surfaceColor, emissionColor;
    float transparency, reflection;

    Sphere(
        const Vec3f &c,
        const float &r,
        const Vec3f &sc, 
        const float &refl = 0,
        const float &transp = 0,
        const Vec3f &ec = Vec3f(0)
    )
    : center(c), radius(r), radius2(r * r), surfaceColor(sc), reflection(refl), transparency(transp), emissionColor(ec) {}

    bool intersect(const Vec3f &rayorg, const Vec3f &raydir, float &t0, float &t1) const {
        Vec3f l = center - rayorg;
        float tca = l.dot(raydir);
        if (tca < 0) return false;
        float d2 = l.dot(l) - tca * tca;
        if (d2 > radius2) return false;
        float thc = sqrt(radius2 - d2);
        t0 = tca - thc;
        t1 = tca + thc;
        return true;
    }
};

#define MAX_RAY_DEPTH 5

float mix(const float &a, const float &b , const float &mix){
    return a * (1 - mix) + b * mix;
}


Vec3f trace(const Vec3f &rayorg, const Vec3f &raydir, const std::vector<Sphere> &spheres, int depth ){
    float tnear = INFINITY;
    const Sphere* sphere = NULL;
    for (unsigned int i = 0; i < spheres.size(); i++){
        float t0 = INFINITY, t1 = INFINITY;
        if (spheres[i].intersect(rayorg, raydir, t0, t1)){
            if (t0 < 0) t0 = t1;
            if (t0 < tnear){
                tnear = t0;
                sphere = &spheres[i];
            }
        }
    }
    if (!sphere) return Vec3f(2);
    Vec3f surfaceColor = 0;
    Vec3f phit = rayorg + raydir * tnear;
    Vec3f nhit = sphere->center - phit;
    nhit.normalize();


    float bias = 1e-4;
    bool inside = false;
    if (raydir.dot(nhit) > 0){
        nhit = -nhit;
        inside = true;
    }
    if ((sphere->transparency > 0 || sphere->reflection > 0) && (depth < MAX_RAY_DEPTH)){
        float facingratio = -raydir.dot(nhit);
        float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
        Vec3f refldir = raydir - nhit * 2 * raydir.dot(nhit);\
        refldir.normalize();
        Vec3f reflectionColor = trace(phit + nhit * bias, refldir, spheres, depth + 1);
        Vec3f refractionColor = 0;
        if (sphere->transparency != 0){
            float ior = 1.1;
            float eta = (inside) ? ior : (1 / ior);
            float cosi = -nhit.dot(raydir);
            float k = 1 - eta * eta * (1 - cosi * cosi);
            Vec3f refrdir = raydir * eta + nhit * (eta *  cosi - sqrt(k));
            refrdir.normalize();
            refractionColor = trace(phit - nhit * bias, refrdir, spheres, depth + 1);
        }
        surfaceColor = (
            reflectionColor * fresneleffect
            + refractionColor * (1 - fresneleffect) * sphere->transparency
            + sphere->surfaceColor * sphere->emissionColor
        );
    } else {
        for (unsigned int i = 0; i < spheres.size(); i++){
            if (spheres[i].emissionColor.x > 0){
                Vec3f transmission = 1;
                Vec3f lightDirection = spheres[i].center - phit;
                lightDirection.normalize();
                for (unsigned int j = 0; j < spheres.size(); j++){
                    if (i != j){
                        float t0, t1;
                        if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)){
                            transmission = 0;
                            break;
                        }
                    }
                }
                surfaceColor += sphere->surfaceColor * transmission * std::max(float(0), nhit.dot(lightDirection)) * spheres[i].emissionColor;
            }
        }
    }
    return surfaceColor + sphere->surfaceColor;         
}


void render(const std::vector<Sphere> &spheres){
    unsigned int width = 640, height = 480;
    Vec3f *image = new Vec3f[width * height], *pixel = image;
    float inWidth = 1.f / width, inHeight = 1.f / height;
    float fov = 30.f , aspectRatio = width / (float)height;
    float angle = fov * M_PI / 180.f;

    for (unsigned int y = 0; y < height; ++y){
        for (unsigned int x = 0; x < width; ++x){
            float xx = (2 * ((x + 0.5) * inWidth) - 1) * angle * aspectRatio;
            float yy = (1 - 2 * ((y + 0.5) * inHeight)) * angle;
            Vec3f raydir(xx, yy, -1);
            raydir.normalize();
            *pixel = trace(Vec3f(0), raydir, spheres, 0);
            pixel++;
        }
    }
    std::ofstream ofs("./out.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned int i = 0; i < width * height; ++i){
        ofs << (unsigned char)(255 * std::max(0.f, std::min(1.f, image[i].x)));
        ofs << (unsigned char)(255 * std::max(0.f, std::min(1.f, image[i].y)));
        ofs << (unsigned char)(255 * std::max(0.f, std::min(1.f, image[i].z)));
    }
    ofs.close();
    delete[] image;
}


int main(int argc, char **argv){
    srand48(13);
    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3f( 0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
    spheres.push_back(Sphere(Vec3f( 0.0,      0, -20),     4, Vec3f(1.00, 0.32, 0.36), 1, 0.5));
    spheres.push_back(Sphere(Vec3f( 5.0,     -1, -15),     2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));
    spheres.push_back(Sphere(Vec3f( 5.0,      0, -25),     3, Vec3f(0.65, 0.77, 0.97), 1, 0.0));
    spheres.push_back(Sphere(Vec3f(-5.5,      0, -15),     3, Vec3f(0.90, 0.90, 0.90), 1, 0.0));
    spheres.push_back(Sphere(Vec3f( 0.0,     20, -30),     3, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(3)));
    render(spheres);
    
    return 0;

}