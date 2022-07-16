#include <Wire.h>

#include <Hc_sr04.h>

volatile uint8_t dist [4];

yh::rec::Hc_sr04 uts [4] = {
    yh::rec::Hc_sr04(2, A0),
    yh::rec::Hc_sr04(4, A1),
    yh::rec::Hc_sr04(7, A2),
    yh::rec::Hc_sr04(8, A3)
};

const uint8_t led_pins [4] = {3, 5, 6, 9};

volatile char tx_data_special_request;

#define DIST_0 'w'
#define DIST_1 'x'
#define DIST_2 'y'
#define DIST_3 'z'

#define TX_DATA_DEFAULT_REGISTER DIST_0

void rx (int);
void tx ();

void setup () {
    dist[0] = dist[1] = dist[2] = dist[3] = 0;
    tx_data_special_request = TX_DATA_DEFAULT_REGISTER;
    uts[0].begin();
    uts[1].begin();
    uts[2].begin();
    uts[3].begin();
    Wire.begin('U'); // 'U' == 85 in decimal (ASCII)
    Wire.onReceive(rx);
    Wire.onRequest(tx);
}

void loop () {
    for (uint8_t i = 0; i < 3; i++) {
        const uint16_t temp_dist = uts[i].read_dist_cm();
        const uint8_t dist_8bit = (temp_dist == 888) ? 255 : temp_dist;
        noInterrupts();
        dist[i] = dist_8bit;
        interrupts();
        analogWrite(led_pins[i], dist_8bit);
    }
}

void rx (int len) {
    while (Wire.available()) {
        tx_data_special_request = Wire.read();
    }
}

void tx () {
    const uint8_t tx_data_special_request_for_switch = tx_data_special_request;
    switch (tx_data_special_request_for_switch) {
        case DIST_0: Wire.write(dist[0]);
        case DIST_1: Wire.write(dist[1]);
        case DIST_2: Wire.write(dist[2]);
        case Dist_3: Wire.write(dist[3]);
        break;
    }
    tx_data_special_request = TX_DATA_DEFAULT_REGISTER;
}