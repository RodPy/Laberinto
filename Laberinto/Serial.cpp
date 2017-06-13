#include <iostream>
#include <windows.h> 
#include "SerialClass.h"
using namespace std;

void main() {
	Serial* Arduino = new Serial("COM8");
	

	while (Arduino->IsConnected()) {
		char palabra[] = "geekytheory";

		cout << "Arduino conectado" << endl;

		cout << "Enviando :" << palabra << endl;
		Arduino->WriteData(palabra, sizeof(palabra) - 1);
		
		Sleep(500);

		Arduino->ReadData(palabra, sizeof(palabra) - 1);
		cout << "Recibido: " << palabra << endl;

		cout << "-------------------" << endl;
		Sleep(1000);
	}
}