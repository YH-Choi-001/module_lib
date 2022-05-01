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

                Quaternion (const double nw, const double nx, const double ny, const double nz) : w(nw), x(nx), y(ny), z(nz) { }

                Quaternion (const Euler_angle &init) {
                    const double d_roll_rad_div_2  = PI / 360.0 * init.roll;
                    const double d_pitch_rad_div_2 = PI / 360.0 * init.pitch;
                    const double d_yaw_rad_div_2   = PI / 360.0 * init.yaw;
                    // delta Quaternion
                    // a little note that sin(pi - theta) == sin(theta), and cos(-theta) == cos(theta)
                    const double
                        cy = cos(d_yaw_rad_div_2),
                        sy = sin(d_yaw_rad_div_2),
                        cp = cos(d_pitch_rad_div_2),
                        sp = sin(d_pitch_rad_div_2),
                        cr = cos(d_roll_rad_div_2),
                        sr = sin(d_roll_rad_div_2);

                    // Given that: pow(e, ix) == cos(x) + i sin(x),
                    //  (cos(y) + k sin(y))(cos(p) + j sin(p))(cos(r) + i sin(r))
                    // == cos(y) * cos(p) * cos(r) + k sin(y) * j sin(p) * i sin(r)
                    //  + cos(y) * cos(p) * i sin(r) + k sin(y) * j sin(p) * cos(r)
                    //  + cos(y) * j sin(p) * cos(r) + k sin(y) * cos(p) * i sin(r)
                    //  + k sin(y) * cos(p) * cos(r) + cos(y) * j sin(p) * i sin(r)

                    // == cos(y) * cos(p) * cos(r) + kji sin(y) * sin(p) * sin(r)
                    //  + i cos(y) * cos(p) * sin(r) + kj sin(y) * sin(p) * cos(r)
                    //  + j cos(y) * sin(p) * cos(r) + ki sin(y) * cos(p) * sin(r)
                    //  + k sin(y) * cos(p) * cos(r) + ji cos(y) * sin(p) * sin(r)

                    // == cos(y) * cos(p) * cos(r) + (+1) sin(y) * sin(p) * sin(r)
                    //  + i cos(y) * cos(p) * sin(r) + (-i) sin(y) * sin(p) * cos(r)
                    //  + j cos(y) * sin(p) * cos(r) + (j) sin(y) * cos(p) * sin(r)
                    //  + k sin(y) * cos(p) * cos(r) + (-k) cos(y) * sin(p) * sin(r)

                    // == cos(y) * cos(p) * cos(r) + sin(y) * sin(p) * sin(r)
                    //  + i cos(y) * cos(p) * sin(r) - i sin(y) * sin(p) * cos(r)
                    //  + j cos(y) * sin(p) * cos(r) + j sin(y) * cos(p) * sin(r)
                    //  + k sin(y) * cos(p) * cos(r) - k cos(y) * sin(p) * sin(r)

                    // == cos(y) * cos(p) * cos(r) + sin(y) * sin(p) * sin(r)
                    //  + (cos(y) * cos(p) * sin(r) - sin(y) * sin(p) * cos(r))i
                    //  + (cos(y) * sin(p) * cos(r) + sin(y) * cos(p) * sin(r))j
                    //  + (sin(y) * cos(p) * cos(r) - cos(y) * sin(p) * sin(r))k

                    w = cy * cp * cr + sy * sp * sr;
                    x = cy * cp * sr - sy * sp * cr;
                    y = cy * sp * cr + sy * cp * sr;
                    z = sy * cp * cr - cy * sp * sr;
                }

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


        typedef int16_t i_sin_type;
        #define INT_MUL_CONST 32767

        i_sin_type i_sin_raw (int16_t angle_in_degrees);

        double sin (double angle_in_degrees);

        double cos (double angle_in_degrees);

    }
}

#endif // #ifndef MATHS_EXTENSION_H