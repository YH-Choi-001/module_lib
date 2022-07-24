#include <Btn7971b.h>
#include <Encoder_2ch.h>
#include <pid.h>

yh::rec::Btn7971b mtrs[4] = {
  yh::rec::Btn7971b(5, 13),
  yh::rec::Btn7971b(11, 10),
  yh::rec::Btn7971b(6, 12),
  yh::rec::Btn7971b(3, 2)
};

yh::rec::Encoder_2ch_pulse encoders[4] = {
  yh::rec::Encoder_2ch_pulse(A0, A1, 255, 140), // A1
  yh::rec::Encoder_2ch_pulse(A2, A3, 255, 140), // A3
  yh::rec::Encoder_2ch_pulse(4, 7, 255, 140),  // 7
  yh::rec::Encoder_2ch_pulse(8, 9, 255, 140)   // 9
};

yh::rec::Pid corrs[4] = {
  yh::rec::Pid(0.5, 0.00000001, 10),
  yh::rec::Pid(0.5, 0.00000001, 10),
  yh::rec::Pid(0.5, 0.00000001, 10),
  yh::rec::Pid(0.5, 0.00000001, 10)
};

int16_t motor_spds[4] = {
  0,
  0,
  0,
  0
};

unsigned long prev_time;
uint32_t prev_spd_set_time;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    encoders[0].begin();
    encoders[1].begin();
    encoders[2].begin();
    encoders[3].begin();
    mtrs[0].begin();
    mtrs[1].begin();
    mtrs[2].begin();
    mtrs[3].begin();
    motor_spds[0] = -50;
    motor_spds[2] = -motor_spds[0];
    motor_spds[3] = motor_spds[0];
    motor_spds[1] = -motor_spds[3];
    prev_time = micros();
    prev_spd_set_time = millis();
}

void loop () {
    const int16_t s0 = encoders[0].get_spd_mean();
    mtrs[0] += corrs[0].update_corr(motor_spds[0], s0);
    const int16_t s1 = encoders[1].get_spd_mean();
    mtrs[1] += corrs[1].update_corr(motor_spds[1], s1);
    const int32_t s2 = encoders[2].get_spd_mean();
    mtrs[2] += corrs[2].update_corr(motor_spds[2], s2);
    const int16_t s3 = encoders[3].get_spd_mean();
    mtrs[3] += corrs[3].update_corr(motor_spds[3], s3);
    Serial.print(s0);
    Serial.print('\t');
    Serial.print(s1);
    Serial.print('\t');
    Serial.print(s2);
    Serial.print('\t');
    Serial.print(s3);
    Serial.print('\n');
    if ((millis() - prev_spd_set_time) > 5000) {
        if (motor_spds[2] == 50) {
            motor_spds[0] = -100;
            motor_spds[2] = -motor_spds[0];
            motor_spds[3] = motor_spds[0];
            motor_spds[1] = -motor_spds[3];
        } else if (motor_spds[2] == 100) {
            motor_spds[0] = -150;
            motor_spds[2] = -motor_spds[0];
            motor_spds[3] = motor_spds[0];
            motor_spds[1] = -motor_spds[3];
        } else if (motor_spds[2] == 150) {
            motor_spds[0] = -50;
            motor_spds[2] = -motor_spds[0];
            motor_spds[3] = motor_spds[0];
            motor_spds[1] = -motor_spds[3];
        }
        prev_spd_set_time = millis();
    }
}

void loop_B () {
    // put your main code here, to run repeatedly:
//    Serial.print(encoders[0].get_instantaneous_velocity());
    const int16_t L = encoders[0].get_spd_mean();
    if (L < -48) {
      mtrs[0]++;
    } else if (L > -52) {
      mtrs[0]--;
    }
    Serial.print(L);
    Serial.print('\t');
//    Serial.print(encoders[2].get_instantaneous_velocity());
    const int16_t R = encoders[2].get_spd_mean();
    if (R < 48) {
      mtrs[2]++;
    } else if (R > 52) {
      mtrs[2]--;
    }
    Serial.print(R);
    Serial.print('\n');
}
