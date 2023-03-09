#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "face_recognition.h"


int main() {
    face_recognition_init(); // ?????????

    face_recognition_send_command(cmd, sizeof(cmd)); // ????
    while(1);
		return 0;	// ???
}
