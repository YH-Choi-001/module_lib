#include <Trigo_lookup.h>

volatile double a, b, c, d, e, f, g, h, i, j, k, l;

void setup () {
    a = yh::rec::trigo_lookups::sinA(15);
    b = yh::rec::trigo_lookups::sinA(30);
    c = yh::rec::trigo_lookups::sinA(45);
    d = yh::rec::trigo_lookups::sinA(60);
    e = yh::rec::trigo_lookups::sinA(75);
    f = yh::rec::trigo_lookups::sinA(90);
    g = yh::rec::trigo_lookups::cosA(15);
    h = yh::rec::trigo_lookups::cosA(30);
    i = yh::rec::trigo_lookups::cosA(45);
    j = yh::rec::trigo_lookups::cosA(60);
    k = yh::rec::trigo_lookups::cosA(75);
    l = yh::rec::trigo_lookups::cosA(90);
}

void loop () {
    //
}