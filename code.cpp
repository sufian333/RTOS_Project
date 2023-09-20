/* ERIKA Enterprise. */
#include "ee.h"
#include <string.h>
/* Arduino SDK. */
#include "Arduino.h"

/*
 * Pin 13 has an LED connected on most Arduino boards.
 */
int led = 13;
int count=0;    // Message duration for 180s
static char msg[200];


extern "C" {

/* TASKs */
DeclareTask(task_sender);
/* Idle Hook */

void tweet_change();
void led_on();
void led_off();
extern void idle_hook(void);

} /* extern "C" */

void setup(void) {
	/* initialize the digital pin as an output. */
	pinMode(led, OUTPUT);
	digitalWrite(led, LOW);
	//Serial.begin(115200);
	tweet_change();
}

void idle_hook(void) {
}

int main(void) {
	init();

	setup();

	StartOS(OSDEFAULTAPPMODE);

	return 0;
}

/*
 * TASK SENDER
 */
TASK(task_sender) {
	static int i = 0;
	static int index = 0;
	static int msg_delay=0;
	static int count=0;
	if (count==1800)  //180s
	{	tweet_change();
		msg_delay=5;
		count=0;
		index=0;
		i=0;
	}
	if (msg_delay!=0){  //Delay of 0.5s
		led_off();
		msg_delay--;
	}
	else{
		count++; 		//counts for 180s
		if(msg[i]=='/'){	//Check for ending of msg
	//	Serial.print("\n");
			i=0;
		}
		switch (msg[i]){
		case '.':
			if(index!=1){	// '.' = 1
				led_on();
				index++;
			}
			else{
				led_off();  //after each symbol single 0 delay
				index=0;
				i++;
			}
			break;
		case '-':			// '-' = 111
			if(index!=3){
				led_on();
				index++;
			}
			else{
				led_off(); 	//after each symbol single 0 delay
				index=0;
				i++;
				}
			break;
		case ',':      //Check for codeword pause
			led_off();
			index++;
			if (index==2){    // two zeros because one zero automatically add after symbol so three zeros pause after codeword
				index=0;
				i++;
			}
			break;
		case ' ':
			led_off();
			index++;
			if (index==6){	// six zeros because one zero automatically add after symbol so seven zeros pause after each word
				index=0;
				i++;
			}
			break;
		default:
			i=0;
			break;
		}
	}
	TerminateTask();
}

void led_on(){
	digitalWrite(led, HIGH);		//led on
	//Serial.print("1");
}

void led_off(){
	digitalWrite(led, LOW);			//led off
	//Serial.print("0");
}


void tweet_change(){
	//Serial.print("Tweet Change\n");
	static int msg_num=0;
		switch (msg_num){
				case 0:
					strcpy(msg,".- ..-.,.,.-,-,....,.,.-. ..,-. -,....,. ....,.-,-.,-.. ..,... -...,.,-,-,.,.-. -,....,.-,-. .- -...,..,.-.,-.. ..,-. -,....,. .-,..,.-. /");	// msg1
					msg_num++;
					break;
				case 1:
					strcpy(msg,".- ...,....,---,.-.,- .--.,.,-.,-.-.,..,.-.. ..,... ..-,...,..-,.-,.-..,.-..,-.-- -...,.,-,-,.,.-. -,....,.-,-. .- .-..,---,-.,--. --,.,--,---,.-.,-.-- .-,-.,-.-- -..,.-,-.-- /");	// msg2
					msg_num++;
					break;
				case 2:
					strcpy(msg,".-,-.-.,-.-.,.,.--.,- ...,---,--,.,-,....,..,-.,--. -,....,.-,- -.--,---,..- -.-.,.-,-.,-.,---,- -.-.,....,.-,-.,--.,. .-,-.,-.. -.--,---,..- .--,..,.-..,.-.. ..-.,.,.,.-.. -...,.,-,-,.,.-. /");	// msg3
					msg_num++;
					break;
				case 3:
					strcpy(msg,".-,-..,...-,.,-.,-,..-,.-.,. -.-.,.-,-. -...,. .-.,.,.-,.-.. ....,.-,.--.,.--.,..,-.,.,...,... /");	// msg4
					msg_num++;
					break;
				case 4:
					strcpy(msg,".-,.-..,.-.. -,....,. .,..-.,..-.,---,.-.,- -.--,---,..- .-,.-.,. --,.-,-.-,..,-.,--. .--,..,.-..,.-.. ..-,.-..,-,..,--,.-,-,.,.-..,-.-- .--.,.-,-.-- ---,..-.,..-. /");	// msg5
					msg_num=0;
					break;
				default:
					msg_num=0;
					break;
				}
}

