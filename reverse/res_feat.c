//==== IpConfig @ c003b9bc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void IpConfig(int param_1)

{
  uint uVar1;
  
  uVar1 = _tx_counter_get();
  if (param_1 == 0) {
    _ticks_0 = _ticks_0 * _ticks_0;
  }
  else {
    _ticks_0 = param_1 * param_1;
  }
  _ticks_0 = (_ticks_0 ^ uVar1) & 0xff;
  if (_ticks_0 < 0xc9) {
    if (_ticks_0 < 100) {
      _ticks_0 = 100;
      _tx_timer_deactivate_api(0xc12e9394);
      _tx_timer_change(0xc12e9394,_ticks_0,_ticks_0);
      _tx_timer_activate_api(0xc12e9394);
      IpConfigMyCoach();
      return;
    }
  }
  else {
    _ticks_0 = 200;
  }
  _tx_timer_deactivate_api(0xc12e9394);
  _tx_timer_change(0xc12e9394,_ticks_0,_ticks_0);
  _tx_timer_activate_api(0xc12e9394);
  IpConfigMyCoach();
  return;
}


//==== IpConfigStart @ c003bab0

void IpConfigStart(void)

{
  uint uVar1;
  
  IpCompsInit();
  uVar1 = _tx_counter_get();
  _tx_timer_create(0xc12e9394,0xc003be38,IpConfig,uVar1 | 1,100,100,1);
  return;
}


//==== IpConfigInit @ c003bb14

undefined4 IpConfigInit(void)

{
  s_ipConfigInitialized = (code)0x1;
  IpConfigStart();
  return 1;
}


//==== IpConfigAssertFeature @ c003bb3c

void IpConfigAssertFeature(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  if ((s_ipConfigInitialized != (code)0x0) && (iVar1 = IpConfigComponent(), iVar1 != 0)) {
    return;
  }
  Os_InterruptsDisable();
  Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),s_No_hardware_support___d__d__d___c003be44
            ,s_ipConfigInitialized,param_1,param_2);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


//==== IpConfigMyCoach @ c003bbbc

void IpConfigMyCoach(void)

{
  return;
}


//==== IpConfigGetFeature @ c003bbcc

undefined4 IpConfigGetFeature(void)

{
  return 0xffffffff;
}


//==== IpConfigComponent @ c003bca4

bool IpConfigComponent(undefined4 param_1,int param_2)

{
  uint uVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  bool bVar5;
  
  iVar4 = IpGetCoachVersion();
  uVar1 = (uint)(iVar4 << 0x14) >> 0x1c;
  bVar5 = true;
  switch(param_1) {
  case 0x18:
  case 0x1b:
    bVar2 = uVar1 == 6;
    bVar3 = uVar1 < 2;
    break;
  case 0x19:
    bVar2 = uVar1 == 4;
    bVar3 = uVar1 == 0;
    break;
  case 0x1a:
    if (uVar1 == 0 || uVar1 == 4) {
      return false;
    }
    bVar3 = uVar1 == 1;
    bVar2 = uVar1 == 6;
    break;
  case 0x1c:
    bVar3 = uVar1 < 2;
    goto LAB_c003bd84;
  case 0x1d:
    bVar2 = uVar1 == 6;
    bVar3 = uVar1 == 0;
    break;
  case 0x1e:
  case 0x21:
  case 0x22:
    if (uVar1 == 0 || uVar1 == 4) {
      return false;
    }
  case 0x23:
LAB_c003bd50:
    if (uVar1 == 6) {
      return false;
    }
    return true;
  case 0x1f:
    if (uVar1 == 0 || uVar1 == 4) {
      return false;
    }
    if (uVar1 == 2 || uVar1 == 1) {
      return false;
    }
    goto LAB_c003bd50;
  case 0x20:
    if (param_2 == 4) {
      return true;
    }
    if (param_2 != 8) {
      return false;
    }
    return uVar1 != 0 && uVar1 != 4;
  case 0x24:
    bVar5 = uVar1 != 6;
  default:
    goto switchD_c003bce8_default;
  }
  bVar3 = (bool)(bVar3 | bVar2);
LAB_c003bd84:
  if (bVar3) {
    return false;
  }
switchD_c003bce8_default:
  return bVar5;
}


