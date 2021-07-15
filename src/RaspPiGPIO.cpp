/***************************************************************
 * Name:      RaspPiGPIO.cpp
 * Purpose:   Code for GPIO
 * Author:     ()
 * Created:   2021-05-29
 * Copyright:  ()
 * License:
 **************************************************************/

#include "../include/RaspPiGPIO.h"

RaspPiGPIO::RaspPiGPIO()
{
    //ctor
    #define GPIO_BASE               (PERI_BASE + 0x200000) /* GPIO controller */

    #define PAGE_SIZE (4*1024)
    #define BLOCK_SIZE (4*1024)

    wxString TmpStrg;
    int   mem_fd;

    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0)
        {
        //wxMessageBox(wxT("can't open /dev/mem\n"),wxT("ERROR"),  wxICON_EXCLAMATION);
        exit (1);
        }

    /* mmap GPIO */
    gpio_map = mmap(
        NULL,               // any address in our space will do
        BLOCK_SIZE,         // map length
        PROT_READ|PROT_WRITE,   // Enable reading & writing to mapped memory
        MAP_SHARED,         // shared with other processes
        mem_fd,             // file to map
        GPIO_BASE
        );

    close (mem_fd);

    if (gpio_map == MAP_FAILED) {
        TmpStrg.Printf(wxT("mmap error %d\n"),(int) gpio_map);
        //wxMessageBox(wxT("Error"),wxT("Error"),wxICON_EXCLAMATION,this);
        exit(-1);
        }

    gpio = (volatile unsigned int *)gpio_map;
}

RaspPiGPIO::~RaspPiGPIO()
{
    //dtor
    wxString TmpStrg;

    /* munmap GPIO */
    if (munmap(gpio_map,         // address in our space will do
        BLOCK_SIZE              // map length
        ) == -1) {
        TmpStrg.Printf(wxT("munmap error %d\n"),(int) -1);
        //wxMessageBox(wxT("Error"),wxT("Error"),wxICON_EXCLAMATION,this);
        exit(-1);
        }
}


void RaspPiGPIO::GPIO_SetInput(unsigned int pin)
{
    unsigned int regOffset, shift;

    regOffset = pin / 10;
    shift = (pin % 10)*3;

    *(gpio + GPFSEL0 + regOffset) &= ~(7 << shift);
}


void RaspPiGPIO::GPIO_SetStdOutput(unsigned int pin)
{
    unsigned int regOffset, shift;

    regOffset = pin / 10;
    shift = (pin % 10)*3;

    *(gpio + GPFSEL0 + regOffset) &= ~(7 << shift);
    *(gpio + GPFSEL0 + regOffset) |= (1 << shift);
}


void RaspPiGPIO::GPIO_SetAltOutput(unsigned int pin,
     unsigned int alternatefunction)
{
    unsigned int regOffset, shift, alternate;

    switch (alternatefunction)
    {
        case 0: alternate = 4; break;
        case 1: alternate = 5; break;
        case 2: alternate = 6; break;
        case 3: alternate = 7; break;
        case 4: alternate = 3; break;
        case 5: alternate = 2; break;
        default: break;
    }

  /*
    100    4        Function 0              -4
    101    5        Function 1              -4
    110    6        Function 2              -4
    111    7        Function 3              -4
    011    3        Function 4               1
    010    2        Function 5               3
  */

    regOffset = pin / 10;
    shift = (pin % 10)*3;

    *(gpio + GPFSEL0 + regOffset) &= ~(7 << shift);
    *(gpio + GPFSEL0 + regOffset) |= (alternate << shift);
}


void RaspPiGPIO::GPIO_Set(unsigned int pin)
{
    unsigned int regOffset, shift;

    regOffset = pin / 32;
    shift = (pin % 32);

    *(gpio + GPSET0 + regOffset) = (1 << shift);
}



void RaspPiGPIO::GPIO_Clr(unsigned int pin)
{
    unsigned int regOffset, shift;

    regOffset = pin / 32;
    shift = (pin % 32);

    *(gpio + GPCLR0 + regOffset) = (1 << shift);
}


unsigned int RaspPiGPIO::GPIO_Get(unsigned int pin)
{
    unsigned int regOffset, shift;

    regOffset = pin / 32;
    shift = (pin % 32);

    return ((*(gpio + GPLEV0 + regOffset)) >> shift) & 0x0001;
}


void RaspPiGPIO::GPIO_SetResistor(unsigned int pin,
     unsigned int ResistorSelect)
{
    unsigned int regOffset, shift;

    regOffset = pin / 15;
    shift = (pin % 15)*2;

    *(gpio + GPPUD0 + regOffset) &= ~(3 << shift);
    *(gpio + GPPUD0 + regOffset) |= (ResistorSelect << shift);
}

