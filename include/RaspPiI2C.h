#ifndef RASPPII2C_H
#define RASPPII2C_H

#include "../include/definition.h"

// wxwidgets scope
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// header file for Memory Mapped Files / Memory Management declarations
#include <sys/mman.h>

// #defines
// I2C Adresses
#define I2C_C       0
//#define BSC0_C          *(BSC0_BASE + 0x00)
//#define BSC0_C          *(BSC0_BASE + I2C_C)
#define BSC0_C          *(I2Cio + I2C_C)

#define I2C_S       1
//#define BSC0_S          *(BSC0_BASE + 0x01)
//#define BSC0_S          *(BSC0_BASE + I2C_S)
#define BSC0_S          *(I2Cio + I2C_S)

#define I2C_DLEN    2
//#define BSC0_DLEN     *(BSC0_BASE + 0x02)
//#define BSC0_DLEN     *(BSC0_BASE + DLEN)
#define BSC0_DLEN       *(I2Cio + I2C_DLEN)

#define I2C_A       3
//#define BSC0_A          *(BSC0_BASE + 0x03)
//#define BSC0_A          *(BSC0_BASE + I2C_A)
#define BSC0_A          *(I2Cio + I2C_A)

#define I2C_FIFO    4
//#define BSC0_FIFO     *(BSC0_BASE + 0x04)
//#define BSC0_FIFO     *(BSC0_BASE + FIFO)
#define BSC0_FIFO       *(I2Cio + I2C_FIFO)

#define I2C_DIV     5
//#define BSC0_FIFO     *(BSC0_BASE + 0x05)
//#define BSC0_FIFO     *(BSC0_BASE + I2C_DIV)
#define BSC0_DIV        *(I2Cio + I2C_DIV)

#define I2C_DEL     6
//#define BSC0_FIFO     *(BSC0_BASE + 0x06)
//#define BSC0_FIFO     *(BSC0_BASE + I2C_DEL)
#define BSC0_DEL        *(I2Cio + I2C_DEL)

#define I2C_CLKT    7
//#define BSC0_FIFO     *(BSC0_BASE + 0x07)
//#define BSC0_FIFO     *(BSC0_BASE + I2C_CLKT)
#define BSC0_CLKT       *(I2Cio + I2C_CLKT)


// Bits
#define I2C_I2CENb 15
#define BSC_C_I2CEN     (1 << 15)
//#define BSC_C_I2CEN     (1 << I2C_I2CENb)

#define I2C_INTRb  10
#define BSC_C_INTR      (1 << 10)
//#define BSC_C_INTR      (1 << I2C_INTRb)

#define I2C_INTTb   9
#define BSC_C_INTT      (1 << 9)
//#define BSC_C_INTT      (1 << I2C_INTTb)

#define I2C_INTDb   8
#define BSC_C_INTD      (1 << 8)
//#define BSC_C_INTD      (1 << INTDb)

#define I2C_STb     7
#define BSC_C_ST        (1 << 7)
//#define BSC_C_ST        (1 << I2C_STb)

#define I2C_CLb     7
#define BSC_C_CLEAR     (1 << 4)
//#define BSC_C_CLEAR     (1 << I2C_CLb)

#define I2C_READb   0
#define BSC_C_READ      1
//#define BSC_C_READ      (1 << I2C_READb   0)


#define START_READ      BSC_C_I2CEN|BSC_C_ST|BSC_C_CLEAR|BSC_C_READ
#define START_WRITE     BSC_C_I2CEN|BSC_C_ST


#define BSC_S_CLKT    (1 << 9)
#define BSC_S_ERR     (1 << 8)
#define BSC_S_RXF     (1 << 7)
#define BSC_S_TXE     (1 << 6)
#define BSC_S_RXD     (1 << 5)
#define BSC_S_TXD     (1 << 4)
#define BSC_S_RXR     (1 << 3)
#define BSC_S_TXW     (1 << 2)
#define BSC_S_DONE    (1 << 1)
#define BSC_S_TA      1

#define CLEAR_STATUS    BSC_S_CLKT|BSC_S_ERR|BSC_S_DONE


class RaspPiI2C
{
    public:
        RaspPiI2C();
        virtual ~RaspPiI2C();

        void *I2Cio_map;

        // I/O access
        volatile unsigned int *I2Cio;

        void I2C_Start(void);
        void I2C_Stop(void);

        void I2C_Enable(void);
        void I2C_Disable(void);
        void Wait_Done(void);
        void I2C_Write(unsigned int device_address, unsigned int value);
        void I2C_Read(unsigned int length);
        void I2C_Read_Only(unsigned int device_address, unsigned int length);
        void I2C_FIFO_to_Array(unsigned int data[], unsigned int number);

    protected:

    private:
};

#endif // RASPPII2C_H






