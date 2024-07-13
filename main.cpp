#include "mbed.h"
#include "arm_book_lib.h"



#ifndef ON
#define ON  1   
#endif

#ifndef OFF
#define OFF  !(ON) //Declaracion de MACRO
#endif

#define delay(ms)   thread_sleep_for(ms)
#define TIEMPO_ENTRE_MUESTRAS   10
#define NUMERO_MUESTRAS 100

//--------- Definición de entradas y salidas ----------

AnalogIn tempSensor (A1);
DigitalOut maxTempLED (D2);
DigitalOut minTempLED (D3);
DigitalOut normalTempLED (D4);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//-------------- Declaración de variables ---------------

float maxTempPermitida = 50.0;
float minTempPermitida = 25.0;
float tempMedia = (maxTempPermitida + minTempPermitida)/2;
int registroTiepoAcumulado = 0;
float lm35VecLectura[NUMERO_MUESTRAS];
float lm35suma = 0.0;
float lm35Promedio = 0.0;
float lm35TempC = 0.0;

//--------- Declaración de Funciones prototipos ----------
void initOuputs();
void checkearSobrecalentamiento();
void checkearEnfriamiento();
void checkearRegistroTiempo();
int tempConverter(float ratio);                                 //El ratio es la señal analógica convertida a digital entre 0.0 y 1.0
void samplerPromedio();    


// main() runs in its own thread in the OS
int main()
{
    initOuputs();
    while (true) {
        samplerPromedio();
        checkearSobrecalentamiento());
        checkearEnfriamiento();
        checkearRegistroTiempo();
    }
}

void initOuputs(){
    maxTempLED = OFF;
    minTempLED = OFF;
    normalTempLED = ON;
}

int tempConverter(float temp){                                    //Convierte señal eléctrica a ºC y actualiza el valor de temperatura
    float tempC = temp * 3.3/0.01;
    return tempC;
}

void samplerPromedio(){
    static int lm35Muestras = 0;
    int i = 0;
    lm35VecLectura[lm35Muestras] = tempSensor.read();
    lm35Muestras++;
    if(lm35Muestras >= NUMERO_MUESTRAS) {
        lm35Muestras = 0;
    };

    lm35Suma = 0.0;
    for (i = 0; i < NUMERO_MUESTRAS; i++) {
        lm35Suma = lm35Suma + lm35VecLectura[i];
    }
    lm35Promedio = lm35suma / NUMERO_MUESTRAS;
    lm35TempC = tempConverter( lm35Promedio );
}


void checkearSobrecalentamiento(){                                        //Chequea si la temperatura está por arriba del limite
    if(lm35TempC >= maxTempPermitida){
        maxTempLED = ON;                                        //Activa un ventilador (representado por un LED)
        normalTempLED = OFF;
        uartUsb.write("Alcanzó la máxima temperatura\r\n", 30); 
        while (lm35TempC > tempMedia){
            maxTempLED = ON;
            samplerPromedio();
        }
        maxTempLED = OFF;                                       //Apaga el ventilador
        normalTempLED = ON;
        uartUsb.write("Temperatura normalizada\r\n", 24);    
    }
}


void checkearEnfriamiento(){                                        //Chequea si la temperatura está por abajo del limite
    if(lm35TempC <= minTempPermitida){
        minTempLED = ON;                                        //Activa una resistencia calefactora (representado por un LED)
        normalTempLED = OFF;
        uartUsb.write("Alcanzó la mínima temperatura\r\n", 30);
        while (lm35TempC <= tempMedia){
            minTempLED = ON;
            samplerPromedio();   
        }
        minTempLED = OFF;                                       //Apaga la resistencia calefactora
        normalTempLED = ON;
        uartUsb.write("Temperatura normalizada\r\n", 24);
    }
}


void uartTask(){                                                    //Envia un registro de temperatura por Serial
    char str[30];
    sprintf(str, "Temperatura actual:%.2f \xB0 C\r\n", lm35TempC);  
    int stringLength = strlen(str);   //Keil Studio no me tomó la función strlen()
    uartUsb.write(str, stringLength);    
}


void checkearRegistroTiempo(){                                            //Chequea el tiempo entre envíos de registros
    registroTiempoAcumulado = registroTiempoAcumulado + TIEMPO_ENTRE_MUESTRAS;
    delay(TIEMPO_ENTRE_MUESTRAS);
    if(registroTiempoAcumulado == 86400){                              //Tiempo de 20 min entre registros
        uartTask();                                             //Envía registro de temperatura 
        aregistroTiempoAcumulado = 0;
    }
}