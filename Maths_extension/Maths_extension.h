#ifndef MATHS_EXTENSION_H
#define MATHS_EXTENSION_H __DATE__ ", " __TIME__

namespace yh {
    namespace rec {

        class Euler_angle;
        class Quaternion;

        class Euler_angle {
            public:
                // angles
                double
                    roll,
                    pitch,
                    yaw;

                // default constructor
                Euler_angle () : roll(0.0), pitch(0.0), yaw(0.0) { }

                Euler_angle (const Euler_angle &init) : roll(init.roll), pitch(init.pitch), yaw(init.yaw) { }

                Euler_angle (const double r, const double p, const double y) : roll(r), pitch(p), yaw(y) { }

                inline void operator = (const Euler_angle rhs) {
                    this->roll  = rhs.roll;
                    this->pitch = rhs.pitch;
                    this->yaw   = rhs.yaw;
                }

        };

        class Quaternion {
            public:
                // vectors
                double
                    w,
                    x,
                    y,
                    z;

                // default constructor
                Quaternion () : w(1.0), x(0.0), y(0.0), z(0.0) { }

                Quaternion (const Quaternion &init) : w(init.w), x(init.x), y(init.y), z(init.z) { }

                Quaternion (double nw, double nx, double ny, double nz) : w(nw), x(nx), y(ny), z(nz) { }

                inline void operator = (const Quaternion rhs) {
                    this->w = rhs.w;
                    this->x = rhs.x;
                    this->y = rhs.y;
                    this->z = rhs.z;
                }

                inline void operator *= (const Quaternion rhs) {
                    const double
                        product_w = (w * rhs.w   -   x * rhs.x   -   y * rhs.y   -   z * rhs.z),
                        product_x = (w * rhs.x   +   x * rhs.w   +   y * rhs.z   -   z * rhs.y),
                        product_y = (w * rhs.y   -   x * rhs.z   +   y * rhs.w   +   z * rhs.x),
                        product_z = (w * rhs.z   +   x * rhs.y   -   y * rhs.x   +   z * rhs.w);
                    this->w = product_w;
                    this->x = product_x;
                    this->y = product_y;
                    this->z = product_z;
                }

                // call me to check if bias from 1 is inacceptable
                inline double vectors_squared () { return (w * w + x * x + y * y + z * z); }

                inline void normalize () {
                    const double m = sqrt(this->vectors_squared());
                    w /= m;
                    x /= m;
                    y /= m;
                    z /= m;
                }
        };

        Quaternion operator * (const Quaternion lhs, const Quaternion rhs);

    }
}

#endif // #ifndef MATHS_EXTENSION_H