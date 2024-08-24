#ifndef __BMI088_H
#define __BMI088_H

typedef struct {
  struct {
    float x, y, z;
  } accel;

  struct {
    float x, y, z;
  } gyro;

  float temp;
} bmi088_t;

void bmi088_init(void);
void bmi088_read_accel(bmi088_t* bmi088);
void bmi088_read_gyro(bmi088_t* bmi088);
void bmi088_read_temp(bmi088_t* bmi088);
void bmi088_read_all(bmi088_t* bmi088);

#define CS_Accel_Pin GPIO_PIN_4
#define CS_Accel_GPIO_Port GPIOA
#define CS_Gyro_Pin GPIO_PIN_0
#define CS_Gyro_GPIO_Port GPIOB

#define INT_Accel_Pin GPIO_PIN_4
#define INT_Accel_GPIO_Port GPIOC
#define INT_Accel_EXTI_IRQn EXTI4_IRQn
#define INT_Gyro_Pin GPIO_PIN_5
#define INT_Gyro_GPIO_Port GPIOC
#define INT_Gyro_EXTI_IRQn EXTI9_5_IRQn

#define BMI088_TEMP_FACTOR 0.125f
#define BMI088_TEMP_OFFSET 23.0f

#define BMI088_ACCEL_3G_SEN 0.0008974358974f
#define BMI088_ACCEL_6G_SEN 0.00179443359375f
#define BMI088_ACCEL_12G_SEN 0.0035888671875f
#define BMI088_ACCEL_24G_SEN 0.007177734375f

#define BMI088_GYRO_2000_SEN 0.00106526443603169529841533860381f
#define BMI088_GYRO_1000_SEN 0.00053263221801584764920766930190693f
#define BMI088_GYRO_500_SEN 0.00026631610900792382460383465095346f
#define BMI088_GYRO_250_SEN 0.00013315805450396191230191732547673f
#define BMI088_GYRO_125_SEN 0.000066579027251980956150958662738366f

#endif /* __BMI088_H */
