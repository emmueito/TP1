# Trabajo practico N° 1
## Deshidratador Tecnologico
### Proyecto del curso de sistemas embebidos
#### Jose Mariano Emmanuel

[Pagina de Educacion en Tecnologia](https://www.emueito.ar/)

---

  El siguiente proyecto *Deshidratador Tecnologico* Es el resultado de una combinacion de conocimientos nuevos y conocimientos ya adquiridos en otras etapas de formacion. 

  Quisiera agradecer a los organizadores y al equipo de docentes, que con tanta dedicacion deciden transmitir sus saberes, y asi formar nuevos ciudadanos capaces de caminar con la tecnologia. 

  **Proglogo:**

  Proyecto de Deshidratador de Alimentos 



  ![Deshidratador](Imagen_2.jpg)

  ![Imagen de componentes](imagen1.png "Imagen")

  



![Imagen del proyecto](https://m.media-amazon.com/images/I/61XAvgLpF7L._AC_SL1100_.jpg)

**Objetivo:**

Desarrollar un sistema de deshidratación de alimentos utilizando energía solar calórica y fotovoltaica, controlado por un Arduino Uno. Este sistema permitirá conservar una amplia variedad de alimentos por un período prolongado, extendiendo su vida útil y abriendo nuevas posibilidades para su uso y consumo.


**Descripción:**

El sistema consistirá en un deshidratador modular compuesto por bandejas apilables que albergarán los alimentos a deshidratar. Cada bandeja estará equipada con un sensor de temperatura y un elemento calefactor regulado por la placa Nucleo F429ZI. La energía para el funcionamiento del sistema provendrá de paneles solares fotovoltaicos, almacenándose en una batería para su uso durante las horas sin luz solar.

Funcionamiento:

* 	Carga de alimentos: Los alimentos frescos y lavados se colocan en las bandejas del deshidratador.

* 	Control de temperatura: El Arduino Uno leerá los datos de los sensores de temperatura en cada bandeja y ajustará la potencia de los elementos calefactores para mantener la temperatura deseada durante todo el proceso de deshidratación.

* 	Energía solar: Durante el día, los paneles solares fotovoltaicos proporcionarán energía eléctrica al sistema, alimentando el Arduino Uno y los elementos calefactores.

* 	Batería: La energía solar excedente se almacenará en una batería para su uso durante la noche o en días nublados, garantizando un funcionamiento continuo del sistema.

* 	Monitoreo y control: El usuario podrá monitorear el estado del sistema y ajustar parámetros como la temperatura y el tiempo de deshidratación a través de una interfaz de usuario, como una pantalla LCD o una aplicación móvil conectada al Arduino Uno.

**Componentes:**


*	Arduino Uno
    * Opsional f429zi
        1. u otra de la familia Nucleo
            1. _1Microcontrolador



*	Sensores de temperatura (para cada bandeja)
    * LM 35


*	Elementos calefactores (para cada bandeja)
    * Resistores

•	Paneles solares fotovoltaicos

•	Batería

•	Pantalla LCD o aplicación móvil (opcional)

 
**Código de programación:**

```c++  

    #include "mbed.h"        
    #include "arm_book_lib.h"

    //=====[Declaration and initialization of public global objects]===============

    DigitalIn enterButton(BUTTON1);
    DigitalIn gasDetector(D2);
    DigitalIn overTempDetector(D3);
    DigitalIn aButton(D4);
    DigitalIn bButton(D5);
    DigitalIn cButton(D6);
    DigitalIn dButton(D7);

    DigitalOut alarmLed(LED1);
    DigitalOut incorrectCodeLed(LED3);
    DigitalOut systemBlockedLed(LED2);

    UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

    //=====[Declaration and initialization of public global variables]=============

    bool alarmState = OFF;
    int numberOfIncorrectCodes = 0;

    //=====[Declarations (prototypes) of public functions]=========================

    void inputsInit();
    void outputsInit();

    void alarmActivationUpdate();
    void alarmDeactivationUpdate();

    void uartTask();
    void availableCommands();

    //=====[Main function, the program entry point after power on or reset]========

    int main()
    {
        inputsInit();
        outputsInit();
        while (true) {
            alarmActivationUpdate();
            alarmDeactivationUpdate();
            uartTask();
        }
    }

    //=====[Implementations of public functions]===================================

    void inputsInit()
    {
        gasDetector.mode(PullDown);
        overTempDetector.mode(PullDown);
        aButton.mode(PullDown);
        bButton.mode(PullDown);
        cButton.mode(PullDown);
        dButton.mode(PullDown);
    }

    void outputsInit()
    {
        alarmLed = OFF;
        incorrectCodeLed = OFF;
        systemBlockedLed = OFF;
    }

    void alarmActivationUpdate()
    {
        if ( gasDetector || overTempDetector ) {
            alarmState = ON;
        }
        alarmLed = alarmState;
    }

    void alarmDeactivationUpdate()
    {
        if ( numberOfIncorrectCodes < 5 ) {
            if ( aButton && bButton && cButton && dButton && !enterButton ) {
                incorrectCodeLed = OFF;
            }
            if ( enterButton && !incorrectCodeLed && alarmState ) {
                if ( aButton && bButton && !cButton && !dButton ) {
                    alarmState = OFF;
                    numberOfIncorrectCodes = 0;
                } else {
                    incorrectCodeLed = ON;
                    numberOfIncorrectCodes = numberOfIncorrectCodes + 1;
                }
            }
        } else {
            systemBlockedLed = ON;
        }
    }

    void uartTask()
    {
        char receivedChar = '\0';
        if( uartUsb.readable() ) {
            uartUsb.read( &receivedChar, 1 );
            if ( receivedChar == '1') {
                if ( alarmState ) {
                    uartUsb.write( "The alarm is activated\r\n", 24);
                } else {
                    uartUsb.write( "The alarm is not activated\r\n", 28);
                }
            } else {
                availableCommands();
            }
        }
    }

    void availableCommands()
    {
        uartUsb.write( "Available commands:\r\n", 21 );
        uartUsb.write( "Press '1' to get the alarm state\r\n\r\n", 36 );
    } 
```




 **El código de programación para el Arduino Uno controlará las siguientes funciones:**__



* •	Lectura de datos de los sensores de temperatura

* •	Ajuste de la potencia de los elementos calefactores

* •	Gestión de la energía solar y la batería

* •	Monitoreo del estado del sistema

* •	Interfaz de usuario (opcional)


**Ventajas:**

•	Conservación de alimentos: El deshidratador permite conservar una amplia variedad de alimentos por un período prolongado, reduciendo el desperdicio y aumentando la disponibilidad de alimentos nutritivos.

•	Energía solar: El uso de energía solar reduce el impacto ambiental y los costos operativos del sistema.

•	Control preciso: El Arduino Uno permite un control preciso de la temperatura y el tiempo de deshidratación, optimizando el proceso para cada tipo de alimento.

•	Monitoreo y control: El usuario puede monitorear el estado del sistema y ajustar parámetros para obtener resultados óptimos.
Consideraciones:

•	Selección de alimentos: Es importante seleccionar alimentos adecuados para el proceso de deshidratación, considerando su contenido de agua y su textura.

•	Preparación de alimentos: Los alimentos deben prepararse adecuadamente antes de deshidratarlos, por ejemplo, cortarlos en rodajas finas o eliminar la piel.

•	Tiempo de deshidratación: El tiempo de deshidratación varía según el tipo de alimento, la temperatura y la humedad del ambiente.

•	Almacenamiento: Los alimentos deshidratados deben almacenarse en recipientes herméticos en un lugar fresco y seco.
Conclusión:

El deshidratador de alimentos con Arduino Uno es una herramienta útil para conservar alimentos de manera natural y eficiente, aprovechando la energía solar y ofreciendo un control preciso del proceso. Este proyecto puede adaptarse a diferentes necesidades y escalas, desde uso doméstico hasta aplicaciones comerciales a pequeña escala.


 
Explicación completa del código Arduino para Deshidratador con Control PID
Descripción:

Este código implementa un control PID (Proporcional-Integral-Derivativo) para el deshidratador de alimentos, permitiendo una regulación más precisa de la temperatura y una mayor estabilidad en el proceso de deshidratación. El código utiliza un sensor LM35 para medir la temperatura actual y un elemento calefactor para ajustar la temperatura. Un potenciómetro permite al usuario establecer la temperatura deseada.

**Librerías necesarias:**

•	[Librería PID para Arduino:](https://github.com/DonnyCraft1/PIDArduino "Librerias Arduino") 

*Componentes:*

|componentes    |  precio   |  informacion  |
|---------------|  -------- |  ------------ |
|Placa Arduino  |   $500    |   [Ver](https://articulo.mercadolibre.com.ar/MLA-706159868-sensor-analogico-temperatura-lm35-55c-a-150c-nubbeo-_JM#is_advertising=true&position=1&search_layout=grid&type=pad&tracking_id=17405246-6044-4c63-96a2-ed818a5f2f3f&is_advertising=true&ad_domain=VQCATCORE_LST&ad_position=1&ad_click_id=N2JkODM0ZmYtMTA4MS00ZDc2LTllNTYtNjA3YjNlMDI1NzZi "Precio Mercadolibre")         |
|LM 35          |   $4000   |   Ver
|Elemento calefactor | $5000    | [ver]()
•	Sensor de temperatura LM35

•	Elemento calefactor (resistencia)

•	Potenciómetro

•	Cables

**Esquema de conexiones:**

•	Conecte el pin de salida del sensor LM35 al pin analógico del Arduino Uno.

•	Conecte el elemento calefactor a un pin de salida del Arduino Uno.

•	Conecte el potenciómetro a otro pin analógico del Arduino Uno.

**Explicación del código:

1. Librería PID:
    Se incluye la librería PID para Arduino en la parte superior del código. Esta librería proporciona las funciones necesarias para implementar un controlador PID. 

2. Definición de pines:
    Se definen constantes para los pines del Arduino Uno utilizados para el sensor LM35 (sensorPin), el elemento calefactor (heaterPin) y el potenciómetro (potPin). Estas constantes facilitan la lectura y escritura de los valores de los pines en el código.

3. Definición de variables:

    Se definen varias variables importantes para el funcionamiento del controlador PID:
•	setPoint: Esta variable almacena la temperatura deseada para el proceso de deshidratación. Inicialmente se establece en 50 grados Celsius, pero el usuario puede ajustarla mediante el potenciómetro.
•	Kp, Ki y Kd: Estas variables representan las constantes proporcional, integral y derivativa del controlador PID. Estas constantes determinan la sensibilidad y la respuesta del controlador a los cambios de temperatura.
•	myPID: Se crea un objeto de tipo PID utilizando la librería PID. Este objeto encapsula la lógica del control PID y las variables necesarias para su funcionamiento.

4. Función de configuración (setup):

    Esta función se ejecuta una vez al iniciar el programa. En ella se realizan las siguientes tareas:
•	Se inicializa la comunicación serial para mostrar información en la pantalla del ordenador.
•	Se configura el pin de salida del PID (heaterPin) para controlar el elemento calefactor.
•	Se establece el tiempo de muestreo del PID (myPID.setSampleTime(1000)) en 1 segundo. Esto significa que el controlador PID calculará la potencia del calentador cada segundo en base a la temperatura actual y la temperatura deseada.
•	Se realiza un autoajuste de los parámetros PID (myPID.autoTune()) utilizando la temperatura actual como punto de referencia. Esto permite ajustar automáticamente Kp, Ki y Kd para obtener un comportamiento adecuado del controlador.
5. Bucle principal (loop):
Este bucle se ejecuta repetidamente mientras el programa está en funcionamiento. En cada iteración del bucle se realizan las siguientes tareas:
•	Se lee el valor analógico del sensor LM35 (sensorValue).
•	Se convierte el valor analógico a temperatura en grados Celsius (celsius).
•	Se lee el valor analógico del potenciómetro (potValue).
•	Se actualiza la temperatura deseada (setPoint) en base al valor del potenciómetro.
•	Se calcula el error de temperatura (error) como la diferencia entre la temperatura deseada y la temperatura actual.
•	Se actualiza el controlador PID (myPID.update(error)) con el valor del error de temperatura.
•	Se obtiene la potencia del calentador (myPID.getOutput()) calculada por el controlador PID.
•	Se imprime información en la pantalla serial sobre la temperatura actual, la temperatura deseada, el error de temperatura y la potencia del calentador.
•	Se introduce un retardo de 1 segundo (delay(1000)) antes de la siguiente iteración del bucle.
Ventajas del control PID:
•	Mayor precisión: El control PID permite una regulación más precisa de la temperatura en comparación con un control simple basado en un comparador.
•	Menor fluctuación: El control PID reduce las fluctuaciones de temperatura, lo que mejora la estabilidad del proceso de deshidratación.
•	Mejor respuesta a cambios: El control PID responde de manera más efectiva a los cambios en la temperatura ambiente o en la carga del deshidratador.
Consideraciones:
•	Selección de parámetros PID: Los parámetros PID (Kp, Ki y Kd) deben seleccionarse cuidadosamente
