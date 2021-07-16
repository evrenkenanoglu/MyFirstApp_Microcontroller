/***************************************************************
 * Name:      MyFirstAppMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2021-05-11
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "MyFirstAppMain.h"

#include "include/RaspPiCLK.h"
#include "include/RaspPiGPIO.h"
#include "include/RaspPiI2C.h"
#include "include/RaspPiPWM.h"

#define TIMER0_PERIOD 500  // ms
#define TIMER1_PERIOD 1000 // ms
#define TIMER2_PERIOD 100  // ms
#define COLOR

#define MP9808_SLAVE_ADDRESS    0x1F
#define MP9808_BYTE_LENGTH      2
#define MP9808_REGISTER_ADDRESS 0x05

// ctor
RaspPiGPIO* myFirstGPIO = new RaspPiGPIO();
RaspPiCLK*  myFirstCLK  = new RaspPiCLK();
RaspPiPWM*  myFirstPWM  = new RaspPiPWM();
RaspPiI2C*  myFirstI2C  = new RaspPiI2C();

// helper functions
enum wxbuildinfoformat
{
    short_f,
    long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f)
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(MyFirstAppFrame, wxFrame)
EVT_CLOSE(MyFirstAppFrame::OnClose)
EVT_MENU(idMenuQuit, MyFirstAppFrame::OnQuit)
EVT_MENU(idMenuAbout, MyFirstAppFrame::OnAbout)
EVT_BUTTON(idButton0, MyFirstAppFrame::LED0)
// EVT_BUTTON(idButton1, MyFirstAppFrame::LED1)
/* begin ************* 2021-06-15 */
EVT_TIMER(idTimer0, MyFirstAppFrame::Timer0)
EVT_TIMER(idTimer1, MyFirstAppFrame::Timer1)
/* end   ************* 2021-06-15 */
// EVT_COMMAND_SCROLL_CHANGED(idSlider0, MyFirstAppFrame::Slider0)

END_EVENT_TABLE()

MyFirstAppFrame::MyFirstAppFrame(wxFrame* frame, const wxString& title) : wxFrame(frame, -1, title, wxPoint(100, 100), wxSize(600, 250))
{
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar     = new wxMenuBar();
    wxMenu*    fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"), 0);
    SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

    init_modules(); // Initialize
}

MyFirstAppFrame::~MyFirstAppFrame()
{
    // dtor
    myFirstGPIO->~RaspPiGPIO();
}

void MyFirstAppFrame::OnClose(wxCloseEvent& event) { Destroy(); }

void MyFirstAppFrame::OnQuit(wxCommandEvent& event) { Destroy(); }

void MyFirstAppFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);

    // Switch LED On
    // myFirstGPIO->GPIO_Set(12);

    wxMessageBox(msg, _("Welcome to..."));

    // Switch LED Off
    // myFirstGPIO->GPIO_Clr(12);
}

void MyFirstAppFrame::LED0(wxCommandEvent& event)
{
    static uint8_t counter             = 0;
    static bool    statusPrev          = false;
    static bool    status              = false;
    static uint8_t statusChangeCounter = 0;
    counter++;
    if ((counter % 3) == 0)
    {
        myFirstGPIO->GPIO_Set(14);
        counter = 0;
        status  = true;
    }
    else
    {
        myFirstGPIO->GPIO_Clr(14);
        status = false;
    }

    if (status != statusPrev)
        statusChangeCounter++;

    switch (statusChangeCounter)
    {
        case 0:
            myButton0->SetBackgroundColour(ColourLightGrey->GetAsString());

            break;
        case 1:
            myButton0->SetBackgroundColour(ColourGreen->GetAsString());

            break;
        case 2:
            myButton0->SetBackgroundColour(ColourYellow->GetAsString());

            break;
        case 3:
            myButton0->SetBackgroundColour(ColourLightGrey->GetAsString());
            statusChangeCounter = 0;
        default:
            break;
    }

    myStaticText0->SetLabel(wxString::Format("Click counter: %d", (counter)));
    status =
}

void MyFirstAppFrame::Timer0(wxTimerEvent& event) {}

/**
 * @brief Periodic I2C Reading and GUI Modification
 *
 * @param event
 */
void MyFirstAppFrame::Timer1(wxTimerEvent& event)
{
    wxString     TmpStrg;
    wxString     TmpStrg1;
    unsigned int Temp[2];
    unsigned int Result;
    uint8_t      firstByte  = 0;
    uint8_t      secondByte = 0;
    uint8_t      sign;

    myFirstI2C->I2C_Write(MP9808_SLAVE_ADDRESS, MP9808_REGISTER_ADDRESS); // Access register with ambient temperature
    myFirstI2C->I2C_Read(MP9808_BYTE_LENGTH);

    myFirstI2C->I2C_FIFO_to_Array(Temp, 2);
    sign       = Temp[0] & 0x10;        // Mask so only 5th bit (sign bit) is available
    firstByte  = (Temp[0] & 0x0F) << 4; // First 4 bits masked out and rest shifted left
    secondByte = Temp[1] >> 4;          // shifted right
    Result     = ((Temp[0] & 0x0F) << 4) + (Temp[1] >> 4);

    if (sign == 0)
        Result = Result;
    else
        Result = -Result;

    float deci = 0;
    if ((Temp[1] & 0x08) == 0x08)
        deci = deci + 0.5;
    else
        ;
    if ((Temp[1] & 0x04) == 0x04)
        deci = deci + 0.25;
    else
        ;
    if ((Temp[1] & 0x02) == 0x02)
        deci = deci + 0.125;
    else
        ;
    if ((Temp[1] & 0x01) == 0x01)
        deci = deci + 0.0625;
    else
        ;

    float Res = Result;
    Res       = Res + deci;

    TmpStrg.Printf(wxT("%3.1f °C"), (Res / 2.0));
    TmpStrg1.Printf(wxT("%3.1f °F"), (Res / 2.0) * 9 / 5 + 32);
    myTextCtrl0->SetValue(TmpStrg);
    myTextCtrl1->SetValue(TmpStrg1);

    myStaticText1->SetLabel(wxString::Format("Previous Value-> %d", tempPacket.prev_temp));
    myStaticText2->SetLabel(wxString::Format("Current  Value-> %d", tempPacket.current_temp));

    uint8_t mode;

    if (tempPacket.current_temp > tempPacket.prev_temp)
        mode = eModeGreater;
    else if (tempPacket.current_temp == tempPacket.prev_temp)
        mode = eModeEqual;
    else
        mode = eModeLower;

    switch (mode)
    {
        case eModeGreater:
            myPanel0->SetBackgroundColour(ColourGreen->GetAsString());

            break;
        case eModeEqual:
            myPanel0->SetBackgroundColour(ColourRed->GetAsString());

            break;

        case eModeLower:
            myPanel0->SetBackgroundColour(ColourYellow->GetAsString());

            break;

        default:
            break;
    }

    tempPacket.prev_temp = tempPacket.current_temp;
}

void MyFirstAppFrame::Timer2(wxTimerEvent& event) {}

void MyFirstAppFrame::PWM_Start(double frequency, double dutyCycle, unsigned int counts, unsigned int mode)
{

    double period;
    double countDuration;
    int    divisor;

    // disable PWM & start from  a clean state
    // Var: both channels
    *(myFirstPWM->pwmio + PWM_CTL) &= 0xFFFF0000;

    // Var: Channel 1
    *(myFirstPWM->pwmio + PWM_CTL) &= 0xFFFFFF00;

    // Var: Channel 2
    *(myFirstPWM->pwmio + PWM_CTL) &= 0xFFFF00FF;

    usleep(10);

    // stop clock and waiting for busy doesn't work, so kill clock
    *(myFirstCLK->clkio + PWMCLK_CNTL) = 0x5A000000 | (1 << 5);

    usleep(10);

    // wait until busy flag is clear
    while ((*(myFirstCLK->clkio + PWMCLK_CNTL)) & 1 << 7)
    {
    }; // 0x00000080

    // calculate divisor value for PWM1 clock .... base frequency is 750/7 MHz
    period        = 1.0 / frequency;
    countDuration = period / (counts * 1.0);
    divisor       = (int)(750000000.0f / (7.0 / countDuration));

    // Set divisor (2 .. 4095)!!!!
    // 0101 1010 = 0x5A
    *(myFirstCLK->clkio + PWMCLK_DIV) = 0x5A000000 | (divisor << 12);

    // source = osc and enable clock
    *(myFirstCLK->clkio + PWMCLK_CNTL) = 0x5A000000 | 0x11;

    // disable PWM & start from  a clean state
    // Var: both channels
    *(myFirstPWM->pwmio + PWM_CTL) &= 0xFFFF0000;

    // Var: Channel 1
    *(myFirstPWM->pwmio + PWM_CTL) &= 0xFFFFFF00;

    // Var: Channel 2
    *(myFirstPWM->pwmio + PWM_CTL) &= 0xFFFF00FF;

    usleep(10);

    // set the number of counts that constitude a period
    *(myFirstPWM->pwmio + PWM_RNG1) = counts;

    usleep(10);

    // set duty cyle
    *(myFirstPWM->pwmio + PWM_DAT1) = (int)((dutyCycle / 100.0) / counts);

    usleep(10);

    // start PWM1
    if (mode == PWMMODE)
        *(myFirstPWM->pwmio + PWM_CTL) |= 0x01;
    else
        *(myFirstPWM->pwmio + PWM_CTL) |= 0x81; // (1 <<7 ) | (1 << 0)
}

void MyFirstAppFrame::timer_create_start()
{
    myTimer0 = new wxTimer(this, idTimer0);
    myTimer0->Start(TIMER0_PERIOD); // ms

    myTimer1 = new wxTimer(this, idTimer1);
    myTimer1->Start(TIMER1_PERIOD); // ms

    myTimer2 = new wxTimer(this, idTimer2);
    myTimer2->Start(TIMER2_PERIOD); // ms
}
/* end   ************* 2021-06-15 */

/**
 * @brief Initilizes i2c, gpio, pwm modules and GUI
 *
 *
 */
void MyFirstAppFrame::init_modules()
{
    wxString TmpStrg;

    ColourButtonOFF = new wxColour(0, 255, 0);
    ColourButtonON  = new wxColour(204, 255, 204);

    ColourLightGrey = new wxColour(192, 192, 192);
    ColourGreen     = new wxColour(0, 255, 0);
    ColourRed       = new wxColour(255, 0, 0);
    ColourYellow    = new wxColour(255, 255, 0);

    ColourRGB_OFF = new wxColour(150 << 8);

    wxPanel* myPanel = new wxPanel(this, wxID_ANY);

    myButton0 = new wxButton(myPanel, idButton0, wxT("LED0"), wxPoint(20, 20), wxSize(80, 40));

    // first to low level then to output -> avoid spikes
    myFirstGPIO->GPIO_Clr(12);
    myButton0->SetBackgroundColour(ColourButtonOFF->GetAsString());
    myFirstGPIO->GPIO_SetStdOutput(12);

    myFirstGPIO->GPIO_Clr(14);
    // myButton1->SetBackgroundColour(ColourButtonOFF->GetAsString());
    myFirstGPIO->GPIO_SetStdOutput(14);

    // PWM0_0
    myFirstGPIO->GPIO_Clr(18);
    myFirstGPIO->GPIO_SetAltOutput(18, 5);

    // I2C
    myFirstGPIO->GPIO_SetAltOutput(0, 0);
    myFirstGPIO->GPIO_SetResistor(0, NORESISTOR);

    myFirstGPIO->GPIO_SetAltOutput(1, 0);
    myFirstGPIO->GPIO_SetResistor(1, NORESISTOR);

    myFirstI2C->I2C_Enable();

    myPanel0 = new wxPanel(myPanel, idPanel0, wxPoint(120, 20), wxSize(80, 40));
    myPanel0->SetBackgroundColour(ColourButtonOFF->GetAsString());

    timer_create_start();

    myStaticText0 = new wxStaticText(myPanel, idStaticText0, wxT(""), wxPoint(20, 100), wxSize(250, 30));
    myStaticText1 = new wxStaticText(myPanel, idStaticText1, wxT(""), wxPoint(300, 100), wxSize(250, 30));
    myStaticText2 = new wxStaticText(myPanel, idStaticText2, wxT(""), wxPoint(300, 140), wxSize(250, 30));

    myTextCtrl0 = new wxTextCtrl(myPanel, idTextCtrl0, wxT(""), wxPoint(300, 20), wxSize(250, 30), wxTE_READONLY);
    myTextCtrl1 = new wxTextCtrl(myPanel, idTextCtrl1, wxT(""), wxPoint(300, 60), wxSize(250, 30), wxTE_READONLY);

    tempPacket.prev_temp    = 0;
    tempPacket.current_temp = 0;
}
