//
// Created by 18292649286 on 2022/10/21.
//

#ifndef _DMP_TEST_H
#define _DMP_TEST_H

unsigned short inv_row_2_scale(const signed char *row);

int DMP_test(void);

unsigned char mpu_dmp_get_data(float *pitch, float *roll, float *yaw);

unsigned char mpu_dmp_init(void);


unsigned short inv_orientation_matrix_to_scalar(
        const signed char *mtx);

unsigned char run_self_test(void);
#endif //INC_13_DEMO_OLED_FLOAT_DMP_TEST_H
