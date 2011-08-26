/* 	Vintage multimeter system monitor
	Copyright (C) 2011 by Sebastian Götte <s@jaseg.de>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <unistd.h>
#include <sys/time.h>

#include "opendevice.h"
#include "../requests.h" //TODO relative path: not so nice.

void help(char** argv){
	printf("Usage: %s [output] [value], where\n\t[value] is a percentage (with optional decimal places) and\n\t[output] is either 0 or 1.\n", argv[0]);
}

int main(int argc, char** argv)
{
	int ec = 0;
	float percentage = 0;
	short output = 0;
	usb_dev_handle* mon;
	if(argc != 3){
		printf("Wrong number of arguments.\n");
		help(argv);
		return 3;
	}
	if(sscanf(argv[1], "%hd", &output) || sscanf(argv[2], "%f", &percentage)){
		printf("Wrong number format.\n");
		help(argv);
		return 4;
	}
	printf("Setting output %d to %f%%\n", output, percentage);
	usb_init();
	if((ec = usbOpenDevice(&mon, 0x16c0, "s@jaseg.de", 0x05dc, "MultiMon", "*", 0, stdout))){ //Ugh. Double reference.
		printf("Cannot open device handle (error code %d).\n", ec);
		return 1;
	}
	if((ec = usb_control_msg(mon, 0x40, MON_SET, output, (short)(65535*percentage/100.0F), 0, 0, 1000))){
		printf("Problems with a motor control transfer (error code %d).\n", ec);
		return 2;
	}
	usb_close(mon);
	return 0;
}


