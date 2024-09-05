#ifndef REFEREE_H
#define REFEREE_H

#include "main.h"

#include "protocol.h"

typedef enum
{
    RED_HERO        = 1,
    RED_ENGINEER    = 2,
    RED_STANDARD_1  = 3,
    RED_STANDARD_2  = 4,
    RED_STANDARD_3  = 5,
    RED_AERIAL      = 6,
    RED_SENTRY      = 7,
    BLUE_HERO       = 11,
    BLUE_ENGINEER   = 12,
    BLUE_STANDARD_1 = 13,
    BLUE_STANDARD_2 = 14,
    BLUE_STANDARD_3 = 15,
    BLUE_AERIAL     = 16,
    BLUE_SENTRY     = 17,
} robot_id_t;
typedef enum
{
    PROGRESS_UNSTART        = 0,
    PROGRESS_PREPARE        = 1,
    PROGRESS_SELFCHECK      = 2,
    PROGRESS_5sCOUNTDOWN    = 3,
    PROGRESS_BATTLE         = 4,
    PROGRESS_CALCULATING    = 5,
} game_progress_t;
typedef __packed struct //0001
{
    uint8_t game_type : 4;
    uint8_t game_progress : 4;
    uint16_t stage_remain_time;
} ext_game_state_t;

typedef __packed struct //0002
{
    uint8_t winner;
} ext_game_result_t;
typedef __packed struct
{
    uint16_t red_1_robot_HP;
    uint16_t red_2_robot_HP;
    uint16_t red_3_robot_HP;
    uint16_t red_4_robot_HP;
    uint16_t red_5_robot_HP;
    uint16_t red_7_robot_HP;
    uint16_t red_base_HP;
    uint16_t blue_1_robot_HP;
    uint16_t blue_2_robot_HP;
    uint16_t blue_3_robot_HP;
    uint16_t blue_4_robot_HP;
    uint16_t blue_5_robot_HP;
    uint16_t blue_7_robot_HP;
    uint16_t blue_base_HP;
} ext_game_robot_HP_t;
typedef __packed struct //0101
{
    uint32_t event_type;
} ext_event_data_t;

typedef __packed struct //0x0102
{
    uint8_t supply_projectile_id;
    uint8_t supply_robot_id;
    uint8_t supply_projectile_step;
    uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;


typedef __packed struct //0x0103//已经删除
{
    //add
//    uint8_t reserved;//保留
    //end
    uint8_t supply_projectile_id;//补
    uint8_t supply_robot_id;
    uint8_t supply_num;
} ext_supply_projectile_booking_t;

typedef __packed struct
{
    uint8_t level;
    uint8_t foul_robot_id;
} ext_referee_warning_t;
typedef __packed struct //0x0201
{
    uint8_t robot_id;
    uint8_t robot_level;
    uint16_t remain_HP;
    uint16_t max_HP;
    uint16_t shooter_heat0_cooling_rate;//机器人枪口每秒冷却值
    uint16_t shooter_heat0_cooling_limit;//枪口热量上限
    /*删除此处
    uint16_t shooter_heat1_cooling_rate;//
    uint16_t shooter_heat1_cooling_limit;//
    */
    /*add*/
    uint16_t chassis_power_limit;
//    uint16_t shooter_heat1_cooling_rate;//
//    uint16_t shooter_heat1_cooling_limit;
//    uint16_t chassis_power1_limit;
    /*end*/
    uint8_t mains_power_gimbal_output : 1;
    uint8_t mains_power_chassis_output : 1;
    uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t;

typedef __packed struct //0x0202
{
    uint16_t chassis_volt;
    uint16_t chassis_current;
    float chassis_power;
    uint16_t chassis_power_buffer;//缓冲能量
    uint16_t shooter_heat0;//第一个17mm发射机构的枪管热量
    uint16_t shooter_heat1;//第二个17mm发射机构的枪管热量
    /*add*/
    uint16_t shooter_42mm_heat;//42mm发射机构的枪管热量
    /*end*/
} ext_power_heat_data_t;

typedef __packed struct //0x0203
{
    float x;
    float y;
    float z;
    float yaw;
} ext_game_robot_pos_t;

typedef __packed struct //0x0204
{
    uint8_t power_rune_buff;
} ext_buff_musk_t;

typedef __packed struct //0x0205
{
    uint8_t energy_point;
    uint8_t attack_time;
} aerial_robot_energy_t;

typedef __packed struct //0x0206
{
    uint8_t armor_type : 4;
    uint8_t hurt_type : 4;
} ext_robot_hurt_t;

typedef __packed struct //0x0207
{
    uint8_t bullet_type;
    //add
    uint8_t shooter_number;//发射机构ID
    //end
    uint8_t bullet_freq;//射速
    float bullet_speed;//初速度
} ext_shoot_data_t;
typedef __packed struct
{
    uint8_t bullet_remaining_num;
} ext_bullet_remaining_t;
typedef __packed struct //0x0301
{
    uint16_t send_ID;
    uint16_t receiver_ID;
    uint16_t data_cmd_id;
    uint16_t data_len;
    uint8_t *data;
} ext_student_interactive_data_t;

//添加结构体
typedef __packed struct
{
uint8_t graphic_name[3];
uint32_t operate_tpye:3;
uint32_t graphic_tpye:3;
uint32_t layer:4;
uint32_t color:4;
uint32_t start_angle:9;
uint32_t end_angle:9;
uint32_t width:10;
uint32_t start_x:11;
uint32_t start_y:11;
uint32_t radius:10;
uint32_t end_x:11;
uint32_t end_y:11;
} graphic_data_struct_t;

//客户端绘制图形
typedef __packed struct
{
graphic_data_struct_t grapic_data_struct[7];//绘制图像的数量即图像数据数组的长度，但要看清楚裁判系统给的增加图形数量对应的内容ID
} ext_client_custom_graphic_t;

typedef __packed struct
{
graphic_data_struct_t grapic_data_struct;
uint8_t data[30];
} ext_client_custom_character_t;

//交互数据信息
typedef __packed struct
{
uint16_t data_cmd_id;	//数据段内容ID
uint16_t sender_ID;	//发送者ID
uint16_t receiver_ID;	//接受者ID
 ext_client_custom_character_t char_custom;//自定义字符图形数据
}ext_student_interactive_str_header_data_t;

//交互数据信息//绘制7个图形
typedef __packed struct
{
uint16_t data_cmd_id;	//数据段内容ID
uint16_t sender_ID;	    //发送者ID
uint16_t receiver_ID;	//接受者ID
ext_client_custom_graphic_t graphic_custom;//自定义图形数据
}ext_student_interactive_header_data_t;	

/*start*/
//绘制5个图形这一部分比较乱。留给下一届修改吧
typedef __packed struct 
{  
uint8_t figure_name[3];  
uint32_t operate_tpye:3;  
uint32_t figure_tpye:3;  
uint32_t layer:4;  
uint32_t color:4;  
uint32_t details_a:9; 
uint32_t details_b:9; 
uint32_t width:10;  
uint32_t start_x:11;  
uint32_t start_y:11;  
uint32_t details_c:10;  
uint32_t details_d:11;  
uint32_t details_e:11;  
}interaction_figure_t; 

typedef __packed struct 
{ 
interaction_figure_t interaction_figure[5]; 
}interaction_figure_3_t; 

typedef __packed struct
{
uint16_t data_cmd_id;	//数据段内容ID
uint16_t sender_ID;	    //发送者ID
uint16_t receiver_ID;	//接受者ID
interaction_figure_3_t graphic_custom;//自定义图形数据
}ext_student_interactive_header_data_2_t;	

/*end*/

/*start绘制两个图形*/
typedef __packed struct 
{ 
  interaction_figure_t interaction_figure[2]; 
}interaction_figure_2_t; 

typedef __packed struct
{
uint16_t data_cmd_id;	//数据段内容ID
uint16_t sender_ID;	    //发送者ID
uint16_t receiver_ID;	//接受者ID
interaction_figure_2_t graphic_custom;//自定义图形数据
}ext_student_interactive_header_data_3_t;
/*end*/
//0x0301 车间通信 发送给客户端


typedef __packed struct
{
uint16_t data_cmd_id;	//数据段内容ID
uint16_t sender_ID;	    //发送者ID
uint16_t receiver_ID;	//接受者ID
graphic_data_struct_t graphic_custom1;//自定义图形数据
}ext_student_interactive_header_data_one_t;	

/*begin:0x0100 删除图层*/

typedef __packed struct 
{ 
uint8_t delete_type; 
uint8_t layer; 
}interaction_layer_delete_t; 

//交互数据信息
typedef __packed struct
{
uint16_t data_cmd_id;	//数据段内容ID
uint16_t sender_ID;	//发送者ID
uint16_t receiver_ID;	//接受者ID
interaction_layer_delete_t clear_custom;//自定义图形数据
}ext_student_interactive_header_clear_t;	
/*end*/

typedef __packed struct
{
	
	uint8_t SOF;
	uint16_t DataLength;
	uint8_t Seq;
	uint8_t CRC8;
	
}xFrameHeader;



typedef __packed struct
{
	xFrameHeader txFreameHeader;
	uint16_t CmdID;
	ext_student_interactive_header_data_t 	dataFreameHeader;
	graphic_data_struct_t									 	CilentData;
	uint16_t   FrameTail;
} ext_SendClient_Data_t;


typedef __packed struct
{
    float data1;
    float data2;
    float data3;
    uint8_t data4;
} custom_data_t;


typedef __packed struct
{
    uint8_t data[64];
} ext_up_stream_data_t;

typedef __packed struct
{
    uint8_t data[32];
} ext_download_stream_data_t;

extern ext_student_interactive_header_data_one_t  line_1;
extern ext_student_interactive_header_data_t custom_grapic_draw;	
extern ext_student_interactive_str_header_data_t ext_str;
extern ext_student_interactive_str_header_data_t ext_shoot;
extern ext_student_interactive_header_data_2_t armor_id_grapic_draw;
extern ext_student_interactive_header_data_3_t boundary_line_grapic_draw;			//自定义图像绘制
extern ext_student_interactive_header_clear_t ext_chear;
extern ext_student_interactive_header_data_3_t boundary_pitch_line_grapic_draw;			//自定义图像绘制
void referee_shoot_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len );

void referee_shoot_line_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len );
       
/*add begin*/
extern uint8_t tx_mode_buff[128];
extern uint8_t tx_shoot_buff[128];
extern uint8_t tx_clear_buff[128];

extern void init_send_char(uint32_t update);
void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len);
void init_send_cline_aim(void);

void init_send_shoot_char(uint32_t update);
void init_send_clear_char(uint8_t delete_type,uint8_t layer);  

void init_send_cline1_aim(void);
void init_send_armor_id_aim(uint32_t update);
    
void init_send_boundary_line(uint32_t update);
void init_send_pitch_line(uint32_t update);
/*end begin*/
extern void init_referee_struct_data(void);
extern void referee_data_solve(uint8_t *frame);

extern void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer);

extern uint8_t get_robot_id(void);

extern void get_shoot_heat0_limit_and_heat0(uint16_t *heat0_limit, uint16_t *heat0);
extern void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1);
//
extern ext_student_interactive_str_header_data_t char_shoot;

void shoot_char(void);

extern ext_student_interactive_str_header_data_t char_chassis;

void char_chassis_show(void);
    
//
#endif
