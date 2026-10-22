//==== HceGetMonitorType @ c00011d8

undefined4 HceGetMonitorType(void)

{
  return 0x21;
}


//==== DebugGetMonitorType @ c0061410

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 DebugGetMonitorType(void)

{
  return _uiMonitorType;
}


//==== DebugConvertMonitorTypeToBaudRate @ c0061f8c

undefined4 DebugConvertMonitorTypeToBaudRate(uint param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0x1c200;
  if ((param_1 & 0x20) == 0) {
    if ((param_1 & 0x200) == 0) {
      if ((param_1 & 0x40) != 0) {
        return 0x70800;
      }
      if ((param_1 & 0x80) == 0) {
        if ((param_1 & 0x10) == 0) {
          if ((param_1 & 0x400) == 0) {
            Printf_B(s_Invalid_baud_rate__x_c006d000);
          }
          else {
            uVar1 = 0x9600;
          }
        }
        else {
          uVar1 = 0x2580;
        }
      }
      else {
        uVar1 = 0xe1000;
      }
    }
    else {
      uVar1 = 0x38400;
    }
  }
  return uVar1;
}


//==== DebugConsoleMain @ c006232c

void DebugConsoleMain(void)

{
  undefined4 uVar1;
  byte *pbVar2;
  size_t sVar3;
  byte *pbVar4;
  byte local_120 [256];
  byte local_20 [8];
  
  memset(local_120,0,0x100);
  local_20[0] = 0;
  Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),s_ENTERING_DEBUG_CONSOLE____c006d048);
  func_0xc01a3ff4(s_DEBUG_c006d06c);
  _tx_thread_interrupt_control(0);
  DebugPollingModeEnable(1);
  sVar3 = 0;
LAB_c00623a0:
  pbVar4 = local_120 + sVar3;
LAB_c00623c0:
  while( true ) {
    uVar1 = UartGetUserId(0);
    UartReadPollingFifo(uVar1,local_20,1);
    if (local_20[0] == 0xd || local_20[0] == 10) break;
    if (local_20[0] == 0 || (local_20[0] ^ 8) == 0) goto LAB_c00623a8;
    *pbVar4 = local_20[0];
    uVar1 = UartGetUserId(0);
    UartWritePollingFifo(uVar1,local_20,1);
    pbVar4 = pbVar4 + 1;
    if (local_20 <= pbVar4) break;
  }
  goto LAB_c006244c;
LAB_c00623a8:
  if ((local_20[0] ^ 8) == 0 && local_120 < pbVar4) {
    pbVar4 = pbVar4 + -1;
    uVar1 = UartGetUserId(0);
    UartWritePollingFifo(uVar1,&DAT_c006d074,3);
    *pbVar4 = 0;
  }
  if (local_20 <= pbVar4) {
LAB_c006244c:
    func_0xc01a4010(local_120);
    sVar3 = strlen((char *)local_120);
    func_0xc01a45f0(local_120);
    pbVar2 = local_120 + sVar3;
    if (sVar3 != 0) {
      do {
        pbVar2 = pbVar2 + -1;
        sVar3 = sVar3 - 1;
        *pbVar2 = 0;
        pbVar4 = local_120;
      } while (sVar3 != 0);
      goto LAB_c00623c0;
    }
    goto LAB_c00623a0;
  }
  goto LAB_c00623c0;
}


//==== MonitorDumpAll @ c0062f28

void MonitorDumpAll(void)

{
  OsSysBoostPriority();
  OsSysDump(0);
  OsSysDump(0xffffffff);
  OsSysRestorePriority();
  return;
}


//==== MonitorCrash @ c0062fc8

void MonitorCrash(char *param_1)

{
  undefined4 *in_zero;
  undefined4 uVar1;
  char *pcVar2;
  code *pcVar3;
  uint local_14;
  
  local_14 = 0xffff;
  pcVar3 = _tx_thread_system_stack_ptr_Cop;
  if ((EBase & 0x3ff) != 1) {
    pcVar3 = _tx_thread_system_stack_ptr;
  }
  sscanf(param_1,&DAT_c006da80,&local_14);
  if (local_14 == 6) {
    RunThreadOnCop(MonitorCrashCop,0);
    return;
  }
  if (local_14 < 7) {
    if (local_14 == 2) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                s_Testing_syshalt_without_interrup_c006dd68);
      _tx_thread_interrupt_control(0);
      pcVar2 = s_Testing_syshalt_without_interrup_c006dd8c;
      uVar1 = 0x656;
LAB_c006336c:
                    /* WARNING: Subroutine does not return */
      OsSysHaltEx(pcVar2,s_Os_Misc_Debug_c_c006cf18,uVar1);
    }
    if (2 < local_14) {
      if (local_14 == 4) {
        Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                  s_Testing_failed_assert_with_inter_c006ddb0);
        uVar1 = 0x663;
      }
      else {
        if (local_14 < 5) {
          Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                    s_Testing_exception_without_interr_c006dbf8);
          uVar1 = _tx_thread_interrupt_control(0);
          trap(7);
          Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),s_ulZero_became__d_c006da34,0);
          _tx_thread_interrupt_control(uVar1);
          return;
        }
        Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                  s_Testing_failed_assert_without_in_c006ddd8);
        _tx_thread_interrupt_control(0);
        uVar1 = 0x668;
      }
                    /* WARNING: Subroutine does not return */
      __assfail(s_Failed_assertion___s__at_line__d_c006d0ec,&DAT_c006d118,uVar1,
                s_Os_Misc_Debug_c_c006cf18);
    }
    if (local_14 == 0) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                s_Testing_syshalt_w__interrupts_c006de04);
      pcVar2 = s_Testing_syshalt_with_interrupts_c006de24;
      uVar1 = 0x64b;
      goto LAB_c006336c;
    }
    if (local_14 == 1) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                s_Testing_exception_w__interrupts_c006dd44);
      trap(7);
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),s_ulZero_became__d_c006da34,0);
      return;
    }
  }
  else {
    if (local_14 == 10) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                s_Testing_execution_from_null____w_c006dc84);
      (*(code *)0x0)();
      return;
    }
    if (local_14 < 0xb) {
      if (local_14 != 8) {
        if (8 < local_14) {
          Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                    s_Testng_memset_to_null____will_tr_c006dd0c);
          memset((void *)0x0,-1,0x180);
          CACHE_WriteBackInvalidateDataCacheRegionEx(0,0xa0,0,0);
          return;
        }
        RunThreadOnCop(MonitorCrashCop,1);
        return;
      }
      memset(*(void **)(*(int *)(pcVar3 + 0xc) + 0x88),-1,700);
      memset((void *)0xc12e3ef8,-1,700);
      uVar1 = *(undefined4 *)(PTR__libc_data0_c014e844 + 8);
      pcVar2 = s_Thread_table___memory_pools_corr_c006dc20;
      goto LAB_c0063078;
    }
    if (local_14 == 0xc) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                s_Testing_disabling_interrupts__wi_c006dc4c);
      _tx_thread_interrupt_control(0);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    if (local_14 < 0xc) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                s_Testing_read_from_null__will_tri_c006dcc4);
      uVar1 = *in_zero;
      CACHE_WriteBackInvalidateDataCacheRegionEx(0,0xa0,0,0);
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),s_ulZero_is__d_c006dcfc,uVar1);
      return;
    }
    if (local_14 == 0xffff) {
      uVar1 = *(undefined4 *)(PTR__libc_data0_c014e844 + 8);
      pcVar2 = s_Defined_crash_types__0__Syshalt_1_c006dab4;
      goto LAB_c0063078;
    }
  }
  Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
            s_crash__d_undefined__No_crash_wil_c006da84);
  uVar1 = *(undefined4 *)(PTR__libc_data0_c014e844 + 8);
  pcVar2 = s_Defined_crash_types__0__Syshalt_1_c006dab4;
LAB_c0063078:
  Fprintf_B(uVar1,pcVar2);
  return;
}


