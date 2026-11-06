#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#include "global.h"

void traffic_enter_mode(Mode_t newMode);  /* goi khi doi mode / luc khoi dong */
void traffic_run(void);                   /* goi trong vong lap chinh         */

#endif /* INC_TRAFFIC_LIGHT_H_ */
