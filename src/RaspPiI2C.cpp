#include "../include/RaspPiI2C.h"

RaspPiI2C::RaspPiI2C()
{
    //ctor
    #define I2CIO_BASE    (PERI_BASE + 0x205000) // I2C controller

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
    I2Cio_map = mmap(
        NULL,               // any address in our space will do
        BLOCK_SIZE,         // map length
        PROT_READ|PROT_WRITE,   // Enable reading & writing to mapped memory
        MAP_SHARED,         // shared with other processes
        mem_fd,             // file to map
        I2CIO_BASE
        );

    close (mem_fd);

    if (I2Cio_map == MAP_FAILED) {
        TmpStrg.Printf(wxT("mmap error %d\n"),(int) I2Cio_map);
        //wxMessageBox(wxT("Error"),wxT("Error"),wxICON_EXCLAMATION,this);
        exit(-1);
        }

    I2Cio = (volatile unsigned int *)I2Cio_map;
}

RaspPiI2C::~RaspPiI2C()
{
    //dtor
    wxString TmpStrg;

    /* munmap I2CIO */
    if (munmap(I2Cio_map,         // address in our space will do
        BLOCK_SIZE              // map length
        ) == -1) {
        TmpStrg.Printf(wxT("munmap error %d\n"),(int) -1);
        //wxMessageBox(wxT("Error"),wxT("Error"),wxICON_EXCLAMATION,this);
        exit(-1);
        }
}



void RaspPiI2C::I2C_Enable(void)
{
    // I2C Enable

    *(I2Cio + I2C_C) |= (1 << I2C_I2CENb);
    // BSC0 |= (1 << I2C_I2CENb);
}


void RaspPiI2C::I2C_Disable(void)
{
    // I2C Disable

    *(I2Cio + I2C_C) &= ~(1 << I2C_I2CENb);
    // BSC0 &= ~(1 << I2C_I2CENb);
}


void RaspPiI2C::Wait_Done(void)
{
        unsigned int timeout = 50;

        while((!(BSC0_S & BSC_S_DONE)) && --timeout)
        {
            usleep(1000);
        }

        if	(timeout == 0)
        {
        	//wxMessageBox(wxT("Error"),wxT("I2C Wait Done Timeout Error"),wxICON_EXCLAMATION,this);
        	exit(-1);
        }
}


void RaspPiI2C::I2C_Write(unsigned int device_address, unsigned int value)
{
    BSC0_A = device_address;
    // *(I2Cio + I2C_A) = device_address;;

    BSC0_DLEN = 0;
    // *(I2Cio + I2C_DLEN) = 1;

    //BSC0_FIFO = value;
    //*(I2Cio + I2C_FIFO) = value;

    BSC0_S = CLEAR_STATUS;
    // *(I2Cio + I2C_S) = CLEAR_STATUS;

    BSC0_C = START_WRITE;
    // *(I2Cio + I2C_C) = START_WRITE;

   Wait_Done();
}


void RaspPiI2C::I2C_Read(unsigned int length)
{
    BSC0_DLEN = length;
    // *(I2Cio + I2C_DLEN) = 1;

    BSC0_S = CLEAR_STATUS;
    // *(I2Cio + I2C_S) = CLEAR_STATUS;

    BSC0_C = START_READ;
    // *(I2Cio + I2C_C) = START_READ;

   Wait_Done();
}


void RaspPiI2C::I2C_Read_Only(unsigned int device_address, unsigned int length)
{
    BSC0_A = device_address;
    // *(I2Cio + I2C_A) = device_address;;

    BSC0_DLEN = length;
    // *(I2Cio + I2C_DLEN) = 1;

    BSC0_S = CLEAR_STATUS;
    // *(I2Cio + I2C_S) = CLEAR_STATUS;

    BSC0_C = START_READ;
    // *(I2Cio + I2C_C) = START_READ;

   Wait_Done();
}









void RaspPiI2C::I2C_FIFO_to_Array(unsigned int data[], unsigned int number)
{
    unsigned int i;

    for (i = 0; i<number; i++)
    {
        data[i] = *(I2Cio + I2C_FIFO);
    }
}

