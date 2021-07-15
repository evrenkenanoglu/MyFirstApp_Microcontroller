/***************************************************************
 * Name:      RaspPiGPIO.h
 * Purpose:   Header for GPIO
 * Author:     ()
 * Created:   2021-05-29
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef RASPPIGPIO_H
#define RASPPIGPIO_H

#include "../include/definition.h"

#define GPFSEL0     0
#define GPFSEL1     1
#define GPFSEL2     2
#define GPFSEL3     3
#define GPFSEL4     4
#define GPFSEL5     5

#define GPSET0      7
#define GPSET1      8

#define GPCLR0     10
#define GPCLR1     11

#define GPLEV0     13
#define GPLEV1     14

#define GPEDS0     16
#define GPEDS1     17

#define GPREN0     19
#define GPREN1     20

#define GPFEN0     22
#define GPFEN1     23

#define GPHEN0     25
#define GPHEN1     26

#define GPLEN0     28
#define GPLEN1     29

#define GPAREN0    31
#define GPAREN1    32

#define GPAFEN0    34
#define GPAFEN1    35

#define GPPUD0     57
#define GPPUD1     58
#define GPPUD2     59
#define GPPUD3     60

#define NORESISTOR      00
#define PUPRESISTOR     01
#define PDNRESISTOR     02



// wxwidgets scope
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// header file for Memory Mapped Files / Memory Management declarations
#include <sys/mman.h>


class RaspPiGPIO
{
    public:
        RaspPiGPIO();
        virtual ~RaspPiGPIO();

        void GPIO_SetInput(unsigned int pin);
        void GPIO_SetStdOutput(unsigned int pin);
        void GPIO_SetAltOutput(unsigned int pin, unsigned int alternate);
        void GPIO_Set(unsigned int pin);
        void GPIO_Clr(unsigned int pin);
        unsigned int GPIO_Get(unsigned int pin);
        void GPIO_SetResistor(unsigned int pin, unsigned int ResistorSelect);


        void *gpio_map;
        // I/O access
        volatile unsigned int *gpio; // access the variable from the HW at the moment of access!

    protected:

    private:

};

#endif // RASPPIGPIO_H
