//==== AppBootStartSectionProc @ c009d35c

void AppBootStartSectionProc(undefined4 param_1)

{
  OsThreadCreateEx(s_SectionInit_c00a2268,AppBootSectionInit,param_1,0x3f4,0x3f4,3,1);
  return;
}


//==== AppBootIsCopReady @ c009ec10

uint AppBootIsCopReady(void)

{
  int local_10 [2];
  
  local_10[0] = 0;
  _tx_event_flags_get(txBootEvent,0x4000000,0,local_10,0);
  return (uint)(local_10[0] << 5) >> 0x1f;
}


//==== AppBootSectionInit @ c009ef74

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void AppBootSectionInit(uint param_1)

{
  code *pcVar1;
  uint uVar2;
  int iVar3;
  code *pcVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  code *pcVar8;
  code *pcVar9;
  int iVar10;
  uint local_38;
  code *local_34;
  code *local_30;
  code *local_2c;
  undefined1 *local_28 [2];
  
  local_38 = 0;
  uVar2 = HceFastBootReturnTotalSections();
  TimingMark(8,s_Section_Wait_c00a2638);
  HceFastBootGetConfig(&local_38);
  if (1 < param_1) {
    AppGetBootEventBySection(param_1 - 1,0);
  }
  iVar3 = HceFastBootSectionMarkers(param_1,&local_34,&local_30);
  if (iVar3 == 0) {
                    /* WARNING: Subroutine does not return */
    OsSysHaltEx(s_Boot__Invalid_section_number_c00a2218,s_Zcam_Zboot_Boot_c_c00a212c,0x6ff);
  }
  AppGetBootEvent(0x20000,1);
  _DAT_c12fd010 = param_1;
  if ((param_1 == 1) || (_eBootType != 0)) {
    if (uVar2 <= param_1) {
      DcuMemoryProtectionEnable(0,0);
      DcuMemoryProtectionSetHandler(0,0);
      goto joined_r0xc009f29c;
    }
    local_2c = local_30;
    local_28[0] = &DAT_c12dae30;
    if (((local_38 & 1) != 0) && (uVar2 - 1 == param_1)) {
      HceFastBootSectionMarkers(uVar2,&local_2c,local_28);
    }
    DcuMemoryProtectionConfig(0,local_2c,(int)local_28[0] - (int)local_2c,8,0,0,0,1);
    DcuMemoryProtectionSetHandler(0,BootMemoryViolationHandler);
    uVar5 = 1;
  }
  else {
    uVar5 = 0;
  }
  DcuMemoryProtectionEnable(0,uVar5);
joined_r0xc009f29c:
  if (param_1 == 3) {
    AppGetBootEvent(0x80000,0);
    TimingMark(8,s_Got_rsrc_init_c00a267c);
  }
  TimingMarkF(8,s__Section__d_Init____Start_c00a2648,param_1);
  iVar3 = _tx_counter_get();
  pcVar1 = local_30;
  pcVar9 = local_34;
  if ((bCppInitOnce_0 == (code)0x0) && ((code *)0xc122de4b < local_30)) {
    CppSetupUnexpected();
    func_0xc04f9cb8(0xc122de4c);
    bCppInitOnce_0 = (code)0x1;
  }
  pcVar8 = (code *)&DAT_c00d0c30;
  do {
    if (pcVar8 != (code *)0x0) {
      pcVar4 = *(code **)pcVar8;
      if (((pcVar4 != (code *)0x0) && (pcVar9 <= pcVar4)) && (pcVar4 < pcVar1)) {
        (*pcVar4)();
        *(undefined4 *)pcVar8 = 0;
      }
    }
    pcVar8 = pcVar8 + -4;
  } while (pcVar8 != __CTOR_LIST__);
  pcVar9 = sSectionInit;
  iVar10 = 0x37;
LAB_c009f148:
  iVar6 = *(int *)(pcVar9 + 8);
  do {
    iVar10 = iVar10 + -1;
    if (iVar6 == -1) {
      if (*(code **)(pcVar9 + 4) < local_34) {
        uVar7 = *(uint *)(pcVar9 + 8);
        goto LAB_c009f138;
      }
      if (local_30 < *(code **)(pcVar9 + 4)) {
        uVar7 = *(uint *)(pcVar9 + 8);
        goto LAB_c009f138;
      }
      *(uint *)(pcVar9 + 8) = param_1;
      if (*(uint *)(pcVar9 + 8) != param_1) break;
      _DAT_c12fd014 = *(code **)pcVar9;
    }
    else {
      uVar7 = *(uint *)(pcVar9 + 8);
LAB_c009f138:
      if (uVar7 != param_1) break;
      _DAT_c12fd014 = *(code **)pcVar9;
    }
    (*_DAT_c12fd014)();
    _DAT_c12fd014 = (code *)0x0;
    if (iVar10 < 0) goto LAB_c009f1a8;
    iVar6 = *(int *)(pcVar9 + 0x18);
    pcVar9 = pcVar9 + 0x10;
  } while( true );
  pcVar9 = pcVar9 + 0x10;
  if (iVar10 < 0) {
LAB_c009f1a8:
    iVar6 = param_1 * 4 + _pulSectionTimingsInit;
    iVar10 = _tx_counter_get();
    *(int *)(iVar6 + -4) = iVar10 - iVar3;
    TimingMarkF(8,s__Section__d_Init____End_c00a2664,param_1);
    AppSetBootEvent(0x20000);
    if ((_pfxMountTlMedia != 0) &&
       (iVar10 = HceFastBootQueryMountPartition(param_1), iVar3 = _pfxMountTlMedia, iVar10 != 0)) {
      *(int *)(_pfxMountTlMedia + 0x1e4) = iVar10;
      AppClearBootEvent(0x10000);
      OsThreadCreateExSilent(s_AppLoadPartition_c00a268c,AppLoadPartition,iVar3,0x3f4,0x3f4,3,1);
      AppGetBootEvent(0x10000,2);
    }
    AppSetBootEventBySection(param_1);
    if (param_1 == 2) {
      AppSetBootEvent(0x20000000);
    }
    if (((local_38 & 1) != 0) && (uVar2 - 1 == param_1)) {
      AppInitFinished();
      TimingMark(8,s_APP_INIT_FINISHED_c00a26a0);
      AppSetBootEvent(0x40000000);
    }
    if (param_1 == uVar2) {
      if (((local_38 & 1) == 0) || (_eBootType == 0)) {
        AppInitFinished();
        TimingMark(8,s_APP_INIT_FINISHED_c00a26a0);
        AppSetBootEvent(0x40000000);
      }
      if (_pfxMountTlMedia != 0) {
        func_0xc01df1f0();
        PoolFreeEx(0,_pfxMountTlMedia,s_Zcam_Zboot_Boot_c_c00a212c,0x78a);
        _pfxMountTlMedia = 0;
      }
      AppPrintVersion();
      if (_pulSectionTimingsLoad != 0) {
        PoolFreeEx(0,_pulSectionTimingsLoad,s_Zcam_Zboot_Boot_c_c00a212c,0x795);
        _pulSectionTimingsLoad = 0;
      }
      if (_pulSectionTimingsInit != 0) {
        PoolFreeEx(0,_pulSectionTimingsInit,s_Zcam_Zboot_Boot_c_c00a212c,0x79a);
        _pulSectionTimingsInit = 0;
      }
    }
    AppStartHcePreBootProc(param_1);
    return;
  }
  goto LAB_c009f148;
}


//==== AppBootPrimary @ c009f46c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void AppBootPrimary(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 auStack_18 [8];
  
  if ((EBase & 0x3ff) != 1) {
    uVar1 = HceFastBootReturnTotalSections();
    _pulSectionTimingsLoad =
         (undefined4 *)PoolCallocEx(0,uVar1,4,1,s_Zcam_Zboot_Boot_c_c00a212c,0x1fc);
    *_pulSectionTimingsLoad = _ulEarlyBootTime;
    uVar1 = HceFastBootReturnTotalSections();
    _pulSectionTimingsInit = PoolCallocEx(0,uVar1,4,1,s_Zcam_Zboot_Boot_c_c00a212c,0x203);
    TimingMark(8,s__Boot_Prim____Start_c00a26b4);
    func_0xc00006c4();
    InitBlockedConfigList();
    _tx_event_flags_create(txBootEvent,s_BootEvent_c00a26c8);
    AppSetBootEvent(0x10000000);
    AppSetBootEvent(0x8000000);
    AppSetBootEvent(0x20000);
    AppBootSectionInit(1);
    iVar2 = DmaCfgShadowRead();
    uVar1 = 0;
    if ((uint)(iVar2 << 0x18) >> 0x1a != 0x1c) {
      iVar2 = HceGetResourceLocation(1);
      uVar1 = 0;
      if (iVar2 == 0) {
        uVar1 = HceGetResidentMemoryType();
        iVar2 = AppResourceToMediaType(uVar1,auStack_18);
        if ((iVar2 == 1 || iVar2 == 4) || (iVar2 == 5)) {
          iVar2 = FcuMediaInfoGet();
          if (iVar2 == 0) {
                    /* WARNING: Subroutine does not return */
            OsSysHaltEx(s_psMediaInfo_is_NULL_c00a26e8,s_Zcam_Zboot_Boot_c_c00a212c,0x281);
          }
          uVar1 = 2;
          if (*(char *)(*(int *)(iVar2 + 0x125c) + 0x398) != '\x01') {
            uVar1 = 0;
          }
        }
        else {
          uVar1 = 0;
        }
      }
    }
    _eBootType = uVar1;
    HceSecondaryBootDelay();
    AppLoadSecondarySegments(0);
    AppWaitBootComplete();
    func_0xc03092e8(0x9b00,0);
    TimingMark(8,s__Boot_Prim____End_c00a26d4);
    PerfInitGroup(4,100,0,PerfCaptBasePrintFunction);
  }
  if ((EBase & 0x3ff) == 1) {
    CopPrimaryBoot();
    return;
  }
  return;
}


//==== InflatingStreamDecode @ c00c94f0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 InflatingStreamDecode(int *param_1,undefined4 *param_2,uint param_3)

{
  bool bVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  code *pcVar7;
  undefined4 uVar8;
  uint local_98 [10];
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_48;
  int local_44;
  undefined4 local_40;
  int local_3c;
  uint local_38;
  undefined4 local_34;
  int local_30;
  
  local_48 = *param_2;
  local_40 = param_2[2];
  local_44 = param_2[1];
  local_3c = param_2[3];
  local_38 = param_3 & 0xff;
  uVar8 = 0xffffffff;
  iVar5 = *param_1;
  local_98[0] = 0;
  local_98[1] = 0;
  local_98[2] = 0;
  local_98[3] = 0;
  local_98[4] = 0;
  local_98[5] = 0;
  local_98[6] = 0;
  local_98[7] = 0;
  local_98[8] = 0;
  local_70 = 0;
  local_6c = 0;
  local_68 = 0;
  local_64 = 0;
  local_60 = 0;
  local_5c = 0;
  iVar6 = -4;
  local_58 = 0;
  local_54 = 0;
  local_50 = 0;
  local_34 = 0xffffffff;
  local_30 = -4;
  puVar3 = (undefined4 *)param_1[1];
  if (local_3c != 0) {
    _siAccumulatedOutput_0 = local_3c + _siAccumulatedOutput_0;
    local_30 = (*(code *)*puVar3)(puVar3,&local_40,&local_70);
    if (local_30 == 0) {
      CACHE_InvalidateDataCacheRegionEx(param_2[2],param_2[3],0,0);
      uVar8 = (**(code **)(iVar5 + 4))(iVar5,&local_70);
      (*(code *)puVar3[2])(puVar3,&local_70);
    }
  }
  uVar4 = 0;
  if ((_s_uiExpectedOutputSum_1 < _siAccumulatedOutput_0) && (local_44 != 0)) {
    pcVar7 = (code *)puVar3[1];
    while (iVar6 = (*pcVar7)(puVar3,&local_48,&local_40,local_98), uVar2 = local_34, iVar6 == 0) {
      CACHE_WriteBackInvalidateDataCacheRegionEx(*param_2,param_2[1],0,0);
      if ((local_98[0] & 5) != 0) {
        iVar6 = (*(code *)puVar3[4])(puVar3);
        _s_uiExpectedOutputSum_1 = iVar6 + _s_uiExpectedOutputSum_1;
      }
      uVar8 = (**(code **)(iVar5 + 4))(iVar5,local_98);
      (*(code *)puVar3[2])(puVar3,local_98);
      bVar1 = 10000000 < uVar4;
      uVar4 = uVar4 + 1;
      if (bVar1) {
                    /* WARNING: Subroutine does not return */
        OsSysHaltEx(s_Inflate_stuck_c00cb018,s_Os_Inflate_StreamHandling_Inflat_c00cb028,0xab);
      }
      iVar6 = 0;
      if ((_siAccumulatedOutput_0 <= _s_uiExpectedOutputSum_1) || (local_44 == 0))
      goto LAB_c00c9694;
      pcVar7 = (code *)puVar3[1];
    }
  }
  else {
LAB_c00c9694:
    uVar2 = local_34;
    if (local_30 == 0 || iVar6 == 0) {
      if ((_siAccumulatedOutput_0 < _s_uiExpectedOutputSum_1) ||
         ((local_44 != 0 && (_siAccumulatedOutput_0 == _s_uiExpectedOutputSum_1)))) {
        *param_2 = local_48;
        param_2[3] = 0;
        param_2[2] = 0;
        param_2[1] = local_44;
        return local_34;
      }
      uVar2 = uVar8;
      if (local_38 != 0) {
        (*(code *)puVar3[2])(puVar3,local_98);
        local_98[8] = _siAccumulatedOutput_0;
        (*(code *)puVar3[3])(puVar3,local_98);
        uVar2 = (**(code **)(iVar5 + 4))(iVar5,local_98);
        _siAccumulatedOutput_0 = 0;
        _s_uiExpectedOutputSum_1 = 0;
      }
    }
  }
  local_34 = uVar2;
  *param_2 = local_48;
  param_2[3] = local_3c;
  param_2[2] = local_40;
  param_2[1] = local_44;
  return local_34;
}


//==== InflatingStreamGetStatus @ c00c9850

void InflatingStreamGetStatus(int *param_1)

{
  (**(code **)(*param_1 + 8))();
  return;
}


//==== InflatingStreamDestroy @ c00c9874

void InflatingStreamDestroy(int *param_1)

{
  int iVar1;
  
  if (param_1 == (int *)0x0) {
    return;
  }
  if (param_1[1] == 0) {
    iVar1 = *param_1;
  }
  else {
    InflatingStreamDataDestroy(param_1[1]);
    iVar1 = *param_1;
  }
  if (iVar1 == 0) {
    PoolFreeEx(0,param_1,s_Os_Inflate_StreamHandling_Inflat_c00cb028,0xf0);
    return;
  }
  InflateProcessorDestroy(iVar1);
  PoolFreeEx(0,param_1,s_Os_Inflate_StreamHandling_Inflat_c00cb028,0xf0);
  return;
}


//==== InflatingStreamCreate @ c00c9914

int InflatingStreamCreate(undefined4 *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  
  puVar1 = (undefined4 *)PoolAllocEx(0,8,0,s_Os_Inflate_StreamHandling_Inflat_c00cb028,0x47);
  iVar2 = -4;
  if (puVar1 != (undefined4 *)0x0) {
    *puVar1 = 0;
    iVar2 = InflateProcessorCreate(puVar1);
    if (iVar2 == 0) {
      puVar1[1] = 0;
      iVar2 = InflatingStreamDataCreate(puVar1 + 1);
      if (iVar2 == 0) {
        *param_1 = puVar1;
        return 0;
      }
    }
  }
  InflatingStreamDestroy(puVar1);
  return iVar2;
}


//==== InflatingStreamDataDestroy @ c00ca9bc

void InflatingStreamDataDestroy(int param_1)

{
  if (param_1 == 0) {
    return;
  }
  if (*(int *)(param_1 + 0x14) == 0) {
    PoolFreeEx(0,param_1,s_Os_Inflate_StreamHandling_Inflat_c00cb154,0x9b);
    return;
  }
  PoolFreeEx(0,*(int *)(param_1 + 0x14),s_Os_Inflate_StreamHandling_Inflat_c00cb154,0x99);
  PoolFreeEx(0,param_1,s_Os_Inflate_StreamHandling_Inflat_c00cb154,0x9b);
  return;
}


//==== InflatingStreamDataCreate @ c00caa54

undefined4 InflatingStreamDataCreate(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  
  puVar1 = (undefined4 *)PoolAllocEx(0,0x18,0,s_Os_Inflate_StreamHandling_Inflat_c00cb154,100);
  if (puVar1 != (undefined4 *)0x0) {
    puVar2 = (undefined4 *)PoolAllocEx(0,0x20,0,s_Os_Inflate_StreamHandling_Inflat_c00cb154,0x6b);
    if (puVar2 != (undefined4 *)0x0) {
      *param_1 = puVar1;
      puVar1[5] = puVar2;
      puVar1[1] = InflatingStreamSetupInputJob;
      *puVar1 = InflatingStreamSetupOutputJob;
      puVar1[2] = InflatingStreamClearJob;
      puVar1[3] = InflatingStreamSetupFinishJob;
      puVar1[4] = GetCurrentBlockUncompressedSise;
      puVar2[4] = 0;
      puVar2[6] = 0;
      puVar2[5] = 0;
      puVar2[7] = 0;
      *puVar2 = 0;
      puVar2[2] = 0;
      puVar2[1] = 0;
      *(undefined2 *)((int)puVar2 + 0xe) = 0;
      *(undefined2 *)(puVar2 + 3) = 0;
      return 0;
    }
  }
  InflatingStreamDataDestroy(puVar1);
  return 0xfffffffc;
}


//==== InflatingStreamSetupOutputJob @ c00cab68

undefined4 InflatingStreamSetupOutputJob(int param_1,uint *param_2,uint *param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  
  iVar2 = *(int *)(param_1 + 0x14);
  uVar4 = 0xfffffffd;
  if (((param_2[1] != 0) && (uVar1 = *param_2, uVar1 != 0)) &&
     (*(int *)(iVar2 + 0x18) + *(int *)(iVar2 + 0x1c) != uVar1)) {
    *(undefined4 *)(iVar2 + 0x1c) = 0;
    *(uint *)(iVar2 + 0x18) = uVar1;
    uVar1 = param_2[1];
    uVar3 = *param_2;
    uVar4 = 0;
    *param_3 = *param_3 | 0x20;
    param_3[7] = uVar3;
    param_3[8] = uVar1;
  }
  return uVar4;
}


//==== InflatingStreamSetupInputJob @ c00cabc8

/* WARNING: Removing unreachable block (ram,0xc00cac1c) */

undefined4 InflatingStreamSetupInputJob(int param_1,uint *param_2,int *param_3,uint *param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  undefined4 uVar8;
  int iVar9;
  
  uVar5 = param_2[1];
  iVar7 = *(int *)(param_1 + 0x14);
  uVar8 = 0xfffffffd;
  if ((uVar5 == 0) || ((void *)*param_2 == (void *)0x0)) {
    return 0xfffffffd;
  }
  uVar3 = *(uint *)(iVar7 + 0x14);
  if (uVar3 < 0x10) {
    uVar4 = 0x10 - uVar3;
    if (uVar5 <= 0x10 - uVar3) {
      uVar4 = uVar5;
    }
    memcpy((void *)(uVar3 * 0x10 + iVar7),(void *)*param_2,uVar4);
    uVar6 = *param_2;
    *(uint *)(iVar7 + 0x14) = *(int *)(iVar7 + 0x14) + uVar4;
    uVar5 = param_2[1] - uVar4;
    param_2[1] = uVar5;
    uVar3 = *(uint *)(iVar7 + 0x14);
    *param_2 = uVar6 + uVar4;
    if (uVar3 < 0x10) {
      return 0xfffffffd;
    }
  }
  uVar3 = *(uint *)(iVar7 + 0x14);
  uVar4 = *(ushort *)(iVar7 + 0xc) + 0x10;
  uVar6 = *(ushort *)(iVar7 + 0xe) + uVar4;
  if (uVar3 < uVar4) {
    uVar1 = uVar4 - uVar3;
    if (uVar5 <= uVar4 - uVar3) {
      uVar1 = uVar5;
    }
    param_4[2] = uVar1;
    uVar5 = *param_4 | 1;
    if (*(int *)(iVar7 + 0x14) != 0x10) {
      uVar5 = *param_4 | 2;
    }
    *param_4 = uVar5;
    param_2[1] = param_2[1] - uVar1;
    iVar9 = *(int *)(iVar7 + 0x14);
    param_4[1] = *param_2;
    uVar3 = iVar9 + uVar1;
    *param_2 = *param_2 + uVar1;
    *(uint *)(iVar7 + 0x14) = uVar3;
    if (uVar4 <= uVar3) goto LAB_c00cace0;
LAB_c00cad4c:
    if (uVar3 < uVar6) {
      uVar5 = *param_4;
      goto LAB_c00cadf4;
    }
    iVar9 = *(int *)(iVar7 + 8);
  }
  else {
LAB_c00cace0:
    if (uVar3 < uVar6) {
      uVar5 = param_2[1];
      if (uVar6 - uVar3 < param_2[1]) {
        uVar5 = uVar6 - uVar3;
      }
      param_4[4] = uVar5;
      uVar3 = *param_4 | 4;
      if (*(uint *)(iVar7 + 0x14) != uVar4) {
        uVar3 = *param_4 | 8;
      }
      *param_4 = uVar3;
      param_2[1] = param_2[1] - uVar5;
      param_4[3] = *param_2;
      uVar3 = *(int *)(iVar7 + 0x14) + uVar5;
      *param_2 = *param_2 + uVar5;
      *(uint *)(iVar7 + 0x14) = uVar3;
      goto LAB_c00cad4c;
    }
    iVar9 = *(int *)(iVar7 + 8);
  }
  uVar5 = (iVar9 + uVar6) - uVar3;
  if (param_2[1] <= uVar5) {
    uVar5 = param_2[1];
  }
  uVar5 = uVar5 & 0xffffff80;
  if (uVar5 == 0) {
    uVar5 = *param_4;
    goto LAB_c00cadf4;
  }
  uVar4 = param_3[1];
  if (uVar4 == 0) {
    uVar5 = *param_4;
    goto LAB_c00cadf4;
  }
  if (uVar3 == uVar6) {
    if (*param_3 != 0) {
      uVar1 = *(uint *)(iVar7 + 4);
      if (uVar4 < *(uint *)(iVar7 + 4)) {
        uVar1 = uVar4;
      }
      param_3[1] = uVar4 - uVar1;
      iVar2 = *(int *)(iVar7 + 0x1c);
      uVar3 = *(uint *)(iVar7 + 0x14);
      *param_3 = *param_3 + uVar1;
      *(uint *)(iVar7 + 0x1c) = iVar2 + uVar1;
    }
    if (uVar3 != uVar6) {
      uVar3 = *(uint *)(iVar7 + 0x10);
      goto LAB_c00cad94;
    }
    uVar4 = *param_2;
LAB_c00cada0:
    *(uint *)(iVar7 + 0x10) = uVar4 + uVar5;
    uVar4 = *param_2;
    uVar3 = *param_4 | 0x40;
  }
  else {
    uVar3 = *(uint *)(iVar7 + 0x10);
LAB_c00cad94:
    uVar4 = *param_2;
    if (uVar3 != uVar4) goto LAB_c00cada0;
    uVar3 = *param_4;
  }
  *param_4 = uVar3 | 0x10;
  param_4[6] = uVar5;
  uVar3 = param_2[1];
  param_4[5] = uVar4;
  param_2[1] = uVar3 - uVar5;
  uVar3 = *param_2;
  iVar2 = *(int *)(iVar7 + 0x14) + uVar5;
  if (iVar2 == iVar9 + uVar6) {
    iVar2 = 0;
  }
  *(int *)(iVar7 + 0x14) = iVar2;
  *param_2 = uVar3 + uVar5;
  uVar5 = *param_4;
LAB_c00cadf4:
  if (uVar5 != 0x20 && uVar5 != 0) {
    uVar8 = 0;
  }
  return uVar8;
}


//==== InflatingStreamClearJob @ c00caee8

void InflatingStreamClearJob(undefined4 param_1,undefined4 *param_2)

{
  *param_2 = 0;
  param_2[1] = 0;
  param_2[3] = 0;
  param_2[7] = 0;
  param_2[5] = 0;
  param_2[2] = 0;
  param_2[4] = 0;
  param_2[8] = 0;
  param_2[6] = 0;
  return;
}


//==== InflatingStreamSetupFinishJob @ c00caf1c

void InflatingStreamSetupFinishJob(undefined4 param_1,uint *param_2)

{
  *param_2 = *param_2 | 0x100;
  return;
}


