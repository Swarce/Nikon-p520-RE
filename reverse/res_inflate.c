//==== AppElfLoadFile @ c009ce20

undefined4 AppElfLoadFile(undefined4 param_1,uint param_2,uint param_3,undefined4 param_4)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  undefined1 auStack_b0 [42];
  ushort local_86;
  ushort local_84;
  int local_78 [2];
  uint local_70;
  uint local_68;
  undefined4 local_64;
  uint local_58;
  uint local_54;
  uint local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  uint local_40;
  uint local_3c;
  uint local_38;
  uint local_34;
  undefined4 local_30;
  int local_2c;
  
  local_40 = param_2 & 0xff;
  local_3c = param_3 & 0xff;
  local_58 = 0;
  local_54 = 0;
  local_50 = 0;
  local_4c = 0;
  local_48 = 0;
  local_44 = 0;
  iVar2 = func_0xc01d82f4(param_1,auStack_b0,0x34,0);
  uVar5 = 0;
  if (iVar2 == 0) {
    local_38 = (uint)local_84;
    local_34 = (uint)local_86;
    uVar9 = 0;
    if (local_38 != 0) {
      local_2c = -0x3ff60000;
      do {
        while( true ) {
          uVar10 = 0;
          iVar2 = func_0xc01e4680(param_1);
          if (iVar2 != 0) {
            return 0;
          }
          iVar2 = func_0xc01d82f4(param_1,local_78,0x20,0);
          if (iVar2 != 0) {
            return 0;
          }
          local_30 = local_64;
          if (local_70 + 0x40400000 < 0x8000) break;
          uVar8 = local_68;
          uVar10 = local_70 & 0x9000ffff;
          if (((local_70 & 0xbfc00000) == 0xbfc00000) ||
             (uVar10 = local_70, (local_70 & 0x90000000) == 0x90000000)) goto LAB_c009cec4;
          bVar1 = local_68 != 0;
          uVar10 = local_70 | 0x20000000;
          if (local_78[0] == 1 && bVar1) goto LAB_c009cfc8;
LAB_c009cee4:
          uVar9 = uVar9 + 1;
          if (local_38 <= uVar9) goto LAB_c009d0fc;
        }
        uVar8 = 0;
LAB_c009cec4:
        bVar1 = uVar8 != 0;
        if (local_78[0] != 1 || !bVar1) goto LAB_c009cee4;
LAB_c009cfc8:
        iVar2 = func_0xc01e4680(param_1);
        if (iVar2 != 0) {
          return 0;
        }
        uVar4 = uVar10;
        if (local_3c != 0) {
          uVar4 = PoolAllocEx(0,uVar8,1,local_2c + 0x212c,0x5f7);
        }
        TimingMark(8,s__Segment_Load____Start_c00a2238);
        iVar2 = DAT_c00a2700;
        if (local_40 == 1) {
          uVar7 = 0;
          if (!bVar1) goto LAB_c009d16c;
          uVar6 = 0;
          do {
            iVar3 = uVar8 - uVar7;
            if (uVar7 + iVar2 <= uVar8) {
              iVar3 = iVar2;
            }
            uVar7 = uVar7 + iVar3;
            iVar3 = func_0xc01d82f4(param_1,uVar6 * 4 + uVar4,iVar3,0);
            if (iVar3 != 0) {
              return 0;
            }
            uVar6 = uVar7 >> 2;
          } while (uVar7 < uVar8);
          TimingMark(8,s__Segment_Load____End_c00a2250);
          if (local_3c == 0) goto LAB_c009cee4;
          local_48 = 0;
          _tx_queue_send(DAT_c00a2704,&local_58,0xffffffff);
          local_4c = local_30;
          local_48 = 3;
          local_58 = uVar4;
          local_54 = uVar10;
          local_50 = uVar8;
          _tx_queue_send(DAT_c00a2704,&local_58,0xffffffff);
          local_48 = 1;
          _tx_queue_send(DAT_c00a2704,&local_58,0xffffffff);
        }
        else {
          iVar2 = func_0xc01d82f4(param_1,uVar4,uVar8,0);
          if (iVar2 != 0) {
            return 0;
          }
LAB_c009d16c:
          TimingMark(8,s__Segment_Load____End_c00a2250);
          if (local_3c == 0) goto LAB_c009cee4;
          local_48 = 0;
          _tx_queue_send(DAT_c00a2704,&local_58,0xffffffff);
          local_4c = local_30;
          local_48 = 3;
          local_58 = uVar4;
          local_54 = uVar10;
          local_50 = uVar8;
          _tx_queue_send(DAT_c00a2704,&local_58,0xffffffff);
          local_48 = 1;
          _tx_queue_send(DAT_c00a2704,&local_58,0xffffffff);
        }
        uVar9 = uVar9 + 1;
      } while (uVar9 < local_38);
    }
LAB_c009d0fc:
    local_48 = 5;
    local_44 = param_4;
    _tx_queue_send(0xc12fd04c,&local_58,0xffffffff);
    uVar5 = 1;
  }
  return uVar5;
}


//==== AppElfLoad @ c009d25c

undefined4
AppElfLoad(undefined4 param_1,undefined4 param_2,undefined1 param_3,undefined1 param_4,
          undefined4 param_5)

{
  int iVar1;
  undefined1 auStack_210 [504];
  
  memset(auStack_210,0,0x1f8);
  iVar1 = func_0xc01dac84(param_1,auStack_210,param_2,0);
  if (iVar1 != 0) {
    return 0;
  }
  iVar1 = AppElfLoadFile(auStack_210,param_3,param_4,param_5);
  if (iVar1 == 0) {
    func_0xc01e38cc(auStack_210);
    return 0;
  }
  func_0xc01e38cc(auStack_210);
  return 1;
}


//==== AppElfLoadScratchpad @ c00a0380

undefined4 AppElfLoadScratchpad(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  undefined1 auStack_18 [8];
  
  uVar1 = HceGetResidentMemoryType();
  uVar1 = AppResourceToMediaType(uVar1,auStack_18);
  iVar2 = FcuMediaInfoGet(uVar1);
  puVar4 = *(undefined4 **)(iVar2 + 0x125c);
  uVar1 = BootTlInit(uVar1);
  iVar2 = HceGetResourceLocation(1);
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    OsSysHaltEx(s_Supported_only_from_NAND_source_b_c00a28c4,s_Zcam_Zboot_Loader_Loader_c_c00a2878,
                0x185);
  }
  if (puVar4[3] == 0) {
    uVar3 = LoadScratchpadSegments(uVar1,*puVar4);
    BootTlDestroy(uVar1);
    return uVar3;
  }
  uVar3 = LoadScratchpadSegments(uVar1,puVar4[4]);
  BootTlDestroy(uVar1);
  return uVar3;
}


//==== CreateInflater @ c00a044c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void CreateInflater(void)

{
  _DAT_c12fd084 = PoolAllocEx(0,0x78,0,s_Zcam_Zboot_Inflater_Inflater_c_c00a28f4,0xab);
  _tx_queue_create(0xc12fd04c,s_InflaterQueue_c00a2914,6,_DAT_c12fd084,0x78);
  OsThreadCreateEx(s_Inflater_c00a2924,InflateElfStream,0,0x3f4,0x3f4,3,1);
  return;
}


//==== DestroyInflater @ c00a04e0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DestroyInflater(void)

{
  int iVar1;
  undefined4 in_a3;
  int local_28 [6];
  
  while (iVar1 = _tx_queue_receive(0xc12fd04c,local_28,0,in_a3), iVar1 == 0) {
    while( true ) {
      in_a3 = 0xcd;
      if (local_28[0] == 0) break;
      PoolFreeEx(0,local_28[0],s_Zcam_Zboot_Inflater_Inflater_c_c00a28f4);
      local_28[0] = 0;
      iVar1 = _tx_queue_receive(0xc12fd04c,local_28,0);
      if (iVar1 != 0) goto LAB_c00a0558;
    }
  }
LAB_c00a0558:
  _tx_queue_delete(0xc12fd04c);
  PoolFreeEx(0,_DAT_c12fd084,s_Zcam_Zboot_Inflater_Inflater_c_c00a28f4,0xd6);
  _DAT_c12fd084 = 0;
  return;
}


//==== InflateElfStream @ c00a0590

/* WARNING: Removing unreachable block (ram,0xc00a0680) */
/* WARNING: Removing unreachable block (ram,0xc00a0658) */
/* WARNING: Removing unreachable block (ram,0xc00a062c) */
/* WARNING: Removing unreachable block (ram,0xc00a0670) */
/* WARNING: Removing unreachable block (ram,0xc00a0718) */

void InflateElfStream(void)

{
  undefined4 uVar1;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20 [2];
  
  local_20[0] = 0;
  local_48 = 0;
  local_44 = 0;
  local_40 = 0;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  do {
    _tx_queue_receive(0xc12fd04c,&local_38,0xffffffff);
    switch(local_28) {
    case 0:
      InflatingStreamCreate(local_20);
      break;
    case 1:
      InflatingStreamDestroy(local_20[0]);
      break;
    case 3:
      local_40 = local_34;
      local_3c = local_2c;
      local_48 = local_38;
      local_44 = local_30;
      TimingMark(8,s__Segment_Inflate____Start_c00a2930);
      uVar1 = InflatingStreamDecode(local_20[0],&local_48,1);
      TimingMark(8,s__Segment_Inflate____End_c00a294c);
      InflatingStreamGetStatus(local_20[0],uVar1,1,1);
      PoolFreeEx(0,local_38,s_Zcam_Zboot_Inflater_Inflater_c_c00a28f4,0x85);
      break;
    case 4:
      DestroyInflater();
      return;
    case 5:
      AppBootStartSectionProc(local_24);
    case 2:
      break;
    default:
                    /* WARNING: Subroutine does not return */
      OsSysHaltEx(s_Inflater__Unrecognized_type_c00a2964,s_Zcam_Zboot_Inflater_Inflater_c_c00a28f4,
                  0x96);
    }
  } while( true );
}


//==== IP_Config @ c00c99c4

undefined4 IP_Config(void)

{
  return 0xfffffffc;
}


//==== IP_GetStatus @ c00c99cc

byte IP_GetStatus(undefined4 param_1,int param_2,uint param_3,char param_4)

{
  bool bVar1;
  bool bVar2;
  
  bVar2 = (param_3 & 1) == 0;
  bVar1 = (param_3 & 2) == 0;
  do {
    if (bVar2 == false) {
      bVar2 = ((*(uint *)(param_2 + 0x2c) ^ *(uint *)(param_2 + 8)) & 0x150) == 0;
    }
    if (bVar1 == false) {
      bVar1 = ((*(uint *)(param_2 + 0x2c) ^ *(uint *)(param_2 + 8)) & 0xa0) == 0;
    }
  } while ((param_4 == '\x01') && (bVar2 != true || bVar1 != true));
  return bVar2 & bVar1;
}


//==== IP_Reset @ c00c9a38

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void IP_Reset(int param_1)

{
  undefined4 *puVar1;
  
  puVar1 = *(undefined4 **)(param_1 + 0x10);
  _DAT_b9800004 = 1;
  *puVar1 = 0;
  puVar1[0x2b] = 0;
  return;
}


//==== IP_ConfigureClient @ c00c9a5c

void IP_ConfigureClient(undefined4 param_1,undefined4 *param_2)

{
  int iVar1;
  
  iVar1 = param_2[1];
  DcuClientSet(param_1,1,*param_2);
  DcuClientSet(param_1,2,iVar1 + 3U >> 2);
  DcuClientSubmit(param_1);
  DcuClientStart(param_1);
  return;
}


//==== IP_LoadVlcTable @ c00c9ab8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void IP_LoadVlcTable(int *param_1,uint param_2,int *param_3)

{
  uint uVar1;
  undefined4 *puVar2;
  int *piVar3;
  undefined4 *puVar4;
  uint uVar5;
  
  uVar5 = (uint)param_3[1] >> 2;
  puVar4 = (undefined4 *)*param_3;
  uVar1 = uVar5;
  if ((param_2 & 5) != 0) {
    if ((*param_1 == 0) || (*param_1 == 3)) {
      _DAT_b9800020 = 0;
      *param_1 = 1;
      param_1[0x2b] = 0;
    }
    puVar2 = (undefined4 *)&DAT_b9800028;
    if ((param_2 & 3) == 0) {
      puVar2 = (undefined4 *)&DAT_b980002c;
    }
    *puVar2 = *puVar4;
    piVar3 = (int *)&DAT_b9800030;
    if ((param_2 & 3) == 0) {
      piVar3 = (int *)&DAT_b9800034;
    }
    *piVar3 = param_1[0x2b];
  }
  for (; uVar1 != 0; uVar1 = uVar1 - 1) {
    _DAT_b9800024 = *puVar4;
    puVar4 = puVar4 + 1;
  }
  param_1[0x2b] = param_1[0x2b] + uVar5;
  return;
}


//==== IP_DeinitializeIsrs @ c00c9b74

void IP_DeinitializeIsrs(int param_1,int param_2)

{
  if (param_2 != 1) {
    _tx_isr_clear();
    _tx_isr_clear(0x12d);
    _tx_isr_clear(299);
    _tx_isr_unregister(param_1 + 4);
    _tx_isr_unregister(param_1 + 0x38);
    _tx_isr_unregister(param_1 + 0x6c);
    return;
  }
  _tx_isr_mask(0xc4);
  _tx_isr_mask(0x12d);
  _tx_isr_mask(299);
  return;
}


//==== IP_Wait4flush @ c00c9bf0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void IP_Wait4flush(int param_1)

{
  bool bVar1;
  uint uVar2;
  
  if (param_1 != 0) {
    uVar2 = 0;
    do {
      bVar1 = 10000000 < uVar2;
      uVar2 = uVar2 + 1;
      if (bVar1) {
                    /* WARNING: Subroutine does not return */
        OsSysHaltEx(s_Inflate_stuck_c00cb054,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x29a);
      }
    } while ((char)_DAT_b9800014 == '\0');
  }
  return;
}


//==== IsrInfEOB @ c00c9c58

void IsrInfEOB(undefined4 param_1,undefined4 *param_2)

{
  *param_2 = 0;
  param_2[0x2b] = 0;
  return;
}


//==== InflateProcessorCreate @ c00c9c68

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 InflateProcessorCreate(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_24;
  int local_20 [2];
  
  puVar1 = puRamc00caf48;
  memset(puRamc00caf48,0,0xcc);
  puVar1[0x2a] = 0;
  *puVar1 = 0;
  local_20[0] = 0x34;
  uVar2 = PoolAllocEx(0,0x14,0,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x33b);
  puVar1[0x2c] = uVar2;
  uVar2 = PoolAllocEx(0,0x14,0,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x33c);
  puVar1[0x2d] = uVar2;
  uVar2 = PoolAllocEx(0,0x14,0,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x33d);
  puVar1[0x2e] = uVar2;
  uVar2 = PoolAllocEx(0,0x14,0,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x33e);
  puVar1[0x2f] = uVar2;
  uVar3 = PoolAllocEx(0,0x14,0,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x33f);
  uVar2 = _DAT_c12e3eec;
  puVar1[0x30] = uVar3;
  iVar4 = PoolCreateEx(uVar2,local_20[0] * 500,1,1,local_20,
                       s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x345);
  puVar1[0x31] = iVar4;
  if ((((puVar1[0x30] != 0) && (puVar1[0x2e] != 0)) && (puVar1[0x2f] != 0)) &&
     (((puVar1[0x2c] != 0 && (puVar1[0x2d] != 0)) && (iVar4 != 0)))) {
    LinkedListCreate(puVar1[0x2c],0,0);
    LinkedListCreate(puVar1[0x2d],0,0);
    LinkedListCreate(puVar1[0x2e],0,0);
    LinkedListCreate(puVar1[0x2f],0,0);
    LinkedListCreate(puVar1[0x30],0,0);
    _tx_isr_register(puVar1 + 1,0xc4,IsrInfEOB,puVar1,2,1);
    _tx_isr_unmask(0xc4);
    _tx_isr_register(puVar1 + 0xe,0x12d,IsrInfEOI,puVar1,2,1);
    _tx_isr_unmask(0x12d);
    _tx_isr_register(puVar1 + 0x1b,299,IsrInfEOO,puVar1,2,1);
    _tx_isr_unmask(299);
    CguUnitSetEnabled(_sInflateCguUnits,1);
    _DAT_b980001c = 1;
    _DAT_b9800004 = 1;
    _DAT_b980000c = puVar1[0x2a];
    local_24 = 1;
    local_2c = 0;
    local_30 = 0;
    uVar2 = DcuClientOpen(0xe0,&local_30);
    puVar1[0x28] = uVar2;
    DcuSetClientParam(uVar2,0,0x20);
    DcuSetClientDataConfig(puVar1[0x28],0);
    local_30 = 1;
    local_24 = 1;
    local_2c = 0;
    uVar2 = DcuClientOpen(0xde,&local_30);
    puVar1[0x29] = uVar2;
    DcuSetClientParam(uVar2,0,0x20);
    *param_1 = 0xc00caf38;
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  OsSysHaltEx(s_Inflate___Failed_allocate_memory_c00cb094,
              s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x34a);
}


//==== IP_QueueAdd @ c00c9f70

void IP_QueueAdd(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  
  uVar1 = _tx_thread_interrupt_control(0);
  LinkedListInsertToHead(param_1,param_2);
  *(int *)(param_2 + 0x30) = *(int *)(param_2 + 0x30) + 1;
  _tx_thread_interrupt_control(uVar1);
  return;
}


//==== IP_QueueTop @ c00c9fcc

undefined4 IP_QueueTop(undefined4 param_1)

{
  undefined4 local_10 [2];
  
  LinkedListEnd(param_1,local_10);
  return local_10[0];
}


//==== IP_QueueRemove @ c00c9fec

int IP_QueueRemove(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = IP_QueueTop();
  if (iVar1 != 0) {
    uVar2 = _tx_thread_interrupt_control(0);
    LinkedListRemoveNode(param_1,iVar1);
    *(int *)(iVar1 + 0x30) = *(int *)(iVar1 + 0x30) + -1;
    _tx_thread_interrupt_control(uVar2);
  }
  return iVar1;
}


//==== IP_ReleaseJobs @ c00ca05c

void IP_ReleaseJobs(undefined4 param_1,undefined4 param_2)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  
  uVar3 = 0;
  do {
    while( true ) {
      iVar2 = IP_QueueRemove(param_1);
      if (iVar2 == 0) {
        return;
      }
      if (*(int *)(iVar2 + 0x30) == 0) break;
      bVar1 = 10000000 < uVar3;
      uVar3 = uVar3 + 1;
      if (bVar1) goto LAB_c00ca0d8;
    }
    PoolFreeEx(param_2,iVar2,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x500);
    bVar1 = uVar3 < 0x989681;
    uVar3 = uVar3 + 1;
  } while (bVar1);
LAB_c00ca0d8:
                    /* WARNING: Subroutine does not return */
  OsSysHaltEx(s_Inflate_stuck_c00cb054,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x504);
}


//==== InflateProcessorDestroy @ c00ca10c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 InflateProcessorDestroy(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x10);
  IP_DeinitializeIsrs(iVar1,1);
  DcuClientStop(*(undefined4 *)(iVar1 + 0xa0),1);
  DcuClientClose(*(undefined4 *)(iVar1 + 0xa0));
  DcuClientStop(*(undefined4 *)(iVar1 + 0xa4),1);
  DcuClientClose(*(undefined4 *)(iVar1 + 0xa4));
  _DAT_b980001c = 0;
  CguUnitSetEnabled(_sInflateCguUnits,0);
  IP_DeinitializeIsrs(iVar1,0);
  IP_ReleaseJobs(*(undefined4 *)(iVar1 + 0xb0),*(undefined4 *)(iVar1 + 0xc4));
  IP_ReleaseJobs(*(undefined4 *)(iVar1 + 0xb4),*(undefined4 *)(iVar1 + 0xc4));
  IP_ReleaseJobs(*(undefined4 *)(iVar1 + 0xb8),*(undefined4 *)(iVar1 + 0xc4));
  IP_ReleaseJobs(*(undefined4 *)(iVar1 + 0xbc),*(undefined4 *)(iVar1 + 0xc4));
  IP_ReleaseJobs(*(undefined4 *)(iVar1 + 0xc0),*(undefined4 *)(iVar1 + 0xc4));
  LinkedListDestroy(*(undefined4 *)(iVar1 + 0xb0));
  LinkedListDestroy(*(undefined4 *)(iVar1 + 0xb4));
  LinkedListDestroy(*(undefined4 *)(iVar1 + 0xb8));
  LinkedListDestroy(*(undefined4 *)(iVar1 + 0xbc));
  LinkedListDestroy(*(undefined4 *)(iVar1 + 0xc0));
  PoolDestroy(*(undefined4 *)(iVar1 + 0xc4));
  PoolFreeEx(0,*(undefined4 *)(iVar1 + 0xb0),s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x36e);
  PoolFreeEx(0,*(undefined4 *)(iVar1 + 0xb4),s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x36f);
  PoolFreeEx(0,*(undefined4 *)(iVar1 + 0xb8),s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x370);
  PoolFreeEx(0,*(undefined4 *)(iVar1 + 0xbc),s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x371);
  PoolFreeEx(0,*(undefined4 *)(iVar1 + 0xc0),s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x372);
  return 0;
}


//==== IP_QueueTransfer @ c00ca26c

void IP_QueueTransfer(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = IP_QueueTop();
  uVar2 = _tx_thread_interrupt_control(0);
  LinkedListRemoveNode(param_1,uVar1);
  LinkedListInsertToHead(param_2,uVar1);
  _tx_thread_interrupt_control(uVar2);
  return;
}


//==== IP_Execute @ c00ca2d8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void IP_Execute(int *param_1)

{
  bool bVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  char local_20;
  char local_1f;
  char local_1e;
  char local_1d;
  char local_1c [4];
  
  local_20 = '\x01';
  local_1f = '\x01';
  local_1e = '\0';
  local_1d = '\0';
  local_1c[0] = '\0';
  uVar2 = _tx_thread_interrupt_control(0);
  LinkedListIsEmpty(param_1[0x2d],&local_1e);
  if (local_1e == '\x01') {
    LinkedListIsEmpty(param_1[0x2c],&local_20);
    if (local_20 == '\0') {
      IP_QueueTransfer(param_1[0x2c],param_1[0x2d]);
      iVar4 = IP_QueueTop(param_1[0x2d]);
      if ((*(uint *)(iVar4 + 8) & 0x100) == 0) {
        if ((*(uint *)(iVar4 + 8) & 0x40) != 0) {
          _DAT_b9800010 = 2;
          IP_Wait4flush(0);
          *(uint *)(iVar4 + 0x2c) = *(uint *)(iVar4 + 0x2c) | *(uint *)(iVar4 + 8) & 0x40;
        }
        if (*(int *)(iVar4 + 0x20) == 0) {
          iVar4 = param_1[0x2f];
        }
        else {
          if ((uint)((*(int *)(iVar4 + 0x20) + 3) * 0x2000000) >> 0x1b != 0) {
                    /* WARNING: Subroutine does not return */
            OsSysHaltEx(s_Input_buffer_size_doesn_t_meet_a_c00cb0b8,
                        s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x44f);
          }
          IP_ConfigureClient(param_1[0x28],iVar4 + 0x1c);
          iVar4 = param_1[0x2f];
        }
      }
      else {
        param_1[0x32] = *(int *)(iVar4 + 0x28);
        if ((param_1[0x32] & 0x7fU) == 0) {
          uVar3 = *(uint *)(iVar4 + 8);
        }
        else {
          uVar3 = 0;
          do {
            bVar1 = 10000000 < uVar3;
            uVar3 = uVar3 + 1;
            if (bVar1) {
                    /* WARNING: Subroutine does not return */
              OsSysHaltEx(s_Inflate_stuck_c00cb054,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x2ac
                         );
            }
          } while ((_DAT_b980003c & 0xfff) != 0xa0);
          _DAT_b9800010 = 1;
          IP_Wait4flush(1);
          uVar3 = *(uint *)(iVar4 + 8);
        }
        *(uint *)(iVar4 + 0x2c) = *(uint *)(iVar4 + 0x2c) | uVar3 & 0x100;
        local_20 = '\x01';
        *param_1 = 0;
        iVar4 = param_1[0x2f];
      }
    }
    else {
      iVar4 = param_1[0x2f];
    }
  }
  else {
    iVar4 = param_1[0x2f];
  }
  LinkedListIsEmpty(iVar4,&local_1d);
  if ((local_1d == '\x01') && (LinkedListIsEmpty(param_1[0x2e],&local_1f), local_1f == '\0')) {
    IP_QueueTransfer(param_1[0x2e],param_1[0x2f]);
    iVar4 = IP_QueueTop(param_1[0x2f]);
    uVar3 = *(uint *)(iVar4 + 8) & 0x80;
    if (uVar3 != 0) {
      *(uint *)(iVar4 + 0x2c) = *(uint *)(iVar4 + 0x2c) | uVar3;
    }
    if (*(int *)(iVar4 + 0x28) != 0) {
      IP_ConfigureClient(param_1[0x29],iVar4 + 0x24);
    }
  }
  if (*param_1 == 1) {
    if (local_20 != '\0') goto LAB_c00ca3a8;
    *param_1 = 2;
    _DAT_b9800000 = 1;
  }
  else {
    if ((local_20 == '\0') || (local_1f == '\0')) {
      if (*param_1 != 0) {
        _DAT_b9800018 = (uint)(local_20 == '\0') | (uint)(local_1f == '\0') << 1;
        *param_1 = 2;
        goto LAB_c00ca3a8;
      }
    }
    else if (*param_1 != 0) goto LAB_c00ca3a8;
    if ((local_1f != '\0') || (LinkedListIsEmpty(param_1[0x30],local_1c), local_1c[0] != '\0')) {
LAB_c00ca3a8:
      _tx_thread_interrupt_control(uVar2);
      return;
    }
    _DAT_b9800018 = (uint)(local_1f == '\0') << 1;
  }
  _tx_thread_interrupt_control(uVar2);
  return;
}


//==== IP_SubmitJob @ c00ca610

int IP_SubmitJob(int param_1,uint *param_2)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  uint *puVar4;
  uint *puVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int *piVar10;
  undefined1 local_20 [8];
  
  piVar10 = *(int **)(param_1 + 0x10);
  iVar2 = PoolAllocEx(piVar10[0x31],1,0,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x381);
  local_20[0] = 0;
  if (iVar2 == 0) {
                    /* WARNING: Subroutine does not return */
    OsSysHaltEx(s_Inflate___no_memory__Enlarge_fix_c00cb0f0,
                s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x385);
  }
  puVar5 = (uint *)(iVar2 + 8);
  puVar4 = param_2;
  do {
    uVar8 = *puVar4;
    uVar7 = puVar4[1];
    uVar6 = puVar4[2];
    uVar3 = puVar4[3];
    puVar4 = puVar4 + 4;
    *puVar5 = uVar8;
    puVar5[1] = uVar7;
    puVar5[2] = uVar6;
    puVar5[3] = uVar3;
    puVar5 = puVar5 + 4;
  } while (puVar4 != param_2 + 8);
  *puVar5 = *puVar4;
  *(undefined4 *)(iVar2 + 0x2c) = 0;
  *(undefined4 *)(iVar2 + 0x30) = 0;
  if ((*param_2 & 0xa0) == 0) {
    iVar9 = *piVar10;
  }
  else {
    IP_QueueAdd(piVar10[0x2e],iVar2);
    LinkedListIsEmpty(piVar10[0x2f],local_20);
    iVar9 = *piVar10;
  }
  if (iVar9 == 4) {
    IP_Execute(piVar10);
  }
  uVar3 = *param_2;
  if (*piVar10 == 1) {
LAB_c00ca798:
    if ((uVar3 & 0x150) == 0) goto LAB_c00ca6f0;
    iVar9 = piVar10[0x2c];
  }
  else {
    if ((uVar3 & 0xf) != 0) {
      if (((uVar3 & 1) != 0) && (uVar6 = 0, *piVar10 != 0)) {
        bVar1 = false;
        do {
          uVar6 = uVar6 + 1;
          if (bVar1) {
                    /* WARNING: Subroutine does not return */
            OsSysHaltEx(s_Inflate_stuck_c00cb054,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x3b3);
          }
          bVar1 = 10000000 < uVar6;
        } while (*piVar10 != 0);
      }
      if ((uVar3 & 3) != 0) {
        IP_LoadVlcTable(piVar10,uVar3 & 3,param_2 + 1);
        *(uint *)(iVar2 + 0x2c) = *(uint *)(iVar2 + 0x2c) | *param_2 & 3;
        uVar3 = *param_2;
      }
      if ((uVar3 & 0xc) != 0) {
        IP_LoadVlcTable(piVar10,uVar3 & 0xc,param_2 + 3);
        *(uint *)(iVar2 + 0x2c) = *(uint *)(iVar2 + 0x2c) | *param_2 & 0xc;
        uVar3 = *param_2;
        goto LAB_c00ca798;
      }
    }
    if ((uVar3 & 0x150) == 0) goto LAB_c00ca6f0;
    iVar9 = piVar10[0x2c];
  }
  IP_QueueAdd(iVar9,iVar2);
LAB_c00ca6f0:
  if (*piVar10 != 2) {
    IP_Execute(piVar10);
  }
  return iVar2;
}


//==== IsrInfEOO @ c00ca838

void IsrInfEOO(undefined4 param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = IP_QueueTop(param_2[0x2f]);
  if (iVar1 == 0) {
                    /* WARNING: Subroutine does not return */
    OsSysHaltEx(s_Inflate_EOO_has_no_job__c00cb124,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x31b
               );
  }
  iVar2 = param_2[0x2f];
  *(uint *)(iVar1 + 0x2c) = *(uint *)(iVar1 + 0x2c) | *(uint *)(iVar1 + 8) & 0xa0;
  iVar1 = IP_QueueRemove(iVar2);
  if (*(int *)(iVar1 + 0x30) == 0) {
    PoolFreeEx(param_2[0x31],iVar1,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x323);
    iVar1 = *param_2;
  }
  else {
    iVar1 = *param_2;
  }
  if (iVar1 != 2) {
    return;
  }
  *param_2 = 4;
  IP_Execute(param_2);
  return;
}


//==== IsrInfEOI @ c00ca8f4

void IsrInfEOI(undefined4 param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = IP_QueueTop(param_2[0x2d]);
  if (iVar1 == 0) {
                    /* WARNING: Subroutine does not return */
    OsSysHaltEx(s_Inflate_EOI_has_no_job__c00cb13c,s_Os_Inflate_HwAcceleration_Inflat_c00cb064,0x2e5
               );
  }
  iVar2 = param_2[0x2d];
  iVar3 = param_2[0x30];
  *(uint *)(iVar1 + 0x2c) = *(uint *)(iVar1 + 0x2c) | *(uint *)(iVar1 + 8) & 0x150;
  IP_QueueTransfer(iVar2,iVar3);
  if ((*param_2 != 2) &&
     ((((*param_2 != 4 && (*param_2 != 0)) || (iVar1 = IP_QueueTop(param_2[0x2c]), iVar1 == 0)) ||
      ((*(uint *)(iVar1 + 8) & 0x100) == 0)))) {
    return;
  }
  *param_2 = 3;
  IP_Execute(param_2);
  return;
}


//==== GetCurrentBlockUncompressedSise @ c00caf10

undefined4 GetCurrentBlockUncompressedSise(int param_1)

{
  return *(undefined4 *)(*(int *)(param_1 + 0x14) + 4);
}


