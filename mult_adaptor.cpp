#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <time.h>
#include <curl/curl.h>
#include <string.h>


#define DOOR5_PHYS_PIN_NO_D0	3
#define DOOR5_PHYS_PIN_NO_D1	5
#define DOOR5_PHYS_PIN_NO_OPEN	7
#define DOOR5_PHYS_PIN_NO_BTN	8

#define DOOR4_PHYS_PIN_NO_D0	10
#define DOOR4_PHYS_PIN_NO_D1	11
#define DOOR4_PHYS_PIN_NO_OPEN	12
#define DOOR4_PHYS_PIN_NO_BTN	13

#define DOOR3_PHYS_PIN_NO_D0	15
#define DOOR3_PHYS_PIN_NO_D1	16
#define DOOR3_PHYS_PIN_NO_OPEN	18
#define DOOR3_PHYS_PIN_NO_BTN	19

#define DOOR2_PHYS_PIN_NO_D0	21
#define DOOR2_PHYS_PIN_NO_D1	22
#define DOOR2_PHYS_PIN_NO_OPEN	23
#define DOOR2_PHYS_PIN_NO_BTN	24

#define DOOR1_PHYS_PIN_NO_D0	29
#define DOOR1_PHYS_PIN_NO_D1	31
#define DOOR1_PHYS_PIN_NO_OPEN	32
#define DOOR1_PHYS_PIN_NO_BTN	33

#define DOOR0_PHYS_PIN_NO_D0	35
#define DOOR0_PHYS_PIN_NO_D1	36
#define DOOR0_PHYS_PIN_NO_OPEN	37
#define DOOR0_PHYS_PIN_NO_BTN	38

int open_pins[6]={
		DOOR0_PHYS_PIN_NO_OPEN,
		DOOR1_PHYS_PIN_NO_OPEN,
		DOOR2_PHYS_PIN_NO_OPEN,
		DOOR3_PHYS_PIN_NO_OPEN,
		DOOR4_PHYS_PIN_NO_OPEN,
		DOOR5_PHYS_PIN_NO_OPEN,
};

int d0_pins[6]={
		DOOR0_PHYS_PIN_NO_D0,
		DOOR1_PHYS_PIN_NO_D0,
		DOOR2_PHYS_PIN_NO_D0,
		DOOR3_PHYS_PIN_NO_D0,
		DOOR4_PHYS_PIN_NO_D0,
		DOOR5_PHYS_PIN_NO_D0,
};

int d1_pins[6]={
		DOOR0_PHYS_PIN_NO_D1,
		DOOR1_PHYS_PIN_NO_D1,
		DOOR2_PHYS_PIN_NO_D1,
		DOOR3_PHYS_PIN_NO_D1,
		DOOR4_PHYS_PIN_NO_D1,
		DOOR5_PHYS_PIN_NO_D1,
};

int btn_pins[6]={
		DOOR0_PHYS_PIN_NO_BTN,
		DOOR1_PHYS_PIN_NO_BTN,
		DOOR2_PHYS_PIN_NO_BTN,
		DOOR3_PHYS_PIN_NO_BTN,
		DOOR4_PHYS_PIN_NO_BTN,
		DOOR5_PHYS_PIN_NO_BTN,
};

// the event counter
#define BIT_NUM 25
volatile int eventCounter[6] = {BIT_NUM ,BIT_NUM ,BIT_NUM ,BIT_NUM ,BIT_NUM ,BIT_NUM };
volatile unsigned int data[6];
volatile int bswitch[6]={0,0,0,0,0,0};
int door_codes[6]={2015,2016,2017,2018,2019,2020};
int open_durations[6]={0,0,0,0,0,0};
int btn_durations[6]={0,0,0,0,0,0};

time_t t_last[6];
time_t t_now[6];

char str_req_url[256];

// -------------------------------------------------------------------------
// myInterrupt:  called every time an event occurs
void myInterrupt0_d1(void) {
	int gap;
	t_now[0]=time(NULL);
	gap = (int)(t_now[0]-t_last[0]);
	t_last[0]=t_now[0];
	if(gap>1)
		eventCounter[0]=BIT_NUM;
	if(eventCounter[0]>=0)
   		data[0] |= (1<<eventCounter[0]);

	if(eventCounter[0]==0)
   		bswitch[0]=1;
   	eventCounter[0]--;
}

void myInterrupt0_d0(void) {
	int gap;
	t_now[0]=time(NULL);
	gap = (int)(t_now[0]-t_last[0]);
	t_last[0]=t_now[0];
	if(gap>1)
		eventCounter[0]=BIT_NUM;
	if(eventCounter[0]>=0)
   		data[0] &= (~(1<<eventCounter[0]));
	if(eventCounter[0]==0)
   		bswitch[0]=1;
   	eventCounter[0]--;
}

void myInterrupt1_d1(void) {
	int gap;
	t_now[1]=time(NULL);
	gap = (int)(t_now[1]-t_last[1]);
	t_last[1]=t_now[1];
	if(gap>1)
		eventCounter[1]=BIT_NUM;
	if(eventCounter[1]>=0)
   		data[1] |= (1<<eventCounter[1]);

	if(eventCounter[1]==0)
   		bswitch[1]=1;
   	eventCounter[1]--;
}

void myInterrupt1_d0(void) {
	int gap;
	t_now[1]=time(NULL);
	gap = (int)(t_now[1]-t_last[1]);
	t_last[1]=t_now[1];
	if(gap>1)
		eventCounter[1]=BIT_NUM;
	if(eventCounter[1]>=0)
   		data[1] &= (~(1<<eventCounter[1]));
	if(eventCounter[1]==0)
   		bswitch[1]=1;
   	eventCounter[1]--;
}

void myInterrupt2_d1(void) {
	int gap;
	t_now[2]=time(NULL);
	gap = (int)(t_now[2]-t_last[2]);
	t_last[2]=t_now[2];
	if(gap>1)
		eventCounter[2]=BIT_NUM;
	if(eventCounter[2]>=0)
   		data[2] |= (1<<eventCounter[2]);

	if(eventCounter[2]==0)
   		bswitch[2]=1;
   	eventCounter[2]--;
}

void myInterrupt2_d0(void) {
	int gap;
	t_now[2]=time(NULL);
	gap = (int)(t_now[2]-t_last[2]);
	t_last[2]=t_now[2];
	if(gap>1)
		eventCounter[2]=BIT_NUM;
	if(eventCounter[2]>=0)
   		data[2] &= (~(1<<eventCounter[2]));
	if(eventCounter[2]==0)
   		bswitch[2]=1;
   	eventCounter[2]--;
}

void myInterrupt3_d1(void) {
	int gap;
	t_now[3]=time(NULL);
	gap = (int)(t_now[3]-t_last[3]);
	t_last[3]=t_now[3];
	if(gap>1)
		eventCounter[3]=BIT_NUM;
	if(eventCounter[3]>=0)
   		data[3] |= (1<<eventCounter[3]);

	if(eventCounter[3]==0)
   		bswitch[3]=1;
   	eventCounter[3]--;
}

void myInterrupt3_d0(void) {
	int gap;
	t_now[3]=time(NULL);
	gap = (int)(t_now[3]-t_last[3]);
	t_last[3]=t_now[3];
	if(gap>1)
		eventCounter[3]=BIT_NUM;
	if(eventCounter[3]>=0)
   		data[3] &= (~(1<<eventCounter[3]));
	if(eventCounter[3]==0)
   		bswitch[3]=1;
   	eventCounter[3]--;
}

void myInterrupt4_d1(void) {
	int gap;
	t_now[4]=time(NULL);
	gap = (int)(t_now[4]-t_last[4]);
	t_last[4]=t_now[4];
	if(gap>1)
		eventCounter[4]=BIT_NUM;
	if(eventCounter[4]>=0)
   		data[4] |= (1<<eventCounter[4]);

	if(eventCounter[4]==0)
   		bswitch[4]=1;
   	eventCounter[4]--;
}

void myInterrupt4_d0(void) {
	int gap;
	t_now[4]=time(NULL);
	gap = (int)(t_now[4]-t_last[4]);
	t_last[4]=t_now[4];
	if(gap>1)
		eventCounter[4]=BIT_NUM;
	if(eventCounter[4]>=0)
   		data[4] &= (~(1<<eventCounter[4]));
	if(eventCounter[4]==0)
   		bswitch[4]=1;
   	eventCounter[4]--;
}

void myInterrupt5_d1(void) {
	int gap;
	t_now[5]=time(NULL);
	gap = (int)(t_now[5]-t_last[5]);
	t_last[5]=t_now[5];
	if(gap>1)
		eventCounter[5]=BIT_NUM;
	if(eventCounter[5]>=0)
   		data[5] |= (1<<eventCounter[5]);

	if(eventCounter[5]==0)
   		bswitch[5]=1;
   	eventCounter[5]--;
}

void myInterrupt5_d0(void) {
	int gap;
	t_now[5]=time(NULL);
	gap = (int)(t_now[5]-t_last[5]);
	t_last[5]=t_now[5];
	if(gap>1)
		eventCounter[5]=BIT_NUM;
	if(eventCounter[5]>=0)
   		data[5] &= (~(1<<eventCounter[5]));
	if(eventCounter[5]==0)
   		bswitch[5]=1;
   	eventCounter[5]--;
}

void (*f_int_d0[6])(void);
void (*f_int_d1[6])(void);

void init_int_call_array(void)
{
	f_int_d0[0]=&myInterrupt0_d0;
	f_int_d0[1]=&myInterrupt1_d0;
	f_int_d0[2]=&myInterrupt2_d0;
	f_int_d0[3]=&myInterrupt3_d0;
	f_int_d0[4]=&myInterrupt4_d0;
	f_int_d0[5]=&myInterrupt5_d0;

	f_int_d1[0]=&myInterrupt0_d1;
	f_int_d1[1]=&myInterrupt1_d1;
	f_int_d1[2]=&myInterrupt2_d1;
	f_int_d1[3]=&myInterrupt3_d1;
	f_int_d1[4]=&myInterrupt4_d1;
	f_int_d1[5]=&myInterrupt5_d1;

}

int init_pins(int index)
{
	//////////////////////////////////////////////////////////////////////
		if ( wiringPiISR (d0_pins[index], INT_EDGE_FALLING,f_int_d0[index] ) < 0 ) {
			return 1;
		}
		if ( wiringPiISR (d1_pins[index], INT_EDGE_FALLING, f_int_d1[index]) < 0 ) {
			return 1;
		}


		pullUpDnControl(d0_pins[index], PUD_UP);
		pullUpDnControl(d1_pins[index], PUD_UP);

		pinMode (open_pins[index], OUTPUT);
		digitalWrite (open_pins[index], LOW);

		pinMode(btn_pins[index],INPUT);
		pullUpDnControl(btn_pins[index],PUD_DOWN);
		///////////////////////////////////////////////////////////////////////
		return 0;

}

int init_config()
{
	FILE * file = fopen("/home/pi/card_reader_adaptor/config.txt","r");
	if(file==NULL)
		return 1;
	fgets(str_req_url,256,file);
	int len = strlen(str_req_url);
	str_req_url[len-1]=0;
	fclose(file);
	
	return 0;
}

int initialize()
{
	if(init_config())
	{
		return 1;	
	}

	// sets up the wiringPi library, using physical pin indices
	if (wiringPiSetupPhys() < 0) {
		return 1;
	}

	init_int_call_array();
	for(int i=0; i<2 ;i++)
	{
		if(init_pins(i)!=0)
			return 1;
	}

	for(int i=0; i<6;i++)
  		t_last[i] = time(NULL);

	return 0;
}

int query_privilege(unsigned int id_no,int index ,int door_code){

	CURL* geturl;
	int status_code;

	char str[256];
	sprintf(str,"{\"cardId\":%d,\"door\":%d}",id_no,door_code);
        printf("%s\n", str);
	sprintf(str,str_req_url,id_no,index);
        printf("%s\n", str);
	struct curl_slist *list = NULL;

	geturl = curl_easy_init();
	if(geturl){

		 list = curl_slist_append(list, "Content-Type: application/json");
                curl_easy_setopt(geturl, CURLOPT_HTTPHEADER, list);
                curl_easy_setopt(geturl, CURLOPT_URL,str);
                curl_easy_setopt(geturl, CURLOPT_HTTPPOST, 1L);
                curl_easy_setopt(geturl, CURLOPT_TIMEOUT, 2L);
		str[0]=0;
                curl_easy_setopt(geturl, CURLOPT_POSTFIELDS,str);
                //curl_easy_setopt(geturl, CURLOPT_NOBODY, 1);
                curl_easy_perform(geturl);
                curl_easy_getinfo (geturl, CURLINFO_RESPONSE_CODE, &status_code);

                curl_easy_cleanup(geturl);

	}
	return status_code;
}

inline void open_timer(int & open_time, int pin_no)
{
	open_time--;
	if(open_time==0)
	{
		digitalWrite (pin_no,  LOW);
	}
	else if(open_time<0)
	{
		open_time=0;
	}
}

void open_door_or_not(int index)
{
	FILE * log_ptr;
	int status_code;
	unsigned int card_no;

	if(bswitch[index])
	{
		data[index]&= 0x1ffffff;
		data[index] = data[index]>>1;
		printf("%d\n",data[index]);

		card_no=data[index];
		status_code = query_privilege(card_no,index,door_codes[index]);

		log_ptr=fopen("/var/log/card_records.log","a+");
		if(200 == status_code){
			pinMode (open_pins[index], OUTPUT);
			digitalWrite(open_pins[index],HIGH);
			open_durations[index]=40;
			fprintf(log_ptr,"%u %u %u OPEN\n",card_no,door_codes[index],time(NULL));
		}
		else if(401 == status_code)
		{
			printf("Permission not granted!\n");
			fprintf(log_ptr,"%u %u %u DENY\n",card_no,door_codes[index],time(NULL));
		}
		else
		{
			printf("No response or not a proper response");
			fprintf(log_ptr,"%u %u %u EXCEPTION\n",card_no,door_codes[index],time(NULL));
		}

		fflush(log_ptr);
		fclose(log_ptr);

		bswitch[index]=0;
	}


	/////////////////// deal with button pressed
	btn_durations[index]=btn_durations[index]<<1;	
	if(digitalRead(btn_pins[index]))
	{
		btn_durations[index]=btn_durations[index]|0x01;
	}
	int temp=0;
	temp += ((btn_durations[index])&0x01);
	temp += ((btn_durations[index]>>1)&0x01);
	temp += ((btn_durations[index]>>1)&0x01);
	temp += ((btn_durations[index]>>1)&0x01);
	temp += ((btn_durations[index]>>1)&0x01); 
	if(temp>3)
	{
		pinMode (open_pins[index], OUTPUT);
		digitalWrite(open_pins[index],HIGH);
		open_durations[index]=40;
	}
	///////////////////
}


void operate()
{

	int index=0;


	// display counter value every second.
	while ( 1 )
	{
		usleep(100000); // wait 1 second

		//for(int i=0; i<6;i++)
		//{
		//	open_door_or_not(i,open_time[i]);
		//}

		//for performance consideration, abandon the for-loop approach
		open_door_or_not(0);
		open_door_or_not(1);
		open_door_or_not(2);
		open_door_or_not(3);
		open_door_or_not(4);
		open_door_or_not(5);

		//for(int i=0; i<6;i++)
		//{
		//	open_timer(openTime[i],open_pins[i]);
		//}

		//for performance consideration, abandon the for-loop approach
		open_timer(open_durations[0],open_pins[0]);
		open_timer(open_durations[1],open_pins[1]);
		open_timer(open_durations[2],open_pins[2]);
		open_timer(open_durations[3],open_pins[3]);
		open_timer(open_durations[4],open_pins[4]);
		open_timer(open_durations[5],open_pins[5]);

	}
}

// main
int main(int argc, char** argv) {

	if(initialize()!=0)
		return 1;

	operate();

	return 0;

}

