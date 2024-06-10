

#include <stdio.h>
#include "main.h"
#include "math.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


unsigned short inv_row_2_scale(const signed char *row);

//q30��ʽ,longתfloatʱ�ĳ���.
#define q30  1073741824.0f

//�����Ƿ�������
static signed char gyro_orientation[9] = {1, 0, 0,
                                          0, 1, 0,
                                          0, 0, 1
};

//MPU6050�Բ���
//����ֵ:0,����
//    ����,ʧ��
unsigned char run_self_test(void) {
    int result;
    //char test_packet[4] = {0};
    long gyro[3], accel[3];
    result = mpu_run_self_test(gyro, accel);
    if (result == 0x3) {
        /* Test passed. We can trust the gyro data here, so let's push it down
        * to the DMP.
        */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long) (gyro[0] * sens);
        gyro[1] = (long) (gyro[1] * sens);
        gyro[2] = (long) (gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
        return 0;
    } else return 1;
}

//�����Ƿ������
unsigned short inv_orientation_matrix_to_scalar(
        const signed char *mtx) {
    unsigned short scalar;
    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

//����ת��
unsigned short inv_row_2_scale(const signed char *row) {
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

//�պ���,δ�õ�.
void mget_ms(unsigned long *time) {

}

//mpu6050,dmp��ʼ��
//����ֵ:0,����
//    ����,ʧ��
//��MPU6050�Ĺٷ�DMP��˵����ִ��run_self_testʱ������ʹ��z������ֱ����ƽ��
//z�����ϻ������¶�����
#define DEFAULT_MPU_HZ    200

unsigned char mpu_dmp_init(void) {
    unsigned char res = 0;
    struct int_param_s *p = 0;
    //MPU_IIC_Init(); 	//��ʼ��IIC����
    if (mpu_init(p) == 0)    //��ʼ��MPU6050
    {
        res = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);//��������Ҫ�Ĵ�����
        if (res)return 1;
        res = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//����FIFO
        if (res)return 2;
        res = mpu_set_sample_rate(DEFAULT_MPU_HZ);    //���ò�����
        if (res)return 3;
        res = dmp_load_motion_driver_firmware();        //����dmp�̼�
        if (res)return 4;
        res = dmp_set_orientation(
                inv_orientation_matrix_to_scalar(gyro_orientation));//���������Ƿ���
        if (res)return 5;
        res = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |    //����dmp����
                                 DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL |
                                 DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL);
        if (res)return 6;
        res = dmp_set_fifo_rate(DEFAULT_MPU_HZ);    //����DMP�������(��󲻳���200Hz)
        if (res)return 7;
        res = run_self_test();        //�Լ�
        if (res)return 8;
        res = mpu_set_dmp_state(1);    //ʹ��DMP
        if (res)return 9;
    } else return 10;
    return 0;
}

//�õ�dmp����������
//pitch:������ ����:0.1��   ��Χ:-90.0�� <---> +90.0��
//roll:�����  ����:0.1��   ��Χ:-180.0��<---> +180.0��
//yaw:�����   ����:0.1��   ��Χ:-180.0��<---> +180.0��
//����ֵ:0,����
//    ����,ʧ��
unsigned char mpu_dmp_get_data(float *pitch, float *roll, float *yaw) {
    float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
    unsigned long sensor_timestamp;
    short gyro[3], accel[3], sensors;
    unsigned char more;
    long quat[4];
    if (dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))return 1;
    /* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
     * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
    **/
    /*if (sensors & INV_XYZ_GYRO )
    send_packet(PACKET_TYPE_GYRO, gyro);
    if (sensors & INV_XYZ_ACCEL)
    send_packet(PACKET_TYPE_ACCEL, accel); */
    /* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
     * The orientation is set by the scalar passed to dmp_set_orientation during initialization.
    **/
    if (sensors & INV_WXYZ_QUAT) {
        q0 = quat[0] / q30;    //q30��ʽת��Ϊ������
        q1 = quat[1] / q30;
        q2 = quat[2] / q30;
        q3 = quat[3] / q30;
        //����õ�������/�����/�����
        *pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;    // pitch
        *roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;    // roll
        *yaw = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;    //yaw
    } else return 2;
    return 0;
}

#include "mpu6050.h"
int DMP_test(void) {
    unsigned char i;
    float pitch, roll, yaw;         //ŷ����
    HAL_Delay(500);
		unsigned long	STEPS = 0;
    //��MPU6050�Ĺٷ�DMP��˵������ʼ��ʱ������ʹ��z������ֱ����ƽ��
    //z�����ϻ������¶�����
    i = mpu_dmp_init();
    while (i) {
        HAL_Delay(500);
        i = mpu_dmp_init();
        printf("MPU6050 init error:%d\r\n", i);
			printf("systick [%d]\r\n", HAL_GetTick());
    }
    printf("MPU6050 init OK\r\n");
		dmp_set_pedometer_step_count(0);
		mpu_set_accel_fsr(8);
    while (1) {
        if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0) {
						dmp_get_pedometer_step_count(&STEPS);
					
					MPU6050_Get();
					//printf("pitch:%f\t,roll:%f\t,yaw:%f -- step:%d\r\n", pitch, roll, yaw, STEPS);
					printf("x:%d - y:%d - z:%d   \r\n", ax, ay, az);
            //printf("$%f %f %f;", pitch, roll, yaw);
						HAL_Delay(500);
        }
    }
}

