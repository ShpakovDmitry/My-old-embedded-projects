#ifndef _STDIO_H_
 #include <stdio.h>
#else 
 #define _STDIO_H_
#endif

#include "myssdlib.h"


int main()
{

	struct mech_param body[1];
	
	body[0].x=body[0].y=body[0].z=body[0].vx=body[0].vy=body[0].vz=body[0].mass=body[0].radius=1;
	
	printf("%d\n", sizeof(body)/sizeof(body[0]) );
	
	
	force_gravity(&body, (int)sizeof(body)/sizeof(body[0]) );
	new_position(&body, (int)sizeof(body)/sizeof(body[0]) );
	
	printf("%2.15lf\n%2.15lf\n%2.15lf\n", body[0].x, body[0].y, body[0].z );
	
	
	/*struct mech_param body[2];
	int i,a=0;	

	body[0].x=body[0].y=body[0].z=0; body[0].mass=5.94e+24;
	body[1].x=body[1].y=0;
	body[1].z=6371000;
	body[1].mass=0;

	force_gravity(&body[0],(int)sizeof(body)/sizeof(body[0]) );

	//test(&body[0]);

	printf("%.20f\n%.20f\n",body[0].az, body[1].az);

	printf("%d\n",5&1);
*/


	

	return 0;
}
