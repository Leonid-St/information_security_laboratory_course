#include "MainForm.h"
//#include "Laba1.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array < String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(0);
    zilaba1::MainForm1 form;
    Application::Run(% form);
}