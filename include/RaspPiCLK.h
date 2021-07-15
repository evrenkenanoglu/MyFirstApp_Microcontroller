#ifndef RASPPICLK_H
#define RASPPICLK_H

#include "../include/definition.h"

// wxwidgets scope
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// header file for Memory Mapped Files / Memory Management declarations
#include <sys/mman.h>

#define PWMCLK_CNTL 40
#define PWMCLK_DIV  41

class RaspPiCLK
{
public:
    RaspPiCLK();
    virtual ~RaspPiCLK();

    void* clk_map;
    // I/O access
    volatile unsigned int* clkio;

    void PWMCLK_Start(void);
    void PWMCLK_Stop(void);
    void PWMCLK_SetDivisor(unsigned int Divisor);

protected:
private:
};

#endif // RASPPICLK_H
