/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       shoot.c/h
  * @brief      �������.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "shoot.h"
#include "main.h"

#include "cmsis_os.h"

#include "bsp_laser.h"
#include "bsp_fric.h"
#include "arm_math.h"
#include "user_lib.h"
#include "referee.h"

#include "CAN_receive.h"
#include "gimbal_behaviour.h"
#include "detect_task.h"
#include "pid.h"

#include "stdlib.h"

#include "usart.h"

#define shoot_fric1_on(pwm) fric1_on((pwm)) //Ħ����1pwm�궨��
#define shoot_fric2_on(pwm) fric2_on((pwm)) //Ħ����2pwm�궨��
#define shoot_fric_off()    fric_off()      //�ر�����Ħ����

#define shoot_laser_on()    laser_on()      //���⿪���궨��
#define shoot_laser_off()   laser_off()     //����رպ궨��
//΢������IO
#define BUTTEN_TRIG_PIN HAL_GPIO_ReadPin(BUTTON_TRIG_GPIO_Port, BUTTON_TRIG_Pin)


/**
  * @brief          ���״̬�����ã�ң�����ϲ�һ�ο��������ϲ��رգ��²�1�η���1�ţ�һֱ�����£���������䣬����3min׼��ʱ�������ӵ�
  * @param[in]      void
  * @retval         void
  */
static void shoot_set_mode(void);
/**
  * @brief          ������ݸ���
  * @param[in]      void
  * @retval         void
  */
static void shoot_feedback_update(void);

/**
  * @brief          ��ת��ת����
  * @param[in]      void
  * @retval         void
  */
static void trigger_motor_turn_back(void);

/**
  * @brief          ������ƣ����Ʋ�������Ƕȣ����һ�η���
  * @param[in]      void
  * @retval         void
  */
static void shoot_bullet_control(void);


shoot_control_t shoot_control;          //�������

int RAMP_STOP = 0;		//б��ֹͣ���ٶȸı���
	
/**
  * @brief          �����ʼ������ʼ��PID��ң����ָ�룬���ָ��
  * @param[in]      void
  * @retval         ���ؿ�
  */
void shoot_init(void)
{
		
        static const fp32 Trigger_speed_pid[3] = {TRIGGER_SPEED_PID_KP, TRIGGER_SPEED_PID_KI, TRIGGER_SPEED_PID_KD};
		static const fp32 Trigger_angle_pid[3] = {TRIGGER_ANGLE_PID_KP, TRIGGER_ANGLE_PID_KI, TRIGGER_ANGLE_PID_KD};
		static const fp32 Fric1_speed_pid[3] = {FRIC1_KP, FRIC1_KI	, FRIC1_KD};
		static const fp32 Fric2_speed_pid[3] = {FRIC2_KP, FRIC2_KI	, FRIC2_KD};
		
    shoot_control.shoot_mode = SHOOT_STOP;
    //ң����ָ��
    shoot_control.shoot_rc = get_remote_control_point();
    //���ָ��  //���ǲ�������ٷ������õ���2006������Ӣ��ʵ���õ�3508��
    shoot_control.shoot_motor_measure = get_trigger_motor_measure_point();
	shoot_control .shoot_fric1_measure = get_fric_motor_measure_point(0);
	shoot_control .shoot_fric2_measure = get_fric_motor_measure_point(1);
    shoot_control .fric2_ramp.input = shoot_control .shoot_fric2_measure->speed_rpm;
    shoot_control .fric1_ramp.input = shoot_control .shoot_fric1_measure->speed_rpm;   
    //��ʼ��PID
    PID_init(&shoot_control.trigger_motor_speed_pid, PID_POSITION, Trigger_speed_pid, TRIGGER_ANGLE_PID_MAX_KP, TRIGGER_ANGLE_PID_MAX_KI);
    //��һ���ǶȻ�PID
	PID_init(&shoot_control.trigger_motor_angle_pid, PID_POSITION, Trigger_angle_pid, TRIGGER_SPEED_PID_MAX_KP,TRIGGER_SPEED_PID_MAX_KI);
	//����Ħ����PID
	PID_init(&shoot_control.fric1_ramp_pid ,PID_POSITION ,Fric1_speed_pid,FRIC12_PID_MAX_OUT,FRIC12_PID_MAX_IOUT);
	PID_init(&shoot_control.fric2_ramp_pid ,PID_POSITION ,Fric2_speed_pid,FRIC12_PID_MAX_OUT,FRIC12_PID_MAX_IOUT);
	//��������
    shoot_feedback_update();
    
	//�����˺�����shoot_control.fric1_ramp.max_value��ֵ
	ramp_init(&shoot_control.fric1_ramp, -SHOOT_CONTROL_TIME * 0.001f,  -FRIC_DOWN, FRIC_OFF);
    ramp_init(&shoot_control.fric2_ramp, -SHOOT_CONTROL_TIME * 0.001f, FRIC_DOWN, FRIC_OFF);

    
	shoot_control.fric_current1 = 0;
    shoot_control.fric_current2 = 0;
	shoot_control .fric1_speed = 0;
	shoot_control .fric1_speed = 0;
	shoot_control .fric1_set_speed = 0;
	shoot_control.trigger_flag = 0;
	shoot_control.angle = 0;
	shoot_control .add_angle  = 0;
	shoot_control .angle_set  = 0;
	shoot_control .ecd_Sum = 0;
		
    shoot_control.given_current = 0;
    shoot_control.rpm_speed = 0.0f;
    shoot_control.key_time = 0;
}

/**
  * @brief          ���ѭ��
  * @param[in]      void
  * @retval         ����can����ֵ
  */
uint16_t addspped = 0;
int fric_speed = 0;
int shoot_down_state = 0;
int16_t shoot_control_loop(void)
{
 
    shoot_set_mode();        //����״̬��
    shoot_feedback_update(); //��������

    if (shoot_control.shoot_mode == SHOOT_STOP)
    {
            //���ò�����
            shoot_control.trigger_rpm_speed_set = 0.0f;
			PID_calc(&shoot_control.trigger_motor_speed_pid, MOTOR_TO_TRIGGER_RPM*shoot_control.shoot_motor_measure->speed_rpm ,
				 																shoot_control.trigger_rpm_speed_set);
            shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_speed_pid.out);	
			//����Ħ����
            if(shoot_control.last_shoot_mode != SHOOT_STOP)
            {
                shoot_down_state = 0;
            }
            if(shoot_down_state == 0)
            {
            if(shoot_control .shoot_fric1_measure ->speed_rpm != 0)
            {
                addspped = addspped +1;
                if(addspped == -FRIC_SPEED)
                {
                    addspped = -FRIC_SPEED;
                }
                fric_speed = FRIC_SPEED+addspped;
                			
            PID_calc (&shoot_control .fric1_ramp_pid,shoot_control .shoot_fric1_measure ->speed_rpm , fric_speed);
			PID_calc (&shoot_control .fric2_ramp_pid,shoot_control .shoot_fric2_measure ->speed_rpm ,-fric_speed);
			
            shoot_control .fric_current1 = shoot_control.fric1_ramp_pid.out;//shoot_control.fric1_ramp.out;//shoot_control.fric1_ramp_pid.out;//PID_calc (&shoot_control .fric1_ramp_pid,shoot_control .shoot_fric1_measure ->speed_rpm ,0);
			shoot_control .fric_current2 = shoot_control.fric2_ramp_pid.out;//shoot_control.fric2_ramp.out;//shoot_control.fric2_ramp_pid.out;//PID_calc (&shoot_control .fric2_ramp_pid,shoot_control .shoot_fric2_measure ->speed_rpm ,0);

            }
            }
            if(fabs(shoot_control .shoot_fric1_measure ->speed_rpm) < 1500)
            {
                shoot_down_state = 1;
                addspped = 0;
                fric_speed = 0;
                shoot_control .fric_current1 = 0;//shoot_control.fric1_ramp.out;//shoot_control.fric1_ramp_pid.out;//PID_calc (&shoot_control .fric1_ramp_pid,shoot_control .shoot_fric1_measure ->speed_rpm ,0);
                shoot_control .fric_current2 = 0;//shoot_control.fric2_ramp.out;//shoot_control.fric2_ramp_pid.out;//PID_calc (&shoot_control .fric2_ramp_pid,shoot_control .shoot_fric2_measure ->speed_rpm ,0);
		
            }
       }
       
    else if (shoot_control.shoot_mode == SHOOT_READY_FRIC)
    {
        //���ò����ֵ��ٶ�
			shoot_laser_on();
			shoot_control.trigger_rpm_speed_set = 0.0f;
			PID_calc(&shoot_control.trigger_motor_speed_pid, shoot_control.shoot_motor_measure->speed_rpm ,
																		shoot_control.trigger_rpm_speed_set);
			PID_calc (&shoot_control .fric1_ramp_pid,shoot_control .shoot_fric1_measure ->speed_rpm ,FRIC_SPEED/*FRIC12_SPEED_TO_RPM*/);
			PID_calc (&shoot_control .fric2_ramp_pid,shoot_control .shoot_fric2_measure ->speed_rpm ,-FRIC_SPEED/*-FRIC12_SPEED_TO_RPM*/);
			
			shoot_control.fric_current1 = (int16_t)(shoot_control.fric1_ramp_pid.out);
			shoot_control.fric_current2 = (int16_t)(shoot_control.fric2_ramp_pid.out);
			shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_speed_pid.out);
    }
        //���Ħ����ת������˵���е�
    if(shoot_control .shoot_mode == SHOOT_FRIC_STOP)
    {
        	PID_calc (&shoot_control .fric1_ramp_pid,shoot_control .shoot_fric1_measure ->speed_rpm ,-FRIC_SPEED);
			PID_calc (&shoot_control .fric2_ramp_pid,shoot_control .shoot_fric2_measure ->speed_rpm ,+FRIC_SPEED);
			
			shoot_control.fric_current1 = (int16_t)(shoot_control.fric1_ramp_pid.out);
			shoot_control.fric_current2 = (int16_t)(shoot_control.fric2_ramp_pid.out);
            shoot_control.given_current = 0;
            shoot_control .shoot_ready_fric_time --;
            if( shoot_control .shoot_ready_fric_time <= 500)
            {
                shoot_control .shoot_mode = SHOOT_READY_FRIC;
                shoot_control.shoot_ready_fric_time = 0;
            }
            
    }
    else if(shoot_control.shoot_mode ==SHOOT_READY_BULLET)//װ��д���ٶȻ�
    {
			
			PID_calc (&shoot_control .fric1_ramp_pid,shoot_control .shoot_fric1_measure ->speed_rpm ,FRIC_SPEED/*FRIC12_SPEED_TO_RPM*/);
			PID_calc (&shoot_control .fric2_ramp_pid,shoot_control .shoot_fric2_measure ->speed_rpm ,-FRIC_SPEED/*-FRIC12_SPEED_TO_RPM*/);
			shoot_control.fric_current1 = (int16_t)(shoot_control.fric1_ramp_pid.out);
			shoot_control.fric_current2 = (int16_t)(shoot_control.fric2_ramp_pid.out);
        if(shoot_control.key == SWITCH_TRIGGER_OFF)
        {
            //���ò����ֵĲ����ٶ�,��������ת��ת����(������ҲҪ�ĳɽǶ�)
							shoot_control.trigger_rpm_speed_set = SHOOT_READY_BULLET_RPM;//0.006*19;//trigger_speed_set�Ұ������rpm��ģ�ÿ��ת60��

							trigger_motor_turn_back();
        }

        else
        {
            shoot_control.trigger_rpm_speed_set = 0.0f;

        }
								

			PID_calc(&shoot_control.trigger_motor_speed_pid, MOTOR_TO_TRIGGER_RPM*shoot_control.shoot_motor_measure->speed_rpm ,
													shoot_control.trigger_rpm_speed_set);
			shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_speed_pid.out);
    
	}

    else if (shoot_control.shoot_mode == SHOOT_READY)
    {
			
					shoot_laser_on();
                    //���ò����ֵ��ٶ�
					//shoot_control.trigger_rpm_speed_set  = 0.0f;
					shoot_control .add_angle =0;
					shoot_control .angle_set = shoot_control .angle +shoot_control .add_angle;
					PID_calc (&shoot_control .fric1_ramp_pid,shoot_control .shoot_fric1_measure ->speed_rpm ,FRIC_SPEED/*FRIC12_SPEED_TO_RPM*/);
					PID_calc (&shoot_control .fric2_ramp_pid,shoot_control .shoot_fric2_measure ->speed_rpm ,-FRIC_SPEED/*-FRIC12_SPEED_TO_RPM*/);
			
					PID_calc(&shoot_control .trigger_motor_angle_pid ,shoot_control .angle ,shoot_control .angle_set );
					PID_calc(&shoot_control.trigger_motor_speed_pid, MOTOR_TO_TRIGGER_RPM*shoot_control.shoot_motor_measure->speed_rpm ,
																															shoot_control .trigger_motor_angle_pid .out );
					
					shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_speed_pid.out);
					shoot_control.fric_current1 =(int16_t)(shoot_control.fric1_ramp_pid.out);
					shoot_control.fric_current2 =(int16_t)(shoot_control.fric2_ramp_pid.out);
			
			
    }
    else if (shoot_control.shoot_mode == SHOOT_BULLET)
    {
			
			shoot_laser_on();
			shoot_control.trigger_rpm_speed_set = SHOOT_BULLET_RPM;
			
			PID_calc (&shoot_control .fric1_ramp_pid,shoot_control .shoot_fric1_measure ->speed_rpm ,FRIC_SPEED/*FRIC12_SPEED_TO_RPM*/);
			PID_calc (&shoot_control .fric2_ramp_pid,shoot_control .shoot_fric2_measure ->speed_rpm ,-FRIC_SPEED/*-FRIC12_SPEED_TO_RPM*/);
			PID_calc(&shoot_control.trigger_motor_speed_pid, MOTOR_TO_TRIGGER_RPM*shoot_control.shoot_motor_measure->speed_rpm ,
																																												shoot_control.trigger_rpm_speed_set);
       
			//�жϵ����Ƿ����ȥ������ɽ�����һ��״̬
				shoot_bullet_control();
				shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_speed_pid.out);

				trigger_motor_turn_back();	
		  
			shoot_control.fric_current1 = (int16_t)(shoot_control.fric1_ramp_pid.out);
			shoot_control.fric_current2 = (int16_t)(shoot_control.fric2_ramp_pid.out);
    }

    else if(shoot_control.shoot_mode == SHOOT_DONE)
    {
						shoot_laser_on();
						PID_calc (&shoot_control .fric1_ramp_pid,shoot_control .shoot_fric1_measure ->speed_rpm ,FRIC_SPEED/*FRIC12_SPEED_TO_RPM*/);
						PID_calc (&shoot_control .fric2_ramp_pid,shoot_control .shoot_fric2_measure ->speed_rpm ,-FRIC_SPEED/*-FRIC12_SPEED_TO_RPM*/);			
						shoot_control.fric_current1 = (int16_t)(shoot_control.fric1_ramp_pid.out);
						shoot_control.fric_current2 = (int16_t)(shoot_control.fric2_ramp_pid.out);
//					//�ٶȻ�����
						shoot_control.trigger_rpm_speed_set  = 0.0f;
						PID_calc(&shoot_control.trigger_motor_speed_pid, MOTOR_TO_TRIGGER_RPM*shoot_control.shoot_motor_measure->speed_rpm ,
																																												shoot_control.trigger_rpm_speed_set);
						shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_speed_pid.out);
		}
		
		//����״̬
		
		if(shoot_control.shoot_mode == SHOOT_BULLET_STOP )
		{		
				
				//Ħ���֣����ֹͣģʽһ��
				PID_calc (&shoot_control .fric1_ramp_pid,shoot_control .shoot_fric1_measure ->speed_rpm ,FRIC_SPEED/*FRIC12_SPEED_TO_RPM*/);
				PID_calc (&shoot_control .fric2_ramp_pid,shoot_control .shoot_fric2_measure ->speed_rpm ,-FRIC_SPEED/*-FRIC12_SPEED_TO_RPM*/);
				shoot_control.fric_current1 = (int16_t)(shoot_control.fric1_ramp_pid.out);
				shoot_control.fric_current2 = (int16_t)(shoot_control.fric2_ramp_pid.out);
				
				if(shoot_control .return_back_flag == BAKE_TIME_OVER)//BAKE_TIME_OVER�ǿ�����־��
				{
					
						shoot_control .add_angle = +1.20f;//�����ʱ��ת���ӽǶ�Ϊ��
						shoot_control .angle_set = shoot_control .angle + shoot_control .add_angle;
						shoot_control .return_back_flag = 0;
				}
				
				PID_calc(&shoot_control.trigger_motor_angle_pid, shoot_control.angle ,shoot_control.angle_set);
				PID_calc(&shoot_control.trigger_motor_speed_pid, shoot_control.shoot_motor_measure->speed_rpm ,
														shoot_control.trigger_motor_angle_pid.out);
				shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_speed_pid.out);
				if(fabs(shoot_control .rpm_speed) < 0.001)
				{
					shoot_control .return_back_flag ++;

				}
				if(fabs(shoot_control .angle - shoot_control .angle_set) < 0.1 || shoot_control .return_back_flag == 1000)
				{
					shoot_control .shoot_mode = SHOOT_READY_BULLET;
					shoot_control .return_back_flag = 0;
				}
		}
		
		//Ħ���ֵ��
		CAN_cmd_firc(shoot_control.fric_current1,shoot_control.fric_current2,0,0);
		//���ز����������ֵ
   return shoot_control.given_current;//���ص���������ǵ��̲��������ġ�
}

/**
  * @brief          ���״̬�����ã�ң�����ϲ�һ�ο��������ϲ��رգ��²�1�η���1�ţ�һֱ�����£���������䣬����3min׼��ʱ�������ӵ�
  *
	* @param[in]      void
  * @retval         void
  */
static void shoot_set_mode(void)
{
    shoot_control .last_shoot_mode = shoot_control.shoot_mode;
	static int8_t last_s = RC_SW_UP;

    //�ϲ��жϣ� һ�ο������ٴιر�  ������״̬����ΪĦ���ּ���
    if ((switch_is_up(shoot_control.shoot_rc->rc.s[SHOOT_RC_MODE_CHANNEL]) && !switch_is_up(last_s) && shoot_control.shoot_mode == SHOOT_STOP))
    {
        shoot_control.shoot_mode = SHOOT_READY_FRIC;
    }
    else if(shoot_control.shoot_mode == SHOOT_READY_FRIC && fabs(shoot_control.shoot_fric1_measure->speed_rpm)<500 && fabs(shoot_control.shoot_fric1_measure->speed_rpm) < 500)
    {
        shoot_control .shoot_ready_fric_time ++;
        if(shoot_control .shoot_ready_fric_time  > 1000)
        {
            shoot_control .shoot_mode = SHOOT_FRIC_STOP;
        }
    }
    else if ((switch_is_up(shoot_control.shoot_rc->rc.s[SHOOT_RC_MODE_CHANNEL]) && !switch_is_up(last_s) && shoot_control.shoot_mode != SHOOT_STOP))
    {
        shoot_control.shoot_mode = SHOOT_STOP;
    }

    //�����е��� ����ʹ�ü��̿���Ħ���� ��SHOOT_ON_KEYBOARD��Ӧ���Ǽ����ϵ�Q
    if (switch_is_mid(shoot_control.shoot_rc->rc.s[SHOOT_RC_MODE_CHANNEL]) && (shoot_control.shoot_rc->key.v & SHOOT_ON_KEYBOARD) && shoot_control.shoot_mode == SHOOT_STOP)
    {
        shoot_control.shoot_mode = SHOOT_READY_FRIC;
    }
    //�����е��� ����ʹ�ü��̹ر�Ħ���� //Ҳ��һ���궨��
    else if (switch_is_mid(shoot_control.shoot_rc->rc.s[SHOOT_RC_MODE_CHANNEL]) && (shoot_control.shoot_rc->key.v & SHOOT_OFF_KEYBOARD) && shoot_control.shoot_mode != SHOOT_STOP)
    {
        shoot_control.shoot_mode = SHOOT_STOP;
    }
		//��λ����λ�����У���Ħ���ּ��ٵ�����ٶȡ�ģʽ���ó�װ��
    if(shoot_control.shoot_mode == SHOOT_READY_FRIC && fabs(shoot_control.shoot_fric1_measure->speed_rpm) > fabs(FRIC_SPEED+50) && shoot_control.shoot_fric2_measure->speed_rpm > -FRIC_SPEED-50)
    {
        shoot_control.shoot_mode = SHOOT_READY_BULLET;
    }
		//װ����ϣ�΢�����ر����£���״̬���ó�׼�������΢������û�б����¾�һֱ��װ����״̬
    else if(shoot_control.shoot_mode == SHOOT_READY_BULLET && shoot_control.key == SWITCH_TRIGGER_ON)
    {
        shoot_control.shoot_mode = SHOOT_READY;
    }
    else if(shoot_control.shoot_mode == SHOOT_READY && shoot_control.key == SWITCH_TRIGGER_OFF)//λ������û�б�����
    {
        shoot_control.shoot_mode = SHOOT_READY_BULLET;
    }
    else if(shoot_control.shoot_mode == SHOOT_READY)
    {
        //�²�һ�λ�����갴��һ�Σ��������״̬����!switch_is_down(last_s)ȷ����һ��״̬���ǿ������¡�
        if ((switch_is_down(shoot_control.shoot_rc->rc.s[SHOOT_RC_MODE_CHANNEL]) && !switch_is_down(last_s)) || (shoot_control.press_l && shoot_control.last_press_l == 0) || (shoot_control.press_r && shoot_control.last_press_r == 0))
        {
			shoot_control.move_flag = 0;
            shoot_control.shoot_mode = SHOOT_BULLET;
        }
    }
    else if(shoot_control.shoot_mode == SHOOT_DONE)		//SHOOT_DONEת����ģʽ��Ψһ;��
    {
        if(shoot_control.key == SWITCH_TRIGGER_OFF)//��¼΢���������µ�ʱ��
        {
            shoot_control.key_time++;
            if(shoot_control.key_time > SHOOT_DONE_KEY_OFF_TIME_time)	//��ֹ�󴥷�
            {
                shoot_control.key_time = 0;
                shoot_control.shoot_mode = SHOOT_READY_BULLET;
            }
        }
        else
        {
            shoot_control.key_time = 0;
            shoot_control.shoot_mode = SHOOT_READY;//SHOOT_BULLET;
        }
    }
    
    //��������
    if(shoot_control.shoot_rc->key.v != KEY_PRESSED_OFFSET_SHIFT)//shift�ر��������ƣ�ֻ��һֱ���Ų�û���������ƣ�
    {
        get_shoot_heat0_limit_and_heat0(&shoot_control.heat_limit, &shoot_control.heat);
        if(!toe_is_error(REFEREE_TOE) && (shoot_control.heat + SHOOT_HEAT_REMAIN_VALUE > shoot_control.heat_limit))
        {
            if(shoot_control.shoot_mode == SHOOT_BULLET)
            {
                shoot_control.shoot_mode =SHOOT_READY;
            }
        }
    }
    //��ʽ�����޸Ĵ˴�
    //�����̨״̬�� ����״̬���͹ر����
    if (gimbal_cmd_to_shoot_stop())
    {
        shoot_control.shoot_mode = SHOOT_STOP;
    }

    last_s = shoot_control.shoot_rc->rc.s[SHOOT_RC_MODE_CHANNEL];
}
/**
  * @brief          ������ݸ���
  * @param[in]      void
  * @retval         void
  */
static void shoot_feedback_update(void)
{

	if(shoot_control .last_shoot_mode != shoot_control.shoot_mode)
	{
		PID_clear(&shoot_control .trigger_motor_angle_pid);
		PID_clear(&shoot_control .trigger_motor_speed_pid);
	}
	  //��¼��һ��
	shoot_control .last_angle = shoot_control .angle;
	
    static fp32 speed_fliter_1 = 0.0f;
    static fp32 speed_fliter_2 = 0.0f;
    static fp32 speed_fliter_3 = 0.0f;

    //�����ֵ���ٶ��˲�һ��
    static const fp32 fliter_num[3] = {1.725709860247969f, -0.75594777109163436f, 0.030237910843665373f};

    //���׵�ͨ�˲�
    speed_fliter_1 = speed_fliter_2;
    speed_fliter_2 = speed_fliter_3;
    speed_fliter_3 = speed_fliter_2 * fliter_num[0] + speed_fliter_1 * fliter_num[1] + (shoot_control.shoot_motor_measure->speed_rpm /** MOTOR_RPM_TO_SPEED)*/ * fliter_num[2]);
    shoot_control.rpm_speed = speed_fliter_3;
		
 
		
	//���Ȧ�����ã� ��Ϊ�������תһȦ�� �������ת 19Ȧ������������ݴ������������ݣ����ڿ��������Ƕ�
	//shoot_control.ecd_count�������������ݣ����⼸������ˣ�
    if (shoot_control.shoot_motor_measure->ecd - shoot_control.shoot_motor_measure->last_ecd > HALF_ECD_RANGE)
    {
        shoot_control.ecd_Sum--;
    }
    else if (shoot_control.shoot_motor_measure->ecd - shoot_control.shoot_motor_measure->last_ecd < -HALF_ECD_RANGE)
    {
        shoot_control.ecd_Sum++;
    }
			
	//shoot_control .angle  = shoot_control .ecd_Sum * MOTOR_ECD_TO_ANGLE_3508;//�������ʼλ�õ����ڵ�������ת���ĽǶ�
    //���������Ƕ�
    shoot_control.angle = (shoot_control.ecd_Sum * ECD_RANGE + shoot_control.shoot_motor_measure->ecd) * MOTOR_ECD_TO_ANGLE_3508;
    
	//΢������
    shoot_control.key = BUTTEN_TRIG_PIN;//΢�����ر�������1����λ��0��
	shoot_control.key_first = BUTTEN_TRIG_PIN;
    //��갴��
    shoot_control.last_press_l = shoot_control.press_l;
    shoot_control.last_press_r = shoot_control.press_r;
    shoot_control.press_l = shoot_control.shoot_rc->mouse.press_l;
    shoot_control.press_r = shoot_control.shoot_rc->mouse.press_r;
    //������ʱ����ʱ����Ҫ��
//    if (shoot_control.press_l)
//    {
//        if (shoot_control.press_l_time < PRESS_LONG_TIME)
//        {
//            shoot_control.press_l_time++;
//        }
//    }
//    else
//    {
//        shoot_control.press_l_time = 0;
//    }

//    if (shoot_control.press_r)
//    {
//        if (shoot_control.press_r_time < PRESS_LONG_TIME)
//        {
//            shoot_control.press_r_time++;
//        }
//    }
//    else
//    {
//        shoot_control.press_r_time = 0;
//    }

    //��������µ�ʱ���ʱ���������䲻��Ҫ��
//    if (shoot_control.shoot_mode != SHOOT_STOP && switch_is_down(shoot_control.shoot_rc->rc.s[SHOOT_RC_MODE_CHANNEL]))
//    {

//        if (shoot_control.rc_s_time < RC_S_LONG_TIME)
//        {
//            shoot_control.rc_s_time++;
//        }
//    }
//    else
//    {
//        shoot_control.rc_s_time = 0;
//    }

    //����Ҽ����¼���Ħ���֣�ʹ�������������� �Ҽ��������
//    static uint16_t up_time = 0;
//    if (shoot_control.press_r)
//    {
//        up_time = UP_ADD_TIME;
//    }

//    if (up_time > 0)
//    {
//        shoot_control.fric1_ramp.max_value = FRIC_UP;
//        shoot_control.fric2_ramp.max_value = -FRIC_UP;
//        up_time--;
//    }
//    else
//    {
//        shoot_control.fric1_ramp.max_value = FRIC_DOWN;
//        shoot_control.fric2_ramp.max_value = -FRIC_DOWN;
//    }


}
//��������
static void trigger_motor_turn_back(void)
{

				if(fabs(shoot_control.rpm_speed )< 0.00000000001)
				{
					shoot_control .return_back_flag  ++;
					if(shoot_control .return_back_flag > BAKE_TIME_OVER)
					{
						shoot_control .return_back_flag = BAKE_TIME_OVER;
						shoot_control.shoot_mode = SHOOT_BULLET_STOP;
					}
					
				}
}

/**
  * @brief          ������ƣ����Ʋ�������Ƕȣ����һ�η���
  * @param[in]      void
  * @retval         void
  */
static void shoot_bullet_control(void)
{

    if(shoot_control.key == SWITCH_TRIGGER_OFF)//��⵽λ�������ɿ������ж�Ϊ�Ѿ���ɷ��䡣
    {

        shoot_control.shoot_mode = SHOOT_DONE;
				//shoot_control.rpm_speed = 0;
    }
	
}

