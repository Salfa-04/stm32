#ifndef __MCTRL_VELOCITY_H
#define __MCTRL_VELOCITY_H

void mctrl_velocity_init(void);
void mctrl_chassis_control_velocity(float x, float y, float w);
void mctrl_velocity_callback(void);

#endif /* __MCTRL_VELOCITY_H */
