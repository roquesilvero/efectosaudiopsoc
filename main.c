#include "project.h"
#include"math.h"
#include <stdbool.h>

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /*Habilitación de cada uno de los módulos/componentes*/
    /*Nota: Cuando se procesa información de audio, el ADC_DelSig se utiliza en modo continuo.*/

    ADC_Start();
    ADC_Enable();
    Filter_Enable();
    Filter_Start();
    Opamp_Start();
    //Opamp_Start();
    VDAC8_Start();
    VDAC8_Enable();
    /*Fin habilitación*/

    uint16 MaxDelay=100;//cantidad de elementos anteriores guardados
    float DelayLine[MaxDelay]; //ultimos valores calculados de y

    uint16  xLP;
    float   xfl,yfl;// entrada en flotante para calculos
    float Beta=0.5;

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //se muestrean los primeros valores necesarios

    uint16 y=0;
    uint8 encendido=0;
    bool boton_anterior=Boton_Read();

    for(int i=0; i<MaxDelay; i++) //inicializacion del arreglo DelayLine
    {
        DelayLine[i]=0; //valores iniciales cero
    }
    for(;;) //proceso a ser aplicado a la señal de entrada
    {
        xLP=ADC_Read16(); /* Starts ADC conversions, waits for the conversion to be complete, stops ADC conversion
        and returns the signed 16-bit value of result*/
        Filter_Write16(Filter_CHANNEL_A,xLP); /*Writes a new 16-bit sample to the Filter’s input staging register*/
        xLP=Filter_Read16(Filter_CHANNEL_A); /*Reads the current value on the Filter’s output holding register. Two-byte read
        of the most significant bytes.*/

        /* Place your application code here. */
        xfl=((float)xLP-127)/127;

        if(encendido)
        {
            yfl=xfl+Beta*DelayLine[MaxDelay-1];
            for(int j=MaxDelay-1; j>=1; j--) //Desplazamiento a la derecha de elementos
            {
                  DelayLine[j]=DelayLine[j-1];
            }
            DelayLine[0]=yfl; // Se agrega al inicio del vector
            y=(uint16)(127*yfl)+127;
            VDAC8_SetValue(y);
        }
        else
            VDAC8_SetValue(xLP);
        LED_Write(encendido);
        uint8 boton_actual=Boton_Read();

        if(boton_anterior!=boton_actual && boton_anterior==0 )
        {
            encendido++;
            if(encendido==2)
                encendido=0;
        }
        boton_anterior =boton_actual;
    }
}
/* [] END OF FILE */
