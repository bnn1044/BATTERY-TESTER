#include <Button.h>
/********************Button & Menu Variable*****************/
// Set up for button
Button Select  = Button(6,PULLUP);    //6
Button Start   = Button(10,PULLUP);    //7
Button Minus   = Button(5,PULLUP);    //8
Button Plus    = Button(9,PULLUP);    //9

boolean GetStartKey(){
  if ( Start.uniquePress()){
    if ( !TestStart ){
      TestStart = 1;
    }else
      TestStart = 0;
  }
  return TestStart;
}
void GetSetPoint(){
  int CurrentChangeReq = CurrentSetPoint;
  int VoltageSetPointReq = CutOffVoltage;
  int TemperaterReq      = TempsetPoint ;
   if ( Select.isPressed()){
       
        SettingItem = SettingItem+1;
        if ( SettingItem >= 2 ){
           SettingItem = 0;
        }
  }
    //voltage change
    if ( Minus.isPressed()){
      switch ( SettingItem ){
        case 0 : 
          VoltageSetPointReq = VoltageSetPointReq - ChangeStep;
          CutOffVoltage  = VoltageSetPointReq;
          break;
        /*case 1:
          CurrentChangeReq = CurrentChangeReq - ChangeStep;
          CurrentSetPoint = CurrentChangeReq;
          break;*/
        case 1:
          TemperaterReq = TemperaterReq - ChangeStep;
          TempsetPoint = TemperaterReq;
          break;
      }
    }
    if ( Plus.isPressed()){
       switch ( SettingItem ){
        case 0 : 
          VoltageSetPointReq = VoltageSetPointReq + ChangeStep;
          CutOffVoltage  = VoltageSetPointReq;
          break;
        /*case 1:
          CurrentChangeReq = CurrentChangeReq + ChangeStep;
          CurrentSetPoint = CurrentChangeReq;
          break;*/
        case 1:
          TemperaterReq = TemperaterReq + ChangeStep;
          TempsetPoint = TemperaterReq;
          break;
      }
    }
}

