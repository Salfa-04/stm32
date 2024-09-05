#include "referee.h"
#include "string.h"
#include "stdio.h"
#include "CRC8_CRC16.h"
#include "protocol.h"
#include "chassis_behaviour.h"
#include "usart.h"
#include "shoot.h"
#include "arm_math.h"

#define SCREEN_WIDTH 1080
#define SCREEN_LENGTH 1920
//������ID���
#define RED_Hero_1 1
#define RED_Hero_1_Client 0x0101
#define BLUE_Hero_1 101
#define BLUE_Hero_1_Client 0x0165
ext_student_interactive_header_data_t custom_grapic_draw;			//�Զ���ͼ�����



frame_header_struct_t referee_receive_header;//����frame_header
frame_header_struct_t referee_send_header;

ext_game_state_t game_state;
ext_game_result_t game_result;
ext_game_robot_HP_t game_robot_HP_t;//������Ѫ��

ext_event_data_t field_event;//�벹��վ�����������й�
ext_supply_projectile_action_t supply_projectile_action_t;//Ӣ�۲���Ҫ������//�벹���й�//������
ext_supply_projectile_booking_t supply_projectile_booking_t;//�Ѿ�ɾ��//�벹���й�//�о�������ܻ�������
ext_referee_warning_t referee_warning_t;//��Υ�洦���й�


ext_game_robot_state_t robot_state;//�����˵ȼ����ݷ���
ext_power_heat_data_t power_heat_data_t;//��������
ext_game_robot_pos_t game_robot_pos_t;//������λ������
ext_buff_musk_t buff_musk_t;//�ڲ���ϵͳ�ֲ�δ�ҵ�
aerial_robot_energy_t robot_energy_t;//���л�����
ext_robot_hurt_t robot_hurt_t;//��Ѫ����
ext_shoot_data_t shoot_data_t;//�����Ѿ��޸�//������
ext_bullet_remaining_t bullet_remaining_t;//�ֲ�δ�ҵ�
ext_student_interactive_data_t student_interactive_data_t;




void init_referee_struct_data(void)
{
    memset(&referee_receive_header, 0, sizeof(frame_header_struct_t));
    memset(&referee_send_header, 0, sizeof(frame_header_struct_t));

    memset(&game_state, 0, sizeof(ext_game_state_t));
    memset(&game_result, 0, sizeof(ext_game_result_t));
    memset(&game_robot_HP_t, 0, sizeof(ext_game_robot_HP_t));//������Ѫ��


    memset(&field_event, 0, sizeof(ext_event_data_t));
    memset(&supply_projectile_action_t, 0, sizeof(ext_supply_projectile_action_t));
    memset(&supply_projectile_booking_t, 0, sizeof(ext_supply_projectile_booking_t));
    memset(&referee_warning_t, 0, sizeof(ext_referee_warning_t));


    memset(&robot_state, 0, sizeof(ext_game_robot_state_t));
    memset(&power_heat_data_t, 0, sizeof(ext_power_heat_data_t));
    memset(&game_robot_pos_t, 0, sizeof(ext_game_robot_pos_t));
    memset(&buff_musk_t, 0, sizeof(ext_buff_musk_t));
    memset(&robot_energy_t, 0, sizeof(aerial_robot_energy_t));
    memset(&robot_hurt_t, 0, sizeof(ext_robot_hurt_t));
    memset(&shoot_data_t, 0, sizeof(ext_shoot_data_t));
    memset(&bullet_remaining_t, 0, sizeof(ext_bullet_remaining_t));


    memset(&student_interactive_data_t, 0, sizeof(ext_student_interactive_data_t));

}

void referee_data_solve(uint8_t *frame)
{
    uint16_t cmd_id = 0;

    uint8_t index = 0;

    memcpy(&referee_receive_header, frame, sizeof(frame_header_struct_t));

    index += sizeof(frame_header_struct_t);

    memcpy(&cmd_id, frame + index, sizeof(uint16_t));
    index += sizeof(uint16_t);

    switch (cmd_id)
    {
        case GAME_STATE_CMD_ID:
        {
            memcpy(&game_state, frame + index, sizeof(ext_game_state_t));
        }
        break;
        case GAME_RESULT_CMD_ID:
        {
            memcpy(&game_result, frame + index, sizeof(game_result));
        }
        break;
        case GAME_ROBOT_HP_CMD_ID:
        {
            memcpy(&game_robot_HP_t, frame + index, sizeof(ext_game_robot_HP_t));
        }
        break;


        case FIELD_EVENTS_CMD_ID:
        {
            memcpy(&field_event, frame + index, sizeof(field_event));
        }
        break;
        case SUPPLY_PROJECTILE_ACTION_CMD_ID:
        {
            memcpy(&supply_projectile_action_t, frame + index, sizeof(supply_projectile_action_t));
        }
        break;
        case SUPPLY_PROJECTILE_BOOKING_CMD_ID:
        {
            memcpy(&supply_projectile_booking_t, frame + index, sizeof(supply_projectile_booking_t));
        }
        break;
        case REFEREE_WARNING_CMD_ID:
        {
            memcpy(&referee_warning_t, frame + index, sizeof(ext_referee_warning_t));
        }
        break;

        case ROBOT_STATE_CMD_ID:
        {
            memcpy(&robot_state, frame + index, sizeof(robot_state));
        }
        break;
        case POWER_HEAT_DATA_CMD_ID:
        {
            memcpy(&power_heat_data_t, frame + index, sizeof(power_heat_data_t));
        }
        break;
        case ROBOT_POS_CMD_ID:
        {
            memcpy(&game_robot_pos_t, frame + index, sizeof(game_robot_pos_t));
        }
        break;
        case BUFF_MUSK_CMD_ID:
        {
            memcpy(&buff_musk_t, frame + index, sizeof(buff_musk_t));
        }
        break;
        case AERIAL_ROBOT_ENERGY_CMD_ID:
        {
            memcpy(&robot_energy_t, frame + index, sizeof(robot_energy_t));
        }
        break;
        case ROBOT_HURT_CMD_ID:
        {
            memcpy(&robot_hurt_t, frame + index, sizeof(robot_hurt_t));
        }
        break;
        case SHOOT_DATA_CMD_ID:
        {
            memcpy(&shoot_data_t, frame + index, sizeof(shoot_data_t));
        }
        break;
        case BULLET_REMAINING_CMD_ID:
        {
            memcpy(&bullet_remaining_t, frame + index, sizeof(ext_bullet_remaining_t));
        }
        break;
        case STUDENT_INTERACTIVE_DATA_CMD_ID:
        {
            memcpy(&student_interactive_data_t, frame + index, sizeof(student_interactive_data_t));
        }
        break;
        default:
        {
            break;
        }
    }
}

void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer)
{
    *power = power_heat_data_t.chassis_power;
    *buffer = power_heat_data_t.chassis_power_buffer;

}

uint8_t get_robot_id(void)
{
    return robot_state.robot_id;
}

void get_shoot_heat0_limit_and_heat0(uint16_t *heat0_limit, uint16_t *heat0)
{
    *heat0_limit = robot_state.shooter_heat0_cooling_limit;//��������
    *heat0 = power_heat_data_t.shooter_42mm_heat;//
}

//void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1)
//{
//    *heat1_limit = robot_state.shooter_heat1_cooling_limit;
//    *heat1 = power_heat_data_t.shooter_heat1;
//}

ext_student_interactive_header_data_one_t  line_1;
void init_send_cline1_aim(void)
{
//��ʼ��ͼ�����ݱ���
	//�Զ���ͼ�λ���
	{
		line_1.data_cmd_id=0x0101;  //0x0101����һ��ͼ�� 0x0102��������ͼ�� 0x0103�������ͼ��  0x0104 �����߸�ͼ�Σ�����ID����ѯ����ϵͳ�ֲᣩ

		line_1.sender_ID=get_robot_id();	//��ȡ������ID
		
        if(line_1.sender_ID == RED_Hero_1)
		line_1.receiver_ID=RED_Hero_1_Client;			//������ID�������ֿͻ���ID
        else if(line_1.sender_ID == BLUE_Hero_1)
		line_1.receiver_ID=BLUE_Hero_1_Client;
		//�Զ���ͼ������
		{
			line_1.graphic_custom1.graphic_name[0] = 97;
			line_1.graphic_custom1.graphic_name[1] = 97;
			line_1.graphic_custom1.graphic_name[2] = 0;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			line_1.graphic_custom1.operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			line_1.graphic_custom1.graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			line_1.graphic_custom1.layer=3;//ͼ����
			line_1.graphic_custom1.color=1;//��ɫ
			line_1.graphic_custom1.start_angle=0;
			line_1.graphic_custom1.end_angle=0;
			line_1.graphic_custom1.width=1;
			line_1.graphic_custom1.start_x=SCREEN_LENGTH/2;
			line_1.graphic_custom1.start_y=SCREEN_WIDTH/2+250;
			line_1.graphic_custom1.end_x=SCREEN_LENGTH/2;
			line_1.graphic_custom1.end_y=SCREEN_WIDTH/2-250;
			//line_1.graphic_custom1.radius=0;
		}
    }
}

#define ADD 40
uint32_t Y_ADD = ADD;
void init_send_cline_aim(void)
{
    
    
    //��ʼ��ͼ�����ݱ���
	//�Զ���ͼ�λ���
	{
		custom_grapic_draw.data_cmd_id=0x0104;  //0x0101����һ��ͼ�� 0x0102��������ͼ�� 0x0103�������ͼ��  0x0104 �����߸�ͼ�Σ�����ID����ѯ����ϵͳ�ֲᣩ

		custom_grapic_draw.sender_ID=get_robot_id();	//��ȡ������ID
		
        if(custom_grapic_draw.sender_ID == RED_Hero_1)
		custom_grapic_draw.receiver_ID=RED_Hero_1_Client;			//������ID�������ֿͻ���ID
        else if(custom_grapic_draw.sender_ID == BLUE_Hero_1)
		custom_grapic_draw.receiver_ID=BLUE_Hero_1_Client;
        //����װ�װ巴��
        
		//�Զ���ͼ������
		{
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 0;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer=3;//ͼ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].color=1;//��ɫ
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].width=2;
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x=SCREEN_LENGTH/2 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y=SCREEN_WIDTH/2+250+Y_ADD;
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x=SCREEN_LENGTH/2 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y=SCREEN_WIDTH/2-250+Y_ADD;
			custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius=0;
		}
	//����ͼ��2
		{//���������׼��
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[0] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[1] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 1;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=3;//ͼ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=4;//��ɫ
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].width=2;
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_x=SCREEN_LENGTH/2-60 - 10;//SCREEN_LENGTH/2-150;
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_y=SCREEN_WIDTH/2-250+Y_ADD;//SCREEN_WIDTH/2+Y_ADD;
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_x=SCREEN_LENGTH/2+60 - 10;//SCREEN_LENGTH/2+150;
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_y=SCREEN_WIDTH/2-250+Y_ADD;//SCREEN_WIDTH/2+Y_ADD;
			custom_grapic_draw.graphic_custom.grapic_data_struct[1].radius=0;
		}
	
		{
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[0] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[1] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[2] = 2;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].layer=3;//ͼ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].color=8;//��ɫ
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].width=1;
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_x=SCREEN_LENGTH/2-110 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_y=SCREEN_WIDTH/2-182;
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_x=SCREEN_LENGTH/2+110 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_y=SCREEN_WIDTH/2-182;
			custom_grapic_draw.graphic_custom.grapic_data_struct[2].radius=0;
		}
	
		{
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[0] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[1] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[2] = 3;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].layer=3;//ͼ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].color=1;//��ɫ
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].width=1;
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_x=SCREEN_LENGTH/2-80 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_y=SCREEN_WIDTH/2-80+Y_ADD;
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_x=SCREEN_LENGTH/2+80 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_y=SCREEN_WIDTH/2-80+Y_ADD;
			custom_grapic_draw.graphic_custom.grapic_data_struct[3].radius=0;
		}		
		//��׼�����±ߵ���
        {
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[0] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[1] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[2] = 4;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].layer=3;//ͼ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].color=1;//��ɫ
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].width=1;
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_x=SCREEN_LENGTH/2-60 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_y=SCREEN_WIDTH/2-160+Y_ADD-15;
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_x=SCREEN_LENGTH/2+60 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_y=SCREEN_WIDTH/2-160+Y_ADD-15;
			custom_grapic_draw.graphic_custom.grapic_data_struct[4].radius=0;
		}
		{
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[0] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[1] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[2] = 5;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_tpye=7;//ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].layer=3;//ͼ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].color=1;//��ɫ
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].width=1;
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_x=SCREEN_LENGTH/2-40 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_y=SCREEN_WIDTH/2+160+Y_ADD;
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_x=SCREEN_LENGTH/2+40 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_y=SCREEN_WIDTH/2+160+Y_ADD;
			custom_grapic_draw.graphic_custom.grapic_data_struct[5].radius=0;
		}
		{
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[0] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[1] = 97;
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[2] = 6;//ͼ�������ظ����һͼ����ʾ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_tpye=2;//ͼ�����ͣ�2Ϊ��Բ
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].layer=3;//ͼ����
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].color=2;//��ɫ
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_angle=0;
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].width=2;
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_x=SCREEN_LENGTH/2 - 10;
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_y=SCREEN_WIDTH/2+Y_ADD;
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_x=0;//SCREEN_LENGTH/2;
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_y=0;//SCREEN_WIDTH/2;
			custom_grapic_draw.graphic_custom.grapic_data_struct[6].radius=50;
		}
	}
}

//װ�װ����˷���
ext_student_interactive_header_data_2_t armor_id_grapic_draw;			//�Զ���ͼ�����
#define CENTER_X SCREEN_LENGTH-SCREEN_LENGTH/4
#define CENTER_Y SCREEN_WIDTH/2+150
#define R 50
#define x 50
#define y 50
extern chassis_move_t chassis_move;
uint32_t color[4]= {4,3,1,2};

void init_send_armor_id_aim(uint32_t update)
{
    
    {
		armor_id_grapic_draw.data_cmd_id=0x0103;  //0x0101����һ��ͼ�� 0x0102��������ͼ�� 0x0103�������ͼ��  0x0104 �����߸�ͼ�Σ�����ID����ѯ����ϵͳ�ֲᣩ

		armor_id_grapic_draw.sender_ID=get_robot_id();	//��ȡ������ID
		
        if(armor_id_grapic_draw.sender_ID == RED_Hero_1)
		armor_id_grapic_draw.receiver_ID=RED_Hero_1_Client;			//������ID�������ֿͻ���ID
        else if(armor_id_grapic_draw.sender_ID == BLUE_Hero_1)
		armor_id_grapic_draw.receiver_ID=BLUE_Hero_1_Client;
		//�Զ���ͼ������
		{
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].figure_name[0] = 10;
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].figure_name[1] = 10;
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].figure_name[2] = 0;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].operate_tpye=update;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].figure_tpye=2;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].layer=4;//ͼ����
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].color=7;//��ɫ
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].details_a=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].details_b=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].width=5;
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].start_x=CENTER_X;
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].start_y=CENTER_Y;
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].details_c=R;
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].details_d=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[0].details_e=0;
		}
	//����ͼ��2
		{
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].figure_name[0] = 10;
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].figure_name[1] = 10;
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].figure_name[2] = 1;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].operate_tpye=update;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].figure_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].layer=4;//ͼ����
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].color=color[0];//��ɫ
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].details_a=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].details_b=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].width=3;
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].start_x=CENTER_X;
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].start_y=CENTER_Y;
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].details_c=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].details_d=CENTER_X+R*(arm_sin_f32(-chassis_move.chassis_yaw_motor->relative_angle));
			armor_id_grapic_draw.graphic_custom.interaction_figure[1].details_e=CENTER_Y+R*(arm_cos_f32(-chassis_move.chassis_yaw_motor->relative_angle));
		}
	
		{
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].figure_name[0] = 10;
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].figure_name[1] = 10;
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].figure_name[2] = 2;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].operate_tpye=update;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].figure_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].layer=4;//ͼ����
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].color=color[1];//��ɫ
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].details_a=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].details_b=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].width=3;
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].start_x=CENTER_X;
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].start_y=CENTER_Y;
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].details_c=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].details_d=CENTER_X+R*(arm_sin_f32(-chassis_move.chassis_yaw_motor->relative_angle + PI*0.5f));
			armor_id_grapic_draw.graphic_custom.interaction_figure[2].details_e=CENTER_Y+R*(arm_cos_f32(-chassis_move.chassis_yaw_motor->relative_angle + PI*0.5f));
		}
	
		{
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].figure_name[0] = 10;
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].figure_name[1] = 10;
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].figure_name[2] = 3;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].operate_tpye=update;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].figure_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].layer=4;//ͼ����
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].color=color[2];//��ɫ
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].details_a=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].details_b=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].width=3;
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].start_x=CENTER_X;
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].start_y=CENTER_Y;
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].details_c=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].details_d=CENTER_X+R*(arm_sin_f32(-chassis_move.chassis_yaw_motor->relative_angle+PI));
			armor_id_grapic_draw.graphic_custom.interaction_figure[3].details_e=CENTER_Y+R*(arm_cos_f32(-chassis_move.chassis_yaw_motor->relative_angle+PI));
		}		
		{
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].figure_name[0] = 10;
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].figure_name[1] = 10;
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].figure_name[2] = 4;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].operate_tpye=update;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].figure_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].layer=4;//ͼ����
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].color=color[3];//��ɫ
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].details_a=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].details_b=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].width=3;
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].start_x=CENTER_X;
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].start_y=CENTER_Y;
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].details_c=0;
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].details_d=CENTER_X+R*(arm_sin_f32(-chassis_move.chassis_yaw_motor->relative_angle+1.5f*PI));
			armor_id_grapic_draw.graphic_custom.interaction_figure[4].details_e=CENTER_Y+R*(arm_cos_f32(-chassis_move.chassis_yaw_motor->relative_angle+1.5f*PI));
		}
    }
}
    

//��ʻ���룺����ײ��
ext_student_interactive_header_data_3_t boundary_line_grapic_draw;			//�Զ���ͼ�����
#define LINE_X SCREEN_LENGTH/2
#define LINE_Y 0


void init_send_boundary_line(uint32_t update)
{
    
    {
		boundary_line_grapic_draw.data_cmd_id=0x0102;  //0x0101����һ��ͼ�� 0x0102��������ͼ�� 0x0103�������ͼ��  0x0104 �����߸�ͼ�Σ�����ID����ѯ����ϵͳ�ֲᣩ

		boundary_line_grapic_draw.sender_ID=get_robot_id();	//��ȡ������ID
		
        if(boundary_line_grapic_draw.sender_ID == RED_Hero_1)
		boundary_line_grapic_draw.receiver_ID=RED_Hero_1_Client;			//������ID�������ֿͻ���ID
        else if(boundary_line_grapic_draw.sender_ID == BLUE_Hero_1)
		boundary_line_grapic_draw.receiver_ID=BLUE_Hero_1_Client;
		//�Զ���ͼ������
		{
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].figure_name[0] = 20;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].figure_name[1] = 20;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].figure_name[2] = 0;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].operate_tpye=update;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].figure_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].layer=4;//ͼ����
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].color=1;//��ɫ
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].details_a=0;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].details_b=0;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].width=3;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].start_x=LINE_X/2;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].start_y=LINE_Y;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].details_c=R;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].details_d=LINE_X - LINE_X/4;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[0].details_e=LINE_Y + SCREEN_WIDTH*0.4;
		}
	//����ͼ��2
		{
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].figure_name[0] = 20;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].figure_name[1] = 20;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].figure_name[2] = 1;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].operate_tpye=update;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].figure_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].layer=4;//ͼ����
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].color=1;//��ɫ
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].details_a=0;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].details_b=0;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].width=3;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].start_x=LINE_X*1.5f;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].start_y=LINE_Y;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].details_c=0;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].details_d=LINE_X + LINE_X/4;
			boundary_line_grapic_draw.graphic_custom.interaction_figure[1].details_e=LINE_Y + SCREEN_WIDTH*0.4;
		}

    }
}

//pitch״̬
//��ʻ��أ�����ײ��
//װ�װ����˷���
ext_student_interactive_header_data_3_t boundary_pitch_line_grapic_draw;			//�Զ���ͼ�����
#define LINE_X SCREEN_LENGTH/2
#define LINE_Y 0


void init_send_pitch_line(uint32_t update)
{
    
    {
		boundary_pitch_line_grapic_draw.data_cmd_id=0x0102;  //0x0101����һ��ͼ�� 0x0102��������ͼ�� 0x0103�������ͼ��  0x0104 �����߸�ͼ�Σ�����ID����ѯ����ϵͳ�ֲᣩ

		boundary_pitch_line_grapic_draw.sender_ID=get_robot_id();	//��ȡ������ID
		
        if(boundary_pitch_line_grapic_draw.sender_ID == RED_Hero_1)
		boundary_pitch_line_grapic_draw.receiver_ID=RED_Hero_1_Client;			//������ID�������ֿͻ���ID
        else if(boundary_pitch_line_grapic_draw.sender_ID == BLUE_Hero_1)
		boundary_pitch_line_grapic_draw.receiver_ID=BLUE_Hero_1_Client;
		//�Զ���ͼ������
		{
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].figure_name[0] = 30;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].figure_name[1] = 30;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].figure_name[2] = 0;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].figure_tpye=4;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].layer=4;//ͼ����
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].color=1;//��ɫ
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].details_a=0;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].details_b=180;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].width=2;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].start_x=CENTER_X;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].start_y=CENTER_Y+120;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].details_c=0;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].details_d=50;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[0].details_e=50;
		}
	//����ͼ��2
		{
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].figure_name[0] = 30;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].figure_name[1] = 30;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].figure_name[2] = 1;//ͼ����
			//���������ֽڴ������ͼ����������ͼ�������������ж���
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].operate_tpye=update;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].figure_tpye=0;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].layer=4;//ͼ����
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].color=1;//��ɫ
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].details_a=0;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].details_b=0;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].width=3;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].start_x=CENTER_X;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].start_y=CENTER_Y+120;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].details_c=0;
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].details_d=CENTER_X + R*(arm_cos_f32(-chassis_move.chassis_pitch_motor->relative_angle));
			boundary_pitch_line_grapic_draw.graphic_custom.interaction_figure[1].details_e=CENTER_Y+100 +  R*(-arm_sin_f32(-chassis_move.chassis_pitch_motor->relative_angle));
		}

    }
}

ext_student_interactive_str_header_data_t ext_str;
unsigned char str_sendbuf[20] = {0};
unsigned char init_sta[20] = "nomarl"; 
extern chassis_behaviour_e chassis_behaviour_mode;
chassis_behaviour_e chassis_behaviour_last_mode;
uint8_t char_mode = 1;
void init_send_char(uint32_t update)
{
	ext_str.data_cmd_id = 0x0110;
	ext_str.sender_ID=get_robot_id();	//��ȡ������ID
		
	if(ext_str.sender_ID == RED_Hero_1)
		ext_str.receiver_ID=RED_Hero_1_Client;			//������ID�������ֿͻ���ID
	else if(ext_str.sender_ID == BLUE_Hero_1)
		ext_str.receiver_ID=BLUE_Hero_1_Client;
//	if(chassis_behaviour_mode != chassis_behaviour_last_mode)
//    {
//        char_mode = 2;
//    }

	//�����ַ�
	{
		ext_str.char_custom.grapic_data_struct.graphic_name[0] = 2;
		ext_str.char_custom.grapic_data_struct.graphic_name[1] = 2;
		ext_str.char_custom.grapic_data_struct.graphic_name[2] = 2;//ͼ����
		//���������ֽڴ������ͼ����������ͼ�������������ж���
		ext_str.char_custom.grapic_data_struct.operate_tpye=update;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		ext_str.char_custom.grapic_data_struct.graphic_tpye=7;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
		ext_str.char_custom.grapic_data_struct.layer=1;//ͼ����
		ext_str.char_custom.grapic_data_struct.color=1;//��ɫ
		ext_str.char_custom.grapic_data_struct.start_angle=20;
		ext_str.char_custom.grapic_data_struct.end_angle=30;
		ext_str.char_custom.grapic_data_struct.width=4;
		ext_str.char_custom.grapic_data_struct.start_x=SCREEN_LENGTH/10 + 110;
		ext_str.char_custom.grapic_data_struct.start_y=SCREEN_WIDTH/1.5 - 100;
		ext_str.char_custom.grapic_data_struct.end_x=0;
		ext_str.char_custom.grapic_data_struct.end_y=0;
		ext_str.char_custom.grapic_data_struct.radius=0;
	}
	
	if(chassis_behaviour_mode == CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW)
    {
		//strcpy((char*)init_sta,"normal      ");
        strcpy((char*)ext_str.char_custom.data,"normal      ");
    }
	if(chassis_behaviour_mode == CHASSIS_VECTOR_SMALL_GYRO)
    {
        //strcpy((char*)init_sta,"small_gyro  ");
        strcpy((char*)ext_str.char_custom.data,"small_gyro  ");
    }
    if(chassis_behaviour_mode != CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW && chassis_behaviour_mode != CHASSIS_VECTOR_SMALL_GYRO)
    {
		//strcpy((char*)init_sta,"disable     ");
        strcpy((char*)ext_str.char_custom.data,"disable     ");
	}
//    chassis_behaviour_last_mode = chassis_behaviour_mode;
//	sprintf((char *)str_sendbuf,"%s\n",init_sta);
//	memcpy(ext_str.char_custom.data,str_sendbuf,sizeof(str_sendbuf));		
}

ext_student_interactive_str_header_data_t char_chassis;

void char_chassis_show(void)
{
    char_chassis.data_cmd_id = 0x0110;
    char_chassis.sender_ID=get_robot_id();
    
    if(char_chassis.sender_ID == RED_Hero_1)
	char_chassis.receiver_ID=RED_Hero_1_Client;
    //������ID�������ֿͻ���ID
	else if(char_chassis.sender_ID == BLUE_Hero_1)
	char_chassis.receiver_ID=BLUE_Hero_1_Client;
    
    char_chassis.char_custom.grapic_data_struct.graphic_name[0] = 1;
	char_chassis.char_custom.grapic_data_struct.graphic_name[1] = 1;
	char_chassis.char_custom.grapic_data_struct.graphic_name[2] = 1;//ͼ����
	//���������ֽڴ������ͼ����������ͼ�������������ж���
	char_chassis.char_custom.grapic_data_struct.operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
	char_chassis.char_custom.grapic_data_struct.graphic_tpye=7;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
	char_chassis.char_custom.grapic_data_struct.layer=1;//ͼ����
	char_chassis.char_custom.grapic_data_struct.color=1;//��ɫ
	char_chassis.char_custom.grapic_data_struct.start_angle=20;
	char_chassis.char_custom.grapic_data_struct.end_angle=30;
	char_chassis.char_custom.grapic_data_struct.width=1;
	char_chassis.char_custom.grapic_data_struct.start_x=SCREEN_LENGTH/10-20;
	char_chassis.char_custom.grapic_data_struct.start_y=SCREEN_WIDTH/1.5 - 100;
	char_chassis.char_custom.grapic_data_struct.end_x=0;
	char_chassis.char_custom.grapic_data_struct.end_y=0;
	char_chassis.char_custom.grapic_data_struct.radius=0;
    
    
    strcpy((char *)char_chassis.char_custom.data ,"chass:");
		
}


//shoot
ext_student_interactive_str_header_data_t char_shoot;

void shoot_char(void)
{
    char_shoot.data_cmd_id = 0x0110;
    char_shoot.sender_ID=get_robot_id();
    
    if(char_shoot.sender_ID == RED_Hero_1)
	char_shoot.receiver_ID=RED_Hero_1_Client;
    //������ID�������ֿͻ���ID
	else if(char_shoot.sender_ID == BLUE_Hero_1)
	char_shoot.receiver_ID=BLUE_Hero_1_Client;
    
    char_shoot.char_custom.grapic_data_struct.graphic_name[0] = 3;
	char_shoot.char_custom.grapic_data_struct.graphic_name[1] = 3;
	char_shoot.char_custom.grapic_data_struct.graphic_name[2] = 3;//ͼ����
	//���������ֽڴ������ͼ����������ͼ�������������ж���
	char_shoot.char_custom.grapic_data_struct.operate_tpye=1;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
	char_shoot.char_custom.grapic_data_struct.graphic_tpye=7;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
	char_shoot.char_custom.grapic_data_struct.layer=2;//ͼ����
	char_shoot.char_custom.grapic_data_struct.color=6;//��ɫ
	char_shoot.char_custom.grapic_data_struct.start_angle=20;
	char_shoot.char_custom.grapic_data_struct.end_angle=30;
	char_shoot.char_custom.grapic_data_struct.width=1;
	char_shoot.char_custom.grapic_data_struct.start_x=SCREEN_LENGTH/10-20;//87;
	char_shoot.char_custom.grapic_data_struct.start_y=SCREEN_WIDTH/1.5;//1500;
	char_shoot.char_custom.grapic_data_struct.end_x=0;
	char_shoot.char_custom.grapic_data_struct.end_y=0;
	char_shoot.char_custom.grapic_data_struct.radius=0;
    
    
    strcpy((char *)char_shoot.char_custom.data ,"shoot:");
    //sprintf((char *)str_send_shootbuf,"%s\n",init_shoot_sta);
    //memcpy(char_shoot.char_custom.data,str_send_shootbuf,sizeof(str_send_shootbuf));		
}
ext_student_interactive_str_header_data_t ext_shoot;
unsigned char str_send_shootbuf[20] = {0};
unsigned char init_shoot_sta[20] = "NO"; 
extern shoot_control_t shoot_control; 
uint8_t shoot_mode = 1;

void init_send_shoot_char(uint32_t update)
{
    static shoot_mode_e shoot_mode_last;
	ext_shoot.data_cmd_id = 0x0110;
	ext_shoot.sender_ID=get_robot_id();	//��ȡ������ID
		
	if(ext_shoot.sender_ID == RED_Hero_1)
		ext_shoot.receiver_ID=RED_Hero_1_Client;			//������ID�������ֿͻ���ID
	else if(ext_shoot.sender_ID == BLUE_Hero_1)
		ext_shoot.receiver_ID=BLUE_Hero_1_Client;
	
//    if(shoot_control .shoot_mode != shoot_mode_last)
//    {
//        shoot_mode = 2;
//    }
	//�����ַ�
	{
		ext_shoot.char_custom.grapic_data_struct.graphic_name[0] = 4;
		ext_shoot.char_custom.grapic_data_struct.graphic_name[1] = 4;
		ext_shoot.char_custom.grapic_data_struct.graphic_name[2] = 4;//ͼ����
		//���������ֽڴ������ͼ����������ͼ�������������ж���
		ext_shoot.char_custom.grapic_data_struct.operate_tpye=update;//ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		ext_shoot.char_custom.grapic_data_struct.graphic_tpye=7;//ͼ�����ͣ�0Ϊֱ�ߣ�1Ϊ���Σ�2Ϊ��Բ��3Ϊ��Բ��4ΪԲ��
		ext_shoot.char_custom.grapic_data_struct.layer=2;//ͼ����
		ext_shoot.char_custom.grapic_data_struct.color=6;//��ɫ
		ext_shoot.char_custom.grapic_data_struct.start_angle=20;
		ext_shoot.char_custom.grapic_data_struct.end_angle=30;
		ext_shoot.char_custom.grapic_data_struct.width=3;
		ext_shoot.char_custom.grapic_data_struct.start_x=SCREEN_LENGTH/10+110;//87;
		ext_shoot.char_custom.grapic_data_struct.start_y=SCREEN_WIDTH/1.5;//1500;
		ext_shoot.char_custom.grapic_data_struct.end_x=0;
		ext_shoot.char_custom.grapic_data_struct.end_y=0;
		ext_shoot.char_custom.grapic_data_struct.radius=0;
	}
	 
	if(shoot_control.shoot_mode == SHOOT_STOP)
    {
		//strcpy((char*)init_shoot_sta,"NO    ");
        strcpy((char *)ext_shoot.char_custom.data ,"NO         ");
    }
	if(shoot_control.shoot_mode == SHOOT_READY)
    {
		//strcpy((char*)init_shoot_sta,"OK    ");
        strcpy((char *)ext_shoot.char_custom.data ,"OK         ");
    }
	if(shoot_control .shoot_mode == SHOOT_BULLET_STOP) 
    {
		//strcpy((char*)init_shoot_sta,"kadan ");
        strcpy((char *)ext_shoot.char_custom.data ,"kadan      ");
    }
    if(shoot_control.shoot_mode !=SHOOT_STOP && shoot_control.shoot_mode !=SHOOT_READY 
                                        && shoot_control.shoot_mode !=SHOOT_BULLET_STOP)
    {
        //strcpy((char*)init_shoot_sta,"READY ");
        strcpy((char *)ext_shoot.char_custom.data ,"READY      ");
    }
    if(shoot_control .shoot_mode == SHOOT_FRIC_STOP)
    {
        //strcpy((char*)init_shoot_sta,"firc_error ");
        strcpy((char *)ext_shoot.char_custom.data ,"firc_error");
    }
	//shoot_mode_last = shoot_control .shoot_mode;
    
//	sprintf((char *)str_send_shootbuf,"%s\n",init_shoot_sta);
//	memcpy(ext_shoot.char_custom.data,str_send_shootbuf,sizeof(str_send_shootbuf));		
}


/*begin:0x0100 ɾ��ͼ��*/
ext_student_interactive_header_clear_t ext_chear;

void init_send_clear_char(uint8_t delete_type,uint8_t layer)
{
	ext_chear.data_cmd_id = 0x0100;
	ext_chear.sender_ID=get_robot_id();	//��ȡ������ID
		
	if(ext_chear.sender_ID == RED_Hero_1)
		ext_chear.receiver_ID=RED_Hero_1_Client;			//������ID�������ֿͻ���ID
	else if(ext_chear.sender_ID == BLUE_Hero_1)
		ext_chear.receiver_ID=BLUE_Hero_1_Client;
	//����ַ�
	{
        ext_chear.clear_custom .delete_type  = delete_type;
        ext_chear.clear_custom.layer = layer;
	}
			
}

/*end*/

/**
  * @brief     pack data to bottom device
  * @param[in] sof��framehearder
  * @param[in] cmd_id:  command id of data
  * @param[in] *p_data: pointer to the data to be sent
  * @param[in] len:     the data length
  */
#define MAX_SIZE          128    //�ϴ��������ĳ���
#define frameheader_len  5       //֡ͷ����
#define cmd_len          2       //�����볤��
#define crc_len          2       //CRC16У��
uint8_t seq=0;
uint16_t str_buff;
uint8_t tx_buff[MAX_SIZE];



void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len )
{
	
	uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //����֡����	

	memset(tx_buff,0,frame_length);  //�洢���ݵ���������
	
	/*****֡ͷ���*****/
	tx_buff[0] = sof;//����֡��ʼ�ֽ�
	memcpy(&tx_buff[1],(uint8_t*)&len, sizeof(len));//����֡��data�ĳ���[2��byte]
	tx_buff[3] = seq;//�����
	append_CRC8_check_sum(tx_buff,frameheader_len);  //֡ͷУ��CRC8
	str_buff = cmd_id;
	/*****��������*****/
	memcpy(&tx_buff[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
	
	/*****���ݴ��*****/
	memcpy(&tx_buff[frameheader_len+cmd_len], p_data, len);
	append_CRC16_check_sum(tx_buff,frame_length);  //һ֡����У��CRC16

	if (seq == 0xff) seq=0;
  else seq++;
	
	/*****�����ϴ�*****/
		HAL_UART_Transmit(&huart6,tx_buff,sizeof(tx_buff),0xff);

}


//shoot
uint8_t seq_shoot=0;
uint16_t str_shoot_buff;
uint8_t tx_shoot_buff[MAX_SIZE];


void referee_shoot_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len )
{	
	uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //����֡����	

	memset(tx_shoot_buff,0,frame_length);  //�洢���ݵ���������
	
	/*****֡ͷ���*****/
	tx_shoot_buff[0] = sof;//����֡��ʼ�ֽ�
	memcpy(&tx_shoot_buff[1],(uint8_t*)&len, sizeof(len));//����֡��data�ĳ���[2��byte]
	tx_shoot_buff[3] = seq;//�����
	append_CRC8_check_sum(tx_shoot_buff,frameheader_len);  //֡ͷУ��CRC8
	str_shoot_buff = cmd_id;
	/*****��������*****/
	memcpy(&tx_shoot_buff[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
	
	/*****���ݴ��*****/
	memcpy(&tx_shoot_buff[frameheader_len+cmd_len], p_data, len);
	append_CRC16_check_sum(tx_shoot_buff,frame_length);  //һ֡����У��CRC16

	if (seq == 0xff) seq=0;
  else seq++;
	
	/*****�����ϴ�*****/
		HAL_UART_Transmit(&huart6,tx_shoot_buff,sizeof(tx_shoot_buff),0xff);
}

//��׼��
uint8_t seq_shoot_line=0;
uint16_t str_shoot_line_buff;
uint8_t tx_shoot_line_buff[MAX_SIZE];
uint8_t seq1=0;

void referee_shoot_line_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len )
{	
	uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //����֡����
	memset(tx_shoot_line_buff,0,frame_length);  //�洢���ݵ���������
	/*****֡ͷ���*****/
	tx_shoot_line_buff[0] = sof;//����֡��ʼ�ֽ�
	memcpy(&tx_shoot_line_buff[1],(uint8_t*)&len, sizeof(len));//����֡��data�ĳ���[2��byte]
	tx_shoot_line_buff[3] = seq1;//�����
	append_CRC8_check_sum(tx_shoot_line_buff,frameheader_len);  //֡ͷУ��CRC8
	str_shoot_line_buff = cmd_id;
	/*****��������*****/
	memcpy(&tx_shoot_line_buff[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
	/*****���ݴ��*****/
	memcpy(&tx_shoot_line_buff[frameheader_len+cmd_len], p_data, len);
	append_CRC16_check_sum(tx_shoot_line_buff,frame_length);  //һ֡����У��CRC16
    /**/
	if (seq1 == 0xff) seq1=0;
    else seq1++;
	/*****�����ϴ�*****/
	HAL_UART_Transmit(&huart6,tx_shoot_line_buff,sizeof(tx_shoot_line_buff),0xff);
}



