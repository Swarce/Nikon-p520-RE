//==== DcuVbiFileWrite @ c0027160

void DcuVbiFileWrite(int param_1,char *param_2,char *param_3)

{
  int iVar1;
  int iVar2;
  size_t sVar3;
  undefined4 uVar4;
  
  if ((((param_1 != 0 && param_2 != (char *)0x0) && (param_3 != (char *)0x0)) && (*param_2 != '\0'))
     && ((*param_3 != '\0' &&
         (iVar1 = PoolAllocEx(0,0x1f8,0x202,s_Os_Dcu_DcuConfig_c_c00333dc,0xe5), iVar1 != 0)))) {
    iVar2 = func_0xc01dac84(param_1,iVar1,param_2,1);
    uVar4 = 0xf5;
    if (iVar2 == 0) {
      sVar3 = strlen(param_3);
      func_0xc01eddd4(iVar1,param_3,sVar3);
      func_0xc01e38cc(iVar1);
      func_0xc01e3f10(param_1);
      uVar4 = 0xfd;
    }
    PoolFreeEx(0,iVar1,s_Os_Dcu_DcuConfig_c_c00333dc,uVar4);
    return;
  }
  return;
}


//==== DcuVbiTimingCmdExecution @ c002728c

void DcuVbiTimingCmdExecution(undefined4 *param_1)

{
  char acStack_108 [256];
  
  sprintf(acStack_108,&DAT_c003340c,s__DCIM_100COACH__c00333f0,s_TIMING_LOG_c0033400);
  TimingStop();
  TimingFree();
  TimingAllocate(param_1[3]);
  Os_TaskSleep(param_1[1]);
  TimingStart(*param_1);
  Os_TaskSleep(param_1[2]);
  TimingStop();
  TimingDumpFile(param_1[4],acStack_108,1);
  TimingFree();
  PoolFreeEx(0,param_1,s_Os_Dcu_DcuConfig_c_c00333dc,0x131);
  return;
}


//==== DcuVbiFunctionCmdExecution @ c0027334

void DcuVbiFunctionCmdExecution(undefined4 *param_1)

{
  char cVar1;
  size_t sVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  uint uVar7;
  char acStack_220 [512];
  
  uVar4 = 0;
  OsThreadSleepMS(*param_1);
  switch(param_1[2]) {
  case 0:
    uVar4 = (*(code *)param_1[1])();
    break;
  case 1:
    uVar4 = (*(code *)param_1[1])(param_1[3]);
    break;
  case 2:
    uVar4 = (*(code *)param_1[1])(param_1[3],param_1[4]);
    break;
  case 3:
    uVar4 = (*(code *)param_1[1])(param_1[3],param_1[4],param_1[5]);
    break;
  case 4:
    uVar4 = (*(code *)param_1[1])(param_1[3],param_1[4],param_1[5],param_1[6]);
    break;
  case 5:
    uVar4 = (*(code *)param_1[1])(param_1[3],param_1[4],param_1[5],param_1[6],param_1[7]);
    break;
  case 6:
    uVar4 = (*(code *)param_1[1])(param_1[3],param_1[4],param_1[5],param_1[6],param_1[7],param_1[8])
    ;
    break;
  case 7:
    uVar4 = (*(code *)param_1[1])
                      (param_1[3],param_1[4],param_1[5],param_1[6],param_1[7],param_1[8],param_1[9])
    ;
    break;
  case 8:
    uVar4 = (*(code *)param_1[1])
                      (param_1[3],param_1[4],param_1[5],param_1[6],param_1[7],param_1[8],param_1[9],
                       param_1[10]);
    break;
  case 9:
    uVar4 = (*(code *)param_1[1])
                      (param_1[3],param_1[4],param_1[5],param_1[6],param_1[7],param_1[8],param_1[9],
                       param_1[10],param_1[0xb]);
    break;
  case 10:
    uVar4 = (*(code *)param_1[1])
                      (param_1[3],param_1[4],param_1[5],param_1[6],param_1[7],param_1[8],param_1[9],
                       param_1[10],param_1[0xb],param_1[0xc]);
    break;
  default:
    cVar1 = *(char *)((int)param_1 + 0x3e);
    goto LAB_c00273a4;
  }
  cVar1 = *(char *)((int)param_1 + 0x3e);
LAB_c00273a4:
  if ((cVar1 == '\x01') && (param_1[0x50] != 0)) {
    sprintf(acStack_220,s_FC__0x_lX__ld_params__is_called_a_c0033414,param_1[1],param_1[2],*param_1,
            uVar4);
    sVar2 = strlen(acStack_220);
    iVar3 = func_0xc01eddd4(param_1[0x50],acStack_220,sVar2);
    if ((iVar3 != 0) && (param_1[0x51] != 0)) {
      DcuVbiFileWrite(param_1[0x51],(int)param_1 + 0x3f,acStack_220);
    }
  }
  uVar7 = 0;
  puVar5 = param_1;
  puVar6 = param_1;
  do {
    while (*(char *)(puVar5 + 0xd) == '\x01') {
      PoolFreeEx(0,puVar6[3],s_Os_Dcu_DcuConfig_c_c00333dc,0x1c8);
      uVar7 = uVar7 + 1;
      puVar5 = (undefined4 *)((int)puVar5 + 1);
      puVar6 = puVar6 + 1;
      if ((uint)param_1[2] < uVar7) goto LAB_c0027418;
    }
    uVar7 = uVar7 + 1;
    puVar5 = (undefined4 *)((int)puVar5 + 1);
    puVar6 = puVar6 + 1;
  } while (uVar7 <= (uint)param_1[2]);
LAB_c0027418:
  PoolFreeEx(0,param_1,s_Os_Dcu_DcuConfig_c_c00333dc,0x1d1);
  return;
}


//==== DcuVbiReadCmdExecution @ c0027690

void DcuVbiReadCmdExecution(undefined4 *param_1)

{
  size_t sVar1;
  int iVar2;
  char acStack_208 [512];
  
  OsThreadSleepMS(*param_1);
  sprintf(acStack_208,s_RL_after_delay_of__d_msec____0x__c0033468,*param_1,(undefined4 *)param_1[1],
          *(undefined4 *)param_1[1]);
  sVar1 = strlen(acStack_208);
  iVar2 = func_0xc01eddd4(param_1[0x43],acStack_208,sVar1);
  if (iVar2 != 0) {
    if (param_1[0x44] != 0) {
      DcuVbiFileWrite(param_1[0x44],(int)param_1 + 9,acStack_208);
      PoolFreeEx(0,param_1,s_Os_Dcu_DcuConfig_c_c00333dc,0x203);
      return;
    }
  }
  PoolFreeEx(0,param_1,s_Os_Dcu_DcuConfig_c_c00333dc,0x203);
  return;
}


//==== DcuVbiWriteCmdExecution @ c0027754

void DcuVbiWriteCmdExecution(undefined4 *param_1)

{
  size_t sVar1;
  int iVar2;
  char acStack_208 [512];
  
  OsThreadSleepMS(*param_1);
  *(undefined4 *)param_1[1] = param_1[0x12];
  if (*(char *)(param_1 + 0x13) == '\x01') {
    sprintf(acStack_208,s_WL_after_delay_of__d_msec____0x__c0033498,*param_1,param_1[1],param_1 + 2)
    ;
    sVar1 = strlen(acStack_208);
    iVar2 = func_0xc01eddd4(param_1[0x54],acStack_208,sVar1);
    if (iVar2 != 0) {
      if (param_1[0x55] != 0) {
        DcuVbiFileWrite(param_1[0x55],(int)param_1 + 0x4d,acStack_208);
      }
    }
  }
  PoolFreeEx(0,param_1,s_Os_Dcu_DcuConfig_c_c00333dc,0x23c);
  return;
}


//==== DcuValidateBufferIntegrity @ c0027810

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DcuValidateBufferIntegrity(int param_1)

{
  byte *pbVar1;
  char cVar2;
  undefined1 uVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  size_t sVar7;
  char *pcVar8;
  int *piVar9;
  undefined4 *puVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  undefined1 *puVar13;
  uint uVar14;
  code *pcVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  char *pcVar18;
  bool bVar19;
  uint uVar20;
  undefined1 auStack_178 [8];
  int local_170;
  undefined2 local_128 [4];
  char local_120;
  undefined1 uStack_11f;
  char acStack_11e [62];
  char local_e0;
  undefined1 auStack_df [63];
  char local_a0;
  byte local_9f [63];
  int local_60 [5];
  char *local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  undefined4 *local_38;
  undefined4 *local_34;
  undefined2 local_30;
  uint local_2c;
  
  local_128[0] = DAT_c00334c8;
  local_60[0] = 0;
  local_60[1] = 0;
  local_48 = 1;
  if (param_1 == 0) {
    return;
  }
  local_44 = -0x3ffd0000;
  sprintf((char *)0xc12e64b8,&DAT_c003340c,&DAT_c003371c,s___SP43____txt_c00334cc);
  iVar4 = func_0xc01e4664(param_1,0xc12e64b8,auStack_178);
  if (iVar4 != 0) {
    return;
  }
  iVar4 = PoolAllocEx(0,0x1f8,0x202,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x285);
  if (iVar4 == 0) {
    return;
  }
  local_4c = (char *)PoolAllocEx(0,local_170,0x202,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x28f);
  if (local_4c == (char *)0x0) {
    PoolFreeEx(0,iVar4,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x295);
    return;
  }
  iVar5 = func_0xc01dac84(param_1,iVar4,0xc12e64b8,0);
  if (iVar5 != 0) {
    PoolFreeEx(0,iVar4,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x2a0);
    PoolFreeEx(0,local_4c,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x2a1);
    return;
  }
  iVar5 = func_0xc01d82f4(iVar4,local_4c,local_170 + 1,local_60);
  if (iVar5 != 0) {
    func_0xc01e38cc(iVar4);
    PoolFreeEx(0,iVar4,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x2ad);
    PoolFreeEx(0,local_4c,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x2ae);
    return;
  }
  func_0xc01e38cc(iVar4);
  PoolFreeEx(0,iVar4,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x2b4);
  local_4c[local_60[0]] = '\0';
  if ((local_60[0] == 0) || (*local_4c == '\0')) {
    PoolFreeEx(0,local_4c,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x2be);
    return;
  }
  sprintf((char *)0xc12e64b8,&DAT_c003340c,s__DCIM_100COACH__c00333f0,s_CMD_OUTPUT_LOG_c00334dc);
  iVar4 = PoolAllocEx(0,0x1f8,0x202,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x2d2);
  if (iVar4 != 0) {
    func_0xc01d2154(param_1,0xc12e64b8);
    iVar5 = func_0xc01d576c(param_1,0xc12e64b8);
    if (iVar5 == 0) {
      iVar5 = func_0xc01dac84(param_1,iVar4,0xc12e64b8,1);
      if (iVar5 == 0) goto LAB_c0027b34;
      uVar16 = 0x2f3;
    }
    else {
      uVar16 = 0x2e4;
    }
    PoolFreeEx(0,iVar4,PTR_s_Os_Dcu_DcuConfig_c_c0033760,uVar16);
    iVar4 = 0;
  }
  local_48 = 0;
LAB_c0027b34:
  pcVar6 = (char *)func_0xc04f06b0(local_4c,local_128,local_60 + 1);
  if (pcVar6 != (char *)0x0) {
    local_30 = DAT_c00334ec;
    local_40 = -0x3ed20000;
    do {
      sVar7 = strlen(pcVar6);
      pcVar8 = pcVar6 + sVar7;
      if ((pcVar8[-2] == '\r') || (pcVar8[-2] == '\n')) {
        pcVar8[-2] = '\0';
      }
      else {
        cVar2 = pcVar8[-1];
        if (cVar2 == '\r') {
          pcVar8[-1] = '\0';
        }
        else if (cVar2 == '\n') {
          pcVar8[-1] = '\0';
        }
        else if (cVar2 != '\0') {
          *pcVar8 = '\0';
        }
      }
      strcpy(DAT_c0033764,pcVar6);
      pcVar6 = DAT_c0033768;
      if (*(char *)(local_40 + 0x61b8) != '\0') {
        sscanf(DAT_c0033764,PTR_DAT_c003376c,DAT_c0033768);
        iVar5 = strcmp(pcVar6,PTR_s_TIMING_c0033770);
        if (iVar5 == 0) {
          iVar5 = PoolAllocEx(0,0x14,0x202,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x322);
          if (iVar5 != 0) {
            uVar14 = sscanf(DAT_c0033764,s__s__x__u__u__u_c0033530,pcVar6,iVar5,iVar5 + 4,iVar5 + 8,
                            iVar5 + 0xc);
            if (uVar14 < 5) {
              PoolFreeEx(0,iVar5,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x336);
            }
            else {
              *(int *)(iVar5 + 0x10) = param_1;
              OsThreadCreateDynamicSilent
                        (s_TimingCmdExec_c00335b4,DcuVbiTimingCmdExecution,iVar5,10000,0x3f4,0x3f4,3
                         ,1);
              if (local_48 != 0) {
                sprintf(DAT_c0033774,s__s__Output_is_stored_in__s_c00335c4,PTR_s_TIMING_c0033770,
                        s_TIMING_LOG_c0033400);
                sVar7 = strlen(DAT_c0033774);
                goto LAB_c0027d1c;
              }
            }
          }
        }
        else {
          iVar5 = strcmp(pcVar6,&DAT_c00334fc);
          if (iVar5 == 0) {
            local_60[2] = 1000;
            if (local_48 != 0) {
              sscanf(DAT_c0033764,s__s__lu_c0033578,pcVar6,local_60 + 2);
              if (local_60[2] - 1U < 1000) {
                iVar5 = local_60[2] * 1000;
              }
              else {
                local_60[2] = 1000;
                iVar5 = 1000000;
              }
              local_3c = -0x3ed20000;
              CguRingOscSample(iVar5);
              iVar5 = local_3c;
              pcVar6 = (char *)(local_3c + 0x65b8);
              sprintf(pcVar6,s_ROSC__ld___c0033580,local_60[2]);
              sVar7 = strlen(pcVar6);
              func_0xc01eddd4(iVar4,pcVar6,sVar7);
              uVar14 = 0;
              do {
                sprintf(pcVar6,PTR_s_RO_package___u____c_c0033778,uVar14,0);
                sVar7 = strlen(pcVar6);
                func_0xc01eddd4(iVar4,pcVar6,sVar7);
                uVar20 = 0;
                do {
                  uVar16 = CguRingOscRead(uVar14,uVar20);
                  uVar17 = 0x20;
                  if (uVar20 != 6) {
                    uVar17 = 0x2c;
                  }
                  sprintf(pcVar6,s__8ld__c__c_c0033590,uVar16,uVar17,0);
                  sVar7 = strlen(pcVar6);
                  uVar20 = uVar20 + 1;
                  func_0xc01eddd4(iVar4,pcVar6,sVar7);
                  uVar3 = DAT_c00334ee;
                } while (uVar20 < 7);
                *(undefined2 *)(local_3c + 0x65b8) = local_30;
                *(undefined1 *)(iVar5 + 0x65ba) = uVar3;
                sVar7 = strlen(pcVar6);
                uVar14 = uVar14 + 1;
                func_0xc01eddd4(iVar4,pcVar6,sVar7);
              } while (uVar14 < 0x10);
              *(undefined2 *)(local_3c + 0x65b8) = local_30;
              *(undefined1 *)(iVar5 + 0x65ba) = uVar3;
              sVar7 = strlen(pcVar6);
              func_0xc01eddd4(iVar4,pcVar6,sVar7);
            }
          }
          else {
            iVar5 = strcmp(pcVar6,&DAT_c0033504);
            if (iVar5 == 0) {
              if ((local_48 != 0) &&
                 (piVar9 = (int *)PoolAllocEx(0,0x114,0x202,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x3a9)
                 , piVar9 != (int *)0x0)) {
                uVar14 = sscanf(DAT_c0033764,s__s__u__lx_c003350c,pcVar6,piVar9,piVar9 + 1);
                if (uVar14 < 3) {
                  PoolFreeEx(0,piVar9,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x3ba);
                }
                else {
                  if (*piVar9 == 0) {
                    sprintf(DAT_c0033774,s_RL__0x_lX___0x_lX_c0033518,(undefined4 *)piVar9[1],
                            *(undefined4 *)piVar9[1]);
                    sVar7 = strlen(DAT_c0033774);
                    goto LAB_c0027d1c;
                  }
                  *(undefined1 *)(piVar9 + 2) = 1;
                  piVar9[0x43] = iVar4;
                  piVar9[0x44] = param_1;
                  strcpy((char *)((int)piVar9 + 9),(char *)0xc12e64b8);
                  pcVar6 = s_ReadCmdExec_c0033664;
                  pcVar15 = DcuVbiReadCmdExecution;
LAB_c0028454:
                  OsThreadCreateDynamicSilent(pcVar6,pcVar15,piVar9,8000,0x3f4,0x3f4,3,1);
                }
              }
            }
            else {
              iVar5 = strcmp(pcVar6,&DAT_c0033508);
              if (iVar5 == 0) {
                local_60[3] = 0;
                piVar9 = (int *)PoolAllocEx(0,0x158,0x202,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x3ea);
                if (piVar9 != (int *)0x0) {
                  uVar14 = sscanf(DAT_c0033764,s__s__u__lx__s_c00335e4,pcVar6,piVar9,piVar9 + 1,
                                  &local_120);
                  if (uVar14 < 4) {
                    PoolFreeEx(0,piVar9,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x3fb);
                  }
                  else {
                    iVar5 = strncmp(&local_120,&DAT_c00335f4,2);
                    if (iVar5 == 0) {
                      sscanf(acStack_11e,&DAT_c00335f8,local_60 + 3);
                      iVar5 = *piVar9;
                    }
                    else {
                      iVar5 = strncmp(&local_120,&DAT_c00335fc,2);
                      if (iVar5 == 0) {
                        sVar7 = strlen(&local_120);
                        uVar14 = 0;
                        if (sVar7 != 2) {
                          puVar13 = &stack0xfffffe86 + sVar7;
                          do {
                            pbVar1 = puVar13 + 0x59;
                            puVar13 = puVar13 + -1;
                            uVar20 = uVar14 & 0x1f;
                            uVar14 = uVar14 + 1;
                            local_60[3] = (*pbVar1 - 0x30 << uVar20) + local_60[3];
                          } while (uVar14 < sVar7 - 2);
                        }
                      }
                      else {
                        local_60[3] = func_0xc04f0bcc(&local_120,0,10);
                      }
                      iVar5 = *piVar9;
                    }
                    if (iVar5 != 0) {
                      *(char *)(piVar9 + 0x13) = (char)local_48;
                      piVar9[0x12] = local_60[3];
                      piVar9[0x54] = iVar4;
                      piVar9[0x55] = param_1;
                      strcpy((char *)((int)piVar9 + 0x4d),(char *)0xc12e64b8);
                      strcpy((char *)(piVar9 + 2),&local_120);
                      pcVar6 = s_WriteCmdExec_c0033678;
                      pcVar15 = DcuVbiWriteCmdExecution;
                      goto LAB_c0028454;
                    }
                    *(int *)piVar9[1] = local_60[3];
                    if (local_48 != 0) {
                      sprintf(DAT_c0033774,s_WL__0x_lX_<___s_c0033600,piVar9[1],&local_120);
                      sVar7 = strlen(DAT_c0033774);
                      goto LAB_c0027d1c;
                    }
                  }
                }
              }
              else {
                iVar5 = strcmp(pcVar6,&DAT_c0033540);
                cVar2 = DAT_c0033720;
                if (iVar5 == 0) {
                  local_120 = DAT_c0033720;
                  memset(&uStack_11f,0,0x3f);
                  local_e0 = cVar2;
                  memset(auStack_df,0,0x3f);
                  puVar10 = (undefined4 *)
                            PoolAllocEx(0,0x148,0x202,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x449);
                  uVar14 = 0;
                  if (puVar10 != (undefined4 *)0x0) {
                    *puVar10 = 0;
                    puVar10[1] = 0;
                    puVar10[2] = 0;
                    puVar11 = puVar10;
                    puVar12 = puVar10;
                    do {
                      uVar14 = uVar14 + 1;
                      *(undefined1 *)(puVar11 + 0xd) = 0;
                      puVar12[3] = 0;
                      puVar11 = (undefined4 *)((int)puVar11 + 1);
                      puVar12 = puVar12 + 1;
                    } while (uVar14 < 10);
                    uVar14 = sscanf(DAT_c0033764,s__s__s__s_c003363c,pcVar6,&local_120,&local_e0);
                    if (uVar14 < 3) {
                      PoolFreeEx(0,puVar10,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x466);
                    }
                    else {
                      iVar5 = sscanf(&local_120,&DAT_c0033670,puVar10);
                      if (iVar5 == 0) {
                        *puVar10 = 0;
                      }
                      iVar5 = sscanf(&local_e0,&DAT_c0033674,puVar10 + 1);
                      if ((iVar5 == 0) || (puVar10[1] == 0)) {
                        PoolFreeEx(0,puVar10,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x47d);
                      }
                      else {
                        sVar7 = strlen(pcVar6);
                        for (pcVar6 = DAT_c0033764 + sVar7; *pcVar6 == ' ' || *pcVar6 == '\t';
                            pcVar6 = pcVar6 + 1) {
                        }
                        sVar7 = strlen(&local_120);
                        for (pcVar6 = pcVar6 + sVar7; *pcVar6 == ' ' || *pcVar6 == '\t';
                            pcVar6 = pcVar6 + 1) {
                        }
                        sVar7 = strlen(&local_e0);
                        pcVar8 = DAT_c0033774;
                        for (pcVar6 = pcVar6 + sVar7; *pcVar6 == ' ' || *pcVar6 == '\t';
                            pcVar6 = pcVar6 + 1) {
                        }
                        bVar19 = true;
                        local_2c = 0;
                        local_38 = puVar10;
                        local_34 = puVar10;
                        do {
                          local_a0 = DAT_c0033720;
                          local_60[4] = 0;
                          memset(local_9f,0,0x3f);
                          cVar2 = *pcVar6;
                          if ((((cVar2 == '\n' || cVar2 == '\r') || (cVar2 == '\0')) ||
                              (iVar5 = sscanf(pcVar6,PTR_DAT_c003376c,&local_a0), cVar2 = local_a0,
                              iVar5 == 0)) || (local_a0 == '\0')) break;
                          iVar5 = strncmp(&local_a0,&DAT_c00335f4,2);
                          if (iVar5 == 0) {
                            sscanf(&local_a0,&DAT_c00336b0,local_60 + 4);
                          }
                          else {
                            iVar5 = strncmp(&local_a0,&DAT_c00335fc,2);
                            if (iVar5 == 0) {
                              sVar7 = strlen(&local_a0);
                              uVar14 = 0;
                              if (sVar7 != 2) {
                                puVar13 = &stack0xfffffe86 + sVar7;
                                do {
                                  pbVar1 = puVar13 + 0xd9;
                                  puVar13 = puVar13 + -1;
                                  uVar20 = uVar14 & 0x1f;
                                  uVar14 = uVar14 + 1;
                                  local_60[4] = (*pbVar1 - 0x30 << uVar20) + local_60[4];
                                } while (uVar14 < sVar7 - 2);
                              }
                            }
                            else if (cVar2 == '\'') {
                              local_60[4] = (int)local_9f[0];
                            }
                            else {
                              iVar5 = 0;
                              if (cVar2 == '\"') {
                                cVar2 = pcVar6[1];
                                pcVar6 = pcVar6 + 1;
                                pcVar18 = pcVar6;
                                if (((cVar2 != '\"' && cVar2 != '\'') && (cVar2 != '\n')) &&
                                   ((cVar2 != '\r' && (cVar2 != '\0')))) {
                                  while( true ) {
                                    pcVar18 = pcVar18 + 1;
                                    cVar2 = *pcVar18;
                                    iVar5 = iVar5 + 1;
                                    if ((cVar2 == '\"' || cVar2 == '\'') || (cVar2 == '\n')) break;
                                    if ((cVar2 == '\r') || (cVar2 == '\0')) break;
                                  }
                                }
                                local_60[4] = PoolAllocEx(0,iVar5 + 1,0x202,
                                                          PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x4df);
                                iVar5 = 0;
                                *(undefined1 *)(local_34 + 0xd) = 1;
                                for (; pcVar6 != pcVar18; pcVar6 = pcVar6 + 1) {
                                  *(char *)(iVar5 + local_60[4]) = *pcVar6;
                                  iVar5 = iVar5 + 1;
                                }
                                *(undefined1 *)(local_60[4] + iVar5) = 0;
                                local_a0 = *(char *)(local_44 + 0x371c);
                                pcVar6 = pcVar6 + (*pcVar6 == '\"' || *pcVar6 == '\'');
                              }
                              else {
                                local_60[4] = func_0xc04f0bcc(&local_a0,0,10);
                                if ((local_60[4] == 0) && (local_a0 != '0')) {
                                  bVar19 = false;
                                  if (local_48 != 0) {
                                    sprintf(pcVar8,s_FC__unknown_parameter__s___Skipp_c00336b4,
                                            &local_a0);
                                    sVar7 = strlen(pcVar8);
                                    func_0xc01eddd4(iVar4,pcVar8,sVar7);
                                  }
                                  break;
                                }
                              }
                            }
                          }
                          local_38[3] = local_60[4];
                          sVar7 = strlen(&local_a0);
                          for (pcVar6 = pcVar6 + sVar7; *pcVar6 == ' ' || *pcVar6 == '\t';
                              pcVar6 = pcVar6 + 1) {
                          }
                          local_2c = local_2c + 1;
                          local_34 = (undefined4 *)((int)local_34 + 1);
                          local_38 = local_38 + 1;
                        } while (local_2c < 10);
                        if (bVar19) {
                          puVar10[2] = local_2c;
                          puVar10[0x50] = iVar4;
                          *(char *)((int)puVar10 + 0x3e) = (char)local_48;
                          puVar10[0x51] = param_1;
                          strcpy((char *)((int)puVar10 + 0x3f),(char *)0xc12e64b8);
                          OsThreadCreateDynamicSilent
                                    (s_FunctionCmdExec_c00336e0,DcuVbiFunctionCmdExecution,puVar10,
                                     10000,0x3f4,0x3f4,3,1);
                        }
                        else {
                          uVar14 = puVar10[2];
                          uVar20 = 0;
                          puVar11 = puVar10;
                          puVar12 = puVar10;
                          if (uVar14 != 0) {
                            do {
                              uVar20 = uVar20 + 1;
                              if (*(char *)(puVar12 + 0xd) == '\x01') {
                                PoolFreeEx(0,puVar11[3],PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x540);
                                uVar14 = puVar10[2];
                              }
                              puVar11 = puVar11 + 1;
                              puVar12 = (undefined4 *)((int)puVar12 + 1);
                            } while (uVar20 < uVar14);
                          }
                          PoolFreeEx(0,puVar10,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x543);
                        }
                      }
                    }
                  }
                }
                else {
                  iVar5 = strcmp(pcVar6,s_RAWSTILL_c0033544);
                  if (iVar5 == 0) {
                    _DAT_c0356e38 = _DAT_c0356e38 | 1;
                    if (local_48 != 0) {
                      sprintf(DAT_c0033774,s__s__ulStillScrd____0x01_c003355c,s_RAWSTILL_c0033544);
                      sVar7 = strlen(DAT_c0033774);
LAB_c0027d1c:
                      func_0xc01eddd4(iVar4,DAT_c0033774,sVar7);
                    }
                  }
                  else {
                    iVar5 = strcmp(pcVar6,s_RAWPREVIEW_c0033550);
                    if (iVar5 == 0) {
                      _DAT_c0356e30 = _DAT_c0356e30 | 1;
                      if (local_48 != 0) {
                        sprintf(DAT_c0033774,s__s__ulStillSprd____0x01_c0033620,
                                s_RAWPREVIEW_c0033550);
                        sVar7 = strlen(DAT_c0033774);
                        goto LAB_c0027d1c;
                      }
                    }
                    else {
                      iVar5 = strcmp(pcVar6,s_YUVSTILL_c0033614);
                      if (iVar5 == 0) {
                        _DAT_c0356e38 = _DAT_c0356e38 | 2;
                        if (local_48 != 0) {
                          sprintf(DAT_c0033774,s__s__ulStillScrd____0x02_c0033648,
                                  s_YUVSTILL_c0033614);
                          sVar7 = strlen(DAT_c0033774);
                          goto LAB_c0027d1c;
                        }
                      }
                      else {
                        iVar5 = strcmp(DAT_c0033768,s_YUVPREVIEW_c0033688);
                        if ((iVar5 == 0) && (_DAT_c0356e30 = _DAT_c0356e30 | 2, local_48 != 0)) {
                          sprintf(DAT_c0033774,s__s__ulStillSprd____0x02_c0033694,
                                  s_YUVPREVIEW_c0033688);
                          sVar7 = strlen(DAT_c0033774);
                          func_0xc01eddd4(iVar4,DAT_c0033774,sVar7);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      pcVar6 = (char *)func_0xc04f06b0(0,local_128,local_60 + 1);
    } while (pcVar6 != (char *)0x0);
  }
  PoolFreeEx(0,local_4c,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x582);
  if (local_48 == 0) {
    return;
  }
  func_0xc01e38cc(iVar4);
  func_0xc01e3f10(param_1);
  PoolFreeEx(0,iVar4,PTR_s_Os_Dcu_DcuConfig_c_c0033760,0x586);
  return;
}


//==== DcuDebugDummyClientOpen @ c0028aac

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DcuDebugDummyClientOpen(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  
  iVar2 = param_1 * 0x74;
  local_2c = *(undefined4 *)(iVar2 + -0x3ed197dc);
  local_30 = 1;
  local_1c = 1;
  local_24 = 0x100;
  local_28 = 0;
  local_18 = 0;
  local_20 = 0;
  uVar1 = DcuClientOpenByDram(param_1,0x61,&local_30);
  _DAT_b80025c0 = 0;
  _DAT_b80025c4 = 0;
  *(undefined4 *)(iVar2 + -0x3ed197e0) = uVar1;
  _DAT_b80025c8 = 0;
  DcuClientSubmit(uVar1);
  DcuSetClientControls(*(undefined4 *)(iVar2 + -0x3ed197e0),2,2,2);
  uVar1 = Os_InterruptsDisable();
  DcuClientStart(*(undefined4 *)(iVar2 + -0x3ed197e0));
  DcuSetClientControls(*(undefined4 *)(iVar2 + -0x3ed197e0),1,2,2);
  DcuClientStart(*(undefined4 *)(iVar2 + -0x3ed197e0));
  Os_InterruptsRestore(uVar1);
  return;
}


//==== DcuDebugInit @ c0028b90

void DcuDebugInit(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x74;
  *(undefined4 *)(iVar2 + -0x3ed19804) = 0;
  *(undefined4 *)(iVar2 + -0x3ed19800) = 0;
  *(undefined4 *)(iVar2 + -0x3ed197f4) = 0;
  *(undefined4 *)(iVar2 + -0x3ed197f0) = 0;
  if (param_1 == 0) {
    *(undefined4 *)(iVar2 + -0x3ed197d8) = 0;
  }
  uVar1 = PoolAllocEx(*(undefined4 *)(iVar2 + -0x3ed197d8),0x400,1,s_Os_Dcu_DcuDebug_c_c003377c,
                      0x149);
  *(undefined4 *)(iVar2 + -0x3ed197dc) = uVar1;
  DcuDebugDummyClientOpen(param_1);
  return;
}


//==== DcuDebugConfig @ c0028c0c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool DcuDebugConfig(int param_1,int param_2)

{
  int iVar1;
  undefined *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  uint uVar5;
  uint *puVar6;
  uint uVar7;
  undefined4 *puVar8;
  bool bVar9;
  
  param_1 = param_1 * 0x74;
  bVar9 = *(int *)(param_1 + -0x3ed19804) == 1;
  if (bVar9) {
    Printf_B(s_Client_writing__c003391c);
  }
  bVar9 = !bVar9;
  uVar7 = 0;
  if (bVar9) {
    do {
      puVar6 = (uint *)(uVar7 * 4 + param_2);
      uVar5 = *puVar6;
      if ((uVar5 != 0) &&
         (((*(uint *)(s_dcuDebugOutputConf + uVar7 * 0xc) & uVar5) == 0 ||
          ((uVar5 - 1 & uVar5) != 0)))) {
        bVar9 = false;
        Printf_B(&DAT_c0033790,s_Os_Dcu_DcuDebug_c_c003377c,0x17e,uVar7,uVar5);
      }
      if (bVar9 == true) {
        puVar8 = (undefined4 *)(uVar7 * 4 + param_1 + -0x3ed19848);
        uVar3 = 0;
        switch(*puVar6) {
        case 0:
        case 1:
        case 2:
        case 4:
          uVar3 = 0;
          break;
        case 8:
          uVar3 = 1;
          break;
        case 0x10:
          uVar3 = 2;
          break;
        case 0x20:
          uVar3 = 3;
          break;
        case 0x40:
          uVar3 = 4;
        }
        *puVar8 = uVar3;
        if (uVar5 == 0x40) {
          uVar5 = *(uint *)(param_2 + 0x40);
          if (3 < uVar5) {
            puVar2 = &DAT_c0033868;
            uVar4 = 0x196;
            goto LAB_c0028d8c;
          }
          *(uint *)(param_1 + -0x3ed19808) = uVar5;
        }
        else {
          uVar5 = puVar6[8];
          if (uVar5 < 0xeb) {
            uVar3 = ConvertClientIdToHwId(uVar5);
            puVar8[8] = uVar3;
          }
          else {
            puVar2 = &DAT_c00337ec;
            uVar4 = 0x1a9;
LAB_c0028d8c:
            bVar9 = false;
            Printf_B(puVar2,s_Os_Dcu_DcuDebug_c_c003377c,uVar4,uVar7,uVar3,uVar5);
          }
        }
      }
      uVar7 = uVar7 + 1;
    } while ((bVar9 & uVar7 < 8) != 0);
  }
  if (bVar9 == true) {
    iVar1 = *(int *)(param_2 + 0x48);
    if (iVar1 == 2 || iVar1 == 4) {
      *(int *)(param_1 + -0x3ed197fc) = iVar1;
    }
    else {
      Printf_B(&DAT_c00338e0,s_Os_Dcu_DcuDebug_c_c003377c,0x1bf);
      bVar9 = false;
      *(undefined4 *)(param_1 + -0x3ed197fc) = *(undefined4 *)(param_2 + 0x48);
    }
  }
  if (bVar9 != true) {
    return bVar9;
  }
  if (*(int *)(param_1 + -0x3ed197f4) != 0) {
    PoolFreeEx(*(undefined4 *)(param_1 + -0x3ed197d8),*(int *)(param_1 + -0x3ed197f4),
               s_Os_Dcu_DcuDebug_c_c003377c,0x1d2);
    *(undefined4 *)(param_1 + -0x3ed197f4) = 0;
  }
  uVar3 = *(undefined4 *)(param_2 + 0x44);
  *(undefined4 *)(param_1 + -0x3ed19804) = 0;
  *(undefined4 *)(param_1 + -0x3ed19800) = uVar3;
  uVar7 = *(uint *)(param_2 + 0x4c);
  uVar5 = *(uint *)(param_2 + 0x48);
  if (uVar7 == 0) {
    *(undefined4 *)(param_1 + -0x3ed197e8) = 1;
  }
  else {
    if (uVar5 << 2 == 0) {
      trap(7);
    }
    if (uVar7 / (uVar5 << 2) < *(int *)(param_2 + 0x44) + 1U) {
      if (uVar5 << 3 == 0) {
        trap(7);
      }
      uVar7 = (uVar7 / (uVar5 << 3)) * uVar5 & 0x1fffffff;
      if (uVar5 == 0) {
        trap(7);
      }
      *(uint *)(param_1 + -0x3ed197f8) = uVar7;
      uVar7 = uVar7 / uVar5 - 1;
      *(uint *)(param_1 + -0x3ed197e4) = uVar7;
      if (uVar7 == 0) {
        trap(7);
      }
      *(uint *)(param_1 + -0x3ed197e8) = ((*(int *)(param_2 + 0x44) + uVar7) - 1) / uVar7;
      goto LAB_c0028de8;
    }
    *(undefined4 *)(param_1 + -0x3ed197e8) = 1;
  }
  *(uint *)(param_1 + -0x3ed197f8) = *(int *)(param_2 + 0x44) * uVar5 + uVar5;
  *(undefined4 *)(param_1 + -0x3ed197e4) = *(undefined4 *)(param_2 + 0x44);
LAB_c0028de8:
  _DAT_b8030500 =
       *(uint *)(param_1 + -0x3ed19844) & 0xf | (*(uint *)(param_1 + -0x3ed19840) & 0xf) << 4 |
       (*(uint *)(param_1 + -0x3ed1983c) & 0xf) << 8 |
       (*(uint *)(param_1 + -0x3ed19838) & 0xf) << 0xc |
       (*(uint *)(param_1 + -0x3ed19834) & 0xf) << 0x10 |
       (*(uint *)(param_1 + -0x3ed19830) & 0xf) << 0x14 |
       (*(uint *)(param_1 + -0x3ed1982c) & 0xf) << 0x18;
  _DAT_b8030504 =
       (uint)*(byte *)(param_1 + -0x3ed19824) | (*(uint *)(param_1 + -0x3ed19820) & 0xff) << 8 |
       (*(uint *)(param_1 + -0x3ed1981c) & 0xff) << 0x10 | *(int *)(param_1 + -0x3ed19818) << 0x18;
  *(undefined4 *)(param_1 + -0x3ed197ec) = 0;
  _DAT_b8030508 =
       (uint)*(byte *)(param_1 + -0x3ed19814) | (*(uint *)(param_1 + -0x3ed19810) & 0xff) << 8 |
       (*(uint *)(param_1 + -0x3ed1980c) & 0xff) << 0x10;
  _DAT_b803050c = *(uint *)(param_1 + -0x3ed19808) & 3;
  _DAT_b8030510 = uVar5 & 1;
  return true;
}


//==== DcuDebugCollect @ c0029064

uint DcuDebugCollect(int param_1)

{
  bool bVar1;
  uint uVar2;
  int iVar3;
  uint local_38;
  int local_34;
  undefined4 local_30;
  int local_2c;
  undefined4 local_28;
  uint local_24;
  undefined4 local_20;
  
  param_1 = param_1 * 0x74;
  bVar1 = *(int *)(param_1 + -0x3ed19804) == 1;
  iVar3 = 0;
  if (bVar1) {
    Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033930,
              s_Os_Dcu_DcuDebug_c_c003377c,0x23c);
  }
  uVar2 = (uint)!bVar1;
  if (uVar2 == 1) {
    iVar3 = *(int *)(param_1 + -0x3ed197f8) << 1;
    if (*(int *)(param_1 + -0x3ed197e8) == 1) {
      iVar3 = *(int *)(param_1 + -0x3ed197f8);
    }
    iVar3 = PoolCallocEx(*(undefined4 *)(param_1 + -0x3ed197d8),iVar3,4,0,
                         s_Os_Dcu_DcuDebug_c_c003377c,0x249);
    if (iVar3 == 0) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033930,
                s_Os_Dcu_DcuDebug_c_c003377c,0x24d);
      uVar2 = 0;
    }
  }
  if (uVar2 == 1) {
    *(int *)(param_1 + -0x3ed197f4) = iVar3;
    if (*(int *)(param_1 + -0x3ed197e8) == 1) {
      *(undefined4 *)(param_1 + -0x3ed197f0) = 0;
    }
    else {
      *(int *)(param_1 + -0x3ed197f0) = *(int *)(param_1 + -0x3ed197f8) * 4 + iVar3;
    }
    uVar2 = DcuClientStop(*(undefined4 *)(param_1 + -0x3ed197e0),1);
    if (uVar2 == 0) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033974,
                s_Os_Dcu_DcuDebug_c_c003377c,0x264);
    }
  }
  if ((uVar2 == 1) && (uVar2 = DcuClientClose(*(undefined4 *)(param_1 + -0x3ed197e0)), uVar2 == 0))
  {
    Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033a70,
              s_Os_Dcu_DcuDebug_c_c003377c,0x26d);
  }
  if (uVar2 == 1) {
    local_2c = *(int *)(param_1 + -0x3ed197f8);
    local_34 = *(int *)(param_1 + -0x3ed197f4);
    local_20 = 0;
    *(undefined4 *)(local_2c * 4 + local_34 + -4) = 0xffffffff;
    local_30 = *(undefined4 *)(param_1 + -0x3ed197f0);
    *(undefined4 *)(local_2c * 4 + local_34 + -8) = 0xffffffff;
    local_28 = 0;
    if (*(int *)(param_1 + -0x3ed197e8) != 1) {
      local_28 = *(undefined4 *)(param_1 + -0x3ed197f8);
    }
    local_38 = uVar2;
    local_24 = uVar2;
    iVar3 = DcuClientOpen(0x61,&local_38);
    uVar2 = (uint)(iVar3 != 0);
    *(int *)(param_1 + -0x3ed197e0) = iVar3;
    if (uVar2 == 0) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033974,
                s_Os_Dcu_DcuDebug_c_c003377c,0x287);
    }
  }
  if (uVar2 == 1) {
    *(undefined4 *)(param_1 + -0x3ed19804) = 1;
    uVar2 = DcuClientSubmit(*(undefined4 *)(param_1 + -0x3ed197e0));
    if (uVar2 == 0) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c00339a8,
                s_Os_Dcu_DcuDebug_c_c003377c,0x292);
    }
  }
  if ((uVar2 == 1) &&
     (uVar2 = DcuSetClientControls(*(undefined4 *)(param_1 + -0x3ed197e0),2,2,2), uVar2 == 0)) {
    Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c00339e0,
              s_Os_Dcu_DcuDebug_c_c003377c,0x2a3);
  }
  if (uVar2 == 1) {
    Os_CacheInvalidateDataCacheRegion
              (*(undefined4 *)(param_1 + -0x3ed197f4),*(int *)(param_1 + -0x3ed197f8) << 2);
    uVar2 = DcuClientStart(*(undefined4 *)(param_1 + -0x3ed197e0));
    if (uVar2 == 0) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033a38,
                s_Os_Dcu_DcuDebug_c_c003377c,0x2af);
    }
  }
  return uVar2;
}


//==== DcuDebugWait @ c00293a0

uint * DcuDebugWait(uint *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint local_28;
  uint local_20;
  
  uVar6 = DAT_c0033cf0;
  local_28 = DAT_c0033cec;
  iVar2 = param_2 * 0x74;
  uVar5 = DAT_c0033cec & 0xff;
  local_20 = DAT_c0033cf4;
  if ((uVar5 == 1) && (*(int *)(iVar2 + -0x3ed197f4) == 0)) {
    Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033aa8,
              s_Os_Dcu_DcuDebug_c_c003377c,0x2d0);
    uVar5 = 0;
    local_28 = local_28 & 0xffffff00;
  }
  if (uVar5 == 1) {
    if (*(int *)(iVar2 + -0x3ed197e8) == 0) {
      local_28 = CONCAT31(local_28._1_3_,1);
      param_1[1] = 0;
      *param_1 = local_28;
      param_1[2] = 0;
      return param_1;
    }
    iVar1 = DcuClientWaitComplete(*(undefined4 *)(iVar2 + -0x3ed197e0),0);
    local_28 = CONCAT31(local_28._1_3_,(char)iVar1);
    if (iVar1 == 0) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033b48,
                s_Os_Dcu_DcuDebug_c_c003377c,0x2e6);
      iVar1 = *(int *)(iVar2 + -0x3ed197ec);
    }
    else {
      iVar1 = *(int *)(iVar2 + -0x3ed197ec);
    }
    if (iVar1 == 0) {
      iVar1 = *(int *)(iVar2 + -0x3ed197f4);
    }
    else {
      iVar1 = *(int *)(iVar2 + -0x3ed197f0);
    }
    local_20 = *(uint *)(iVar2 + -0x3ed197e4);
    uVar6 = iVar1 + *(int *)(iVar2 + -0x3ed197fc) * 4;
    Os_CacheInvalidateDataCacheRegion(uVar6 + local_20 * *(int *)(iVar2 + -0x3ed197fc) * 4 + -8,8);
    if (*(int *)(*(int *)(iVar2 + -0x3ed197e4) * *(int *)(iVar2 + -0x3ed197fc) * 4 + uVar6 + -8) ==
        -1) {
      do {
        OsThreadSleepMS(1);
        Os_CacheInvalidateDataCacheRegion
                  (uVar6 + *(int *)(iVar2 + -0x3ed197e4) * *(int *)(iVar2 + -0x3ed197fc) * 4 + -8,8)
        ;
      } while (*(int *)(*(int *)(iVar2 + -0x3ed197e4) * *(int *)(iVar2 + -0x3ed197fc) * 4 + uVar6 +
                       -8) == -1);
      iVar1 = *(int *)(iVar2 + -0x3ed197f8);
    }
    else {
      iVar1 = *(int *)(iVar2 + -0x3ed197f8);
    }
    Os_CacheInvalidateDataCacheRegion(uVar6,iVar1 << 2);
    uVar5 = *(uint *)(iVar2 + -0x3ed197e8);
    if (uVar5 < 2) {
      if (uVar5 == 1) {
        local_20 = *(uint *)(iVar2 + -0x3ed197e4);
        *(undefined4 *)(iVar2 + -0x3ed197e8) = 0;
        iVar1 = DcuClientStop(*(undefined4 *)(iVar2 + -0x3ed197e0),1);
        local_28 = CONCAT31(local_28._1_3_,(char)iVar1);
        if (iVar1 == 0) {
          Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033b8c,
                    s_Os_Dcu_DcuDebug_c_c003377c,0x345);
        }
        if ((char)iVar1 == '\x01') {
          iVar1 = DcuClientClose(*(undefined4 *)(iVar2 + -0x3ed197e0));
          local_28 = CONCAT31(local_28._1_3_,(char)iVar1);
          if (iVar1 == 0) {
            Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033b14,
                      s_Os_Dcu_DcuDebug_c_c003377c,0x34d);
            *(undefined4 *)(iVar2 + -0x3ed19804) = 2;
            DcuDebugDummyClientOpen(param_2);
          }
          else {
            *(undefined4 *)(iVar2 + -0x3ed19804) = 2;
            DcuDebugDummyClientOpen(param_2);
          }
        }
      }
    }
    else {
      iVar4 = *(int *)(iVar2 + -0x3ed197e4);
      iVar1 = 1 - *(int *)(iVar2 + -0x3ed197ec);
      iVar3 = *(int *)(iVar2 + -0x3ed19800) - iVar4;
      *(uint *)(iVar2 + -0x3ed197e8) = uVar5 - 1;
      *(int *)(iVar2 + -0x3ed197ec) = iVar1;
      *(int *)(iVar2 + -0x3ed19800) = iVar3;
      if (uVar5 - 1 == 1) {
        *(int *)(iVar2 + -0x3ed197e4) = iVar3;
        if (iVar1 == 0) {
          DcuClientSet(*(undefined4 *)(iVar2 + -0x3ed197e0),3,
                       iVar3 * *(int *)(iVar2 + -0x3ed197fc) + *(int *)(iVar2 + -0x3ed197fc));
          iVar1 = *(int *)(iVar2 + -0x3ed197ec);
          iVar4 = *(int *)(iVar2 + -0x3ed197e4);
        }
        else {
          DcuClientSet(*(undefined4 *)(iVar2 + -0x3ed197e0),4,
                       iVar3 * *(int *)(iVar2 + -0x3ed197fc) + *(int *)(iVar2 + -0x3ed197fc));
          iVar1 = *(int *)(iVar2 + -0x3ed197ec);
          iVar4 = *(int *)(iVar2 + -0x3ed197e4);
        }
      }
      if (iVar1 == 0) {
        memset(*(void **)(iVar2 + -0x3ed197f4),0,iVar4 * *(int *)(iVar2 + -0x3ed197fc) * 4);
        iVar1 = *(int *)(iVar2 + -0x3ed197e4);
        iVar3 = *(int *)(iVar2 + -0x3ed197fc);
        iVar4 = *(int *)(iVar2 + -0x3ed197f4);
      }
      else {
        memset(*(void **)(iVar2 + -0x3ed197f0),0,iVar4 * *(int *)(iVar2 + -0x3ed197fc) * 4);
        iVar1 = *(int *)(iVar2 + -0x3ed197e4);
        iVar3 = *(int *)(iVar2 + -0x3ed197fc);
        iVar4 = *(int *)(iVar2 + -0x3ed197f0);
      }
      *(undefined4 *)(iVar1 * iVar3 * 4 + iVar4) = 0xffffffff;
      *(undefined4 *)(*(int *)(iVar2 + -0x3ed197e4) * iVar3 * 4 + iVar4 + 4) = 0xffffffff;
      DcuClientSet(*(undefined4 *)(iVar2 + -0x3ed197e0),5,1 - *(int *)(iVar2 + -0x3ed197ec));
      iVar2 = DcuClientStart(*(undefined4 *)(iVar2 + -0x3ed197e0));
      local_28 = CONCAT31(local_28._1_3_,(char)iVar2);
      if (iVar2 == 0) {
        Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0033adc,
                  s_Os_Dcu_DcuDebug_c_c003377c,0x338);
      }
    }
  }
  param_1[1] = uVar6;
  *param_1 = local_28;
  param_1[2] = local_20;
  return param_1;
}


//==== DcuDebugGetInfoType @ c00297f8

undefined4 DcuDebugGetInfoType(int param_1,uint param_2)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (param_2 < 8) {
    uVar1 = *(undefined4 *)(param_2 * 4 + param_1 * 0x74 + -0x3ed19848);
  }
  return uVar1;
}


//==== DcuDebugGetClientId @ c002982c

undefined4 DcuDebugGetClientId(int param_1,uint param_2)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (param_2 < 8) {
    uVar1 = *(undefined4 *)(param_2 * 4 + param_1 * 0x74 + -0x3ed19828);
  }
  return uVar1;
}


//==== DcuDebugGetInfo @ c0029860

int DcuDebugGetInfo(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  code *pcVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  
  iVar4 = param_1 * 0x74;
  if (param_5 == 0) {
    iVar7 = *(int *)(iVar4 + -0x3ed197f4);
    uVar1 = *(int *)(iVar4 + -0x3ed19800) + 1;
  }
  else {
    iVar7 = *(int *)(iVar4 + -0x3ed197dc);
    uVar1 = 0x100;
  }
  iVar3 = -1;
  if (*(int *)(iVar4 + -0x3ed19804) == 2) {
    if ((uint)(param_2 + param_3) <= uVar1) {
      uVar1 = param_2 + param_3;
    }
    uVar8 = param_2 + 1;
    iVar3 = 0;
    if (uVar8 <= uVar1) {
      do {
        iVar4 = iVar3 * 0x24 + param_4;
        iVar6 = 7;
        pcVar5 = s_dcuDebugOutputConf;
        do {
          uVar2 = *(uint *)(pcVar5 + 4);
          iVar6 = iVar6 + -1;
          *(uint *)(iVar4 + 4) =
               (*(uint *)((uVar2 >> 5) * 4 + iVar7) &
               (1 << (*(uint *)(pcVar5 + 8) & 0x1f)) + -1 << (uVar2 & 0x1f)) >> (uVar2 & 0x1f);
          pcVar5 = pcVar5 + 0xc;
          iVar4 = iVar4 + 4;
        } while (-1 < iVar6);
        uVar8 = uVar8 + 1;
        iVar7 = iVar7 + *(int *)(param_1 * 0x74 + -0x3ed197fc) * 4;
        iVar3 = iVar3 + 1;
      } while (uVar8 <= uVar1);
    }
  }
  return iVar3;
}


//==== DcuGetDebugRawData @ c0029968

undefined4 DcuGetDebugRawData(int param_1,int param_2)

{
  if (param_2 == 0) {
    return *(undefined4 *)(param_1 * 0x74 + -0x3ed197f4);
  }
  return *(undefined4 *)(param_1 * 0x74 + -0x3ed197dc);
}


//==== DcuGetDebugRawDataLength @ c0029990

int DcuGetDebugRawDataLength(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = 0x100;
  if (param_2 == 0) {
    iVar1 = *(int *)(param_1 * 0x74 + -0x3ed19800) + 1;
  }
  return iVar1;
}


//==== DcuDebugGetRecordLayoutInternal @ c00299bc

int DcuDebugGetRecordLayoutInternal(int param_1,int *param_2,int *param_3,uint param_4)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  uint uVar4;
  
  if (param_3 != (int *)0x0) {
    iVar1 = param_2[0x12];
    param_3[2] = param_1;
    *param_3 = iVar1;
    param_3[1] = param_4;
    uVar3 = 0;
    piVar2 = param_2;
    if (param_4 != 0) {
      do {
        while( true ) {
          iVar1 = *piVar2;
          param_3[uVar3 * 4 + 3] = iVar1;
          if (iVar1 == 0x40) break;
          uVar4 = uVar3 + 1;
          iVar1 = ConvertClientIdToHwId(piVar2[8]);
          param_3[uVar3 * 4 + 4] = iVar1;
          param_3[uVar3 * 4 + 5] = uVar3 * 0x10;
          param_3[uVar3 * 4 + 6] = 0x10;
          piVar2 = piVar2 + 1;
          uVar3 = uVar4;
          if (param_4 <= uVar4) goto LAB_c0029a78;
        }
        uVar4 = uVar3 + 1;
        param_3[uVar3 * 4 + 4] = param_2[0x10];
        param_3[uVar3 * 4 + 5] = uVar3 * 0x10;
        param_3[uVar3 * 4 + 6] = 0x10;
        piVar2 = piVar2 + 1;
        uVar3 = uVar4;
      } while (uVar4 < param_4);
    }
  }
LAB_c0029a78:
  return param_4 * 0x10 + 0xc;
}


//==== DcuDebugGetRecordLayout @ c0029ab0

void DcuDebugGetRecordLayout(undefined4 param_1,int param_2)

{
  if ((*(int *)(param_2 + 0x48) != 2) && (*(int *)(param_2 + 0x48) != 4)) {
                    /* WARNING: Subroutine does not return */
    OsSysHaltEx(s_Unsupported_debug_client_record_s_c0033bc0,s_Os_Dcu_DcuDebug_c_c003377c,0x48e,8);
  }
  DcuDebugGetRecordLayoutInternal();
  return;
}


//==== DcuDebugGetMaxLayoutSize @ c0029afc

void DcuDebugGetMaxLayoutSize(void)

{
  DcuDebugGetRecordLayoutInternal(0,0,0,8);
  return;
}


//==== DcuDebugClientsPerRecord @ c0029b10

undefined4 DcuDebugClientsPerRecord(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = 3;
  if (param_1 != 2) {
    uVar1 = 7;
  }
  return uVar1;
}


//==== DcuClientOpenByDram @ c0029b24

undefined4 DcuClientOpenByDram(int param_1,uint param_2,int param_3)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  
  if (0 < param_1) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Open_cpp_c0033cf8,0xba,s_Invalid_DRAM_selector__c0033d2c);
  }
  if (0xea < param_2) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Open_cpp_c0033cf8,0xbb,s_Invalid_DCU_client_ID__c0033d44);
  }
  uVar1 = IpcCriticalSectionEnter();
  if (param_2 < 0x86) {
    if ((param_2 < 0x84) && ((param_2 < 0x3a || ((0x3d < param_2 && (3 < param_2 - 0x59))))))
    goto LAB_c0029b88;
  }
  else {
    if (param_2 < 0xc3) goto LAB_c0029b88;
    if (0xc4 < param_2) {
      if (param_2 < 0xc9) {
        IpConfigAssertFeature(0x1f,0);
      }
      goto LAB_c0029b88;
    }
  }
  IpConfigAssertFeature(0x1b,0);
LAB_c0029b88:
  if (0 < param_1) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Open_cpp_c0033cf8,0x5b,s_Invalid_DRAM_selector__c0033d2c);
  }
  if (0xea >= param_2) {
    uVar3 = *(uint *)((uint)(g_clientMutex + (param_1 * 8 + (param_2 >> 5)) * 4) | 0x20000000);
    uVar2 = 1 << (param_2 & 0x1f);
    if ((uVar3 & uVar2) == 0) {
      *(uint *)((uint)(g_clientMutex + (param_1 * 8 + (param_2 >> 5)) * 4) | 0x20000000) =
           uVar3 | uVar2;
    }
    else if (param_2 != 0x61) {
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                s_Warning__DCU_client_concurrency__c0033d08);
      return 0;
    }
    IpcCriticalSectionLeave(uVar1);
    uVar1 = DcuInternalIdtoHandle(param_1,param_2);
    if (param_3 != 0) {
      DcuClientSet(uVar1,0xff,param_3);
    }
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Open_cpp_c0033cf8,0x5c,s_Invalid_DCU_client_ID__c0033d44);
}


//==== DcuClientOpen @ c0029d0c

void DcuClientOpen(undefined4 param_1,undefined4 param_2)

{
  DcuClientOpenByDram(0,param_1,param_2);
  return;
}


//==== DcuClientClose @ c0029d1c

undefined4 DcuClientClose(int param_1)

{
  undefined4 uVar1;
  uint uVar2;
  
  if (param_1 == 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Open_cpp_c0033cf8,0xea,s_Invalid_DCU_client_handle__c0033d5c);
  }
  if (0 < *(int *)(param_1 + 4)) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Open_cpp_c0033cf8,0xeb,s_Invalid_DRAM_selector__c0033d2c);
  }
  if (0xea < *(uint *)(param_1 + 0xc)) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Open_cpp_c0033cf8,0xec,s_Invalid_DCU_client_ID__c0033d44);
  }
  uVar1 = IpcCriticalSectionEnter();
  Os_CacheWriteBackDataCacheRegion(*(undefined4 *)(param_1 + 0x44),*(undefined4 *)(param_1 + 0x24));
  Os_CacheWriteBackDataCacheRegion(param_1,0x48);
  uVar2 = *(uint *)(param_1 + 0xc);
  if (*(int *)(param_1 + 4) < 1) {
    if (uVar2 < 0xeb) {
      *(uint *)((uint)(g_clientMutex + (*(int *)(param_1 + 4) * 8 + (uVar2 >> 5)) * 4) | 0x20000000)
           = *(uint *)((uint)(g_clientMutex + (*(int *)(param_1 + 4) * 8 + (uVar2 >> 5)) * 4) |
                      0x20000000) & ~(1 << (uVar2 & 0x1f));
      IpcCriticalSectionLeave(uVar1);
      return 1;
    }
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Open_cpp_c0033cf8,0x71,s_Invalid_DCU_client_ID__c0033d44);
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Open_cpp_c0033cf8,0x70,s_Invalid_DRAM_selector__c0033d2c);
}


//==== DcuSetBlockScaling @ c0029e58

void DcuSetBlockScaling(int param_1,uint param_2,char param_3,uint *param_4,undefined4 *param_5,
                       undefined4 *param_6,int *param_7,int *param_8,uint *param_9)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  int *piVar6;
  uint uVar7;
  int *piVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  uint *puVar12;
  uint uVar15;
  uint auStack_18 [4];
  uint *puVar13;
  int *piVar14;
  
  iVar11 = 1;
  uVar10 = param_2;
  if (param_3 == '\0') {
    uVar10 = param_2 << 1;
    iVar11 = 2;
  }
  iVar3 = uVar10 + iVar11;
  iVar9 = -(iVar3 * 4 + 0xeU & 0xfffffff8);
  puVar12 = (uint *)((int)auStack_18 + iVar9);
  piVar4 = (int *)((int)auStack_18 + iVar9);
  puVar13 = (uint *)((int)auStack_18 + iVar9);
  piVar14 = (int *)((int)auStack_18 + iVar9);
  piVar5 = (int *)((int)auStack_18 + iVar9);
  piVar6 = (int *)((int)auStack_18 + iVar9);
  uVar7 = 0;
  for (; iVar3 != 0; iVar3 = iVar3 + -1) {
    uVar15 = uVar7 / param_2;
    if (param_2 == 0) {
      trap(7);
    }
    uVar7 = uVar7 + param_1;
    *puVar12 = uVar15;
    puVar12 = puVar12 + 1;
  }
  bVar2 = uVar10 != 0;
  if (param_3 == '\0') {
    *param_6 = *(undefined4 *)((int)auStack_18 + iVar9 + 4);
  }
  if (bVar2) {
    piVar8 = (int *)((int)auStack_18 + iVar11 * 4 + iVar9);
    uVar7 = uVar10;
    do {
      uVar7 = uVar7 - 1;
      *piVar4 = *piVar8 - *piVar4;
      piVar8 = piVar8 + 1;
      piVar4 = piVar4 + 1;
    } while (uVar7 != 0);
  }
  uVar7 = 0xff;
  bVar1 = bVar2;
  uVar15 = uVar10;
  while (bVar1) {
    uVar15 = uVar15 - 1;
    if (*puVar13 < uVar7) {
      uVar7 = *puVar13;
    }
    bVar1 = uVar15 != 0;
    puVar13 = puVar13 + 1;
  }
  *param_4 = uVar7;
  *param_5 = 1;
  while (bVar2) {
    uVar10 = uVar10 - 1;
    *piVar14 = *piVar14 - uVar7;
    bVar2 = uVar10 != 0;
    piVar14 = piVar14 + 1;
  }
  iVar11 = 0;
  iVar9 = 0;
  uVar10 = 0;
  if (param_2 != 0) {
    do {
      while (param_3 != '\0') {
        piVar5 = piVar5 + 2;
        uVar7 = uVar10 & 0x1f;
        uVar10 = uVar10 + 1;
        iVar11 = iVar11 + (*piVar6 << uVar7);
        piVar6 = piVar6 + 1;
        if (param_2 <= uVar10) goto LAB_c0029fd8;
      }
      uVar7 = uVar10 & 0x1f;
      uVar15 = uVar10 & 0x1f;
      uVar10 = uVar10 + 1;
      iVar11 = iVar11 + (*piVar5 << uVar7);
      iVar9 = iVar9 + (piVar5[1] << uVar15);
      piVar6 = piVar6 + 1;
      piVar5 = piVar5 + 2;
    } while (uVar10 < param_2);
  }
LAB_c0029fd8:
  *param_7 = iVar11;
  *param_8 = iVar9;
  *param_9 = param_2;
  return;
}


//==== DcuGetScaleRatio @ c002a000

undefined4 DcuGetScaleRatio(int param_1,uint param_2,int param_3,uint *param_4)

{
  code *pcVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  code *pcVar5;
  uint uVar6;
  code cVar7;
  code cVar8;
  code *pcVar9;
  uint uVar10;
  undefined2 uVar11;
  
  uVar3 = (uint)(byte)cRamc0032cdd;
  if (uVar3 == 0) {
    trap(7);
  }
  uVar6 = 0;
  uVar2 = ((uint)(byte)s_scaleFactors * param_1) / uVar3;
  pcVar1 = s_scaleFactors;
  pcVar9 = s_scaleFactors;
  cVar7 = cRamc0032cdd;
  do {
    pcVar5 = pcVar1;
    cVar8 = *pcVar5;
    uVar6 = uVar6 + 1;
    uVar4 = (uint)(byte)cVar8;
    uVar10 = (uVar4 * param_1) / uVar3;
    if (uVar3 == 0) {
      trap(7);
    }
    if (uVar10 == param_2) {
      param_4[1] = uVar3;
      *param_4 = uVar4;
      uVar11 = CONCAT11(cVar7,cVar8);
      goto LAB_c002a0b4;
    }
    if (param_2 < uVar10) {
      if (param_3 != 2) {
        if (param_3 == 1) {
          cVar8 = *pcVar9;
        }
        else {
          if (uVar10 - param_2 < param_2 - uVar2) goto LAB_c002a0f4;
          cVar8 = *pcVar9;
        }
        cVar7 = pcVar9[1];
      }
LAB_c002a0f4:
      param_4[1] = (uint)(byte)cVar7;
      *param_4 = (uint)(byte)cVar8;
      TimingMarkF(0x8000,s_DcuGetScaleRatio_1__d__d__d__d_c0033d78,param_1,param_2,(uint)(byte)cVar7
                  ,(uint)(byte)cVar8);
      return 1;
    }
    if (0x91 < uVar6) {
      uVar4 = (uint)(byte)*pcVar5;
      uVar11 = *(undefined2 *)pcVar5;
      uVar3 = (uint)(byte)pcVar5[1];
      param_4[1] = uVar3;
      *param_4 = uVar4;
LAB_c002a0b4:
      TimingMarkF(0x8000,s_DcuGetScaleRatio_1__d__d__d__d_c0033d78,param_1,param_2,uVar3,uVar4,
                  uVar11);
      return 1;
    }
    cVar7 = pcVar5[3];
    uVar3 = (uint)(byte)cVar7;
    uVar2 = uVar10;
    pcVar1 = pcVar5 + 2;
    pcVar9 = pcVar5;
  } while( true );
}


//==== DcuInit @ c002a31c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 DcuInit(void)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  
  DAT_c12e6875 = 0;
  DAT_c12e6874 = 0;
  IpcCriticalSectionInit(0);
  DcuBufferSplitInit();
  iVar1 = DcuInternalIdtoHandle(0,10);
  puVar2 = (undefined4 *)DcuGetClientAddr(iVar1,8);
  *puVar2 = *(undefined4 *)(iVar1 + 0x20);
  puVar2 = (undefined4 *)DcuGetClientAddr(iVar1,0x10);
  *puVar2 = 1;
  puVar2 = (undefined4 *)DcuGetClientAddr(iVar1,8);
  *puVar2 = *(undefined4 *)(iVar1 + 0x20);
  puVar2 = (undefined4 *)DcuGetClientAddr(iVar1,0x10);
  *puVar2 = 1;
  iVar1 = DcuInternalIdtoHandle(0,0x1d);
  puVar2 = (undefined4 *)DcuGetClientAddr(iVar1,8);
  *puVar2 = *(undefined4 *)(iVar1 + 0x20);
  puVar2 = (undefined4 *)DcuGetClientAddr(iVar1,0x10);
  *puVar2 = 1;
  puVar2 = (undefined4 *)DcuGetClientAddr(iVar1,8);
  *puVar2 = *(undefined4 *)(iVar1 + 0x20);
  puVar2 = (undefined4 *)DcuGetClientAddr(iVar1,0x10);
  *puVar2 = 1;
  uVar3 = 8;
  if (_DAT_b8003c0c != 9 && _DAT_b8003c10 < 5) {
    uVar3 = 4;
  }
  DcuSetDramParams(uVar3,0x10 << (_DAT_b8003d58 & 0x1f));
  DcuSlotMachineInit__();
  DcuDebugInit(0);
  DcuFifoHandlingInit(0);
  DcuSstlCal1Init(0,1000);
  InitCacheRegisters__();
  return 1;
}


//==== DcuGetDramAttributes @ c002a48c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
DcuGetDramAttributes
          (undefined4 param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4,
          undefined4 *param_5)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  switch(_DAT_b8003c10) {
  default:
    return 0;
  case 1:
    uVar2 = 0x800000;
    uVar1 = 0x200;
    goto LAB_c002a4d0;
  case 2:
    uVar2 = 0x1000000;
    break;
  case 3:
    uVar2 = 0x2000000;
    break;
  case 4:
    uVar2 = 0x4000000;
    break;
  case 5:
    uVar2 = 0x8000000;
    break;
  case 6:
    uVar2 = 0x10000000;
    break;
  case 7:
    uVar2 = 0x20000000;
  }
  uVar1 = 0x400;
LAB_c002a4d0:
  if (param_2 != (undefined4 *)0x0) {
    *param_2 = uVar2;
  }
  if (param_3 != (undefined4 *)0x0) {
    *param_3 = uVar1;
  }
  if (param_4 != (undefined4 *)0x0) {
    *param_4 = 0xc0000000;
  }
  if (param_5 != (undefined4 *)0x0) {
    *param_5 = 0xe0000000;
  }
  return 1;
}


//==== DcuSetDramParams @ c002a534

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 DcuSetDramParams(uint param_1,uint param_2)

{
  char *pcVar1;
  
  pcVar1 = s_Bank_number_is_not_a_power_of_tw_c0033db8;
  if ((param_1 - 1 & param_1) == 0) {
    pcVar1 = s_Bank_width_is_not_a_power_of_two_c0033ddc;
    if ((param_2 - 1 & param_2) == 0) {
      _DAT_c12e687c = param_1;
      _DAT_c12e6878 = param_2;
      return 1;
    }
  }
  Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),pcVar1);
  return 0;
}


//==== DcuSetAttr @ c002a5a8

void DcuSetAttr(int param_1,int param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  
  if (0 < param_1) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Dram_cpp_c0033e00,999,s_Invalid_DRAM_selector__c0033e10);
  }
  puVar1 = (undefined4 *)&DAT_b8003d98;
  if (param_2 != 0) {
    if (param_2 != 1) {
                    /* WARNING: Subroutine does not return */
      SysHaltWithContextF(s_Os_Dcu_Dram_cpp_c0033e00,0x408,
                          s_DcuSetAttr__Invalid_DCU_attribut_c0033e28);
    }
    puVar1 = (undefined4 *)&DAT_b8003c7c;
  }
  *puVar1 = param_3;
  return;
}


//==== DcuInitDram @ c002a614

void DcuInitDram(void)

{
  DcuSetAttr(0,1,1);
  DcuSetAttr(0,0,1);
  DcuMemoryProtectionInit();
  return;
}


//==== DcuGetOptimizedHgap @ c002a648

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int DcuGetOptimizedHgap(int param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  if (param_2 != 0) {
    if (2 < param_2) {
                    /* WARNING: Subroutine does not return */
      SysHaltWithContextF(s_Os_Dcu_Dram_cpp_c0033e00,0x3c7,s_Illegal_Optimization_Level__u_c0033e4c,
                          param_2);
    }
    uVar2 = ((param_1 + _DAT_c12e6878) - 1) / _DAT_c12e6878;
    if (_DAT_c12e6878 == 0) {
      trap(7);
    }
    uVar1 = 3;
    if (2 < uVar2) {
      uVar1 = uVar2;
    }
    if (_DAT_c12e687c == 0) {
      trap(7);
    }
    param_1 = ((((uVar1 + _DAT_c12e687c) - 4) / _DAT_c12e687c) * _DAT_c12e687c + 3) * _DAT_c12e6878;
  }
  return param_1;
}


//==== DcuGetOptimizedWidth @ c002a6dc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint DcuGetOptimizedWidth(int param_1,uint param_2,undefined4 param_3)

{
  int iVar1;
  
  if ((_DAT_c12e687c != 0) && (_DAT_c12e6878 != 0)) {
    iVar1 = DcuGetOptimizedHgap((param_1 * param_2 >> 3) + 3 >> 2,param_3);
    if (param_2 == 0) {
      trap(7);
    }
    return (uint)(iVar1 << 5) / param_2;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Dram_cpp_c0033e00,0x3d9,s_Dram_params_were_not_set__c0033e6c);
}


//==== DcuClientStartEx(tagSDCUCLIENTOBJ*, @ c002a75c

undefined4 DcuClientStartEx_tagSDCUCLIENTOBJ__(int *param_1,int *param_2)

{
  undefined4 *puVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  uint *puVar5;
  
  if (param_1 == (int *)0x0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Start_cpp_c0033e9c,0x58,s_Invalid_DCU_client_handle__c0033eb0);
  }
  if (param_1[2] != 0) {
    return 1;
  }
  if ((*param_1 == 0) && (param_1[1] != 0)) {
    Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
              s__s__Client__d_had_DdrSel__d_but_n_c0033ecc,DcuClientStartEx_tagSDCUCLIENTOBJ__,
              param_1[3],param_1[1]);
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Start_cpp_c0033e9c,0x6b,s_Invalid_DdrSel_for_client_c0033f00);
  }
  if (param_2 == (int *)0x0) {
    DcuCheckClientIsInCurrentSlotMachine(param_1[1],param_1[3]);
    DcuValidateClient(param_1[1],param_1[3]);
    iVar3 = param_1[5];
  }
  else {
    iVar3 = param_1[5];
  }
  if (iVar3 == 0) {
    puVar1 = (undefined4 *)DcuGetClientAddr(param_1,0x3c);
    if (param_2 != (int *)0x0) {
      puVar4 = (undefined4 *)(param_2[2] * 4 + *param_2);
      param_2[2] = param_2[2] + 2;
      *puVar4 = puVar1;
      puVar4[1] = 1;
      puVar5 = (uint *)((param_1[1] * 8 + ((uint)param_1[3] >> 5)) * 4 + -0x3ed197ac);
      *puVar5 = *puVar5 | 1 << (param_1[3] & 0x1fU);
      return 1;
    }
    *puVar1 = 1;
    uVar2 = param_1[3];
  }
  else {
    puVar5 = (uint *)DcuGetClientAddr(param_1,0x20);
    if (param_2 == (int *)0x0) {
      *puVar5 = param_1[6] | param_1[7] | param_1[10] | param_1[0xb] | param_1[0xc] | param_1[0xd] |
                param_1[0xe] | 1;
      uVar2 = param_1[3];
    }
    else {
      puVar1 = (undefined4 *)(param_2[2] * 4 + *param_2);
      param_2[2] = param_2[2] + 2;
      *puVar1 = puVar5;
      puVar1[1] = param_1[6] | param_1[7] | param_1[10] | param_1[0xb] | param_1[0xc] | param_1[0xd]
                  | param_1[0xe] | 1;
      uVar2 = param_1[3];
    }
  }
  puVar5 = (uint *)((param_1[1] * 8 + (uVar2 >> 5)) * 4 + -0x3ed197ac);
  *puVar5 = *puVar5 | 1 << (uVar2 & 0x1f);
  return 1;
}


//==== DcuClientStartToCbc @ c002a9b4

bool DcuClientStartToCbc(int param_1)

{
  int iVar1;
  
  if (param_1 != 0) {
    iVar1 = DcuClientStartEx_tagSDCUCLIENTOBJ__();
    return iVar1 == 1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Start_cpp_c0033e9c,0x3e,s_Invalid_DCU_client_handle__c0033eb0);
}


//==== DcuClientStart @ c002a9f4

bool DcuClientStart(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = DcuClientStartEx_tagSDCUCLIENTOBJ__(param_1,0);
  return iVar1 == 1;
}


//==== DcuClientStopEx(tagSDCUCLIENTOBJ*, @ c002aa18

/* WARNING: Removing unreachable block (ram,0xc002abbc) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 DcuClientStopEx_tagSDCUCLIENTOBJ__(int param_1,uint param_2,int *param_3)

{
  undefined4 *puVar1;
  uint *puVar2;
  undefined4 uVar3;
  uint uVar4;
  undefined4 *puVar5;
  
  if (param_1 == 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Start_cpp_c0033e9c,0xb0,s_Invalid_DCU_client_handle__c0033eb0);
  }
  if (*(int *)(param_1 + 8) != 0) {
    return 1;
  }
  puVar1 = (undefined4 *)DcuGetClientAddr(param_1,0x3c);
  puVar2 = (uint *)DcuGetClientAddr(param_1,0x20);
  uVar3 = Os_InterruptsDisable();
  if (*(int *)(param_1 + 0x14) == 1) {
    if (param_3 == (int *)0x0) {
      *puVar2 = *(uint *)(param_1 + 0x18) | *(uint *)(param_1 + 0x38) | *(uint *)(param_1 + 0x34);
    }
    else {
      puVar5 = (undefined4 *)(param_3[2] * 4 + *param_3);
      param_3[2] = param_3[2] + 2;
      *puVar5 = puVar2;
      puVar5[1] = *(uint *)(param_1 + 0x18) | *(uint *)(param_1 + 0x38) | *(uint *)(param_1 + 0x34);
    }
  }
  if ((param_2 & 1) != 0) {
    uVar4 = *(uint *)(param_1 + 0x10);
    if ((((2 < uVar4 && uVar4 != 8) && (uVar4 != 10)) && (uVar4 != 5)) &&
       (((uVar4 != 4 && (uVar4 != 6)) && (uVar4 != 0xd)))) {
      Os_InterruptsRestore(uVar3);
      return 0;
    }
    if (param_3 == (int *)0x0) {
      *puVar1 = 0;
    }
    else {
      puVar5 = (undefined4 *)(param_3[2] * 4 + *param_3);
      param_3[2] = param_3[2] + 2;
      puVar5[1] = 0;
      *puVar5 = puVar1;
    }
    if ((param_2 & 2) != 0) {
      uVar4 = *puVar2;
      while ((uVar4 & 1) == 0) {
        TimingMarkF(0x10,s_Client__d__Non_configurable__res_c0033f1c,*(undefined4 *)(param_1 + 0xc))
        ;
        if (param_3 == (int *)0x0) {
          *puVar1 = 1;
LAB_c002abc4:
          *puVar1 = 0;
        }
        else {
          puVar5 = (undefined4 *)(param_3[2] * 4 + *param_3);
          param_3[2] = param_3[2] + 2;
          puVar5[1] = 1;
          *puVar5 = puVar1;
          if (param_3 == (int *)0x0) goto LAB_c002abc4;
          puVar5 = (undefined4 *)(param_3[2] * 4 + *param_3);
          param_3[2] = param_3[2] + 2;
          puVar5[1] = 0;
          *puVar5 = puVar1;
        }
        uVar4 = *puVar2;
      }
    }
  }
  Os_InterruptsRestore(uVar3);
  puVar2 = (uint *)((*(int *)(param_1 + 4) * 8 + (*(uint *)(param_1 + 0xc) >> 5)) * 4 + -0x3ed197ac)
  ;
  *puVar2 = *puVar2 & ~(1 << (*(uint *)(param_1 + 0xc) & 0x1f));
  return 1;
}


//==== DcuClientStopToCbc @ c002ad1c

bool DcuClientStopToCbc(int param_1)

{
  int iVar1;
  
  if (param_1 != 0) {
    iVar1 = DcuClientStopEx_tagSDCUCLIENTOBJ__();
    return iVar1 == 1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Start_cpp_c0033e9c,0x46,s_Invalid_DCU_client_handle__c0033eb0);
}


//==== DcuClientStop @ c002ad5c

bool DcuClientStop(int param_1,undefined4 param_2)

{
  int iVar1;
  
  if (param_1 != 0) {
    iVar1 = DcuClientStopEx_tagSDCUCLIENTOBJ__(param_1,param_2,0);
    return iVar1 == 1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Start_cpp_c0033e9c,0x36,s_Invalid_DCU_client_handle__c0033eb0);
}


//==== DcuClientWaitComplete @ c002ada0

undefined4 DcuClientWaitComplete(int param_1,int param_2)

{
  uint *puVar1;
  uint uVar2;
  
  if (param_1 == 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Start_cpp_c0033e9c,0x122,s_Invalid_DCU_client_handle__c0033eb0);
  }
  if ((param_2 == 0) && (*(int *)(param_1 + 0x10) == 8 || *(int *)(param_1 + 0x10) == 1)) {
    puVar1 = (uint *)DcuGetClientAddr(param_1,0x20);
    uVar2 = *puVar1;
    while (((uVar2 & 2) != 0 && (Os_TaskSleep(1), (*puVar1 & 2) != 0))) {
      Os_TaskSleep(1);
      uVar2 = *puVar1;
    }
    return 1;
  }
  return 0;
}


//==== DcuSstlCal1RegistersPrintEnable @ c002af7c

void DcuSstlCal1RegistersPrintEnable(char param_1)

{
  s_sstlRegisterPrintMode = (code)(param_1 == '\x01');
  return;
}


//==== DcuSstlCal1PrintLut @ c002af94

void DcuSstlCal1PrintLut(int param_1,int param_2)

{
  undefined *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  
  puVar1 = &DAT_c0035298;
  if (param_1 != 1) {
    puVar1 = &DAT_c00352a0;
  }
  printf(s_lut__s__d__c00352a4,puVar1,param_2);
  if (param_1 == 1) {
    piVar4 = *(int **)(&DAT_c12e68b8 + param_2 * 4);
    iVar5 = 0;
    if (0 < piVar4[1]) {
      iVar3 = *piVar4;
      while( true ) {
        puVar2 = (undefined4 *)(iVar5 * 8 + iVar3);
        printf(s_0x_x__>_0x_x_c00352b0,*puVar2,puVar2[1]);
        iVar5 = iVar5 + 1;
        if (piVar4[1] <= iVar5) break;
        iVar3 = *piVar4;
      }
      return;
    }
  }
  else {
    piVar4 = *(int **)(&DAT_c12e68a4 + param_2 * 4);
    iVar5 = 0;
    if (0 < piVar4[1]) {
      iVar3 = *piVar4;
      while( true ) {
        puVar2 = (undefined4 *)(iVar5 * 8 + iVar3);
        printf(s_0x_x__>_0x_x_c00352b0,*puVar2,puVar2[1]);
        iVar5 = iVar5 + 1;
        if (piVar4[1] <= iVar5) break;
        iVar3 = *piVar4;
      }
    }
  }
  return;
}


//==== DcuSstlCal1SetCalibrationInterval @ c002b0c0

void DcuSstlCal1SetCalibrationInterval(undefined4 param_1)

{
  SstlCalWriteReg_unsigned
            (0xb8003e88,param_1,s_MIF_SSTL_CAL1_DIV_SELB_c00352c0,
             DcuSstlCal1SetCalibrationInterval____func__,0x185);
  return;
}


//==== DcuSstlCal1SetContinuousCalibration @ c002b0fc

void DcuSstlCal1SetContinuousCalibration(char param_1)

{
  SstlCalWriteReg_unsigned
            (0xb8003e80,param_1 != '\0',s_MIF_SSTL_CAL1_CONT_c00352d8,
             DcuSstlCal1SetContinuousCalibration____func__,0x157);
  return;
}


//==== DcuSstlCal1SetCalibrationDuringRefresh @ c002b13c

void DcuSstlCal1SetCalibrationDuringRefresh(char param_1)

{
  SstlCalWriteReg_unsigned
            (0xb8003e8c,param_1 != '\0',s_MIF_SSTL_CAL1_LOADREF_EN_c00352ec,
             DcuSstlCal1SetCalibrationDuringRefresh____func__,0x195);
  return;
}


//==== DcuSstlCal1SetActive @ c002b17c

void DcuSstlCal1SetActive(char param_1)

{
  if (param_1 != '\0') {
    _tx_timer_activate_api(0xc12e68cc);
    return;
  }
  _tx_timer_deactivate_api(0xc12e68cc);
  return;
}


//==== DcuSstlCal1SetTimerDuration @ c002b1a0

void DcuSstlCal1SetTimerDuration(undefined4 param_1)

{
  int local_18 [2];
  
  _tx_timer_info_get(0xc12e68cc,0,local_18,0,0,0);
  if (local_18[0] == 1) {
    DcuSstlCal1SetActive(0);
    _tx_timer_change(0xc12e68cc,param_1,param_1);
  }
  else {
    _tx_timer_change(0xc12e68cc,param_1,param_1);
  }
  if (local_18[0] != 1) {
    return;
  }
  DcuSstlCal1SetActive(1);
  return;
}


//==== DcuSstlCal1IsReadyToAcceptUpdatedResults @ c002b264

undefined1 DcuSstlCal1IsReadyToAcceptUpdatedResults(void)

{
  undefined1 uVar1;
  
  uVar1 = SstlCalReadReg_unsigned
                    (0xb8003e90,s_MIF_SSTL_CAL1_1_LOAD_READY_c0035318,
                     DcuSstlCal1IsReadyToAcceptUpdatedResults____func__,0x1a2);
  return uVar1;
}


//==== DcuSstlCal1GetResults @ c002b29c

uint * DcuSstlCal1GetResults(uint *param_1)

{
  uint uVar1;
  
  uVar1 = SstlCalReadReg_unsigned
                    (0xb8003e94,s_MIF_SSTL_CAL1_1_c0035334,DcuSstlCal1GetResults____func__,0x1ab);
  *param_1 = uVar1 & 0x3f;
  param_1[1] = (uVar1 << 0x12) >> 0x1a;
  return param_1;
}


//==== DcuSstlCal1SetSstlValid @ c002b2f0

void DcuSstlCal1SetSstlValid(void)

{
  SstlCalWriteReg_unsigned
            (0xb8003eb4,1,s_MIF_SSTL_PARAM_VALID_c003536c,DcuSstlCal1SetSstlValid____func__,0x208);
  return;
}


//==== DcuSstlCal1Recalibrate @ c002b32c

void DcuSstlCal1Recalibrate(void)

{
  _tx_event_flags_set(0xc12e6880,1,0);
  return;
}


//==== DcuSstlCal1StoreLutToFile @ c002b340

void DcuSstlCal1StoreLutToFile(int param_1,int param_2,undefined4 param_3)

{
  char *__s;
  size_t sVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  int *piVar5;
  
  if (param_1 == 1) {
    piVar5 = *(int **)(&DAT_c12e68b8 + param_2 * 4);
    __s = (char *)PoolAllocEx(0,0x50,0,s___Os_Lib_NearestKeyLut_h_c003539c,0x10a);
    iVar4 = 0;
    if (0 < piVar5[1]) {
      iVar2 = *piVar5;
      while( true ) {
        puVar3 = (undefined4 *)(iVar4 * 8 + iVar2);
        sprintf(__s,s_0x_x__>_0x_x_c00352b0,*puVar3,puVar3[1]);
        sVar1 = strlen(__s);
        func_0xc01eddd4(param_3,__s,sVar1);
        iVar4 = iVar4 + 1;
        if (piVar5[1] <= iVar4) break;
        iVar2 = *piVar5;
      }
      PoolFreeEx(0,__s,s___Os_Lib_NearestKeyLut_h_c003539c,0x116);
      return;
    }
  }
  else {
    piVar5 = *(int **)(&DAT_c12e68a4 + param_2 * 4);
    __s = (char *)PoolAllocEx(0,0x50,0,s___Os_Lib_NearestKeyLut_h_c003539c,0x10a);
    iVar4 = 0;
    if (0 < piVar5[1]) {
      iVar2 = *piVar5;
      while( true ) {
        puVar3 = (undefined4 *)(iVar4 * 8 + iVar2);
        sprintf(__s,s_0x_x__>_0x_x_c00352b0,*puVar3,puVar3[1]);
        sVar1 = strlen(__s);
        func_0xc01eddd4(param_3,__s,sVar1);
        iVar4 = iVar4 + 1;
        if (piVar5[1] <= iVar4) break;
        iVar2 = *piVar5;
      }
    }
  }
  PoolFreeEx(0,__s,s___Os_Lib_NearestKeyLut_h_c003539c,0x116);
  return;
}


//==== DcuSstlCal1SetLutRecord @ c002b50c

void DcuSstlCal1SetLutRecord(int param_1,int param_2,uint param_3,uint param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  
  if (param_1 == 1) {
    piVar5 = *(int **)(&DAT_c12e68b8 + param_2 * 4);
    iVar4 = 0;
    iVar3 = piVar5[1];
    if (iVar3 < 1) goto LAB_c002b644;
    iVar2 = *piVar5;
    iVar6 = iVar3;
    do {
      iVar1 = (iVar4 + iVar6) / 2;
      if (*(uint *)(iVar1 * 8 + iVar2) <= param_3) {
        iVar4 = iVar1 + 1;
        iVar1 = iVar6;
      }
      iVar6 = iVar1;
    } while (iVar4 < iVar1);
  }
  else {
    piVar5 = *(int **)(&DAT_c12e68a4 + param_2 * 4);
    iVar4 = 0;
    iVar3 = piVar5[1];
    if (iVar3 < 1) {
LAB_c002b644:
      iVar4 = 0;
      iVar2 = *piVar5;
      iVar6 = 0;
      goto LAB_c002b5c0;
    }
    iVar2 = *piVar5;
    iVar6 = iVar3;
    do {
      while (iVar1 = (iVar4 + iVar6) / 2, param_3 < *(uint *)(iVar1 * 8 + iVar2)) {
        iVar6 = iVar1;
        if (iVar1 <= iVar4) goto LAB_c002b5a8;
      }
      iVar4 = iVar1 + 1;
    } while (iVar4 < iVar6);
  }
LAB_c002b5a8:
  iVar6 = iVar4 * 8;
  if ((iVar4 != 0) && (*(uint *)(iVar6 + iVar2 + -8) == param_3)) {
    *(uint *)(iVar6 + iVar2 + -4) = param_4;
    return;
  }
LAB_c002b5c0:
  iVar3 = PoolReallocEx(0,iVar2,iVar3 * 8 + 8,0,s___Os_Lib_NearestKeyLut_h_c003539c,0x89);
  *piVar5 = iVar3;
  memmove((void *)(iVar3 + iVar6 + 8),(void *)(iVar3 + iVar6),(piVar5[1] - iVar4) * 8);
  iVar3 = *piVar5;
  piVar5[1] = piVar5[1] + 1;
  ((uint *)(iVar6 + iVar3))[1] = param_4;
  *(uint *)(iVar6 + iVar3) = param_3;
  return;
}


//==== DcuSstlCal1Init @ c002b6d8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DcuSstlCal1Init(char param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  code *pcVar7;
  
  puVar6 = (undefined4 *)&DAT_c12e68a4;
  puVar4 = (undefined4 *)&DAT_c12e68b8;
  iVar2 = 4;
  do {
    puVar1 = (undefined4 *)operator(8,s_Os_Dcu_SstlCal1_C14_cpp_c00353b8,0xcd);
    *puVar4 = puVar1;
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar1 = (undefined4 *)operator(8,s_Os_Dcu_SstlCal1_C14_cpp_c00353b8,0xce);
    iVar2 = iVar2 + -1;
    *puVar6 = puVar1;
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar4 = puVar4 + 1;
    puVar6 = puVar6 + 1;
  } while (-1 < iVar2);
  DcuSstlCal1SetCalibrationInterval(_s_sstlCal1DefaultConfig);
  iVar2 = 7;
  DcuSstlCal1SetContinuousCalibration(uRamc0035224);
  DcuSstlCal1SetCalibrationDuringRefresh(uRamc0035225);
  pcVar7 = s_sstlDefaultHighSideLut;
  do {
    uVar3 = *(undefined4 *)pcVar7;
    uVar5 = *(undefined4 *)(pcVar7 + 4);
    DcuSstlCal1SetLutRecord(1,0,uVar3,uVar5);
    DcuSstlCal1SetLutRecord(1,1,uVar3,uVar5);
    DcuSstlCal1SetLutRecord(1,2,uVar3,uVar5);
    DcuSstlCal1SetLutRecord(1,3,uVar3,uVar5);
    iVar2 = iVar2 + -1;
    DcuSstlCal1SetLutRecord(1,4,uVar3,uVar5);
    pcVar7 = pcVar7 + 8;
  } while (-1 < iVar2);
  pcVar7 = s_sstlDefaultLowSideLut;
  iVar2 = 7;
  do {
    uVar3 = *(undefined4 *)pcVar7;
    uVar5 = *(undefined4 *)(pcVar7 + 4);
    DcuSstlCal1SetLutRecord(0,0,uVar3,uVar5);
    DcuSstlCal1SetLutRecord(0,1,uVar3,uVar5);
    DcuSstlCal1SetLutRecord(0,2,uVar3,uVar5);
    DcuSstlCal1SetLutRecord(0,3,uVar3,uVar5);
    iVar2 = iVar2 + -1;
    DcuSstlCal1SetLutRecord(0,4,uVar3,uVar5);
    pcVar7 = pcVar7 + 8;
  } while (-1 < iVar2);
  _tx_event_flags_create(0xc12e6880,s_sstlCal1Event_c00353d0);
  OsThreadCreateEx(s_sstlCal1Calibration_c00353e0,DcuSstlCal1RecalibrateThread_unsigned,0,0x3f4,
                   0x3f4,1,1);
  if (param_1 == '\0') {
    _tx_timer_create(0xc12e68cc,s_SstlCal1Timer_c00353f4,DcuSstlCal1Recalibrate,0,param_2,param_2,0)
    ;
    return;
  }
  DcuSstlCal1Recalibrate(0);
  _tx_timer_create(0xc12e68cc,s_SstlCal1Timer_c00353f4,DcuSstlCal1Recalibrate,0,param_2,param_2,
                   param_1 != '\0');
  return;
}


//==== DcuSstlCal1LoadLutFromFile @ c002b984

void DcuSstlCal1LoadLutFromFile(int param_1,int param_2,undefined4 param_3)

{
  uint uVar1;
  uint uVar2;
  char *__s;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  uint local_38;
  uint local_34;
  uint local_30;
  uint local_2c;
  
  if (param_1 == 1) {
    piVar7 = *(int **)(&DAT_c12e68b8 + param_2 * 4);
    __s = (char *)PoolAllocEx(0,0x50,0,s___Os_Lib_NearestKeyLut_h_c003539c,0xe8);
    piVar7[1] = 0;
    PoolFreeEx(0,*piVar7,s___Os_Lib_NearestKeyLut_h_c003539c,0x70);
    *piVar7 = 0;
    while (iVar3 = func_0xc01d89b8(param_3,__s,0x50), iVar3 != -1) {
      sscanf(__s,s_0x_x__>_0x_x_c0035404,&local_38,&local_34);
      uVar2 = local_34;
      uVar1 = local_38;
      iVar3 = piVar7[1];
      iVar6 = 0;
      if (iVar3 < 1) {
        iVar5 = *piVar7;
        iVar8 = 0;
LAB_c002bc88:
        iVar3 = PoolReallocEx(0,iVar5,iVar3 * 8 + 8,0,s___Os_Lib_NearestKeyLut_h_c003539c,0x89);
        *piVar7 = iVar3;
        memmove((void *)(iVar3 + iVar8 + 8),(void *)(iVar3 + iVar8),(piVar7[1] - iVar6) * 8);
        iVar3 = *piVar7;
        piVar7[1] = piVar7[1] + 1;
        ((uint *)(iVar8 + iVar3))[1] = uVar2;
        *(uint *)(iVar8 + iVar3) = uVar1;
      }
      else {
        iVar5 = *piVar7;
        iVar8 = iVar3;
        do {
          while (iVar4 = (iVar6 + iVar8) / 2, local_38 < *(uint *)(iVar4 * 8 + iVar5)) {
            iVar8 = iVar4;
            if (iVar4 <= iVar6) goto LAB_c002bc70;
          }
          iVar6 = iVar4 + 1;
        } while (iVar6 < iVar8);
LAB_c002bc70:
        iVar8 = iVar6 * 8;
        if ((iVar6 == 0) || (*(uint *)(iVar8 + iVar5 + -8) != local_38)) goto LAB_c002bc88;
        *(uint *)(iVar8 + iVar5 + -4) = local_34;
      }
    }
LAB_c002bb3c:
    PoolFreeEx(0,__s,s___Os_Lib_NearestKeyLut_h_c003539c,0xfc);
    return;
  }
  piVar7 = *(int **)(&DAT_c12e68a4 + param_2 * 4);
  __s = (char *)PoolAllocEx(0,0x50,0,s___Os_Lib_NearestKeyLut_h_c003539c,0xe8);
  piVar7[1] = 0;
  PoolFreeEx(0,*piVar7,s___Os_Lib_NearestKeyLut_h_c003539c,0x70);
  *piVar7 = 0;
LAB_c002ba10:
  iVar3 = func_0xc01d89b8(param_3,__s,0x50);
  if (iVar3 != -1) {
    do {
      sscanf(__s,s_0x_x__>_0x_x_c0035404,&local_30,&local_2c);
      uVar2 = local_2c;
      uVar1 = local_30;
      iVar3 = piVar7[1];
      iVar6 = 0;
      if (iVar3 < 1) {
        iVar5 = *piVar7;
        iVar8 = 0;
      }
      else {
        iVar5 = *piVar7;
        iVar8 = iVar3;
        do {
          while (iVar4 = (iVar6 + iVar8) / 2, local_30 < *(uint *)(iVar4 * 8 + iVar5)) {
            iVar8 = iVar4;
            if (iVar4 <= iVar6) goto LAB_c002baa8;
          }
          iVar6 = iVar4 + 1;
        } while (iVar6 < iVar8);
LAB_c002baa8:
        iVar8 = iVar6 * 8;
        if ((iVar6 != 0) && (*(uint *)(iVar8 + iVar5 + -8) == local_30)) goto code_r0xc002bab8;
      }
      iVar3 = PoolReallocEx(0,iVar5,iVar3 * 8 + 8,0,s___Os_Lib_NearestKeyLut_h_c003539c,0x89);
      *piVar7 = iVar3;
      memmove((void *)(iVar3 + iVar8 + 8),(void *)(iVar3 + iVar8),(piVar7[1] - iVar6) * 8);
      iVar3 = *piVar7;
      piVar7[1] = piVar7[1] + 1;
      ((uint *)(iVar8 + iVar3))[1] = uVar2;
      *(uint *)(iVar8 + iVar3) = uVar1;
      iVar3 = func_0xc01d89b8(param_3,__s,0x50);
      if (iVar3 == -1) break;
    } while( true );
  }
  goto LAB_c002bb3c;
code_r0xc002bab8:
  *(uint *)(iVar8 + iVar5 + -4) = local_2c;
  goto LAB_c002ba10;
}


//==== DcuSstlCal1SetOutputBufferDriveStrength @ c002bcf4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DcuSstlCal1SetOutputBufferDriveStrength(uint *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  
  uVar8 = *param_1;
  iVar1 = _DAT_c12e68b8[1];
  uVar6 = param_1[1];
  if (iVar1 == 0) goto LAB_c002c58c;
  iVar5 = 0;
  if (iVar1 < 1) {
    iVar3 = *_DAT_c12e68b8;
LAB_c002bd98:
    uVar13 = *(uint *)(iVar3 + 4);
  }
  else {
    iVar3 = *_DAT_c12e68b8;
    do {
      while (iVar2 = (iVar5 + iVar1) / 2, uVar6 < *(uint *)(iVar2 * 8 + iVar3)) {
        iVar1 = iVar2;
        if (iVar2 <= iVar5) goto LAB_c002bd90;
      }
      iVar5 = iVar2 + 1;
    } while (iVar5 < iVar1);
LAB_c002bd90:
    if (iVar5 == 0) goto LAB_c002bd98;
    piVar4 = (int *)(iVar5 * 8 + iVar3);
    uVar13 = piVar4[-2] - uVar6;
    if ((piVar4[-2] == uVar6) ||
       (uVar17 = (int)uVar13 >> 0x1f, uVar18 = (int)(*piVar4 - uVar6) >> 0x1f,
       (int)((uVar17 ^ uVar13) - uVar17) < (int)((uVar18 ^ *piVar4 - uVar6) - uVar18))) {
      uVar13 = piVar4[-1];
    }
    else {
      uVar13 = piVar4[1];
    }
  }
  iVar1 = _DAT_c12e68bc[1];
  if (iVar1 == 0) goto LAB_c002c58c;
  iVar5 = 0;
  if (iVar1 < 1) {
    iVar3 = *_DAT_c12e68bc;
LAB_c002be0c:
    uVar18 = *(uint *)(iVar3 + 4);
  }
  else {
    iVar3 = *_DAT_c12e68bc;
    do {
      while (iVar2 = (iVar5 + iVar1) / 2, uVar6 < *(uint *)(iVar2 * 8 + iVar3)) {
        iVar1 = iVar2;
        if (iVar2 <= iVar5) goto LAB_c002be04;
      }
      iVar5 = iVar2 + 1;
    } while (iVar5 < iVar1);
LAB_c002be04:
    if (iVar5 == 0) goto LAB_c002be0c;
    piVar4 = (int *)(iVar5 * 8 + iVar3);
    uVar18 = piVar4[-2] - uVar6;
    if ((piVar4[-2] == uVar6) ||
       (uVar16 = (int)uVar18 >> 0x1f, uVar17 = (int)(*piVar4 - uVar6) >> 0x1f,
       (int)((uVar16 ^ uVar18) - uVar16) < (int)((uVar17 ^ *piVar4 - uVar6) - uVar17))) {
      uVar18 = piVar4[-1];
    }
    else {
      uVar18 = piVar4[1];
    }
  }
  iVar1 = _DAT_c12e68c0[1];
  if (iVar1 == 0) goto LAB_c002c58c;
  iVar5 = 0;
  if (iVar1 < 1) {
    iVar3 = *_DAT_c12e68c0;
LAB_c002be7c:
    uVar17 = *(uint *)(iVar3 + 4);
  }
  else {
    iVar3 = *_DAT_c12e68c0;
    do {
      while (iVar2 = (iVar5 + iVar1) / 2, uVar6 < *(uint *)(iVar2 * 8 + iVar3)) {
        iVar1 = iVar2;
        if (iVar2 <= iVar5) goto LAB_c002be74;
      }
      iVar5 = iVar2 + 1;
    } while (iVar5 < iVar1);
LAB_c002be74:
    if (iVar5 == 0) goto LAB_c002be7c;
    piVar4 = (int *)(iVar5 * 8 + iVar3);
    uVar17 = piVar4[-2] - uVar6;
    if ((piVar4[-2] == uVar6) ||
       (uVar12 = (int)uVar17 >> 0x1f, uVar16 = (int)(*piVar4 - uVar6) >> 0x1f,
       (int)((uVar12 ^ uVar17) - uVar12) < (int)((uVar16 ^ *piVar4 - uVar6) - uVar16))) {
      uVar17 = piVar4[-1];
    }
    else {
      uVar17 = piVar4[1];
    }
  }
  iVar1 = _DAT_c12e68c4[1];
  if (iVar1 == 0) goto LAB_c002c58c;
  iVar5 = 0;
  if (iVar1 < 1) {
    iVar3 = *_DAT_c12e68c4;
LAB_c002beec:
    uVar16 = *(uint *)(iVar3 + 4);
  }
  else {
    iVar3 = *_DAT_c12e68c4;
    do {
      while (iVar2 = (iVar5 + iVar1) / 2, uVar6 < *(uint *)(iVar2 * 8 + iVar3)) {
        iVar1 = iVar2;
        if (iVar2 <= iVar5) goto LAB_c002bee4;
      }
      iVar5 = iVar2 + 1;
    } while (iVar5 < iVar1);
LAB_c002bee4:
    if (iVar5 == 0) goto LAB_c002beec;
    piVar4 = (int *)(iVar5 * 8 + iVar3);
    uVar16 = piVar4[-2] - uVar6;
    if ((piVar4[-2] == uVar6) ||
       (uVar11 = (int)uVar16 >> 0x1f, uVar12 = (int)(*piVar4 - uVar6) >> 0x1f,
       (int)((uVar11 ^ uVar16) - uVar11) < (int)((uVar12 ^ *piVar4 - uVar6) - uVar12))) {
      uVar16 = piVar4[-1];
    }
    else {
      uVar16 = piVar4[1];
    }
  }
  iVar1 = _DAT_c12e68c8[1];
  if (iVar1 == 0) goto LAB_c002c58c;
  iVar5 = 0;
  if (iVar1 < 1) {
    iVar3 = *_DAT_c12e68c8;
LAB_c002bf5c:
    uVar6 = *(uint *)(iVar3 + 4);
  }
  else {
    iVar3 = *_DAT_c12e68c8;
    do {
      while (iVar2 = (iVar5 + iVar1) / 2, uVar6 < *(uint *)(iVar2 * 8 + iVar3)) {
        iVar1 = iVar2;
        if (iVar2 <= iVar5) goto LAB_c002bf54;
      }
      iVar5 = iVar2 + 1;
    } while (iVar5 < iVar1);
LAB_c002bf54:
    if (iVar5 == 0) goto LAB_c002bf5c;
    piVar4 = (int *)(iVar5 * 8 + iVar3);
    uVar12 = piVar4[-2] - uVar6;
    if ((piVar4[-2] == uVar6) ||
       (uVar11 = (int)uVar12 >> 0x1f, uVar10 = (int)(*piVar4 - uVar6) >> 0x1f,
       (int)((uVar11 ^ uVar12) - uVar11) < (int)((uVar10 ^ *piVar4 - uVar6) - uVar10))) {
      uVar6 = piVar4[-1];
    }
    else {
      uVar6 = piVar4[1];
    }
  }
  iVar1 = _DAT_c12e68a4[1];
  if (iVar1 == 0) goto LAB_c002c58c;
  iVar5 = 0;
  if (iVar1 < 1) {
    iVar3 = *_DAT_c12e68a4;
LAB_c002bfd4:
    uVar12 = *(uint *)(iVar3 + 4);
  }
  else {
    iVar3 = *_DAT_c12e68a4;
    do {
      while (iVar2 = (iVar5 + iVar1) / 2, uVar8 < *(uint *)(iVar2 * 8 + iVar3)) {
        iVar1 = iVar2;
        if (iVar2 <= iVar5) goto LAB_c002bfcc;
      }
      iVar5 = iVar2 + 1;
    } while (iVar5 < iVar1);
LAB_c002bfcc:
    if (iVar5 == 0) goto LAB_c002bfd4;
    piVar4 = (int *)(iVar5 * 8 + iVar3);
    uVar12 = piVar4[-2] - uVar8;
    if ((piVar4[-2] == uVar8) ||
       (uVar10 = (int)uVar12 >> 0x1f, uVar11 = (int)(*piVar4 - uVar8) >> 0x1f,
       (int)((uVar10 ^ uVar12) - uVar10) < (int)((uVar11 ^ *piVar4 - uVar8) - uVar11))) {
      uVar12 = piVar4[-1];
    }
    else {
      uVar12 = piVar4[1];
    }
  }
  iVar1 = _DAT_c12e68a8[1];
  if (iVar1 == 0) goto LAB_c002c58c;
  iVar5 = 0;
  if (iVar1 < 1) {
    iVar3 = *_DAT_c12e68a8;
LAB_c002c048:
    uVar11 = *(uint *)(iVar3 + 4);
  }
  else {
    iVar3 = *_DAT_c12e68a8;
    do {
      while (iVar2 = (iVar5 + iVar1) / 2, uVar8 < *(uint *)(iVar2 * 8 + iVar3)) {
        iVar1 = iVar2;
        if (iVar2 <= iVar5) goto LAB_c002c040;
      }
      iVar5 = iVar2 + 1;
    } while (iVar5 < iVar1);
LAB_c002c040:
    if (iVar5 == 0) goto LAB_c002c048;
    piVar4 = (int *)(iVar5 * 8 + iVar3);
    uVar11 = piVar4[-2] - uVar8;
    if ((piVar4[-2] == uVar8) ||
       (uVar7 = (int)uVar11 >> 0x1f, uVar10 = (int)(*piVar4 - uVar8) >> 0x1f,
       (int)((uVar7 ^ uVar11) - uVar7) < (int)((uVar10 ^ *piVar4 - uVar8) - uVar10))) {
      uVar11 = piVar4[-1];
    }
    else {
      uVar11 = piVar4[1];
    }
  }
  iVar1 = _DAT_c12e68ac[1];
  if (iVar1 == 0) goto LAB_c002c58c;
  iVar5 = 0;
  if (iVar1 < 1) {
    iVar3 = *_DAT_c12e68ac;
LAB_c002c0b8:
    uVar10 = *(uint *)(iVar3 + 4);
  }
  else {
    iVar3 = *_DAT_c12e68ac;
    do {
      while (iVar2 = (iVar5 + iVar1) / 2, uVar8 < *(uint *)(iVar2 * 8 + iVar3)) {
        iVar1 = iVar2;
        if (iVar2 <= iVar5) goto LAB_c002c0b0;
      }
      iVar5 = iVar2 + 1;
    } while (iVar5 < iVar1);
LAB_c002c0b0:
    if (iVar5 == 0) goto LAB_c002c0b8;
    piVar4 = (int *)(iVar5 * 8 + iVar3);
    uVar10 = piVar4[-2] - uVar8;
    if ((piVar4[-2] == uVar8) ||
       (uVar15 = (int)uVar10 >> 0x1f, uVar7 = (int)(*piVar4 - uVar8) >> 0x1f,
       (int)((uVar15 ^ uVar10) - uVar15) < (int)((uVar7 ^ *piVar4 - uVar8) - uVar7))) {
      uVar10 = piVar4[-1];
    }
    else {
      uVar10 = piVar4[1];
    }
  }
  iVar1 = _DAT_c12e68b0[1];
  if (iVar1 == 0) goto LAB_c002c58c;
  iVar5 = 0;
  if (iVar1 < 1) {
    iVar3 = *_DAT_c12e68b0;
LAB_c002c128:
    uVar7 = *(uint *)(iVar3 + 4);
  }
  else {
    iVar3 = *_DAT_c12e68b0;
    do {
      while (iVar2 = (iVar5 + iVar1) / 2, uVar8 < *(uint *)(iVar2 * 8 + iVar3)) {
        iVar1 = iVar2;
        if (iVar2 <= iVar5) goto LAB_c002c120;
      }
      iVar5 = iVar2 + 1;
    } while (iVar5 < iVar1);
LAB_c002c120:
    if (iVar5 == 0) goto LAB_c002c128;
    piVar4 = (int *)(iVar5 * 8 + iVar3);
    uVar7 = piVar4[-2] - uVar8;
    if ((piVar4[-2] == uVar8) ||
       (uVar15 = (int)uVar7 >> 0x1f, uVar9 = (int)(*piVar4 - uVar8) >> 0x1f,
       (int)((uVar15 ^ uVar7) - uVar15) < (int)((uVar9 ^ *piVar4 - uVar8) - uVar9))) {
      uVar7 = piVar4[-1];
    }
    else {
      uVar7 = piVar4[1];
    }
  }
  iVar1 = _DAT_c12e68b4[1];
  if (iVar1 == 0) {
LAB_c002c58c:
                    /* WARNING: Subroutine does not return */
    OsSysHaltEx(s_Internal_map_empty_c0035414,s___Os_Lib_NearestKeyLut_h_c003539c,0xb3);
  }
  iVar5 = 0;
  if (iVar1 < 1) {
    iVar3 = *_DAT_c12e68b4;
  }
  else {
    iVar3 = *_DAT_c12e68b4;
    do {
      while (iVar2 = (iVar5 + iVar1) / 2, uVar8 < *(uint *)(iVar2 * 8 + iVar3)) {
        iVar1 = iVar2;
        if (iVar2 <= iVar5) goto LAB_c002c190;
      }
      iVar5 = iVar2 + 1;
    } while (iVar5 < iVar1);
LAB_c002c190:
    if (iVar5 != 0) {
      piVar4 = (int *)(iVar5 * 8 + iVar3);
      uVar15 = piVar4[-2] - uVar8;
      if ((piVar4[-2] == uVar8) ||
         (uVar14 = (int)(*piVar4 - uVar8) >> 0x1f, uVar9 = (int)uVar15 >> 0x1f,
         (int)((uVar9 ^ uVar15) - uVar9) < (int)((uVar14 ^ *piVar4 - uVar8) - uVar14))) {
        uVar8 = piVar4[-1];
      }
      else {
        uVar8 = piVar4[1];
      }
      goto LAB_c002c19c;
    }
  }
  uVar8 = *(uint *)(iVar3 + 4);
LAB_c002c19c:
  SstlCalWriteReg_unsigned
            (0xb8003ea8,
             (uVar12 & 0x3f) << 0x18 | (uVar11 & 0x3f) << 0x12 | (uVar10 & 0x3f) << 0xc |
             (uVar7 & 0x3f) << 6 | uVar8 & 0x3f,s_MIF_SSTL_NSEL_PARAM_c0035358,
             DcuSstlCal1SetOutputBufferDriveStrength____func__,0x1d8);
  SstlCalWriteReg_unsigned
            (0xb8003ea4,
             (uVar13 & 0x3f) << 0x18 | (uVar18 & 0x3f) << 0x12 | (uVar17 & 0x3f) << 0xc |
             (uVar16 & 0x3f) << 6 | uVar6 & 0x3f,s_MIF_SSTL_PSEL_PARAM_c0035344,
             DcuSstlCal1SetOutputBufferDriveStrength____func__,0x1e1);
  return;
}


//==== DcuSstlCal1RecalibrateThread(unsigned @ c002c5a0

void DcuSstlCal1RecalibrateThread_unsigned(void)

{
  undefined1 auStack_20 [8];
  undefined1 auStack_18 [8];
  
  do {
    _tx_event_flags_get(0xc12e6880,1,1,auStack_18,0xffffffff);
    DcuSstlCal1GetResults(auStack_20);
    DcuSstlCal1SetOutputBufferDriveStrength(auStack_20);
    DcuSstlCal1SetSstlValid();
  } while( true );
}


//==== DcuMemoryProtectionInit @ c002c5f0

void DcuMemoryProtectionInit(void)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  iVar1 = 0;
  do {
    Os_IsrInstall(iVar1 + -0x3ffcd198,*(undefined4 *)((int)&DAT_c0032ea0 + iVar1),
                  MemoryProtectionIsr_unsigned,uVar2,2,0);
    uVar2 = uVar2 + 1;
    Os_MutexCreate(&DAT_c0032ec0 + iVar1,
                   *(undefined4 *)((int)&PTR_s_MemoryProtectionMutex1_c0032ef0 + iVar1));
    iVar1 = iVar1 + 0xd0;
  } while (uVar2 < 2);
  return;
}


//==== DcuMemoryProtectionEnableHw(EMemoryProtectionUnit, @ c002c6ac

undefined4 DcuMemoryProtectionEnableHw_EMemoryProtectionUnit_(int param_1,uint param_2)

{
  int iVar1;
  undefined *puVar2;
  
  iVar1 = param_1 * 0xd0;
  puVar2 = &DAT_c0032ec0 + iVar1;
  if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
    __assfail(s_Failed_assertion___s__at_line__d_c003554c,
              s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,0x16f,
              s_Os_Dcu_MemoryProtection_cpp_c00355a0);
  }
  Os_MutexTake(puVar2,0xffffffff);
  if (*(int *)(&DAT_c0032ea8 + iVar1) == 0) {
    **(uint **)(&DAT_c0032ef4 + iVar1) = param_2 & 0xff;
    Os_MutexGive(puVar2);
    return 1;
  }
  Os_CacheWriteBackInvalidateDataCacheRegion
            (*(undefined4 *)(&DAT_c0032ea4 + iVar1),*(int *)(&DAT_c0032ea8 + iVar1));
  **(uint **)(&DAT_c0032ef4 + iVar1) = param_2 & 0xff;
  Os_MutexGive(puVar2);
  return 1;
}


//==== DcuMemoryProtectionConfigHw(EMemoryProtectionUnit, @ c002c788

void DcuMemoryProtectionConfigHw_EMemoryProtectionUnit_
               (int param_1,uint param_2,int param_3,uint param_4,uint param_5,uint param_6,
               byte param_7,undefined4 param_8)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  uint *puVar4;
  undefined4 *puVar5;
  undefined *puVar6;
  undefined4 *puVar7;
  
  iVar2 = param_1 * 0xd0;
  if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
    __assfail(s_Failed_assertion___s__at_line__d_c003554c,
              s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,0x1bd,
              s_Os_Dcu_MemoryProtection_cpp_c00355a0);
  }
  puVar6 = &DAT_c0032ec0 + iVar2;
  Os_MutexTake(puVar6,0xffffffff);
  iVar3 = **(int **)(&DAT_c0032ef4 + iVar2);
  if (iVar3 != 0) {
    DcuMemoryProtectionEnable(param_1,0);
  }
  puVar4 = *(uint **)(&DAT_c0032f00 + iVar2);
  **(uint **)(&DAT_c0032efc + iVar2) = (param_2 & 0x1fffffff) >> 2;
  *puVar4 = ((param_2 + param_3) - 1 & 0x1fffffff) >> 2;
  puVar5 = *(undefined4 **)(&DAT_c0032f04 + iVar2);
  if (param_4 < 0xeb) {
    uVar1 = ConvertClientIdToHwId(param_4);
    *puVar5 = uVar1;
    puVar5 = *(undefined4 **)(&DAT_c0032f08 + iVar2);
  }
  else {
    puVar5 = *(undefined4 **)(&DAT_c0032f08 + iVar2);
  }
  if (param_5 < 0xeb) {
    uVar1 = ConvertClientIdToHwId(param_5);
    *puVar5 = uVar1;
    puVar5 = *(undefined4 **)(&DAT_c0032f0c + iVar2);
  }
  else {
    puVar5 = *(undefined4 **)(&DAT_c0032f0c + iVar2);
  }
  if (param_6 < 0xeb) {
    uVar1 = ConvertClientIdToHwId(param_6);
    puVar7 = *(undefined4 **)(&DAT_c0032f20 + iVar2);
    puVar4 = *(uint **)(&DAT_c0032f1c + iVar2);
    *puVar5 = uVar1;
    *puVar4 = param_7 ^ 1;
    *puVar7 = param_8;
  }
  else {
    puVar5 = *(undefined4 **)(&DAT_c0032f20 + iVar2);
    **(uint **)(&DAT_c0032f1c + iVar2) = param_7 ^ 1;
    *puVar5 = param_8;
  }
  if (iVar3 == 0) {
    Os_MutexGive(puVar6);
    return;
  }
  DcuMemoryProtectionEnable(param_1,1);
  Os_MutexGive(puVar6);
  return;
}


//==== DcuNullProtectionDeactivate(EMemoryProtectionUnit) @ c002c9a4

void DcuNullProtectionDeactivate_EMemoryProtectionUnit_(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * 0xd0;
  if (param_1 < 2) {
    Os_MutexTake(&DAT_c0032ec0 + iVar1,0xffffffff);
    *(undefined4 *)(s_memoryProtectionUnits + iVar1) = *(undefined4 *)(&DAT_c0032f2c + iVar1);
    DcuMemoryProtectionConfigHw_EMemoryProtectionUnit_
              (param_1,*(undefined4 *)(&DAT_c0032ea4 + iVar1),*(undefined4 *)(&DAT_c0032ea8 + iVar1)
               ,*(undefined4 *)(&DAT_c0032eac + iVar1),*(undefined4 *)(&DAT_c0032eb0 + iVar1),
               *(undefined4 *)(&DAT_c0032eb4 + iVar1),(&DAT_c0032eb8)[iVar1],
               *(undefined4 *)(&DAT_c0032ebc + iVar1));
    (&DAT_c0032f25)[iVar1] = 0;
    Os_MutexGive(&DAT_c0032ec0 + iVar1);
    return;
  }
                    /* WARNING: Subroutine does not return */
  __assfail(s_Failed_assertion___s__at_line__d_c003554c,s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,
            0x208,s_Os_Dcu_MemoryProtection_cpp_c00355a0);
}


//==== DcuNullProtectionActivate(EMemoryProtectionUnit) @ c002ca68

void DcuNullProtectionActivate_EMemoryProtectionUnit_(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * 0xd0;
  if (param_1 < 2) {
    Os_MutexTake(&DAT_c0032ec0 + iVar1,0xffffffff);
    *(undefined4 *)(&DAT_c0032f2c + iVar1) = *(undefined4 *)(s_memoryProtectionUnits + iVar1);
    *(undefined4 *)(s_memoryProtectionUnits + iVar1) = 0;
    DcuMemoryProtectionConfigHw_EMemoryProtectionUnit_
              (param_1,0,0x180,0,0,0,0,*(undefined4 *)(&DAT_c0032f28 + iVar1));
    (&DAT_c0032f25)[iVar1] = 1;
    Os_MutexGive(&DAT_c0032ec0 + iVar1);
    return;
  }
                    /* WARNING: Subroutine does not return */
  __assfail(s_Failed_assertion___s__at_line__d_c003554c,s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,
            0x1eb,s_Os_Dcu_MemoryProtection_cpp_c00355a0);
}


//==== DcuMemoryProtectionEnable @ c002cb28

undefined4 DcuMemoryProtectionEnable(int param_1,char param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined *puVar3;
  
  iVar2 = param_1 * 0xd0;
  puVar3 = &DAT_c0032ec0 + iVar2;
  if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
    __assfail(s_Failed_assertion___s__at_line__d_c003554c,
              s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,0x140,
              s_Os_Dcu_MemoryProtection_cpp_c00355a0);
  }
  Os_MutexTake(puVar3,0xffffffff);
  if (((param_2 == '\0') && ((&DAT_c0032f24)[iVar2] == '\x01')) && ((&DAT_c0032f25)[iVar2] == '\0'))
  {
    DcuMemoryProtectionEnableHw_EMemoryProtectionUnit_(param_1,0);
    DcuNullProtectionActivate_EMemoryProtectionUnit_(param_1);
  }
  else {
    if ((param_2 != '\x01') || ((&DAT_c0032f25)[iVar2] != '\x01')) {
      uVar1 = DcuMemoryProtectionEnableHw_EMemoryProtectionUnit_(param_1);
      Os_MutexGive(puVar3);
      return uVar1;
    }
    Os_CacheWriteBackInvalidateDataCache(param_1,1);
    DcuMemoryProtectionEnableHw_EMemoryProtectionUnit_(param_1,0);
    DcuNullProtectionDeactivate_EMemoryProtectionUnit_(param_1);
  }
  uVar1 = DcuMemoryProtectionEnableHw_EMemoryProtectionUnit_(param_1,1);
  Os_MutexGive(puVar3);
  return uVar1;
}


//==== DcuMemoryProtectionConfig @ c002cc80

void DcuMemoryProtectionConfig
               (int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5,undefined4 param_6,undefined1 param_7,undefined4 param_8)

{
  undefined *puVar1;
  int iVar2;
  
  iVar2 = param_1 * 0xd0;
  puVar1 = &DAT_c0032ec0 + iVar2;
  if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
    __assfail(s_Failed_assertion___s__at_line__d_c003554c,
              s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,0x191,
              s_Os_Dcu_MemoryProtection_cpp_c00355a0);
  }
  Os_MutexTake(puVar1,0xffffffff);
  *(undefined4 *)(&DAT_c0032ea4 + iVar2) = param_2;
  *(undefined4 *)(&DAT_c0032ea8 + iVar2) = param_3;
  *(undefined4 *)(&DAT_c0032eac + iVar2) = param_4;
  *(undefined4 *)(&DAT_c0032eb0 + iVar2) = param_5;
  *(undefined4 *)(&DAT_c0032eb4 + iVar2) = param_6;
  (&DAT_c0032eb8)[iVar2] = param_7;
  *(undefined4 *)(&DAT_c0032ebc + iVar2) = param_8;
  if ((&DAT_c0032f25)[iVar2] != '\0') {
    Os_MutexGive(puVar1);
    return;
  }
  DcuMemoryProtectionConfigHw_EMemoryProtectionUnit_
            (param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8);
  Os_MutexGive(puVar1);
  return;
}


//==== DcuMemoryProtectionIsEnabled @ c002cdd0

bool DcuMemoryProtectionIsEnabled(int param_1)

{
  int iVar1;
  int *piVar2;
  undefined *puVar3;
  
  iVar1 = param_1 * 0xd0;
  puVar3 = &DAT_c0032ec0 + iVar1;
  if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
    __assfail(s_Failed_assertion___s__at_line__d_c003554c,
              s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,0x224,
              s_Os_Dcu_MemoryProtection_cpp_c00355a0);
  }
  Os_MutexTake(puVar3,0xffffffff);
  if ((&DAT_c0032f24)[iVar1] == '\x01') {
    if ((&DAT_c0032f25)[iVar1] == '\x01') {
      Os_MutexGive(puVar3);
      return false;
    }
    piVar2 = *(int **)(&DAT_c0032ef4 + iVar1);
  }
  else {
    piVar2 = *(int **)(&DAT_c0032ef4 + iVar1);
  }
  iVar1 = *piVar2;
  Os_MutexGive(puVar3);
  return iVar1 != 0;
}


//==== DcuMemoryProtectionSetHandler @ c002ce84

undefined4 DcuMemoryProtectionSetHandler(int param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = param_1 * 0xd0;
  if (param_1 < 2) {
    Os_MutexTake(&DAT_c0032ec0 + iVar1,0xffffffff);
    uVar2 = *(undefined4 *)(s_memoryProtectionUnits + iVar1);
    *(undefined4 *)(&DAT_c0032f2c + iVar1) = param_2;
    *(undefined4 *)(s_memoryProtectionUnits + iVar1) = param_2;
    Os_MutexGive(&DAT_c0032ec0 + iVar1);
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __assfail(s_Failed_assertion___s__at_line__d_c003554c,s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,
            0x23f,s_Os_Dcu_MemoryProtection_cpp_c00355a0);
}


//==== DcuNullProtectionExec(EMemoryProtectionUnit, @ c002d090

undefined4 DcuNullProtectionExec_EMemoryProtectionUnit_(int param_1,char param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = param_1 * 0xd0;
  if (param_1 < 2) {
    Os_MutexTake(&DAT_c0032ec0 + iVar2,0xffffffff);
    if ((param_2 == '\0') && ((&DAT_c0032f25)[iVar2] == '\x01')) {
      Os_CacheWriteBackInvalidateDataCache();
      DcuMemoryProtectionEnableHw_EMemoryProtectionUnit_(param_1,0);
      DcuNullProtectionDeactivate_EMemoryProtectionUnit_(param_1);
      (&DAT_c0032f24)[iVar2] = 0;
    }
    else if (((param_2 == '\x01') && ((&DAT_c0032f25)[iVar2] == '\0')) &&
            (iVar1 = DcuMemoryProtectionIsEnabled(param_1), iVar1 == 0)) {
      DcuNullProtectionActivate_EMemoryProtectionUnit_(param_1);
      DcuMemoryProtectionEnableHw_EMemoryProtectionUnit_(param_1,1);
      (&DAT_c0032f24)[iVar2] = 1;
    }
    else {
      (&DAT_c0032f24)[iVar2] = param_2;
    }
    Os_MutexGive(&DAT_c0032ec0 + iVar2);
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  __assfail(s_Failed_assertion___s__at_line__d_c003554c,s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,
            0x2aa,s_Os_Dcu_MemoryProtection_cpp_c00355a0);
}


//==== DcuNullProtectionIsEnabled @ c002d1a4

undefined1 DcuNullProtectionIsEnabled(int param_1)

{
  undefined1 uVar1;
  
  if (param_1 < 2) {
    Os_MutexTake(&DAT_c0032ec0 + param_1 * 0xd0,0xffffffff);
    uVar1 = (&DAT_c0032f24)[param_1 * 0xd0];
    Os_MutexGive(&DAT_c0032ec0 + param_1 * 0xd0);
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  __assfail(s_Failed_assertion___s__at_line__d_c003554c,s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,
            0x2cb,s_Os_Dcu_MemoryProtection_cpp_c00355a0);
}


//==== DcuNullProtectionIsActive @ c002d228

undefined1 DcuNullProtectionIsActive(int param_1)

{
  undefined1 uVar1;
  
  if (param_1 < 2) {
    Os_MutexTake(&DAT_c0032ec0 + param_1 * 0xd0,0xffffffff);
    uVar1 = (&DAT_c0032f25)[param_1 * 0xd0];
    Os_MutexGive(&DAT_c0032ec0 + param_1 * 0xd0);
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  __assfail(s_Failed_assertion___s__at_line__d_c003554c,s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,
            0x2db,s_Os_Dcu_MemoryProtection_cpp_c00355a0);
}


//==== DcuNullProtectionConfig @ c002d2ac

void DcuNullProtectionConfig(int param_1,undefined4 param_2)

{
  if (param_1 < 2) {
    Os_MutexTake(&DAT_c0032ec0 + param_1 * 0xd0,0xffffffff);
    *(undefined4 *)(&DAT_c0032f28 + param_1 * 0xd0) = param_2;
    Os_MutexGive(&DAT_c0032ec0 + param_1 * 0xd0);
    return;
  }
                    /* WARNING: Subroutine does not return */
  __assfail(s_Failed_assertion___s__at_line__d_c003554c,s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,
            0x2ec,s_Os_Dcu_MemoryProtection_cpp_c00355a0);
}


//==== DcuNullProtectionType @ c002d334

undefined4 DcuNullProtectionType(int param_1)

{
  undefined4 uVar1;
  
  if (param_1 < 2) {
    Os_MutexTake(&DAT_c0032ec0 + param_1 * 0xd0,0xffffffff);
    uVar1 = *(undefined4 *)(&DAT_c0032f28 + param_1 * 0xd0);
    Os_MutexGive(&DAT_c0032ec0 + param_1 * 0xd0);
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  __assfail(s_Failed_assertion___s__at_line__d_c003554c,s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,
            0x2f9,s_Os_Dcu_MemoryProtection_cpp_c00355a0);
}


//==== DcuNullProtectionPause @ c002d3b8

void DcuNullProtectionPause(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * 0xd0;
  if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
    __assfail(s_Failed_assertion___s__at_line__d_c003554c,
              s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,0x309,
              s_Os_Dcu_MemoryProtection_cpp_c00355a0);
  }
  Os_MutexTake(&DAT_c0032ec0 + iVar1,0xffffffff);
  (&DAT_c0032f30)[iVar1] = (&DAT_c0032f25)[iVar1];
  if ((&DAT_c0032f25)[iVar1] != '\x01') {
    return;
  }
  DcuNullProtectionExec_EMemoryProtectionUnit_(param_1,0);
  return;
}


//==== DcuNullProtectionResume @ c002d458

void DcuNullProtectionResume(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * 0xd0;
  if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
    __assfail(s_Failed_assertion___s__at_line__d_c003554c,
              s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,0x31a,
              s_Os_Dcu_MemoryProtection_cpp_c00355a0);
  }
  if ((&DAT_c0032f30)[iVar1] != '\x01') {
    Os_MutexGive(&DAT_c0032ec0 + iVar1);
    return;
  }
  DcuNullProtectionExec_EMemoryProtectionUnit_(param_1,1);
  Os_MutexGive(&DAT_c0032ec0 + iVar1);
  return;
}


//==== DcuNullProtectionEnable @ c002d4e0

undefined4 DcuNullProtectionEnable(int param_1,undefined1 param_2)

{
  if (param_1 < 2) {
    Os_MutexTake(&DAT_c0032ec0 + param_1 * 0xd0,0xffffffff);
    DcuNullProtectionExec_EMemoryProtectionUnit_(param_1,param_2);
    Os_MutexGive(&DAT_c0032ec0 + param_1 * 0xd0);
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  __assfail(s_Failed_assertion___s__at_line__d_c003554c,s_unitId_<_MEMORY_PROTECTION_UNIT__c0035578,
            0x32a,s_Os_Dcu_MemoryProtection_cpp_c00355a0);
}


//==== DcuSlotMachineInit() @ c002d57c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DcuSlotMachineInit__(void)

{
  code *pcVar1;
  int iVar2;
  code *pcVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 *puVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  code *pcVar11;
  
  pcVar11 = s_slotMacEnabled;
  iVar9 = 0;
  iVar10 = 0;
  do {
    iVar7 = iVar9 * 0x50;
    *pcVar11 = (code)0x0;
    uVar8 = 0;
    puVar6 = (undefined4 *)(iVar10 + -0x3ed197d0);
    puVar4 = (undefined4 *)(iVar10 + -0x3ed197ac);
    pcVar3 = s_sodClientMask + iVar10;
    do {
      pcVar1 = s_slotmachineData + iVar7;
      iVar2 = 1;
      do {
        iVar2 = iVar2 + -1;
        *(undefined4 *)(pcVar1 + 8) = 0xffffffff;
        pcVar1 = pcVar1 + 0x28;
      } while (-1 < iVar2);
      uVar8 = uVar8 + 1;
      *(undefined4 *)pcVar3 = 0;
      *puVar4 = 0;
      *puVar6 = 0xffffffff;
      iVar7 = iVar7 + 4;
      pcVar3 = pcVar3 + 4;
      puVar4 = puVar4 + 1;
      puVar6 = puVar6 + 1;
    } while (uVar8 < 8);
    iVar7 = 0;
    if (_g_sodClientsIds != 0xffff) {
      pcVar3 = g_sodClientsIds;
      iVar2 = 0;
      do {
        pcVar3 = pcVar3 + 4;
        iVar5 = *(int *)pcVar3;
        iVar7 = iVar7 + 1;
        *(uint *)(s_sodClientMask + (iVar9 * 8 + (*(uint *)(g_sodClientsIds + iVar2) >> 5)) * 4) =
             *(uint *)(s_sodClientMask + (iVar9 * 8 + (*(uint *)(g_sodClientsIds + iVar2) >> 5)) * 4
                      ) | 1 << (*(uint *)(g_sodClientsIds + iVar2) & 0x1f);
        iVar2 = iVar7 * 4;
      } while (iVar5 != 0xffff);
    }
    iVar9 = iVar9 + 1;
    iVar10 = iVar10 + 0x20;
    pcVar11 = pcVar11 + 1;
  } while (iVar9 == 0);
  _DAT_b8030460 = 1;
  uRamc0033258 = 0;
  uRamc003324c = 0;
  uRamc0033250 = 0;
  uRamc0033254 = 0;
  return;
}


//==== DcuSlotMachinePages @ c002d6d0

undefined4 DcuSlotMachinePages(void)

{
  return 2;
}


//==== DcuSlotMachineActivePage @ c002d6d8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 DcuSlotMachineActivePage(int param_1)

{
  undefined4 uVar1;
  
  if (0 < param_1) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Slotmac_cpp_c0035620,0x143,s_Invalid_DRAM_selection__c0035660);
  }
  uVar1 = 0;
  if ((_DAT_b8030450 != 0) && (uVar1 = 1, _DAT_b8030450 != 1)) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Slotmac_cpp_c0035620,0x15a,
                        s_Failed_to_get_active_slot_machin_c0035634);
  }
  return uVar1;
}


//==== DcuSlotMachineGetDummy @ c002d744

/* WARNING: Removing unreachable block (ram,0xc002d72c) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 DcuSlotMachineGetDummy(void)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if ((_DAT_b8030450 != 0) && (uVar1 = 1, _DAT_b8030450 != 1)) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Slotmac_cpp_c0035620,0x15a,
                        s_Failed_to_get_active_slot_machin_c0035634);
  }
  return uVar1;
}


//==== DcuSlotMachineLoadToPageEx(SCbcConfigChunk*, @ c002d74c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
DcuSlotMachineLoadToPageEx_SCbcConfigChunk__
          (int *param_1,int param_2,uint *param_3,ushort *param_4,undefined4 param_5,
          undefined4 param_6,uint param_7)

{
  ushort uVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  code *pcVar5;
  int iVar6;
  code *pcVar7;
  undefined4 *puVar8;
  uint uVar9;
  uint *puVar10;
  uint uVar11;
  uint *local_48;
  uint *local_44;
  int local_38;
  
  if (0 < param_2) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Slotmac_cpp_c0035620,0x1e9,s_Invalid_DRAM_selector__c0035838);
  }
  if (1 < param_7) {
    Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),s_Invalid_page_index___d___c0035808,
              param_7);
    return 0xffffffff;
  }
  if (param_3 == (uint *)0x0) {
    Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),s_NULL_slot_machine__c0035824);
    return 0xffffffff;
  }
  uVar2 = Os_InterruptsDisable();
  if (param_7 == 1) {
    local_44 = (uint *)0xb8030458;
    local_48 = (uint *)0xb803044c;
    if (param_1 == (int *)0x0) goto LAB_c002dd1c;
LAB_c002d7d8:
    iVar6 = *param_1;
    puVar8 = (undefined4 *)(param_1[2] * 4 + iVar6);
    param_1[2] = param_1[2] + 2;
    puVar8[1] = param_5;
    *puVar8 = &DAT_b8030408;
    puVar8 = (undefined4 *)(param_1[2] * 4 + iVar6);
    param_1[2] = param_1[2] + 2;
    puVar8[1] = param_6;
    *puVar8 = &DAT_b8030404;
    puVar8 = (undefined4 *)(param_1[2] * 4 + iVar6);
    param_1[2] = param_1[2] + 2;
    puVar8[1] = 1;
    *puVar8 = &DAT_b8030444;
  }
  else {
    local_44 = (uint *)&DAT_b8030454;
    local_48 = (uint *)&DAT_b8030448;
    if (param_1 != (int *)0x0) goto LAB_c002d7d8;
LAB_c002dd1c:
    iVar6 = param_7 * 0x28 + param_2 * 0x50;
    *(undefined4 *)(iVar6 + -0x3ffccff4) = param_6;
    _DAT_b8030444 = 1;
    *(undefined4 *)(s_slotmachineData + iVar6) = param_5;
  }
  local_38 = param_7 * 5;
  iVar6 = param_2 * 0x50 + param_7 * 0x28;
  *(undefined4 *)(iVar6 + -0x3ffccfd4) = 0;
  *(undefined4 *)(iVar6 + -0x3ffccff0) = 0;
  *(undefined4 *)(iVar6 + -0x3ffccfec) = 0;
  *(undefined4 *)(iVar6 + -0x3ffccfe8) = 0;
  *(undefined4 *)(iVar6 + -0x3ffccfe4) = 0;
  *(undefined4 *)(iVar6 + -0x3ffccfe0) = 0;
  *(undefined4 *)(iVar6 + -0x3ffccfdc) = 0;
  *(undefined4 *)(iVar6 + -0x3ffccfd8) = 0;
  uVar1 = (ushort)*param_3;
  pcVar7 = s_dummySlotAdditional + (param_2 * 2 + param_7) * 4;
  *(undefined4 *)pcVar7 = 0;
  if (uVar1 != 0xffff && uVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Slotmac_cpp_c0035620,0x243,
                        s_First_slot_in_slot_machine_must_b_c0035850);
  }
  uVar11 = 0;
  if (uVar1 != 0xffff) {
    uVar1 = (ushort)*param_3;
    puVar10 = param_3;
    do {
      StrongAssertMsgWithContext
                ((*(uint *)(s_sodClientMask + (param_2 * 8 + (uint)(uVar1 >> 5)) * 4) &
                 1 << (uVar1 & 0x1f)) == 0,PTR_s_IsSodClient__dramSel__currentSlo_c0035b30,
                 s_Os_Dcu_Slotmac_cpp_c0035620,0x24e,s_Client__u_is_a_SOD_client_and_ca_c00356b4,
                 (uint)uVar1);
      uVar4 = (*puVar10 << 5) >> 0x15;
      if ((ushort)*puVar10 == 0) {
        if ((*(uint *)(s_dummySlotlength + param_2 * 4) != 0) && (*(int *)pcVar7 == 0)) {
          uVar4 = *(uint *)(s_dummySlotlength + param_2 * 4);
        }
        uVar9 = (uVar4 / 0xc) * 3;
        uVar4 = 3;
        if (2 < uVar9) {
          uVar4 = uVar9;
        }
        *(uint *)pcVar7 = *(int *)pcVar7 + uVar4 * 4;
LAB_c002d9d8:
        StrongAssertWithContext
                  (uVar4 < 0x400,s_hwSlotLen_<__MAXIMUM_SLOT_ALLOWA_c00356f4,
                   s_Os_Dcu_Slotmac_cpp_c0035620,0x26e);
        uVar1 = (ushort)*puVar10;
        if (((uVar1 != 0xffff) && (uVar1 == (ushort)puVar10[2])) && (uVar1 != 0)) {
          Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c003579c,
                    s_Os_Dcu_Slotmac_cpp_c0035620,0x276,uVar1);
                    /* WARNING: Subroutine does not return */
          SysHaltWithContextF(s_Os_Dcu_Slotmac_cpp_c0035620,0x279,
                              s_Consecutive_slots_for_the_same_c_c00357e0);
        }
        iVar6 = (param_7 * 10 + param_2 * 0x14 + (uint)(ushort)((ushort)*puVar10 >> 5)) * 4;
        uVar9 = *puVar10;
        *(uint *)(iVar6 + -0x3ffccff0) =
             *(uint *)(iVar6 + -0x3ffccff0) | 1 << ((ushort)*puVar10 & 0x1f);
        uVar4 = (uVar9 >> 0x1b & 0xf) << 0x12 | ((ushort)*puVar10 & 0xff) << 10 | uVar4 & 0x3ff;
        if (param_1 == (int *)0x0) {
          *local_48 = uVar4;
          uVar4 = puVar10[1];
        }
        else {
          puVar8 = (undefined4 *)(param_1[2] * 4 + *param_1);
          param_1[2] = param_1[2] + 2;
          puVar8[1] = uVar4;
          *puVar8 = local_48;
          uVar4 = puVar10[1];
        }
        if ((uVar4 & 1) == 1) {
          *(uint *)(s_slotSpareMasks + (uVar11 >> 5) * 4) =
               *(uint *)(s_slotSpareMasks + (uVar11 >> 5) * 4) | 1 << (uVar11 & 0x1f);
        }
        uVar11 = uVar11 + 1;
      }
      else if (uVar4 != 0) goto LAB_c002d9d8;
      puVar10 = puVar10 + 2;
      if ((ushort)*puVar10 == 0xffff) goto code_r0xc002dabc;
      uVar1 = (ushort)*puVar10;
    } while( true );
  }
LAB_c002dae0:
  if (param_4 == (ushort *)0x0) {
    iVar6 = param_2 << 3;
  }
  else {
    uVar4 = (uint)*param_4;
    if (uVar4 == 0xffff) {
      iVar6 = param_2 << 3;
    }
    else {
      iVar6 = param_2 * 8;
      do {
        StrongAssertMsgWithContext
                  ((*(uint *)(s_sodClientMask + (iVar6 + (uVar4 >> 5)) * 4) & 1 << (uVar4 & 0x1f))
                   != 0,s_IsSodClient__dramSel__currentSlo_c0035724,s_Os_Dcu_Slotmac_cpp_c0035620,
                   0x2ab,s_Client__u_is_a_GMMU_client_and_c_c003575c,uVar4);
        uVar1 = *param_4;
        param_4 = param_4 + 4;
        iVar3 = (param_7 * 10 + param_2 * 0x14 + (uint)(uVar1 >> 5)) * 4;
        uVar4 = (uint)*param_4;
        *(uint *)(iVar3 + -0x3ffccff0) = *(uint *)(iVar3 + -0x3ffccff0) | 1 << (uVar1 & 0x1f);
      } while (uVar4 != 0xffff);
    }
  }
  pcVar7 = s_slotmachineData + (param_2 * 10 + local_38) * 8;
  pcVar5 = s_sodClientMask + iVar6 * 4;
  iVar6 = 7;
  do {
    iVar6 = iVar6 + -1;
    *(uint *)(pcVar7 + 8) = *(uint *)(pcVar7 + 8) | *(uint *)pcVar5;
    pcVar5 = pcVar5 + 4;
    pcVar7 = pcVar7 + 4;
  } while (-1 < iVar6);
  if (uVar11 < 0x81) {
    if (param_1 == (int *)0x0) {
      *local_44 = uVar11;
    }
    else {
      puVar8 = (undefined4 *)(param_1[2] * 4 + *param_1);
      param_1[2] = param_1[2] + 2;
      puVar8[1] = uVar11;
      *puVar8 = local_44;
    }
    Os_InterruptsRestore(uVar2);
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Slotmac_cpp_c0035620,0x2c0,
                      s_Number_of_slots_in_the_slot_mach_c0035884,uVar11,0x80);
code_r0xc002dabc:
  if ((uVar11 != 0) && ((ushort)param_3[uVar11 * 2 + -2] != 0)) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Slotmac_cpp_c0035620,0x29e,
                        s_Last_slot_in_slot_machine_must_b_c00358d0);
  }
  goto LAB_c002dae0;
}


//==== DcuSlotMachineSwitchByCbc @ c002de50

int DcuSlotMachineSwitchByCbc(int *param_1)

{
  int iVar1;
  int iVar2;
  code *pcVar3;
  code *pcVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined4 uVar7;
  int in_stack_00000018;
  
  iVar1 = DcuSlotMachineLoadToPageEx_SCbcConfigChunk__();
  if (iVar1 == 0) {
    iVar5 = *param_1;
    pcVar4 = s_slotSpareMasks;
    pcVar3 = s_slotSpareMasksAddr;
    iVar2 = 3;
    do {
      uVar7 = *(undefined4 *)pcVar3;
      puVar6 = (undefined4 *)(param_1[2] * 4 + iVar5);
      param_1[2] = param_1[2] + 2;
      *puVar6 = uVar7;
      iVar2 = iVar2 + -1;
      puVar6[1] = *(undefined4 *)pcVar4;
      pcVar3 = pcVar3 + 4;
      pcVar4 = pcVar4 + 4;
    } while (-1 < iVar2);
    puVar6 = (undefined4 *)(param_1[2] * 4 + iVar5);
    param_1[2] = param_1[2] + 2;
    puVar6[1] = (uint)(in_stack_00000018 != 0);
    *puVar6 = &DAT_b8030450;
  }
  return iVar1;
}


//==== DcuSlotMachineLoadToPage @ c002df14

void DcuSlotMachineLoadToPage
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5,undefined4 param_6)

{
  DcuSlotMachineLoadToPageEx_SCbcConfigChunk__(0,param_1,param_2,param_3,param_4,param_5,param_6);
  return;
}


//==== DcuSlotMachineSwitchToPage @ c002df58

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
DcuSlotMachineSwitchToPage(int param_1,uint param_2,undefined4 param_3,undefined4 *param_4)

{
  uint uVar1;
  undefined4 uVar2;
  code *pcVar3;
  code *pcVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  int iVar12;
  
  uVar2 = 0xfffffffe;
  if ((param_1 < 1) && (uVar2 = 0xffffffff, param_2 < 2)) {
    uVar1 = DcuSlotMachineActivePage();
    uVar2 = 0;
    if (uVar1 != param_2) {
      uVar2 = Os_InterruptsDisable();
      *(undefined4 *)(param_1 * 4 + -0x3ed197b0) = param_3;
      if (param_4 == (undefined4 *)0x0) {
        DAT_c12e682c = 0;
      }
      else {
        DAT_c12e682c = 1;
        uVar11 = *param_4;
        uVar10 = param_4[1];
        uVar5 = param_4[2];
        uVar6 = param_4[3];
        uVar7 = param_4[4];
        uVar8 = param_4[5];
        uVar9 = param_4[6];
        iVar12 = param_1 * 0x20;
        *(undefined4 *)(iVar12 + -0x3ed197b4) = param_4[7];
        *(undefined4 *)(iVar12 + -0x3ed197d0) = uVar11;
        *(undefined4 *)(iVar12 + -0x3ed197cc) = uVar10;
        *(undefined4 *)(iVar12 + -0x3ed197c8) = uVar5;
        *(undefined4 *)(iVar12 + -0x3ed197c4) = uVar6;
        *(undefined4 *)(iVar12 + -0x3ed197c0) = uVar7;
        *(undefined4 *)(iVar12 + -0x3ed197bc) = uVar8;
        *(undefined4 *)(iVar12 + -0x3ed197b8) = uVar9;
      }
      pcVar4 = s_slotSpareMasks;
      pcVar3 = s_slotSpareMasksAddr;
      iVar12 = 3;
      do {
        iVar12 = iVar12 + -1;
        **(undefined4 **)pcVar3 = *(undefined4 *)pcVar4;
        pcVar3 = pcVar3 + 4;
        pcVar4 = pcVar4 + 4;
      } while (-1 < iVar12);
      _DAT_b8030404 = *(undefined4 *)(param_2 * 0x28 + -0x3ffccff4);
      _DAT_b8030408 = *(undefined4 *)(s_slotmachineData + param_2 * 0x28);
      _DAT_b8030450 = (uint)(param_2 != 0);
      if (s_slotMacEnabled[param_1] == (code)0x0) {
        _DAT_b803045c = 1;
        s_slotMacEnabled[param_1] = (code)0x1;
        _DAT_b8030400 = 1;
      }
      Os_InterruptsRestore(uVar2);
      uVar2 = 0;
    }
  }
  return uVar2;
}


//==== DcuSlotMachineLoad @ c002e0f8

uint DcuSlotMachineLoad(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                       undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  uint *puVar7;
  int iVar8;
  undefined4 uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint local_1c;
  
  iVar3 = DcuSlotMachineActivePage();
  uVar10 = iVar3 + 1U & 1;
  uVar12 = uVar10;
  uVar4 = DcuSlotMachineLoadToPage(param_1,param_4,param_5,param_6,param_7,uVar10);
  if (-1 < (int)uVar4) {
    uVar4 = DcuSlotMachineSwitchToPage(param_1,uVar10,param_2,param_3);
    uVar9 = *(undefined4 *)(param_1 * 4 + -0x3ed197b0);
    local_1c = 0;
    iVar3 = param_1 * 0x50 + uVar10 * 0x28 + -0x3ffccfdc;
    puVar6 = (uint *)(param_1 * 0x20 + -0x3ed19790);
    iVar8 = 7;
    puVar7 = &local_1c;
    bVar2 = false;
    do {
      uVar5 = *puVar6 & ~*(uint *)(iVar3 + 8);
      *puVar7 = uVar5;
      iVar8 = iVar8 + -1;
      puVar6 = puVar6 + -1;
      iVar3 = iVar3 + -4;
      puVar7 = puVar7 + -1;
      bVar1 = false;
      if ((bVar2) || (uVar5 != 0)) {
        bVar1 = true;
      }
      bVar2 = bVar1;
    } while (-1 < iVar8);
    if (bVar1) {
      uVar5 = (*(int *)(s_onSmChangeValicationStat + param_1 * 0x198) + 1U) % 10;
      *(uint *)(s_onSmChangeValicationStat + param_1 * 0x198) = uVar5;
      puVar6 = &local_1c;
      iVar3 = (param_1 * 0x66 + uVar5 * 8) * 4 + -0x3ffccf8c;
      iVar8 = 7;
      do {
        iVar8 = iVar8 + -1;
        *(uint *)(iVar3 + 0x58) = *puVar6;
        puVar6 = puVar6 + -1;
        iVar3 = iVar3 + -4;
      } while (-1 < iVar8);
      iVar3 = (param_1 * 0x66 + uVar5) * 4;
      uVar11 = *(int *)(param_1 * 0x198 + -0x3ffccfa4) + 1;
      uVar5 = 10;
      if (uVar11 < 0xb) {
        uVar5 = uVar11;
      }
      *(uint *)(param_1 * 0x198 + -0x3ffccfa4) = uVar5;
      *(undefined4 *)(iVar3 + -0x3ffccf78) = param_2;
      *(undefined4 *)(iVar3 + -0x3ffccfa0) = uVar9;
      TimingMarkF(0xffffffff,s__SM__d____Load__d_c0035904,param_1,param_2,param_7,uVar12,0,0,0,0,0,0
                  ,0);
      if ((int)uVar4 < 0) {
        uVar10 = uVar4;
      }
      return uVar10;
    }
  }
  TimingMarkF(0xffffffff,s__SM__d____Load__d_c0035904,param_1,param_2);
  if ((int)uVar4 < 0) {
    uVar10 = uVar4;
  }
  return uVar10;
}


//==== DcuIsClientActive @ c002e378

bool DcuIsClientActive(int param_1,uint param_2)

{
  int iVar1;
  
  iVar1 = DcuSlotMachineActivePage();
  return (*(uint *)((iVar1 * 10 + param_1 * 0x14 + (param_2 >> 5)) * 4 + -0x3ffccff0) &
         1 << (param_2 & 0x1f)) != 0;
}


//==== DcuClientValidateOnStartEnable @ c002e3f4

void DcuClientValidateOnStartEnable(undefined1 param_1)

{
  DAT_c12e6875 = param_1;
  return;
}


//==== DcuValidateClient @ c002e400

void DcuValidateClient(int param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  
  iVar1 = DcuIsClientActive();
  iVar2 = param_1 * 0x58;
  if (iVar1 == 0) {
    uVar4 = (*(int *)(s_onClientStartValidationStat + iVar2) + 1U) % 10;
    uVar5 = *(int *)(iVar2 + -0x3ffcce0c) + 1;
    iVar1 = (param_1 * 0x16 + uVar4) * 4;
    uVar3 = 10;
    if (uVar5 < 0xb) {
      uVar3 = uVar5;
    }
    *(undefined4 *)(iVar1 + -0x3ffcce08) = *(undefined4 *)(param_1 * 4 + -0x3ed197b0);
    *(uint *)(iVar2 + -0x3ffcce0c) = uVar3;
    *(undefined4 *)(iVar1 + -0x3ffccde0) = param_2;
    *(uint *)(s_onClientStartValidationStat + iVar2) = uVar4;
  }
  return;
}


//==== DcuCheckClientIsInCurrentSlotMachine @ c002e4c0

void DcuCheckClientIsInCurrentSlotMachine(int param_1,uint param_2)

{
  if ((DAT_c12e682c != '\0') &&
     ((*(uint *)((param_1 * 8 + (param_2 >> 5)) * 4 + -0x3ed197d0) & 1 << (param_2 & 0x1f)) == 0)) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Slotmac_cpp_c0035620,0x3e5,
                        s_The_client__d_you_tried_to_start_c0035918,param_2);
  }
  return;
}


//==== DcuDumpStartingClientsWithoutSlotStat @ c002e530

void DcuDumpStartingClientsWithoutSlotStat(int param_1)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  undefined4 uVar6;
  uint uVar7;
  
  iVar1 = param_1 * 0x58;
  uVar5 = *(uint *)(s_onClientStartValidationStat + iVar1);
  Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
            s_DRAM__d___d_latest_clients_that_t_c0035968,param_1,10);
  uVar7 = 0;
  if (*(int *)(iVar1 + -0x3ffcce0c) != 0) {
    do {
      iVar3 = (param_1 * 0x16 + uVar5) * 4;
      pcVar2 = s_Latest__c00359c4;
      uVar6 = *(undefined4 *)(iVar3 + -0x3ffccde0);
      uVar4 = *(undefined4 *)(iVar3 + -0x3ffcce08);
      if (uVar7 != 0) {
        pcVar2 = s__c00359d0;
      }
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),pcVar2);
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                PTR_s_SMDB_slotmachine_index___d_c0035b34,uVar4);
      uVar7 = uVar7 + 1;
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),PTR_s_Client_ID___d_c0035b38,uVar6);
      uVar5 = (uVar5 + 9) % 10;
    } while ((uVar7 < 10) && (uVar7 < *(uint *)(iVar1 + -0x3ffcce0c)));
  }
  return;
}


//==== DcuClientValidationOnSmChangeEnable @ c002e698

void DcuClientValidationOnSmChangeEnable(undefined1 param_1)

{
  DAT_c12e6874 = param_1;
  return;
}


//==== DcuDumpStartedClientsWithoutSlotStat @ c002e6a4

void DcuDumpStartedClientsWithoutSlotStat(int param_1)

{
  bool bVar1;
  char *pcVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  undefined4 uVar6;
  int iVar7;
  code *pcVar8;
  uint uVar9;
  uint local_30;
  
  uVar9 = *(uint *)(s_onSmChangeValicationStat + param_1 * 0x198);
  Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
            s_DRAM__d___d_latest_slotmachine_c_c0035a14,param_1,10);
  local_30 = 0;
  if (*(int *)(param_1 * 0x198 + -0x3ffccfa4) != 0) {
    do {
      iVar3 = (param_1 * 0x66 + uVar9) * 4;
      uVar6 = *(undefined4 *)(iVar3 + -0x3ffccf78);
      pcVar2 = s_Latest__c00359c4;
      uVar4 = *(undefined4 *)(iVar3 + -0x3ffccfa0);
      if (local_30 != 0) {
        pcVar2 = s__c00359d0;
      }
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),pcVar2);
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
                PTR_s_SMDB_slotmachine_index___from__d_c0035b3c,uVar4,uVar6);
      iVar3 = (param_1 * 0x66 + uVar9 * 8) * 4 + -0x3ffccf8c;
      iVar7 = 7;
      do {
        iVar7 = iVar7 + -1;
        Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),PTR_s__08X_c0035b40,
                  *(undefined4 *)(iVar3 + 0x58));
        iVar3 = iVar3 + -4;
      } while (-1 < iVar7);
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),PTR_DAT_c0035b44);
      bVar1 = true;
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),PTR_s_Client_IDs__c0035b48);
      iVar3 = 0;
      pcVar8 = s_onSmChangeValicationStat + (param_1 * 0x66 + uVar9 * 8) * 4;
      do {
        uVar5 = 0;
        do {
          if ((*(uint *)(pcVar8 + 0x58) & 1 << (uVar5 & 0x1f)) != 0) {
            if (!bVar1) {
              Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0035acc);
            }
            bVar1 = false;
            Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),&DAT_c0035ac8,
                      iVar3 * 0x20 + uVar5);
          }
          uVar5 = uVar5 + 1;
        } while ((int)uVar5 < 0x20);
        iVar3 = iVar3 + 1;
        pcVar8 = pcVar8 + 4;
      } while (iVar3 < 8);
      local_30 = local_30 + 1;
      Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),PTR_s___c0035b4c);
      uVar9 = (uVar9 + 9) % 10;
    } while ((local_30 < 10) && (local_30 < *(uint *)(param_1 * 0x198 + -0x3ffccfa4)));
  }
  return;
}


//==== DcuSlotMachineResetPtr @ c002e950

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 DcuSlotMachineResetPtr(void)

{
  _DAT_b8030444 = 1;
  return 1;
}


//==== DcuDummySlotOverrideByDram @ c002e968

void DcuDummySlotOverrideByDram(int param_1,undefined4 param_2)

{
  *(undefined4 *)(s_dummySlotlength + param_1 * 4) = param_2;
  return;
}


//==== DcuDummySlotOverride @ c002e980

void DcuDummySlotOverride(undefined4 param_1)

{
  DcuDummySlotOverrideByDram(0,param_1);
  return;
}


//==== DcuClientSet @ c002e9a0

undefined4 DcuClientSet(int param_1,uint param_2,void *param_3)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  
  if (param_1 == 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Set_cpp_c0035b50,0x2b,s_Invalid_DCU_client_handle__c0035b84);
  }
  if (*(size_t *)(param_1 + 0x24) == 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Set_cpp_c0035b50,0x32,s_Client_Not_Defined_c0035ba0);
  }
  iVar3 = *(int *)(param_1 + 0x10);
  if (iVar3 == 5) {
    if ((((param_2 == 0xff) && (1 < *(uint *)((int)param_3 + 0x30))) ||
        (param_2 == 0xc && (void *)0x1 < param_3)) || (param_2 - 0xd < 8)) {
                    /* WARNING: Subroutine does not return */
      SysHaltWithContextF(s_Os_Dcu_Set_cpp_c0035b50,0x44,s_The_field_is_no_longer_supported_c0035b60
                         );
    }
  }
  else if (iVar3 == 0xc) {
    if (param_2 == 0xff) {
      if (1 < *(uint *)((int)param_3 + 0x58)) {
                    /* WARNING: Subroutine does not return */
        SysHaltWithContextF(s_Os_Dcu_Set_cpp_c0035b50,0x5a,
                            s_The_field_is_no_longer_supported_c0035b60);
      }
    }
    else {
      if (param_2 == 0x17 && (void *)0x1 < param_3) {
                    /* WARNING: Subroutine does not return */
        SysHaltWithContextF(s_Os_Dcu_Set_cpp_c0035b50,0x5f,
                            s_The_field_is_no_longer_supported_c0035b60);
      }
      if (0x17 < param_2) {
                    /* WARNING: Subroutine does not return */
        SysHaltWithContextF(s_Os_Dcu_Set_cpp_c0035b50,99,s_The_field_is_no_longer_supported_c0035b60
                           );
      }
    }
  }
  if (iVar3 != 8 || param_2 != 0x9c) {
    if (param_2 == 0xff) {
      memcpy(*(void **)(param_1 + 0x44),param_3,*(size_t *)(param_1 + 0x24));
    }
    else {
      *(void **)(param_2 * 4 + *(int *)(param_1 + 0x44)) = param_3;
      if (*(int *)(param_1 + 0x10) == 1) {
        puVar1 = (uint *)DcuGetClientAddr(param_1,0);
        if (param_2 == 1) {
          *puVar1 = (*(uint *)(*(int *)(param_1 + 0x44) + 4) & 0x1fffffff) >> 2;
        }
        else if (param_2 == 2) {
          puVar1[1] = (*(uint *)(*(int *)(param_1 + 0x44) + 8) & 0x1fffffff) >> 2;
        }
        else {
          if (param_2 == 3) {
            iVar3 = *(int *)(param_1 + 0x20);
            iVar2 = 0xc;
          }
          else {
            if (param_2 != 4) {
              return 1;
            }
            iVar3 = *(int *)(param_1 + 0x20);
            iVar2 = 8;
          }
          *(uint *)((int)puVar1 + iVar2) = iVar3 << 0x1c | (uint)param_3;
        }
      }
    }
  }
  return 1;
}


//==== DcuClientGet @ c002ec50

undefined4 DcuClientGet(int param_1,int param_2,undefined4 *param_3)

{
  if (param_1 == 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Set_cpp_c0035b50,0xbe,s_Invalid_DCU_client_handle__c0035b84);
  }
  if (param_2 != 0xff) {
    *param_3 = *(undefined4 *)(param_2 * 4 + *(int *)(param_1 + 0x44));
    return 1;
  }
  memcpy(param_3,*(void **)(param_1 + 0x44),*(size_t *)(param_1 + 0x24));
  return 1;
}


//==== DcuClientGetStatus @ c002ecc4

undefined4 DcuClientGetStatus(int param_1,byte *param_2,byte *param_3)

{
  int *piVar1;
  int iVar2;
  
  if (param_1 != 0) {
    piVar1 = (int *)DcuGetClientAddr(param_1,0x20);
    iVar2 = *piVar1;
    if (param_2 != (byte *)0x0) {
      *param_2 = (byte)iVar2 & 1;
    }
    if (param_3 != (byte *)0x0) {
      *param_3 = (byte)((uint)(iVar2 << 0x1e) >> 0x1f);
    }
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Set_cpp_c0035b50,0xdb,s_Invalid_DCU_client_handle__c0035b84);
}


//==== DcuBufferSplitInit @ c002ed38

undefined4 DcuBufferSplitInit(void)

{
  int iVar1;
  code *pcVar2;
  undefined4 uVar3;
  
  iVar1 = Os_MutexCreate(s_splitBufferAssignmentMutex,s_SplitBufferAssignment_c0035bb4);
  uVar3 = 0;
  if (iVar1 == 0) {
    pcVar2 = g_splitHwStatus;
    iVar1 = 0xb;
    do {
      iVar1 = iVar1 + -1;
      *pcVar2 = (code)0x0;
      *(undefined4 *)(pcVar2 + 4) = 0;
      pcVar2 = pcVar2 + 0x10;
    } while (-1 < iVar1);
    uVar3 = 1;
  }
  return uVar3;
}


//==== DcuBufferSplitAttachClient @ c002ed90

bool DcuBufferSplitAttachClient(int param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  code *pcVar4;
  
  uVar3 = 0;
  if (*(char *)(param_1 + 0x3c) == '\x01') {
    pcVar4 = g_splitHwStatus;
    do {
      uVar3 = uVar3 + 1;
      if (*(int *)(param_1 + 0xc) == *(int *)(pcVar4 + 4)) {
        if (*pcVar4 != (code)0x1) {
                    /* WARNING: Subroutine does not return */
          __assfail(s_Failed_assertion___s__at_line__d_c0035bcc,
                    s_true____g_splitHwStatus__splitIn_c0035c14,0x4d,
                    s_Os_Dcu_BufferSplit_cpp_c0035bfc);
        }
        break;
      }
      pcVar4 = pcVar4 + 0x10;
    } while (uVar3 < 0xc);
                    /* WARNING: Subroutine does not return */
    __assfail(s_Failed_assertion___s__at_line__d_c0035bcc,&DAT_c0035bf8,0x52,
              s_Os_Dcu_BufferSplit_cpp_c0035bfc);
  }
  Os_MutexTake(s_splitBufferAssignmentMutex,0xffffffff);
  pcVar4 = g_splitHwStatus;
  do {
    if (*pcVar4 == (code)0x0) {
      *pcVar4 = (code)0x1;
      *(undefined1 *)(param_1 + 0x3c) = 1;
      break;
    }
    uVar3 = uVar3 + 1;
    pcVar4 = pcVar4 + 0x10;
  } while (uVar3 < 0xc);
  Os_MutexGive(s_splitBufferAssignmentMutex);
  iVar2 = uVar3 * 0x10;
  if (uVar3 == 0xc) {
    Fprintf_B(*(undefined4 *)(PTR__libc_data0_c014e844 + 8),
              s_All_Split_mechnism_HW_units_are_o_c0035c48);
  }
  else {
    uVar1 = *(undefined4 *)(param_1 + 0xc);
    *(int *)(iVar2 + -0x3ffccd70) = (param_3 - param_2) + -1;
    *(undefined4 *)(iVar2 + -0x3ffccd78) = uVar1;
    *(int *)(iVar2 + -0x3ffccd74) = param_2;
  }
  return uVar3 != 0xc;
}


//==== DcuBufferSplitDetachClient @ c002eefc

undefined4 DcuBufferSplitDetachClient(int param_1)

{
  code *pcVar1;
  uint uVar2;
  
  uVar2 = 0;
  if (*(char *)(param_1 + 0x3c) != '\0') {
    Os_MutexTake(s_splitBufferAssignmentMutex,0xffffffff);
    pcVar1 = g_splitHwStatus;
    do {
      if (*(int *)(pcVar1 + 4) == *(int *)(param_1 + 0xc)) {
        if (*pcVar1 == (code)0x0) {
          Os_MutexGive(s_splitBufferAssignmentMutex);
                    /* WARNING: Subroutine does not return */
          __assfail(s_Failed_assertion___s__at_line__d_c0035bcc,&DAT_c0035bf8,0x8a,
                    s_Os_Dcu_BufferSplit_cpp_c0035bfc);
        }
        *(undefined1 *)(param_1 + 0x3c) = 0;
        Os_MutexGive(s_splitBufferAssignmentMutex);
        goto joined_r0xc002efc4;
      }
      uVar2 = uVar2 + 1;
      pcVar1 = pcVar1 + 0x10;
    } while (uVar2 < 0xc);
    Os_MutexGive(s_splitBufferAssignmentMutex);
joined_r0xc002efc4:
    if (uVar2 == 0xc) {
                    /* WARNING: Subroutine does not return */
      __assfail(s_Failed_assertion___s__at_line__d_c0035bcc,&DAT_c0035bf8,0x9a,
                s_Os_Dcu_BufferSplit_cpp_c0035bfc);
    }
  }
  return 1;
}


//==== DcuFifoHandlingInit @ c002effc

undefined4 DcuFifoHandlingInit(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  code *pcVar3;
  int iVar4;
  
  iVar1 = Os_MutexCreate(s_fifoHandlingAssignmentMutex,s_FifoHandlingAssignment_c0035c78);
  uVar2 = 0;
  if (iVar1 == 0) {
    pcVar3 = s_fifoHandlingHwStatus + param_1 * 0x38;
    iVar4 = 0xed0;
    iVar1 = 1;
    do {
      iVar1 = iVar1 + -1;
      *(int *)(pcVar3 + 0x14) = iVar4 + -0x48000000;
      *(int *)(pcVar3 + 0x18) = param_1;
      iVar4 = iVar4 + 0x40;
      pcVar3 = pcVar3 + 0x1c;
    } while (-1 < iVar1);
    uVar2 = 1;
  }
  return uVar2;
}


//==== DcuGetClientAddr @ c002f088

int DcuGetClientAddr(int param_1,int param_2)

{
  if (*(int *)(param_1 + 0x40) != 0) {
    return *(int *)(param_1 + 0x40) + param_2;
  }
                    /* WARNING: Subroutine does not return */
  __assfail(s_Failed_assertion___s__at_line__d_c00360c0,s_client_>BaseAddress_c00360ec,0x16c,
            s_Os_Dcu_Clients_cpp_c0036100);
}


//==== DcuSetClientControls @ c002f0c8

undefined4 DcuSetClientControls(int param_1,int param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  
  if (param_1 == 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x179,s_Invalid_DCU_client_handle__c0036114);
  }
  if (*(int *)(param_1 + 8) == 0) {
    if (param_2 == 2) {
      *(undefined4 *)(param_1 + 0x14) = 0;
    }
    else {
      *(undefined4 *)(param_1 + 0x14) = 1;
    }
    if (param_3 == 2) {
      *(undefined4 *)(param_1 + 0x18) = 0;
    }
    else {
      uVar1 = 0;
      if (param_3 == 1) {
        uVar1 = 2;
      }
      *(undefined4 *)(param_1 + 0x18) = uVar1;
    }
    if (param_4 == 2) {
      *(undefined4 *)(param_1 + 0x1c) = 0;
      return 1;
    }
    *(undefined4 *)(param_1 + 0x1c) = 4;
    return 1;
  }
  return 1;
}


//==== DcuSetClientThreshold @ c002f170

undefined4 DcuSetClientThreshold(int param_1,char param_2,uint param_3)

{
  undefined4 uVar1;
  
  uVar1 = 8;
  if (param_2 == '\0') {
    uVar1 = 0;
  }
  if (param_1 != 0) {
    if (*(int *)(param_1 + 8) == 0) {
      if (0x3f < param_3) {
                    /* WARNING: Subroutine does not return */
        SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x1b8,
                            s_Client_threshold_value_exceeded_m_c0036130);
      }
      *(uint *)(param_1 + 0x2c) = param_3 << 4;
      *(undefined4 *)(param_1 + 0x28) = uVar1;
    }
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x1ad,s_Invalid_DCU_client_handle__c0036114);
}


//==== DcuSetClientRounding @ c002f1e4

undefined4 DcuSetClientRounding(int param_1,char param_2)

{
  undefined4 uVar1;
  
  uVar1 = 0x400;
  if (param_2 == '\0') {
    uVar1 = 0;
  }
  if (param_1 != 0) {
    if (*(int *)(param_1 + 8) == 0) {
      *(undefined4 *)(param_1 + 0x30) = uVar1;
    }
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x1c8,s_Invalid_DCU_client_handle__c0036114);
}


//==== DcuUpdateClientControlsRegister @ c002f230

undefined4 DcuUpdateClientControlsRegister(int param_1)

{
  uint *puVar1;
  
  if (param_1 != 0) {
    puVar1 = (uint *)DcuGetClientAddr(param_1,0x20);
    *puVar1 = *(uint *)(param_1 + 0x18) | *(uint *)(param_1 + 0x1c) | *(uint *)(param_1 + 0x28) |
              *(uint *)(param_1 + 0x2c) | *(uint *)(param_1 + 0x30) | *(uint *)(param_1 + 0x34);
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x1ec,s_Invalid_DCU_client_handle__c0036114);
}


//==== DcuSubmitClientControls @ c002f2ac

void DcuSubmitClientControls(int param_1)

{
  if (param_1 != 0) {
    DcuSetClientControls();
    DcuUpdateClientControlsRegister(param_1);
    return;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x1de,s_Invalid_DCU_client_handle__c0036114);
}


//==== DcuSetClientParam @ c002f2f8

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 DcuSetClientParam(int param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  if (param_1 == 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x207,s_Invalid_DCU_client_handle__c0036114);
  }
  if (param_2 != 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x26c,
                        s_DcuSetParam__Invalid_DCU_param_I_c003615c);
  }
  uVar1 = param_3;
  uVar2 = _DAT_b8003b24;
  if ((*(int *)(param_1 + 0xc) != 0xde) &&
     (uVar1 = _DAT_b8003b1c, uVar2 = param_3, *(int *)(param_1 + 0xc) != 0xe0)) {
    return 1;
  }
  _DAT_b8003b24 = uVar2;
  _DAT_b8003b1c = uVar1;
  return 1;
}


//==== DcuSetClientDataConfig @ c002f38c

undefined4 DcuSetClientDataConfig(int param_1,undefined4 param_2)

{
  if (param_1 != 0) {
    switch(param_2) {
    case 0:
      *(undefined4 *)(param_1 + 0x20) = 0;
      return 1;
    case 1:
      *(undefined4 *)(param_1 + 0x20) = 1;
      return 1;
    case 2:
      *(undefined4 *)(param_1 + 0x20) = 2;
      return 1;
    case 3:
      *(undefined4 *)(param_1 + 0x20) = 3;
      return 1;
    case 4:
      *(undefined4 *)(param_1 + 0x20) = 4;
      return 1;
    case 5:
      *(undefined4 *)(param_1 + 0x20) = 5;
      return 1;
    case 6:
      *(undefined4 *)(param_1 + 0x20) = 6;
      return 1;
    case 7:
      *(undefined4 *)(param_1 + 0x20) = 7;
      return 1;
    default:
                    /* WARNING: Subroutine does not return */
      SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x2aa,
                          s_Invalid_DCU_client_data_config__c0036180);
    }
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x279,s_Invalid_DCU_client_handle__c0036114);
}


//==== DcuConfigureCodeClient @ c002f4ac

undefined4
DcuConfigureCodeClient
          (undefined4 *param_1,char param_2,int param_3,undefined4 *param_4,undefined4 *param_5)

{
  undefined4 uVar1;
  
  if (param_2 == '\0') {
    *param_1 = 0;
    param_5 = param_4;
  }
  else {
    *param_1 = 1;
  }
  if (param_3 != -1) {
    uVar1 = *param_5;
    param_1[2] = param_3 + 3U >> 2;
    param_1[3] = 0;
    param_1[1] = uVar1;
    return 1;
  }
  uVar1 = *param_5;
  param_1[2] = 0xffffffff;
  param_1[3] = 0;
  param_1[1] = uVar1;
  return 1;
}


//==== DcuSubmitToCbcClientControls @ c002f51c

undefined4 DcuSubmitToCbcClientControls(int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int *in_stack_00000010;
  
  if (param_1 != 0) {
    DcuSetClientControls();
    uVar1 = DcuGetClientAddr(param_1,0x20);
    puVar2 = (undefined4 *)(in_stack_00000010[2] * 4 + *in_stack_00000010);
    in_stack_00000010[2] = in_stack_00000010[2] + 2;
    *puVar2 = uVar1;
    puVar2[1] = *(uint *)(param_1 + 0x18) | *(uint *)(param_1 + 0x1c) | *(uint *)(param_1 + 0x28) |
                *(uint *)(param_1 + 0x2c) | *(uint *)(param_1 + 0x30) | *(uint *)(param_1 + 0x34);
    return 1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x304,s_Invalid_DCU_client_handle__c0036114);
}


//==== DcuGetClientParamBaseAddr @ c002f5c8

void DcuGetClientParamBaseAddr(int param_1)

{
  if (param_1 != 0) {
    DcuGetClientAddr(param_1,0);
    return;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x312,s_Invalid_DCU_client_handle__c0036114);
}


//==== DcuClientGenCbcReqTemplate @ c002f670

void DcuClientGenCbcReqTemplate(int param_1,int *param_2,undefined4 *param_3,undefined4 *param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *puVar5;
  
  if (param_1 != 0) {
    uVar1 = DcuGetClientAddr(param_1,0);
    uVar2 = DcuGetClientAddr(param_1,0xc);
    uVar3 = DcuGetClientAddr(param_1,0x10);
    if (param_3 != (undefined4 *)0x0) {
      uVar1 = func_0xc01bc5ec(param_2,uVar1,0);
      *param_3 = uVar1;
    }
    if (param_4 != (undefined4 *)0x0) {
      uVar1 = func_0xc01bc5ec(param_2,uVar2,0);
      *param_4 = uVar1;
    }
    iVar4 = *param_2;
    puVar5 = (undefined4 *)(param_2[2] * 4 + iVar4);
    param_2[2] = param_2[2] + 2;
    *puVar5 = 10;
    puVar5[1] = 0;
    puVar5 = (undefined4 *)(param_2[2] * 4 + iVar4);
    param_2[2] = param_2[2] + 2;
    *puVar5 = uVar3;
    puVar5[1] = 1;
    return;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x34e,s_Invalid_DCU_client_handle__c0036114);
}


//==== DcuInternalIdtoHandle @ c002f78c

undefined4 DcuInternalIdtoHandle(uint param_1,uint param_2)

{
  uint *puVar1;
  uint uVar2;
  uint *puVar3;
  uint uVar4;
  int iVar5;
  code *pcVar6;
  int iVar7;
  
  if (0 < (int)param_1) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x3d9,s_Invalid_DRAM_selector__c00361a4);
  }
  if (0xea < param_2) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x3da,s_Invalid_DCU_client_ID__c00361bc);
  }
  if (*(int *)((uint)(g_clients + (param_1 * 0xeb + param_2) * 4) | 0x20000000) == 0) {
    iVar7 = param_2 * 0x14;
    pcVar6 = g_clientInfo + iVar7;
    puVar1 = (uint *)PoolAllocEx(0,0x48,0,s_Os_Dcu_Clients_cpp_c0036100,0x38a);
    *(int *)((uint)(g_clients + (param_1 * 0xeb + param_2) * 4) | 0x20000000) = (int)puVar1;
    if ((*(uint *)(iVar7 + -0x3ffcc0b8) & 0x1ff) == 0) {
                    /* WARNING: Subroutine does not return */
      SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x38f,
                          s_Incorrect_size_of_client_s_param_c00361d4,param_2);
    }
    uVar2 = PoolAllocEx(0,*(uint *)(iVar7 + -0x3ffcc0b8) & 0x1ff,0,s_Os_Dcu_Clients_cpp_c0036100,
                        0x393);
    iVar5 = *(int *)(iVar7 + -0x3ffcc0b8);
    puVar1[0x11] = uVar2;
    *puVar1 = (uint)(iVar5 << 0x11) >> 0x1a;
    uVar4 = *(uint *)pcVar6;
    puVar1[1] = param_1;
    puVar1[2] = uVar4 & 0xf;
    iVar5 = *(int *)pcVar6;
    *(undefined1 *)(puVar1 + 0xf) = 0;
    uVar2 = (uint)(iVar5 << 0x13) >> 0x17;
    puVar1[3] = uVar2;
    puVar1[4] = (uint)(*(int *)pcVar6 << 0xe) >> 0x1b;
    puVar1[5] = (uint)(*(int *)pcVar6 << 0xd) >> 0x1f;
    puVar1[6] = (uint)(*(int *)pcVar6 << 0xb) >> 0x1e;
    puVar1[7] = (uint)(*(int *)pcVar6 << 8) >> 0x1d;
    puVar1[10] = (uint)(*(int *)(iVar7 + -0x3ffcc0b8) << 0xd) >> 0x1c;
    puVar1[0xb] = (uint)(*(int *)(iVar7 + -0x3ffcc0b8) << 3) >> 0x16;
    puVar1[0xc] = *(uint *)(iVar7 + -0x3ffcc0b4) & 0x7ff;
    puVar1[0xd] = *(uint *)(iVar7 + -0x3ffcc0b4) >> 0xb;
    puVar1[0xe] = *(uint *)(iVar7 + -0x3ffcc0b0) & 0x3fffff;
    puVar1[8] = (uint)(*(int *)pcVar6 << 5) >> 0x1d;
    puVar1[9] = *(uint *)(iVar7 + -0x3ffcc0b8) & 0x1ff;
    puVar1[0x10] = *(uint *)(iVar7 + -0x3ffcc0ac);
    if (((uVar4 & 0xf) == 0) && (uVar2 != 0)) {
      puVar3 = (uint *)DcuGetClientAddr(puVar1,0x20);
      *puVar3 = puVar1[6] | puVar1[10] | puVar1[0xb] | puVar1[0xc] | puVar1[0xd] | puVar1[0xe];
    }
    if (*(int *)((uint)(g_clients + (param_1 * 0xeb + param_2) * 4) | 0x20000000) == 0) {
                    /* WARNING: Subroutine does not return */
      SysHaltWithContextF(s_Os_Dcu_Clients_cpp_c0036100,0x3e0,s_Invalid_DCU_client_handle__c0036114)
      ;
    }
  }
  return *(undefined4 *)((uint)(g_clients + (param_1 * 0xeb + param_2) * 4) | 0x20000000);
}


//==== DcuClientSubmitEx(tagSDCUCLIENTOBJ*, @ c002fa2c

undefined4 DcuClientSubmitEx_tagSDCUCLIENTOBJ__(int param_1,int *param_2)

{
  code cVar1;
  uint uVar2;
  uint *puVar3;
  undefined4 uVar4;
  char *pcVar5;
  code *pcVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint *puVar14;
  uint uVar15;
  int iVar16;
  int *piVar17;
  uint *puVar18;
  uint uVar19;
  int iVar20;
  uint uVar21;
  uint uVar22;
  uint uVar23;
  uint uVar24;
  undefined4 *puVar25;
  uint uVar26;
  
  if (param_1 == 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Submit_cpp_c0036688,0x73,s_Invalid_DCU_client_handle__c003669c);
  }
  iVar16 = *(int *)(param_1 + 0xc);
  pcVar6 = g_splitHwStatus;
  uVar8 = 0;
  do {
    while (*(int *)(pcVar6 + 4) != iVar16) {
LAB_c002fa94:
      uVar8 = uVar8 + 1;
      pcVar6 = pcVar6 + 0x10;
      if (0xb < uVar8) goto LAB_c002fb88;
    }
    if (*(char *)(param_1 + 0x3c) == '\0') {
      *pcVar6 = (code)0x0;
    }
    iVar7 = uVar8 * 0x40;
    if (param_2 == (int *)0x0) {
      *(uint *)(iVar7 + -0x47ffc100) = (uint)(byte)*pcVar6;
      cVar1 = *pcVar6;
    }
    else {
      cVar1 = *pcVar6;
      piVar17 = (int *)(param_2[2] * 4 + *param_2);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = (uint)(byte)cVar1;
      *piVar17 = iVar7 + -0x47ffc100;
      cVar1 = *pcVar6;
    }
    if (cVar1 != (code)0x1) goto LAB_c002fa94;
    if (param_2 == (int *)0x0) {
      uVar2 = *(uint *)(pcVar6 + 8);
      iVar20 = *(int *)(pcVar6 + 0xc);
      *(undefined4 *)(iVar7 + -0x47ffc0fc) = *(undefined4 *)(pcVar6 + 4);
      *(uint *)(iVar7 + -0x47ffc0f8) = (uVar2 & 0x1fffffff) >> 2;
      *(int *)(iVar7 + -0x47ffc0f4) = iVar20 >> 2;
      goto LAB_c002fa94;
    }
    iVar20 = *param_2;
    piVar17 = (int *)(param_2[2] * 4 + iVar20);
    param_2[2] = param_2[2] + 2;
    *piVar17 = iVar7 + -0x47ffc0fc;
    uVar2 = *(uint *)(pcVar6 + 8);
    piVar17[1] = *(int *)(pcVar6 + 4);
    piVar17 = (int *)(param_2[2] * 4 + iVar20);
    param_2[2] = param_2[2] + 2;
    piVar17[1] = (uVar2 & 0x1fffffff) >> 2;
    *piVar17 = iVar7 + -0x47ffc0f8;
    piVar17 = (int *)(param_2[2] * 4 + iVar20);
    param_2[2] = param_2[2] + 2;
    *piVar17 = iVar7 + -0x47ffc0f4;
    uVar8 = uVar8 + 1;
    piVar17[1] = *(int *)(pcVar6 + 0xc) >> 2;
    pcVar6 = pcVar6 + 0x10;
  } while (uVar8 < 0xc);
LAB_c002fb88:
  uVar4 = 0;
  switch(*(undefined4 *)(param_1 + 0x10)) {
  case 0:
    puVar18 = *(uint **)(param_1 + 0x44);
    TimingMarkF(0x400000,s__DCU_LINE__d__StartAddress_0x_08_c0036300,iVar16,puVar18[1]);
    puVar3 = (uint *)DcuGetClientAddr(param_1,0);
    uVar8 = 2;
    if (puVar18[3] == 0) {
      uVar8 = 0;
    }
    if (param_2 == (int *)0x0) {
      uVar2 = puVar18[1];
      uVar9 = puVar18[2];
      puVar3[9] = *puVar18 | uVar8;
      *puVar3 = (uVar2 & 0x1fffffff) >> 2;
      puVar3[3] = uVar9;
      uVar8 = puVar18[3];
    }
    else {
      iVar16 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 9);
      uVar2 = puVar18[1];
      piVar17[1] = *puVar18 | uVar8;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = (uVar2 & 0x1fffffff) >> 2;
      *piVar17 = (int)puVar3;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 3);
      piVar17[1] = puVar18[2];
      uVar8 = puVar18[3];
    }
    if (uVar8 == 1) {
      if (param_2 != (int *)0x0) {
        piVar17 = (int *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        piVar17[1] = 1;
        *piVar17 = (int)(puVar3 + 0xe);
        goto LAB_c003018c;
      }
      puVar3[0xe] = 1;
    }
    else {
LAB_c003018c:
      if (param_2 != (int *)0x0) {
        iVar16 = *param_2;
        piVar17 = (int *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar3 + 2);
        puVar3 = puVar3 + 4;
        piVar17[1] = *(int *)(param_1 + 0x20);
        goto LAB_c00313dc;
      }
    }
    uVar4 = 1;
    puVar3[2] = *(uint *)(param_1 + 0x20);
    puVar3[4] = 1;
    break;
  case 1:
    puVar3 = *(uint **)(param_1 + 0x44);
    TimingMarkF(0x400000,s__DCU_LINETOGGLE__d__StartAddress_c0036230,iVar16,puVar3[1],puVar3[2]);
    puVar18 = (uint *)DcuGetClientAddr(param_1,0);
    uVar8 = 0;
    uVar2 = 0;
    if (puVar3[6] != 0) {
      uVar8 = 4;
      if (puVar3[6] != 2) {
        uVar8 = 0;
      }
      uVar2 = 2;
    }
    if (param_2 == (int *)0x0) {
      uVar11 = puVar3[1];
      uVar9 = puVar3[2];
      uVar10 = puVar3[3];
      uVar12 = puVar3[4];
      uVar13 = *(int *)(param_1 + 0x20) << 0x1c;
      puVar18[9] = *puVar3 | uVar2 | uVar8;
      *puVar18 = (uVar11 & 0x1fffffff) >> 2;
      puVar18[1] = (uVar9 & 0x1fffffff) >> 2;
      puVar18[3] = uVar10 | uVar13;
      puVar18[2] = uVar12 | uVar13;
      uVar8 = puVar3[6];
    }
    else {
      iVar16 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar18 + 9);
      uVar9 = puVar3[1];
      piVar17[1] = *puVar3 | uVar2 | uVar8;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = (uVar9 & 0x1fffffff) >> 2;
      *piVar17 = (int)puVar18;
      uVar8 = puVar3[2];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      puVar25[1] = (uVar8 & 0x1fffffff) >> 2;
      *puVar25 = puVar18 + 1;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar18 + 3);
      piVar17[1] = puVar3[3] | *(int *)(param_1 + 0x20) << 0x1c;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar18 + 2;
      puVar25[1] = puVar3[4] | *(int *)(param_1 + 0x20) << 0x1c;
      uVar8 = puVar3[6];
    }
    if (uVar8 - 1 < 2) {
      if (param_2 != (int *)0x0) {
        puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        puVar25[1] = 1;
        *puVar25 = puVar18 + 0xd;
        goto LAB_c002fd38;
      }
      puVar18[0xd] = 1;
    }
    else {
LAB_c002fd38:
      if (param_2 != (int *)0x0) {
        puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar18 + 0xe;
        puVar25[1] = puVar3[5];
        return 1;
      }
    }
    uVar4 = 1;
    puVar18[0xe] = puVar3[5];
    break;
  case 2:
    puVar14 = *(uint **)(param_1 + 0x44);
    puVar18 = (uint *)DcuGetClientAddr(param_1,0);
    if (param_2 == (int *)0x0) {
      uVar9 = puVar14[1];
      uVar12 = puVar14[2];
      uVar13 = puVar14[5];
      uVar10 = puVar14[4];
      uVar8 = puVar14[3];
      uVar11 = puVar14[6];
      uVar2 = *(uint *)(param_1 + 0x20);
      puVar18[9] = *puVar14 | puVar14[7] << 1;
      *puVar18 = (uVar9 & 0x1fffffff) >> 2;
      puVar18[1] = (uVar12 & 0x1fffffff) >> 2;
      puVar18[2] = uVar8;
      puVar18[10] = uVar13 << 0xe | uVar10;
      puVar18[0xb] = uVar2;
      puVar18[0xe] = uVar11;
      puVar18[4] = 1;
      return 1;
    }
    iVar16 = *param_2;
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar18 + 9;
    puVar25[1] = *puVar14 | puVar14[7] << 1;
    uVar8 = puVar14[1];
    piVar17 = (int *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *piVar17 = (int)puVar18;
    piVar17[1] = (uVar8 & 0x1fffffff) >> 2;
    uVar8 = puVar14[2];
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar18 + 1;
    puVar25[1] = (uVar8 & 0x1fffffff) >> 2;
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar18 + 2;
    puVar25[1] = puVar14[3];
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar18 + 10;
    puVar25[1] = puVar14[5] << 0xe | puVar14[4];
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar18 + 0xb;
    puVar3 = puVar18 + 4;
    puVar25[1] = *(undefined4 *)(param_1 + 0x20);
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar18 + 0xe;
    puVar25[1] = puVar14[6];
LAB_c00313dc:
    iVar7 = param_2[2];
    uVar4 = 1;
    param_2[2] = iVar7 + 2;
    puVar25 = (undefined4 *)(iVar7 * 4 + iVar16);
    puVar25[1] = 1;
    *puVar25 = puVar3;
    break;
  case 3:
    puVar18 = *(uint **)(param_1 + 0x44);
    uVar8 = DcuGetOptimizedHgap(puVar18[5],2);
    if (uVar8 != puVar18[5]) {
      TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,s_DCU_BLOCKSCALE_c00363bc,
                  *(undefined4 *)(param_1 + 0xc),puVar18[5]);
    }
    TimingMarkF(0x400000,s__DCU_BLOCKSCALE__d__StartAddr1_0_c00363cc,iVar16,puVar18[1],puVar18[2],
                puVar18[5]);
    TimingMarkF(0x400000,s__DCU_BLOCKSCALE__d__BlkHSz__d__u_c0036410,iVar16,puVar18[3],puVar18[4],
                puVar18[6],puVar18[7]);
    TimingMarkF(0x400000,s__DCU_BLOCKSCALE__d__ulScalePatte_c0036468,iVar16,puVar18[8],puVar18[9],
                puVar18[0xb]);
    puVar3 = (uint *)DcuGetClientAddr(param_1,0);
    if (param_2 == (int *)0x0) {
      uVar9 = puVar18[1];
      uVar2 = puVar18[2];
      uVar19 = puVar18[8];
      uVar15 = puVar18[5];
      uVar11 = puVar18[9];
      uVar10 = puVar18[6];
      uVar12 = *(uint *)(param_1 + 0x20);
      uVar8 = puVar18[3];
      uVar21 = puVar18[4];
      uVar13 = puVar18[7];
      puVar3[9] = *puVar18 | puVar18[0xc] << 1;
      *puVar3 = (uVar9 & 0x1fffffff) >> 2;
      puVar3[1] = (uVar2 & 0x1fffffff) >> 2;
      puVar3[2] = uVar8;
      puVar3[0xc] = uVar19 << 0x10 | uVar11;
      puVar3[10] = uVar21;
      puVar3[0xd] = uVar15 << 0xe | uVar10;
      puVar3[0xe] = uVar13;
      puVar3[0xb] = uVar12;
LAB_c0031dc8:
      puVar3[5] = puVar18[0xb];
      puVar3[4] = 1;
      return 1;
    }
    iVar16 = *param_2;
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar3 + 9;
    puVar25[1] = *puVar18 | puVar18[0xc] << 1;
    uVar8 = puVar18[1];
    piVar17 = (int *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *piVar17 = (int)puVar3;
    piVar17[1] = (uVar8 & 0x1fffffff) >> 2;
    uVar8 = puVar18[2];
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    puVar25[1] = (uVar8 & 0x1fffffff) >> 2;
    *puVar25 = puVar3 + 1;
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar3 + 2;
    puVar25[1] = puVar18[3];
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar3 + 0xc;
    puVar25[1] = puVar18[8] << 0x10 | puVar18[9];
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar3 + 10;
    puVar25[1] = puVar18[4];
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar3 + 0xd;
    puVar25[1] = puVar18[5] << 0xe | puVar18[6];
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar3 + 0xe;
    puVar25[1] = puVar18[7];
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar3 + 0xb;
    puVar25[1] = *(undefined4 *)(param_1 + 0x20);
    if (param_2 == (int *)0x0) goto LAB_c0031dc8;
    puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
    param_2[2] = param_2[2] + 2;
    *puVar25 = puVar3 + 5;
    puVar25[1] = puVar18[0xb];
    iVar7 = param_2[2];
    puVar25 = (undefined4 *)(iVar7 * 4 + iVar16);
LAB_c0030b1c:
    puVar3 = puVar3 + 4;
    param_2[2] = iVar7 + 2;
    puVar25[1] = 1;
LAB_c0030408:
    *puVar25 = puVar3;
    goto LAB_c003040c;
  case 4:
    puVar18 = *(uint **)(param_1 + 0x44);
    uVar8 = DcuGetOptimizedHgap(puVar18[6],2);
    uVar2 = puVar18[6];
    if (uVar8 != uVar2) {
      TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,s_DCU_VSTRIP_c00364b0,
                  *(undefined4 *)(param_1 + 0xc),uVar2);
      uVar2 = puVar18[6];
    }
    TimingMarkF(0x400000,s__DCU_VSTRIP__d__StartAddress_0x__c00364bc,iVar16,puVar18[1],uVar2);
    puVar3 = (uint *)DcuGetClientAddr(param_1,0);
    if (param_2 == (int *)0x0) {
      uVar23 = puVar18[10];
      uVar19 = puVar18[0xc];
      uVar11 = puVar18[7];
      uVar21 = puVar18[8];
      uVar8 = *puVar18;
      uVar2 = puVar18[6];
      uVar9 = puVar18[9];
      uVar15 = puVar18[2];
      uVar12 = puVar18[3];
      uVar13 = puVar18[5];
      uVar10 = puVar18[4];
      uVar22 = puVar18[0xb];
      *puVar3 = (puVar18[1] & 0x1fffffff) >> 2;
      puVar3[9] = uVar8 | uVar23 << 1 | uVar19 << 3;
      puVar3[3] = uVar11 << 0xe | uVar15;
      puVar3[0xd] = uVar21 << 0xc | uVar12;
      puVar3[10] = uVar2 << 0xe | uVar13;
      puVar3[0xe] = uVar10 | uVar9 << 0xc;
      puVar3[1] = uVar22;
      uVar8 = puVar18[0xd];
    }
    else {
      uVar8 = puVar18[1];
      iVar16 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = (uVar8 & 0x1fffffff) >> 2;
      *piVar17 = (int)puVar3;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 9);
      piVar17[1] = *puVar18 | puVar18[10] << 1 | puVar18[0xc] << 3;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 3;
      puVar25[1] = puVar18[7] << 0xe | puVar18[2];
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 0xd);
      piVar17[1] = puVar18[8] << 0xc | puVar18[3];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 10;
      puVar25[1] = puVar18[6] << 0xe | puVar18[5];
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 0xe);
      piVar17[1] = puVar18[4] | puVar18[9] << 0xc;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 1);
      piVar17[1] = puVar18[0xb];
      uVar8 = puVar18[0xd];
    }
    if (uVar8 == 0) {
LAB_c0030ffc:
      if (param_2 != (int *)0x0) {
        iVar16 = *param_2;
        puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar3 + 0xb;
        puVar3 = puVar3 + 4;
        puVar25[1] = *(undefined4 *)(param_1 + 0x20);
        goto LAB_c00313dc;
      }
    }
    else {
      if (param_2 != (int *)0x0) {
        iVar16 = *param_2;
        piVar17 = (int *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar3 + 5);
        piVar17[1] = puVar18[0xe] | puVar18[0xf] << 0x10 | puVar18[0x10] << 0x14;
        piVar17 = (int *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar3 + 6);
        piVar17[1] = puVar18[0x11] | puVar18[0x12] << 0x10;
        goto LAB_c0030ffc;
      }
      uVar8 = puVar18[0x12];
      uVar2 = puVar18[0x11];
      puVar3[5] = puVar18[0xe] | puVar18[0xf] << 0x10 | puVar18[0x10] << 0x14;
      puVar3[6] = uVar2 | uVar8 << 0x10;
    }
    uVar4 = 1;
    puVar3[0xb] = *(uint *)(param_1 + 0x20);
    puVar3[4] = 1;
    break;
  case 5:
    puVar18 = *(uint **)(param_1 + 0x44);
    uVar8 = DcuGetOptimizedHgap(puVar18[6],2);
    uVar2 = puVar18[6];
    if (uVar8 != uVar2) {
      TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,s_DCU_VSTRIP_INC_c0036324,
                  *(undefined4 *)(param_1 + 0xc),uVar2);
      uVar2 = puVar18[6];
    }
    TimingMarkF(0x400000,s__DCU_VSTRIP_INC__d__StartAddress_c0036334,iVar16,puVar18[1],uVar2);
    puVar3 = (uint *)DcuGetClientAddr(param_1,0);
    if (param_2 == (int *)0x0) {
      uVar21 = puVar18[10];
      uVar12 = puVar18[7];
      uVar24 = puVar18[8];
      uVar11 = puVar18[6];
      uVar23 = puVar18[9];
      uVar8 = *puVar18;
      uVar9 = puVar18[2];
      uVar15 = puVar18[3];
      uVar10 = puVar18[5];
      uVar2 = puVar18[4];
      uVar19 = puVar18[0xb];
      uVar13 = puVar18[0x16];
      uVar22 = puVar18[0x15];
      *puVar3 = (puVar18[1] & 0x1fffffff) >> 2;
      puVar3[9] = uVar8 | uVar21 << 1;
      puVar3[3] = uVar12 << 0xe | uVar9;
      puVar3[0xd] = uVar24 << 0xc | uVar15;
      puVar3[10] = uVar11 << 0xe | uVar10;
      puVar3[0xe] = uVar2 | uVar23 << 0xc;
      puVar3[1] = uVar19;
      puVar3[5] = uVar22;
      puVar3[6] = uVar13;
    }
    else {
      uVar8 = puVar18[1];
      iVar16 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = (uVar8 & 0x1fffffff) >> 2;
      *piVar17 = (int)puVar3;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 9;
      puVar25[1] = *puVar18 | puVar18[10] << 1;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 3;
      puVar25[1] = puVar18[7] << 0xe | puVar18[2];
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 0xd);
      piVar17[1] = puVar18[8] << 0xc | puVar18[3];
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 10);
      piVar17[1] = puVar18[6] << 0xe | puVar18[5];
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 0xe);
      piVar17[1] = puVar18[4] | puVar18[9] << 0xc;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 1;
      puVar25[1] = puVar18[0xb];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 5;
      puVar25[1] = puVar18[0x15];
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 6);
      piVar17[1] = puVar18[0x16];
      if (param_2 != (int *)0x0) {
        puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar3 + 0xb;
        puVar3 = puVar3 + 4;
        puVar25[1] = *(undefined4 *)(param_1 + 0x20);
        puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        puVar25[1] = 1;
        goto LAB_c0030408;
      }
    }
    uVar4 = 1;
    puVar3[0xb] = *(uint *)(param_1 + 0x20);
    puVar3[4] = 1;
    break;
  case 6:
    puVar18 = *(uint **)(param_1 + 0x44);
    uVar8 = DcuGetOptimizedHgap(puVar18[5],2);
    uVar2 = puVar18[5];
    if (uVar8 != uVar2) {
      TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,s_DCU_VSTRIP_INC2_c0036368,
                  *(undefined4 *)(param_1 + 0xc),uVar2);
      uVar2 = puVar18[5];
    }
    TimingMarkF(0x400000,s__DCU_VSTRIP_INC2__d__StartAddres_c0036378,iVar16,puVar18[1],uVar2);
    puVar3 = (uint *)DcuGetClientAddr(param_1,0);
    if (param_2 == (int *)0x0) {
      uVar2 = puVar18[0xd];
      uVar10 = puVar18[7];
      uVar24 = puVar18[0xc];
      uVar26 = puVar18[0xe];
      uVar11 = puVar18[5];
      uVar21 = puVar18[9];
      uVar22 = puVar18[0xb];
      uVar12 = puVar18[6];
      uVar23 = puVar18[4];
      uVar13 = puVar18[8];
      uVar19 = puVar18[10];
      uVar15 = *(uint *)(param_1 + 0x20);
      uVar8 = puVar18[3];
      uVar9 = *puVar18;
      *puVar3 = (puVar18[1] & 0x1fffffff) >> 2;
      puVar3[1] = uVar8;
      puVar3[3] = uVar12 | uVar10 << 0xe;
      puVar3[9] = uVar9;
      puVar3[10] = uVar23 | uVar11 << 0xe;
      puVar3[0xb] = uVar15;
      puVar3[0xd] = uVar13 | uVar21 << 0xc;
      puVar3[0xe] = uVar19 | uVar22 << 0xc;
      puVar3[5] = uVar24 | uVar2 << 2 | uVar26 << 4;
    }
    else {
      uVar8 = puVar18[1];
      iVar16 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = (uVar8 & 0x1fffffff) >> 2;
      *piVar17 = (int)puVar3;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 1;
      puVar25[1] = puVar18[3];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 3;
      puVar25[1] = puVar18[6] | puVar18[7] << 0xe;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 9;
      puVar25[1] = *puVar18;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 10;
      puVar25[1] = puVar18[4] | puVar18[5] << 0xe;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 0xb;
      puVar25[1] = *(undefined4 *)(param_1 + 0x20);
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 0xd;
      puVar25[1] = puVar18[8] | puVar18[9] << 0xc;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 0xe;
      puVar25[1] = puVar18[10] | puVar18[0xb] << 0xc;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 5;
      puVar25[1] = puVar18[0xc] | puVar18[0xd] << 2 | puVar18[0xe] << 4;
      if (param_2 != (int *)0x0) {
        iVar7 = param_2[2];
        puVar3 = puVar3 + 4;
        param_2[2] = iVar7 + 2;
        puVar25 = (undefined4 *)(iVar7 * 4 + iVar16);
        puVar25[1] = 1;
        goto LAB_c0030408;
      }
    }
    goto LAB_c0031b68;
  case 7:
    puVar3 = *(uint **)(param_1 + 0x44);
    uVar8 = DcuGetOptimizedHgap(puVar3[6],2);
    if (uVar8 != puVar3[6]) {
      TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,s_DCU_VSTRIP_INC3_c00363ac,
                  *(undefined4 *)(param_1 + 0xc),puVar3[6]);
    }
    puVar18 = (uint *)DcuGetClientAddr(param_1,0);
    if (param_2 == (int *)0x0) {
      uVar22 = puVar3[10];
      uVar13 = puVar3[7];
      uVar8 = puVar3[8];
      uVar2 = puVar3[6];
      uVar10 = *puVar3;
      uVar9 = puVar3[2];
      uVar12 = puVar3[3];
      uVar21 = puVar3[5];
      uVar19 = *(uint *)(param_1 + 0x20);
      uVar11 = puVar3[4];
      uVar15 = puVar3[0xb];
      *puVar18 = (puVar3[1] & 0x1fffffff) >> 2;
      puVar18[9] = uVar10 | uVar22 << 1;
      puVar18[3] = uVar13 << 0xe | uVar9;
      puVar18[0xd] = uVar8 << 0xc | uVar12;
      puVar18[10] = uVar2 << 0xe | uVar21;
      puVar18[0xe] = uVar11;
      puVar18[1] = uVar15;
      puVar18[0xb] = uVar19;
    }
    else {
      uVar8 = puVar3[1];
      iVar16 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)puVar18;
      piVar17[1] = (uVar8 & 0x1fffffff) >> 2;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar18 + 9;
      puVar25[1] = *puVar3 | puVar3[10] << 1;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar18 + 3;
      puVar25[1] = puVar3[7] << 0xe | puVar3[2];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar18 + 0xd;
      puVar25[1] = puVar3[8] << 0xc | puVar3[3];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar18 + 10;
      puVar25[1] = puVar3[6] << 0xe | puVar3[5];
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar18 + 0xe);
      piVar17[1] = puVar3[4];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar18 + 1;
      puVar25[1] = puVar3[0xb];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar18 + 0xb;
      puVar25[1] = *(undefined4 *)(param_1 + 0x20);
    }
    iVar16 = 5;
    puVar14 = puVar3;
    do {
      if (param_2 == (int *)0x0) {
        puVar18[5] = puVar14[0xd];
      }
      else {
        piVar17 = (int *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar18 + 5);
        piVar17[1] = puVar14[0xd];
      }
      iVar16 = iVar16 + -1;
      puVar14 = puVar14 + 1;
    } while (-1 < iVar16);
    iVar16 = 5;
    do {
      if (param_2 == (int *)0x0) {
        puVar18[6] = puVar3[0x13];
      }
      else {
        piVar17 = (int *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar18 + 6);
        piVar17[1] = puVar3[0x13];
      }
      iVar16 = iVar16 + -1;
      puVar3 = puVar3 + 1;
    } while (-1 < iVar16);
    if (param_2 == (int *)0x0) {
      puVar18[4] = 1;
      return 1;
    }
    puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
    param_2[2] = param_2[2] + 2;
    puVar18 = puVar18 + 4;
    puVar25[1] = 1;
    goto LAB_c00316b4;
  case 8:
  case 0xc:
    puVar3 = *(uint **)(param_1 + 0x44);
    uVar9 = puVar3[2];
    uVar8 = puVar3[10];
    uVar11 = *puVar3;
    uVar2 = puVar3[0xe];
    uVar10 = puVar3[0xb];
    puVar18 = (uint *)DcuGetClientAddr(param_1,0);
    uVar8 = uVar11 | uVar9 << 1 | uVar8 << 2 | uVar2 << 3 | uVar10 << 5 | puVar3[0x12] << 6 |
            puVar3[0x14] << 7 | puVar3[0x15] << 8;
    if (param_2 == (int *)0x0) {
      uVar2 = *(uint *)(param_1 + 0x20);
      puVar18[9] = uVar8;
      puVar18[10] = uVar2;
      uVar8 = puVar3[0xb];
    }
    else {
      iVar7 = *param_2;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar7);
      param_2[2] = param_2[2] + 2;
      puVar25[1] = uVar8;
      *puVar25 = puVar18 + 9;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar7);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar18 + 10;
      puVar25[1] = *(undefined4 *)(param_1 + 0x20);
      uVar8 = puVar3[0xb];
    }
    if (uVar8 == 1) {
      if (param_2 != (int *)0x0) {
        puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar18 + 0xb;
        puVar25[1] = puVar3[6] | puVar3[0xc] << 6 | puVar3[0xd] << 0xc;
        goto LAB_c00314fc;
      }
      puVar18[0xb] = puVar3[6] | puVar3[0xc] << 6 | puVar3[0xd] << 0xc;
LAB_c0031ec0:
      uVar8 = puVar3[1];
LAB_c0031ec4:
      *puVar18 = (uVar8 & 0x1fffffff) >> 2;
      uVar2 = puVar3[2];
    }
    else {
      if (param_2 == (int *)0x0) {
        puVar18[0xb] = puVar3[6];
        uVar8 = puVar3[1];
        goto LAB_c0031ec4;
      }
      puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar18 + 0xb;
      puVar25[1] = puVar3[6];
LAB_c00314fc:
      if (param_2 == (int *)0x0) goto LAB_c0031ec0;
      uVar8 = puVar3[1];
      piVar17 = (int *)(param_2[2] * 4 + *param_2);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = (uVar8 & 0x1fffffff) >> 2;
      *piVar17 = (int)puVar18;
      uVar2 = puVar3[2];
    }
    if (uVar2 == 1) {
      uVar10 = puVar3[7];
      uVar9 = puVar3[0x10];
      uVar2 = puVar3[0x11];
      TimingMarkF(0x400000,s__DCU_MULTIVSTRIP__d__MultiMode__S_c0036780,iVar16,uVar8,uVar10,uVar9,
                  uVar2);
      if (param_2 == (int *)0x0) {
        uVar11 = puVar3[4];
        uVar8 = puVar3[5];
        puVar18[1] = puVar3[9];
        puVar18[3] = uVar11 << 7 | uVar8;
        uVar8 = puVar3[6];
      }
      else {
        iVar7 = *param_2;
        puVar25 = (undefined4 *)(param_2[2] * 4 + iVar7);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar18 + 1;
        puVar25[1] = puVar3[9];
        puVar25 = (undefined4 *)(param_2[2] * 4 + iVar7);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar18 + 3;
        puVar25[1] = puVar3[4] << 7 | puVar3[5];
        uVar8 = puVar3[6];
      }
      if (uVar8 == 1) {
LAB_c00328c0:
        uVar10 = 0;
      }
      else if (0x3fff < uVar10) {
        if (puVar3[0x16] < 2) {
LAB_c003276c:
                    /* WARNING: Subroutine does not return */
          SysHaltWithContextF(s_Os_Dcu_Submit_cpp_c0036688,0x3df,
                              s__DCU_MULTIVSTRIP__u__Multi_mode_l_c00367d4,iVar16,uVar10,
                              puVar3[0x16],puVar3[6]);
        }
        if (uVar8 == 0) {
          trap(7);
        }
        if (puVar3[0x16] % uVar8 != 0) goto LAB_c003276c;
        goto LAB_c00328c0;
      }
      uVar8 = DcuGetOptimizedHgap(uVar10,2);
      if (uVar8 != uVar10) {
        TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,
                    s_DCU_MULTIVSTRIP___MULTI_MODE_c0036580,*(undefined4 *)(param_1 + 0xc),uVar10,
                    uVar9,uVar2);
      }
      if (param_2 == (int *)0x0) {
        uVar11 = puVar3[3];
        uVar8 = puVar3[0xf];
        puVar18[0xe] = puVar3[8] << 0xe | uVar10;
        puVar18[0xd] = uVar8 | uVar11 << 0xe;
        uVar8 = puVar3[0x10];
      }
      else {
        iVar16 = *param_2;
        piVar17 = (int *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar18 + 0xe);
        piVar17[1] = puVar3[8] << 0xe | uVar10;
        puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar18 + 0xd;
        puVar25[1] = puVar3[0xf] | puVar3[3] << 0xe;
        uVar8 = puVar3[0x10];
      }
      uVar8 = DcuGetOptimizedHgap(uVar8,2);
      if (uVar8 != puVar3[0x10]) {
        TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,
                    s_DCU_MULTIVSTRIP___MULTI_MODE_FIR_c0036868,*(undefined4 *)(param_1 + 0xc),
                    puVar3[0x10],uVar9,uVar2);
      }
      if (param_2 == (int *)0x0) {
        puVar18[2] = puVar3[0x10];
        uVar8 = puVar3[0x11];
      }
      else {
        puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar18 + 2;
        puVar25[1] = puVar3[0x10];
        uVar8 = puVar3[0x11];
      }
      uVar8 = DcuGetOptimizedHgap(uVar8,2);
      if (uVar8 != puVar3[0x11]) {
        TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,
                    s_DCU_MULTIVSTRIP___MULTI_MODE_LAS_c003688c,*(undefined4 *)(param_1 + 0xc),
                    puVar3[0x11]);
      }
      if (param_2 == (int *)0x0) {
        puVar18[0xc] = puVar3[0x11];
        iVar16 = *(int *)(param_1 + 0x10);
      }
      else {
        puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar18 + 0xc;
        puVar25[1] = puVar3[0x11];
LAB_c0031678:
        iVar16 = *(int *)(param_1 + 0x10);
      }
    }
    else {
      if (param_2 == (int *)0x0) {
        puVar18[3] = puVar3[4];
        puVar18[2] = 0;
        puVar18[0xd] = 0x4001;
        uVar8 = puVar3[7];
      }
      else {
        iVar16 = *param_2;
        piVar17 = (int *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar18 + 3);
        piVar17[1] = puVar3[4];
        piVar17 = (int *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar18 + 2);
        piVar17[1] = 0;
        piVar17 = (int *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        piVar17[1] = 0x4001;
        *piVar17 = (int)(puVar18 + 0xd);
        uVar8 = puVar3[7];
      }
      uVar8 = DcuGetOptimizedHgap(uVar8,2);
      if (uVar8 != puVar3[7]) {
        TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,
                    s_DCU_MULTIVSTRIP___NON_MULTI_MODE_c003655c,*(undefined4 *)(param_1 + 0xc),
                    puVar3[7]);
      }
      if (param_2 == (int *)0x0) {
        puVar18[0xe] = puVar3[7] << 0xe;
        uVar8 = puVar3[0x12];
      }
      else {
        puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar18 + 0xe;
        puVar25[1] = puVar3[7] << 0xe;
        uVar8 = puVar3[0x12];
      }
      if (uVar8 == 1) {
        uVar8 = DcuGetOptimizedHgap(puVar3[0x13],2);
        if (uVar8 != puVar3[0x13]) {
          TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,
                      s_DCU_MULTIVSTRIP___NON_MULTI_MODE_c0036840,*(undefined4 *)(param_1 + 0xc),
                      puVar3[0x13]);
        }
        if (param_2 != (int *)0x0) {
          puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
          param_2[2] = param_2[2] + 2;
          *puVar25 = puVar18 + 0xc;
          puVar25[1] = puVar3[0x13];
LAB_c0031650:
          if (param_2 == (int *)0x0) goto LAB_c0031fbc;
          puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
          param_2[2] = param_2[2] + 2;
          *puVar25 = puVar18 + 1;
          puVar25[1] = 0;
          goto LAB_c0031678;
        }
        puVar18[0xc] = puVar3[0x13];
LAB_c0031fbc:
        puVar18[1] = 0;
        iVar16 = *(int *)(param_1 + 0x10);
      }
      else {
        if (param_2 != (int *)0x0) {
          piVar17 = (int *)(param_2[2] * 4 + *param_2);
          param_2[2] = param_2[2] + 2;
          *piVar17 = (int)(puVar18 + 0xc);
          piVar17[1] = 0;
          goto LAB_c0031650;
        }
        puVar18[0xc] = 0;
        puVar18[1] = 0;
        iVar16 = *(int *)(param_1 + 0x10);
      }
    }
    if (iVar16 == 0xc) {
      uVar8 = puVar3[0x16];
      if (uVar8 < 2) {
        if (uVar8 != 1) {
                    /* WARNING: Subroutine does not return */
          SysHaltWithContextF(s_Os_Dcu_Submit_cpp_c0036688,0x449,
                              s_Invalid_parameter__ulFieldNum_____c0036900);
        }
        if (param_2 == (int *)0x0) {
          puVar18[5] = 1;
          puVar18[4] = 1;
          return 1;
        }
        puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        puVar25[1] = 1;
        *puVar25 = puVar18 + 5;
        goto LAB_c0031688;
      }
      uVar2 = puVar3[6] % uVar8;
      if (uVar8 == 0) {
        trap(7);
      }
      uVar8 = (puVar3[6] / uVar8) * puVar3[7];
      if (0x3ffff < uVar8) {
                    /* WARNING: Subroutine does not return */
        SysHaltWithContextF(s_Os_Dcu_Submit_cpp_c0036688,0x42d,
                            s_Multi_VStrip_inc_next_strip_too_b_c00368d0);
      }
      if (param_2 == (int *)0x0) {
        uVar9 = puVar3[5];
        puVar18[5] = 0x80000000;
        puVar18[6] = uVar9 | uVar8 * 0x4000;
        uVar8 = puVar3[0x16];
      }
      else {
        iVar16 = *param_2;
        piVar17 = (int *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        piVar17[1] = -0x80000000;
        *piVar17 = (int)(puVar18 + 5);
        puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar18 + 6;
        puVar25[1] = puVar3[5] | uVar8 * 0x4000;
        uVar8 = puVar3[0x16];
      }
      uVar9 = 0;
      puVar14 = puVar3;
      if (uVar8 != 0) {
        do {
          if (param_2 == (int *)0x0) {
            *puVar18 = (puVar14[0x17] & 0x1fffffff) >> 2;
          }
          else {
            uVar8 = puVar14[0x17];
            piVar17 = (int *)(param_2[2] * 4 + *param_2);
            param_2[2] = param_2[2] + 2;
            piVar17[1] = (uVar8 & 0x1fffffff) >> 2;
            *piVar17 = (int)puVar18;
            uVar8 = puVar3[0x16];
          }
          uVar9 = uVar9 + 1;
          puVar14 = puVar14 + 1;
        } while (uVar9 < uVar8);
      }
      if (param_2 != (int *)0x0) {
        puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar18 + 5;
        puVar25[1] = puVar3[0x16] | uVar2 << 8;
        goto LAB_c0031688;
      }
      puVar18[5] = uVar8 | uVar2 << 8;
    }
    else {
LAB_c0031688:
      if (param_2 != (int *)0x0) {
        puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        puVar18 = puVar18 + 4;
        puVar25[1] = 1;
        goto LAB_c00316b4;
      }
    }
    puVar18[4] = 1;
    uVar4 = 1;
    break;
  case 9:
    puVar18 = *(uint **)(param_1 + 0x44);
    uVar11 = puVar18[9] & 0x3f | (puVar18[10] & 0x3f) << 6 | (puVar18[0xb] & 0x3f) << 0xc;
    uVar13 = puVar18[0xc] & 0xfff | (puVar18[0xe] & 0xfff) << 0xc | (puVar18[0xf] & 0x7f) << 0x18;
    uVar12 = puVar18[0xd] & 0xfff | (*(uint *)(param_1 + 0x20) & 0xf) << 0xc;
    uVar2 = puVar18[8] & 0x3ffff | (puVar18[0x10] & 0x1fff) << 0x12;
    uVar9 = (*puVar18 & 0x1fffffff) >> 2;
    uVar8 = puVar18[0x12] & 1 | (puVar18[0x13] & 1) << 3 | (puVar18[0x11] & 1) << 7 | 0x22;
    uVar10 = puVar18[1];
    puVar3 = (uint *)DcuGetClientAddr(param_1,0);
    if (param_2 == (int *)0x0) {
      uVar21 = puVar18[2];
      uVar15 = puVar18[4];
      uVar19 = puVar18[3];
      *puVar3 = uVar9;
      puVar3[1] = uVar10;
      puVar3[2] = uVar21;
      puVar3[3] = uVar11;
      puVar3[9] = uVar8;
      puVar3[10] = uVar12;
      puVar3[0xb] = uVar13;
      puVar3[0xc] = uVar15;
      puVar3[0xd] = uVar2;
      puVar3[0xe] = uVar19;
    }
    else {
      iVar16 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)puVar3;
      piVar17[1] = uVar9;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      puVar25[1] = uVar10;
      *puVar25 = puVar3 + 1;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 2;
      puVar25[1] = puVar18[2];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      puVar25[1] = uVar11;
      *puVar25 = puVar3 + 3;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      puVar25[1] = uVar8;
      *puVar25 = puVar3 + 9;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      puVar25[1] = uVar12;
      *puVar25 = puVar3 + 10;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      puVar25[1] = uVar13;
      *puVar25 = puVar3 + 0xb;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 0xc;
      puVar25[1] = puVar18[4];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      puVar25[1] = uVar2;
      *puVar25 = puVar3 + 0xd;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 0xe;
      puVar25[1] = puVar18[3];
      if (param_2 != (int *)0x0) {
        puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar3 + 0xc000;
        puVar25[1] = puVar18[5];
        puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar3 + 0xc001;
        puVar25[1] = puVar18[6];
        puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *puVar25 = puVar3 + 0xc002;
        puVar25[1] = puVar18[7];
        iVar7 = param_2[2];
        puVar25 = (undefined4 *)(iVar7 * 4 + iVar16);
        goto LAB_c0030b1c;
      }
    }
    uVar8 = puVar18[7];
    uVar2 = puVar18[6];
    uVar4 = 1;
    puVar3[0xc000] = puVar18[5];
    puVar3[0xc001] = uVar2;
    puVar3[0xc002] = uVar8;
    puVar3[4] = 1;
    break;
  case 10:
    puVar18 = *(uint **)(param_1 + 0x44);
    uVar2 = puVar18[1];
    uVar8 = DcuGetOptimizedHgap(puVar18[5],2);
    if (uVar8 != puVar18[5]) {
      TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,s_DCU_MULTIBLOCK_c00364ec,
                  *(undefined4 *)(param_1 + 0xc),puVar18[5]);
    }
    puVar3 = (uint *)DcuGetClientAddr(param_1,0);
    iVar7 = 0;
    if (((uVar2 == 8) && (((puVar18[8] & 0xf) != 0 || ((puVar18[9] & 0xf) != 0)))) &&
       ((iVar7 = 1, 0x7ffff < puVar18[8] || (0x7ffffff < puVar18[9])))) {
                    /* WARNING: Subroutine does not return */
      __assfail(s_Failed_assertion___s__at_line__d_c003665c,
                s_multiBlockParams_>ulBlockJump_<_b_c00365f4,0x4d1,s_Os_Dcu_Submit_cpp_c0036688);
    }
    if (param_2 == (int *)0x0) {
      uVar8 = puVar18[2];
      uVar9 = *(uint *)(param_1 + 0x20);
      puVar3[9] = *puVar18 | puVar18[0xb] << 1 | puVar18[10] | uVar2 |
                  iVar7 << 4 | puVar18[0xe] << 5;
      *puVar3 = (uVar8 & 0x1fffffff) >> 2;
      puVar3[4] = uVar9;
    }
    else {
      iVar20 = *param_2;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar20);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 9;
      puVar25[1] = *puVar18 | puVar18[0xb] << 1 | puVar18[10] | uVar2 |
                   iVar7 << 4 | puVar18[0xe] << 5;
      uVar8 = puVar18[2];
      piVar17 = (int *)(param_2[2] * 4 + iVar20);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = (uVar8 & 0x1fffffff) >> 2;
      *piVar17 = (int)puVar3;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar20);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 4;
      puVar25[1] = *(undefined4 *)(param_1 + 0x20);
    }
    if (uVar2 == 0) {
      uVar9 = puVar18[5];
      uVar2 = puVar18[8];
      pcVar5 = s__DCU_MULTIBLOCK__d__Horizontal_s_c00366b8;
LAB_c003211c:
      TimingMarkF(0x400000,pcVar5,iVar16,uVar8,uVar9,uVar2,puVar18[9]);
      if (param_2 == (int *)0x0) {
        uVar8 = puVar18[9];
        puVar3[5] = puVar18[8];
        puVar3[6] = uVar8;
LAB_c0031e34:
        uVar8 = puVar18[6];
        goto LAB_c0031e38;
      }
      iVar16 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 5);
      piVar17[1] = puVar18[8];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 6;
      puVar25[1] = puVar18[9];
LAB_c00311d0:
      if (param_2 == (int *)0x0) goto LAB_c0031e34;
      piVar17 = (int *)(param_2[2] * 4 + *param_2);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 0xb);
      piVar17[1] = puVar18[6] << 0xb | puVar18[7];
      uVar8 = puVar18[0xe];
    }
    else {
      if (iVar7 == 1) {
        uVar9 = puVar18[5];
        uVar2 = puVar18[8];
        pcVar5 = s__DCU_MULTIBLOCK__d__vertical_no_s_c003671c;
        goto LAB_c003211c;
      }
      TimingMarkF(0x400000,s__DCU_MULTIBLOCK__d__vertical_sca_c00364fc,iVar16,uVar8,puVar18[5],
                  puVar18[8] >> 4,puVar18[9] >> 4);
      if (param_2 != (int *)0x0) {
        iVar16 = *param_2;
        piVar17 = (int *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar3 + 5);
        piVar17[1] = puVar18[8] >> 4;
        piVar17 = (int *)(param_2[2] * 4 + iVar16);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar3 + 6);
        piVar17[1] = puVar18[9] >> 4;
        goto LAB_c00311d0;
      }
      uVar8 = puVar18[9];
      puVar3[5] = puVar18[8] >> 4;
      puVar3[6] = uVar8 >> 4;
      uVar8 = puVar18[6];
LAB_c0031e38:
      puVar3[0xb] = uVar8 << 0xb | puVar18[7];
      uVar8 = puVar18[0xe];
    }
    if (uVar8 == 1) {
      if (param_2 != (int *)0x0) {
        piVar17 = (int *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar3 + 2);
        piVar17[1] = puVar18[3] | puVar18[0xc] << 8;
        goto LAB_c0031244;
      }
      puVar3[2] = puVar18[3] | puVar18[0xc] << 8;
    }
    else if (param_2 == (int *)0x0) {
      puVar3[2] = puVar18[3];
    }
    else {
      piVar17 = (int *)(param_2[2] * 4 + *param_2);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 2);
      piVar17[1] = puVar18[3];
LAB_c0031244:
      uVar8 = puVar18[0xe];
    }
    if (uVar8 == 1) {
      if (param_2 == (int *)0x0) {
        puVar3[10] = puVar18[4] | puVar18[0xd] << 7 | puVar18[5] << 0xe;
        uVar4 = 1;
      }
      else {
        piVar17 = (int *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        *piVar17 = (int)(puVar3 + 10);
        uVar4 = 1;
        piVar17[1] = puVar18[4] | puVar18[0xd] << 7 | puVar18[5] << 0xe;
      }
    }
    else if (param_2 == (int *)0x0) {
      puVar3[10] = puVar18[4] | puVar18[5] << 0xe;
      uVar4 = 1;
    }
    else {
      puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 10;
      uVar4 = 1;
      puVar25[1] = puVar18[4] | puVar18[5] << 0xe;
    }
    break;
  case 0xb:
    puVar18 = *(uint **)(param_1 + 0x44);
    uVar8 = DcuGetOptimizedHgap(puVar18[3],2);
    if (uVar8 != puVar18[3]) {
      TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,
                  s_DCU_MULTI_SINGLE_BLOCK_c00365dc,*(undefined4 *)(param_1 + 0xc),puVar18[3]);
    }
    puVar3 = (uint *)DcuGetClientAddr(param_1,0);
    if (param_2 == (int *)0x0) {
      uVar2 = *puVar18;
      uVar10 = puVar18[7];
      uVar13 = puVar18[10];
      uVar11 = puVar18[0xd];
      uVar8 = puVar18[6];
      uVar21 = puVar18[9];
      uVar22 = puVar18[8];
      uVar9 = puVar18[0xb];
      uVar12 = puVar18[0xc];
      uVar19 = puVar18[3];
      uVar15 = puVar18[4];
      *puVar3 = puVar18[1] | puVar18[2] << 6 | puVar18[5] << 0xc;
      uVar4 = 1;
      puVar3[2] = uVar8 | uVar10 << 6 | uVar22 << 0xc;
      puVar3[10] = uVar21 | uVar13 << 6 | uVar9 << 0xc;
      puVar3[0xb] = uVar12 | uVar11 << 6 | uVar19 << 0xc;
      puVar3[0xc] = ((uVar2 & 0x1fffffff) >> 2) << 4 | uVar15;
    }
    else {
      iVar16 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)puVar3;
      piVar17[1] = puVar18[1] | puVar18[2] << 6 | puVar18[5] << 0xc;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 2;
      puVar25[1] = puVar18[6] | puVar18[7] << 6 | puVar18[8] << 0xc;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 10;
      puVar25[1] = puVar18[9] | puVar18[10] << 6 | puVar18[0xb] << 0xc;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 0xb;
      puVar25[1] = puVar18[0xc] | puVar18[0xd] << 6 | puVar18[3] << 0xc;
      uVar8 = *puVar18;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 0xc;
      uVar4 = 1;
      puVar25[1] = ((uVar8 & 0x1fffffff) >> 2) << 4 | puVar18[4];
    }
    break;
  case 0xd:
    puVar18 = *(uint **)(param_1 + 0x44);
    uVar13 = puVar18[10];
    uVar10 = puVar18[0xb] & 0xffffff;
    uVar9 = puVar18[0xe];
    uVar8 = puVar18[0xc] & 0x3fff | (puVar18[0xd] & 0xfff) << 0xe;
    uVar2 = puVar18[4] & 1 | (puVar18[5] & 1) << 2 | (puVar18[6] & 1) << 3 | (puVar18[8] & 1) << 7 |
            2;
    uVar15 = puVar18[2] & 0x7f | (puVar18[3] & 0x7f) << 7;
    uVar11 = (*puVar18 & 0x1fffffff) >> 2;
    uVar12 = puVar18[1];
    if (0xffffff < uVar10) {
                    /* WARNING: Subroutine does not return */
      SysHaltWithContextF(s_Os_Dcu_Submit_cpp_c0036688,0x48e,s_Multi_VStrip3_hGap_too_large_c00368b0
                         );
    }
    uVar19 = DcuGetOptimizedHgap(uVar10,2);
    if (uVar19 != uVar10) {
      TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,
                  s_DCU_MULTIVSTRIP___MULTI_MODE_c0036580,*(undefined4 *)(param_1 + 0xc),uVar10);
    }
    puVar3 = (uint *)DcuGetClientAddr(param_1,0);
    if (param_2 == (int *)0x0) {
      *puVar3 = uVar11;
      puVar3[1] = uVar12;
      puVar3[3] = uVar15;
      puVar3[9] = uVar2;
      puVar3[0xb] = uVar13 & 0x3fff;
      puVar3[0xc] = uVar10;
      puVar3[0xd] = uVar8;
      puVar3[0xe] = uVar9;
LAB_c0031e14:
      puVar3[10] = *(uint *)(param_1 + 0x20);
      uVar8 = *puVar18;
    }
    else {
      iVar7 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar7);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = uVar11;
      *piVar17 = (int)puVar3;
      piVar17 = (int *)(param_2[2] * 4 + iVar7);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = uVar12;
      *piVar17 = (int)(puVar3 + 1);
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar7);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 3;
      puVar25[1] = uVar15;
      piVar17 = (int *)(param_2[2] * 4 + iVar7);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = uVar2;
      *piVar17 = (int)(puVar3 + 9);
      piVar17 = (int *)(param_2[2] * 4 + iVar7);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = uVar13 & 0x3fff;
      *piVar17 = (int)(puVar3 + 0xb);
      piVar17 = (int *)(param_2[2] * 4 + iVar7);
      param_2[2] = param_2[2] + 2;
      *piVar17 = (int)(puVar3 + 0xc);
      piVar17[1] = uVar10;
      piVar17 = (int *)(param_2[2] * 4 + iVar7);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = uVar8;
      *piVar17 = (int)(puVar3 + 0xd);
      iVar20 = param_2[2];
      param_2[2] = iVar20 + 2;
      piVar17 = (int *)(iVar20 * 4 + iVar7);
      *piVar17 = (int)(puVar3 + 0xe);
      piVar17[1] = uVar9;
      if (param_2 == (int *)0x0) goto LAB_c0031e14;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar7);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 10;
      puVar25[1] = *(undefined4 *)(param_1 + 0x20);
      uVar8 = *puVar18;
    }
    TimingMarkF(0x400000,s__DCU_MULTIVSTRIP3__d__MultiMode__c00365a0,iVar16,uVar8,uVar10);
    puVar18 = puVar3 + 4;
    if (param_2 == (int *)0x0) {
      puVar3[4] = 1;
      return 1;
    }
    iVar16 = param_2[2];
    param_2[2] = iVar16 + 2;
    puVar25 = (undefined4 *)(iVar16 * 4 + *param_2);
    puVar25[1] = 1;
LAB_c00316b4:
    *puVar25 = puVar18;
LAB_c003040c:
    uVar4 = 1;
    break;
  case 0xe:
    puVar18 = *(uint **)(param_1 + 0x44);
    uVar8 = DcuGetOptimizedHgap(puVar18[6],2);
    uVar2 = puVar18[6];
    if (uVar8 != uVar2) {
      TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,s_DCU_VSTRIP_V_DEINT_c0036298
                  ,*(undefined4 *)(param_1 + 0xc),uVar2);
      uVar2 = puVar18[6];
    }
    TimingMarkF(0x400000,s__DCU_VSTRIP_V_DEINT__d__StartAdd_c00362ac,iVar16,puVar18[1],uVar2);
    puVar3 = (uint *)DcuGetClientAddr(param_1,0);
    if (param_2 == (int *)0x0) {
      uVar11 = puVar18[10];
      uVar21 = puVar18[7];
      uVar23 = puVar18[8];
      uVar13 = puVar18[9];
      uVar8 = puVar18[6];
      uVar15 = *puVar18;
      uVar9 = puVar18[2];
      uVar10 = puVar18[3];
      uVar19 = puVar18[5];
      uVar2 = puVar18[4];
      uVar22 = puVar18[0xb];
      uVar24 = *(uint *)(param_1 + 0x20);
      uVar12 = puVar18[0xc];
      *puVar3 = (puVar18[1] & 0x1fffffff) >> 2;
      puVar3[9] = uVar15 | uVar11 << 1;
      puVar3[3] = uVar21 << 0xe | uVar9;
      puVar3[0xd] = uVar23 << 0xc | uVar10;
      puVar3[10] = uVar8 << 0xe | uVar19;
      puVar3[0xe] = uVar2 | uVar13 << 0xc;
      puVar3[1] = uVar22;
      puVar3[0xb] = uVar24;
      puVar3[5] = uVar12;
    }
    else {
      uVar8 = puVar18[1];
      iVar16 = *param_2;
      piVar17 = (int *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      piVar17[1] = (uVar8 & 0x1fffffff) >> 2;
      *piVar17 = (int)puVar3;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 9;
      puVar25[1] = *puVar18 | puVar18[10] << 1;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 3;
      puVar25[1] = puVar18[7] << 0xe | puVar18[2];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 0xd;
      puVar25[1] = puVar18[8] << 0xc | puVar18[3];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 10;
      puVar25[1] = puVar18[6] << 0xe | puVar18[5];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 0xe;
      puVar25[1] = puVar18[4] | puVar18[9] << 0xc;
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 1;
      puVar25[1] = puVar18[0xb];
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 0xb;
      puVar25[1] = *(undefined4 *)(param_1 + 0x20);
      puVar25 = (undefined4 *)(param_2[2] * 4 + iVar16);
      param_2[2] = param_2[2] + 2;
      *puVar25 = puVar3 + 5;
      puVar25[1] = puVar18[0xc];
      uVar12 = puVar18[0xc];
    }
    if (1 < uVar12) {
      if (param_2 == (int *)0x0) {
        puVar3[2] = 1;
      }
      else {
        puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
        param_2[2] = param_2[2] + 2;
        puVar25[1] = 1;
        *puVar25 = puVar3 + 2;
        uVar12 = puVar18[0xc];
      }
      uVar8 = 0;
      puVar14 = puVar18;
      if (uVar12 != 0) {
        do {
          uVar2 = DcuGetOptimizedHgap(puVar14[0xd],2);
          if (uVar2 != puVar14[0xd]) {
            TimingMarkF(0x800000,s__HGAP_VIOLATION_of__s____d____HG_c0036270,
                        s_DCU_VSTRIP_INC___INTERLACE_c00362e4,*(undefined4 *)(param_1 + 0xc),
                        puVar14[0xd]);
          }
          if (param_2 == (int *)0x0) {
            puVar3[6] = puVar14[0xd] & 0x7ffffff;
            uVar2 = puVar18[0xc];
          }
          else {
            puVar25 = (undefined4 *)(param_2[2] * 4 + *param_2);
            param_2[2] = param_2[2] + 2;
            *puVar25 = puVar3 + 6;
            puVar25[1] = puVar14[0xd] & 0x7ffffff;
            uVar2 = puVar18[0xc];
          }
          uVar8 = uVar8 + 1;
          puVar14 = puVar14 + 1;
        } while (uVar8 < uVar2);
      }
    }
    puVar18 = puVar3 + 4;
    if (param_2 != (int *)0x0) {
      iVar16 = param_2[2];
      param_2[2] = iVar16 + 2;
      puVar25 = (undefined4 *)(iVar16 * 4 + *param_2);
      puVar25[1] = 1;
      goto LAB_c00316b4;
    }
LAB_c0031b68:
    uVar4 = 1;
    puVar3[4] = 1;
  }
  return uVar4;
}


//==== DcuClientSubmitToCbc @ c0032b60

bool DcuClientSubmitToCbc(int param_1)

{
  int iVar1;
  
  if (param_1 != 0) {
    iVar1 = DcuClientSubmitEx_tagSDCUCLIENTOBJ__();
    return iVar1 == 1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Submit_cpp_c0036688,0x61,s_Invalid_DCU_client_handle__c003669c);
}


//==== DcuClientSubmit @ c0032ba0

bool DcuClientSubmit(int param_1)

{
  int iVar1;
  
  if (param_1 != 0) {
    iVar1 = DcuClientSubmitEx_tagSDCUCLIENTOBJ__(param_1,0);
    return iVar1 == 1;
  }
                    /* WARNING: Subroutine does not return */
  SysHaltWithContextF(s_Os_Dcu_Submit_cpp_c0036688,0x59,s_Invalid_DCU_client_handle__c003669c);
}


//==== DcuClientResubmit @ c0032be4

undefined4 DcuClientResubmit(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  if (param_1 == 0) {
                    /* WARNING: Subroutine does not return */
    SysHaltWithContextF(s_Os_Dcu_Submit_cpp_c0036688,0x588,s_Invalid_DCU_client_handle__c003669c);
  }
  iVar1 = DcuGetClientAddr(param_1,0);
  switch(*(undefined4 *)(param_1 + 0x10)) {
  case 0:
  case 2:
  case 3:
  case 8:
    *(undefined4 *)(iVar1 + 0x10) = 1;
  case 1:
  case 5:
  case 7:
  case 10:
  case 0xb:
    uVar2 = DcuClientSubmit(param_1);
    return uVar2;
  case 4:
  case 6:
  case 9:
  default:
    return 0;
  }
}


//==== DcuClientStartEx(tagSDCUCLIENTOBJ*, @ c0033e88

/* WARNING: Control flow encountered bad instruction data */

void DcuClientStartEx_tagSDCUCLIENTOBJ__(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


//==== DcuSstlCal1SetCalibrationInterval::__func__ @ c0035428

/* WARNING: Control flow encountered unimplemented instructions */

void DcuSstlCal1SetCalibrationInterval____func__(void)

{
                    /* WARNING: Unimplemented instruction - Truncating control flow here */
  halt_unimplemented();
}


//==== DcuSstlCal1SetContinuousCalibration::__func__ @ c003544c

/* WARNING: Control flow encountered unimplemented instructions */

void DcuSstlCal1SetContinuousCalibration____func__(void)

{
                    /* WARNING: Unimplemented instruction - Truncating control flow here */
  halt_unimplemented();
}


//==== DcuSstlCal1SetCalibrationDuringRefresh::__func__ @ c0035470

/* WARNING: Control flow encountered unimplemented instructions */

void DcuSstlCal1SetCalibrationDuringRefresh____func__(void)

{
                    /* WARNING: Unimplemented instruction - Truncating control flow here */
  halt_unimplemented();
}


//==== DcuSstlCal1IsReadyToAcceptUpdatedResults::__func__ @ c0035498

/* WARNING: Control flow encountered unimplemented instructions */

void DcuSstlCal1IsReadyToAcceptUpdatedResults____func__(void)

{
                    /* WARNING: Unimplemented instruction - Truncating control flow here */
  halt_unimplemented();
}


//==== DcuSstlCal1GetResults::__func__ @ c00354c4

/* WARNING: Control flow encountered unimplemented instructions */

void DcuSstlCal1GetResults____func__(void)

{
                    /* WARNING: Unimplemented instruction - Truncating control flow here */
  halt_unimplemented();
}


//==== DcuSstlCal1SetSstlValid::__func__ @ c00354dc

/* WARNING: Control flow encountered unimplemented instructions */

void DcuSstlCal1SetSstlValid____func__(void)

{
                    /* WARNING: Unimplemented instruction - Truncating control flow here */
  halt_unimplemented();
}


//==== DcuSstlCal1SetOutputBufferDriveStrength::__func__ @ c00354f4

/* WARNING: Control flow encountered unimplemented instructions */

void DcuSstlCal1SetOutputBufferDriveStrength____func__(void)

{
                    /* WARNING: Unimplemented instruction - Truncating control flow here */
  halt_unimplemented();
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


