/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       referee_usart_task.c/h
  * @brief      RM referee system data solve. RM裁判系统数据处理
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Nov-11-2019     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */
#include "referee_usart_task.h"
#include "main.h"
#include "cmsis_os.h"

#include "bsp_usart.h"
#include "detect_task.h"

#include "CRC8_CRC16.h"
#include "fifo.h"
#include "protocol.h"
#include "referee.h"

#include "main.h"
#include "arm_math.h"
#include "chassis_behaviour.h"

/**
  * @brief          single byte upacked 
  * @param[in]      void
  * @retval         none
  */
/**
  * @brief          单字节解包
  * @param[in]      void
  * @retval         none
  */
static void referee_unpack_fifo_data(void);

 
extern UART_HandleTypeDef huart6;

uint8_t usart6_buf[2][USART_RX_BUF_LENGHT];

fifo_s_t referee_fifo;
uint8_t referee_fifo_buf[REFEREE_FIFO_BUF_LENGTH];
unpack_data_t referee_unpack_obj;

/**
  * @brief          referee task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
/**
  * @brief          裁判系统任务
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void referee_usart_task(void const * argument)
{
    init_referee_struct_data();
    fifo_s_init(&referee_fifo, referee_fifo_buf, REFEREE_FIFO_BUF_LENGTH);
    usart6_init(usart6_buf[0], usart6_buf[1], USART_RX_BUF_LENGHT);

    while(1)
    {

        referee_unpack_fifo_data();
        osDelay(10);
    }
}

//         init_send_clear_char();//更新下一个字符时，必须先清除一下屏幕
//         referee_data_pack_handle(0xA5,0x0301,(uint8_t*)&ext_chear,sizeof(ext_student_interactive_header_clear_t));

//底盘状态
extern chassis_behaviour_e chassis_behaviour_mode;
extern chassis_behaviour_e chassis_behaviour_last_mode;
void referee_usart_ui_task(void const * argument)
{
//      chassis_behaviour_last_mode = chassis_behaviour_mode;  
//      init_send_char(1);
//      referee_data_pack_handle(0xA5,0x0301,(uint8_t*)&ext_str,sizeof(ext_student_interactive_str_header_data_t));
    while(1)
    {

        char_chassis_show();
        referee_data_pack_handle(0xA5,0x0301,(uint8_t*)&char_chassis,sizeof(ext_student_interactive_str_header_data_t));
        osDelay(5);
        init_send_char(1);
        referee_data_pack_handle(0xA5,0x0301,(uint8_t*)&ext_str,sizeof(ext_student_interactive_str_header_data_t));
        osDelay(5);
        init_send_char(2);
        referee_data_pack_handle(0xA5,0x0301,(uint8_t*)&ext_str,sizeof(ext_student_interactive_str_header_data_t));
        osDelay(20);
    }
}

//射击状态
void referee_usart_ui2_task(void const * argument)
{
        while(1)
    {    shoot_char();
         referee_data_pack_handle(0xA5,0x0301,(uint8_t*)&char_shoot,sizeof(ext_student_interactive_str_header_data_t)); 
        osDelay(5);
        init_send_shoot_char(1);
        referee_shoot_data_pack_handle(0xA5,0x0301,(uint8_t*)&ext_shoot,sizeof(ext_student_interactive_str_header_data_t));
        osDelay(5);
        init_send_shoot_char(2);
        referee_shoot_data_pack_handle(0xA5,0x0301,(uint8_t*)&ext_shoot,sizeof(ext_student_interactive_str_header_data_t));
        osDelay(20);
    }
    
}

//瞄准线
void referee_usart_ui3_task(void const * argument)
{
        init_send_cline1_aim();
        referee_shoot_line_data_pack_handle(0xA5,0x0301,(uint8_t*)&line_1,21);//sizeof(ext_student_interactive_header_data_one_t));
    
    while(1)
    {
        init_send_cline_aim();
        referee_shoot_line_data_pack_handle(0xA5,0x0301,(uint8_t*)&custom_grapic_draw,sizeof(custom_grapic_draw));
//        init_send_cline1_aim();
//        referee_shoot_line_data_pack_handle(0xA5,0x0301,(uint8_t*)&line_1,sizeof(ext_student_interactive_header_data_one_t));
//    
        init_send_boundary_line(1);
        referee_shoot_line_data_pack_handle(0xA5,0x0301,(uint8_t*)&boundary_line_grapic_draw,sizeof(ext_student_interactive_header_data_3_t));
        osDelay(5);
        init_send_boundary_line(2);
        referee_shoot_line_data_pack_handle(0xA5,0x0301,(uint8_t*)&boundary_line_grapic_draw,sizeof(ext_student_interactive_header_data_3_t));
      
        osDelay(100);
    }
}


void referee_usart_ui4_task(void const * argumen)
{
    init_send_armor_id_aim(1);
    referee_shoot_line_data_pack_handle(0xA5,0x0301,(uint8_t*)&armor_id_grapic_draw,sizeof(ext_student_interactive_header_data_2_t));
    while(1)
    {
        init_send_armor_id_aim(1);
        referee_shoot_line_data_pack_handle(0xA5,0x0301,(uint8_t*)&armor_id_grapic_draw,sizeof(ext_student_interactive_header_data_2_t));
        osDelay(2);
        init_send_armor_id_aim(2);
        referee_shoot_line_data_pack_handle(0xA5,0x0301,(uint8_t*)&armor_id_grapic_draw,sizeof(ext_student_interactive_header_data_2_t));
        osDelay(2);
        /*add*/
        init_send_pitch_line(1);
        referee_shoot_line_data_pack_handle(0xA5,0x0301,(uint8_t*)&boundary_pitch_line_grapic_draw,sizeof(ext_student_interactive_header_data_3_t));
        osDelay(2);
        init_send_pitch_line(2);
        referee_shoot_line_data_pack_handle(0xA5,0x0301,(uint8_t*)&boundary_pitch_line_grapic_draw,sizeof(ext_student_interactive_header_data_3_t));
        /*end*/
        osDelay(20);
    }
}


/**
  * @brief          single byte upacked 
  * @param[in]      void
  * @retval         none
  */
/**
  * @brief          单字节解包
  * @param[in]      void
  * @retval         none
  */
void referee_unpack_fifo_data(void)
{
  uint8_t byte = 0;
  uint8_t sof = HEADER_SOF;
  unpack_data_t *p_obj = &referee_unpack_obj;

  while ( fifo_s_used(&referee_fifo) )
  {
    byte = fifo_s_get(&referee_fifo);
    switch(p_obj->unpack_step)
    {
      case STEP_HEADER_SOF:
      {
        if(byte == sof)
        {
          p_obj->unpack_step = STEP_LENGTH_LOW;
          p_obj->protocol_packet[p_obj->index++] = byte;
        }
        else
        {
          p_obj->index = 0;
        }
      }break;
      
      case STEP_LENGTH_LOW:
      {
        p_obj->data_len = byte;
        p_obj->protocol_packet[p_obj->index++] = byte;
        p_obj->unpack_step = STEP_LENGTH_HIGH;
      }break;
      
      case STEP_LENGTH_HIGH:
      {
        p_obj->data_len |= (byte << 8);
        p_obj->protocol_packet[p_obj->index++] = byte;

        if(p_obj->data_len < (REF_PROTOCOL_FRAME_MAX_SIZE - REF_HEADER_CRC_CMDID_LEN))
        {
          p_obj->unpack_step = STEP_FRAME_SEQ;
        }
        else
        {
          p_obj->unpack_step = STEP_HEADER_SOF;
          p_obj->index = 0;
        }
      }break;
      case STEP_FRAME_SEQ:
      {
        p_obj->protocol_packet[p_obj->index++] = byte;
        p_obj->unpack_step = STEP_HEADER_CRC8;
      }break;

      case STEP_HEADER_CRC8:
      {
        p_obj->protocol_packet[p_obj->index++] = byte;

        if (p_obj->index == REF_PROTOCOL_HEADER_SIZE)
        {
          if ( verify_CRC8_check_sum(p_obj->protocol_packet, REF_PROTOCOL_HEADER_SIZE) )
          {
            p_obj->unpack_step = STEP_DATA_CRC16;
          }
          else
          {
            p_obj->unpack_step = STEP_HEADER_SOF;
            p_obj->index = 0;
          }
        }
      }break;  
      
      case STEP_DATA_CRC16:
      {
        if (p_obj->index < (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
        {
           p_obj->protocol_packet[p_obj->index++] = byte;  
        }
        if (p_obj->index >= (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
        {
          p_obj->unpack_step = STEP_HEADER_SOF;
          p_obj->index = 0;

          if ( verify_CRC16_check_sum(p_obj->protocol_packet, REF_HEADER_CRC_CMDID_LEN + p_obj->data_len) )
          {
            referee_data_solve(p_obj->protocol_packet);
          }
        }
      }break;

      default:
      {
        p_obj->unpack_step = STEP_HEADER_SOF;
        p_obj->index = 0;
      }break;
    }
  }
}


void USART6_IRQHandler(void)
{
    static volatile uint8_t res;
    if(USART6->SR & UART_FLAG_IDLE)
    {
        __HAL_UART_CLEAR_PEFLAG(&huart6);

        static uint16_t this_time_rx_len = 0;

        if ((huart6.hdmarx->Instance->CR & DMA_SxCR_CT) == RESET)
        {
            __HAL_DMA_DISABLE(huart6.hdmarx);
            this_time_rx_len = USART_RX_BUF_LENGHT - __HAL_DMA_GET_COUNTER(huart6.hdmarx);
            __HAL_DMA_SET_COUNTER(huart6.hdmarx, USART_RX_BUF_LENGHT);
            huart6.hdmarx->Instance->CR |= DMA_SxCR_CT;
            __HAL_DMA_ENABLE(huart6.hdmarx);
            fifo_s_puts(&referee_fifo, (char*)usart6_buf[0], this_time_rx_len);
            detect_hook(REFEREE_TOE);
        }
        else
        {
            __HAL_DMA_DISABLE(huart6.hdmarx);
            this_time_rx_len = USART_RX_BUF_LENGHT - __HAL_DMA_GET_COUNTER(huart6.hdmarx);
            __HAL_DMA_SET_COUNTER(huart6.hdmarx, USART_RX_BUF_LENGHT);
            huart6.hdmarx->Instance->CR &= ~(DMA_SxCR_CT);
            __HAL_DMA_ENABLE(huart6.hdmarx);
            fifo_s_puts(&referee_fifo, (char*)usart6_buf[1], this_time_rx_len);
            detect_hook(REFEREE_TOE);
        }
    }
}

