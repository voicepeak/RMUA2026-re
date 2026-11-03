// ADDRESS 026ea000

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_026ea000(undefined4 *param_1)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  ushort auStack_28 [8];
  undefined8 uStack_18;
  
  if (param_1 == (undefined4 *)0x0) {
    return;
  }
  auStack_28[0] = 0;
  lVar1 = func_0x026e89c0();
  if ((lVar1 != 0) && (lVar1 = func_0x05703420(lVar1), lVar1 != 0)) {
    lVar2 = func_0x026c84e0();
    if ((*(int *)(lVar2 + 0x38) <= *(int *)(*(long *)(lVar1 + 0x10) + 0x38)) &&
       (((*(long *)(*(long *)(*(long *)(lVar1 + 0x10) + 0x30) + (long)*(int *)(lVar2 + 0x38) * 8) ==
          lVar2 + 0x30 && (*(long *)(lVar1 + 800) != 0)) &&
        (plVar3 = *(long **)(*(long *)(lVar1 + 800) + 0x378), plVar3 != (long *)0x0)))) {
      plVar3 = (long *)(**(code **)(*plVar3 + 0x10))(plVar3,auStack_28);
      if ((auStack_28[0] & 1) != 0) {
        _ZdlPv(uStack_18);
      }
      if (plVar3 != (long *)0x0) {
        (**(code **)(*plVar3 + 0x1d8))
                  (*param_1,param_1[1],param_1[2],param_1[3],_DAT_006c12c8,plVar3);
        goto LAB_026ea05d;
      }
    }
  }
  if (2 < DAT_0722748c) {
    func_0x026e90b0();
  }
LAB_026ea05d:
  _ZdlPv(param_1);
  return;
}



// ADDRESS 026ea0f0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_026ea0f0(float *param_1)

{
  long lVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  undefined8 uStack_28;
  float *pfStack_20;
  undefined1 auStack_18 [8];
  
  if (((_DAT_0744e0d0 != 0) && (param_1 != (float *)0x0)) &&
     (lVar1 = *(long *)(_DAT_0744e0d0 + 0x180), lVar1 != 0)) {
    if (2 < DAT_0722748c) {
      func_0x026ea230();
    }
    iVar5 = _DAT_0715c400;
    if (_DAT_0715c400 == 0) {
      if (((((param_1[2] != 0.0) || (NAN(param_1[2]))) ||
           ((*param_1 != 0.0 || ((NAN(*param_1) || (param_1[1] != 0.0)))))) || (NAN(param_1[1]))) ||
         ((param_1[3] != 0.0 || (NAN(param_1[3]))))) {
        _DAT_0715c400 = 2;
        iVar5 = 2;
      }
      else {
        iVar5 = 0;
      }
      if (2 < DAT_0722748c) {
        func_0x026ea260();
        iVar5 = _DAT_0715c400;
      }
    }
    if (iVar5 == 2) {
      if (2 < DAT_0722748c) {
        func_0x026ea290();
      }
      pfStack_20 = (float *)_Znwm(0x10);
      fVar2 = param_1[1];
      fVar3 = param_1[2];
      fVar4 = param_1[3];
      *pfStack_20 = *param_1;
      pfStack_20[1] = fVar2;
      pfStack_20[2] = fVar3;
      pfStack_20[3] = fVar4;
      lVar6 = func_0x026fd930();
      lVar7 = 0;
      if (*(long *)(*(long *)(*(long *)(lVar1 + 0x10) + 0x30) + (long)*(int *)(lVar6 + 0x38) * 8) ==
          lVar6 + 0x30) {
        lVar7 = lVar1;
      }
      uStack_28 = 0;
      func_0x026ea2c0(auStack_18,*(undefined8 *)(lVar7 + 0x1a8),selected_026ea000,&pfStack_20,
                      &uStack_28);
      _ZNSt3__16futureIvED1Ev(auStack_18);
    }
  }
  return;
}



// ADDRESS 026f06f0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_026f06f0(undefined8 *param_1,undefined8 *param_2)

{
  long lVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  long lVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  long lVar10;
  undefined8 uStack_40;
  undefined4 *puStack_38;
  undefined1 auStack_30 [8];
  
  if (param_1 == (undefined8 *)0x0) {
    return;
  }
  uVar9 = *param_1;
  iVar5 = strcmp(&UNK_00ae75f0,uVar9);
  if (iVar5 == 0) {
    func_0x026e8c70(param_1[1],param_2);
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00a61db7,uVar9);
  if (iVar5 == 0) {
    func_0x026e9370(param_1[1],param_2);
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00b77d00,uVar9);
  if (iVar5 == 0) {
    func_0x026e9460();
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00a8a911,uVar9);
  if ((param_2 != (undefined8 *)0x0) && (iVar5 == 0)) {
    *param_2 = 0;
    param_2[1] = 0;
    *(undefined4 *)(param_2 + 2) = _DAT_0715c3d0;
    *(undefined4 *)((long)param_2 + 0x14) = _DAT_0715c3d4;
    *(undefined4 *)(param_2 + 3) = _DAT_0715c3d8;
  }
  iVar5 = strcmp(&UNK_00a0370c,uVar9);
  if (iVar5 == 0) {
    func_0x026e96c0(param_1[1]);
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00b36ac3);
  if ((((iVar5 == 0) && (_DAT_0744e0d0 != 0)) &&
      (puVar2 = (undefined4 *)param_1[1], puVar2 != (undefined4 *)0x0)) &&
     (lVar1 = *(long *)(_DAT_0744e0d0 + 0x180), lVar1 != 0)) {
    puStack_38 = (undefined4 *)_Znwm(0x10);
    uVar6 = puVar2[1];
    uVar3 = puVar2[2];
    uVar4 = puVar2[3];
    *puStack_38 = *puVar2;
    puStack_38[1] = uVar6;
    puStack_38[2] = uVar3;
    puStack_38[3] = uVar4;
    lVar7 = func_0x026fd930();
    lVar10 = 0;
    if (*(long *)(*(long *)(*(long *)(lVar1 + 0x10) + 0x30) + (long)*(int *)(lVar7 + 0x38) * 8) ==
        lVar7 + 0x30) {
      lVar10 = lVar1;
    }
    uStack_40 = 0;
    func_0x026e9ad0(auStack_30,*(undefined8 *)(lVar10 + 0x1a8),&UNK_026e99e0,&puStack_38,&uStack_40)
    ;
    _ZNSt3__16futureIvED1Ev(auStack_30);
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00accaea);
  if (((iVar5 == 0) && (_DAT_0744e0d0 != 0)) &&
     ((puVar2 = (undefined4 *)param_1[1], puVar2 != (undefined4 *)0x0 &&
      (lVar1 = *(long *)(_DAT_0744e0d0 + 0x180), lVar1 != 0)))) {
    puStack_38 = (undefined4 *)_Znwm(0x10);
    uVar6 = puVar2[1];
    uVar3 = puVar2[2];
    uVar4 = puVar2[3];
    *puStack_38 = *puVar2;
    puStack_38[1] = uVar6;
    puStack_38[2] = uVar3;
    puStack_38[3] = uVar4;
    lVar7 = func_0x026fd930();
    lVar10 = 0;
    if (*(long *)(*(long *)(*(long *)(lVar1 + 0x10) + 0x30) + (long)*(int *)(lVar7 + 0x38) * 8) ==
        lVar7 + 0x30) {
      lVar10 = lVar1;
    }
    uStack_40 = 0;
    func_0x026e9de0(auStack_30,*(undefined8 *)(lVar10 + 0x1a8),&UNK_026e9cf0,&puStack_38,&uStack_40)
    ;
    _ZNSt3__16futureIvED1Ev(auStack_30);
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_009f63fd,uVar9);
  if (iVar5 == 0) {
    selected_026ea0f0(param_1[1]);
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00b77d08,uVar9);
  if (iVar5 == 0) {
    func_0x026ea4e0();
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00b77d19,uVar9);
  if (iVar5 == 0) {
    func_0x026ea640(param_1[1],param_2);
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00b93188,uVar9);
  if (iVar5 == 0) {
    func_0x026ea8e0(param_1[1],param_2);
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00a1f3c3,uVar9);
  if (iVar5 == 0) {
    func_0x026eabb0();
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00af50a0,uVar9);
  if (iVar5 == 0) {
    func_0x026eaef0();
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00a1f3d2,uVar9);
  if (iVar5 == 0) {
    func_0x026eb0a0();
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00ab255e,uVar9);
  if ((param_2 != (undefined8 *)0x0) && (iVar5 == 0)) {
    if (_DAT_07142fa0 == 2) {
      *(undefined4 *)param_2 = _DAT_0715c410;
      *(undefined4 *)((long)param_2 + 4) = _DAT_0715c414;
      uVar6 = _DAT_0715c418;
    }
    else {
      if (_DAT_07142fa0 != 1) goto LAB_026f0a6d;
      lVar1 = *(long *)(_DAT_0715c440 + 0x18);
      *(undefined4 *)param_2 = *(undefined4 *)(lVar1 + 0xc);
      *(undefined4 *)((long)param_2 + 4) = *(undefined4 *)(lVar1 + 0x10);
      uVar6 = *(undefined4 *)(lVar1 + 0x14);
    }
    *(undefined4 *)(param_2 + 1) = uVar6;
  }
LAB_026f0a6d:
  iVar5 = strcmp(&UNK_00b0ec65,uVar9);
  if (((iVar5 == 0) && (param_2 != (undefined8 *)0x0)) && (_DAT_07142fa0 == 1)) {
    *(undefined4 *)param_2 = *(undefined4 *)(*(long *)(_DAT_0715c440 + 0x18) + 8);
  }
  iVar5 = strcmp(&UNK_00b6a9a5,uVar9);
  if (iVar5 == 0) {
    func_0x026eb280(param_1[1]);
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00b51166,uVar9);
  if (iVar5 == 0) {
    func_0x026eb580();
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00b36ad1,uVar9);
  if (((iVar5 == 0) && (puVar2 = (undefined4 *)param_1[1], puVar2 != (undefined4 *)0x0)) &&
     ((_DAT_0744e0d0 != 0 && (*(long *)(_DAT_0744e0d0 + 0x180) != 0)))) {
    puVar8 = (undefined8 *)_Znwm(0x18);
    puVar8[1] = 0x20;
    *puVar8 = 0xbbf448;
    *(undefined4 *)(puVar8 + 2) = *puVar2;
    *(undefined1 *)((long)puVar8 + 0x14) = *(undefined1 *)(puVar2 + 2);
    func_0x026fd930();
    if (_DAT_0715c430 != (code *)0x0) {
      (*_DAT_0715c430)(puVar8);
    }
  }
  uVar9 = *param_1;
  iVar5 = strcmp(&UNK_00aa4959,uVar9);
  if (iVar5 == 0) {
    func_0x026eb860();
    uVar9 = *param_1;
  }
  iVar5 = strcmp(&UNK_00b28bdb,uVar9);
  if (((iVar5 == 0) && (puVar2 = (undefined4 *)param_1[1], puVar2 != (undefined4 *)0x0)) &&
     ((_DAT_0744e0d0 != 0 && (*(long *)(_DAT_0744e0d0 + 0x180) != 0)))) {
    puVar8 = (undefined8 *)_Znwm(0x18);
    puVar8[1] = 0x21;
    *puVar8 = 0xbbf470;
    *(undefined4 *)(puVar8 + 2) = *puVar2;
    *(undefined4 *)((long)puVar8 + 0x14) = puVar2[2];
    func_0x026fd930();
    if (_DAT_0715c430 != (code *)0x0) {
      (*_DAT_0715c430)(puVar8);
    }
  }
  uVar9 = *param_1;
  iVar5 = strcmp(&UNK_00a8a91c,uVar9);
  if (((iVar5 == 0) && (param_2 != (undefined8 *)0x0)) && (_DAT_07142fa0 == 2)) {
    *(undefined1 *)param_2 = DAT_0715c41c;
  }
  iVar5 = strcmp(&UNK_00b43c8d,uVar9);
  if (((iVar5 == 0) && (_DAT_0744e0d0 != 0)) && (*(long *)(_DAT_0744e0d0 + 0x180) != 0)) {
    puVar8 = (undefined8 *)_Znwm(0x10);
    puVar8[1] = 0x22;
    *puVar8 = 0xbbf498;
    func_0x026fd930();
    if (_DAT_0715c430 != (code *)0x0) {
      (*_DAT_0715c430)(puVar8);
    }
  }
  iVar5 = strcmp(&UNK_00b51166,*param_1);
  if (iVar5 == 0) {
    puVar8 = (undefined8 *)_Znwm(0x20);
    *puVar8 = 0xbbfff8;
    *(undefined4 *)(puVar8 + 3) = 0;
    uVar4 = _UNK_006cb17c;
    uVar3 = _UNK_006cb178;
    uVar6 = _UNK_006cb174;
    *(undefined4 *)(puVar8 + 1) = _DAT_006cb170;
    *(undefined4 *)((long)puVar8 + 0xc) = uVar6;
    *(undefined4 *)(puVar8 + 2) = uVar3;
    *(undefined4 *)((long)puVar8 + 0x14) = uVar4;
    if (((_DAT_0744e0d0 != 0) && (*(long *)(_DAT_0744e0d0 + 0x180) != 0)) &&
       (func_0x026fd930(), _DAT_0715c430 != (code *)0x0)) {
      (*_DAT_0715c430)(puVar8);
    }
  }
  return;
}



// ADDRESS 0612efc0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong _ZN3msr6airlib17MultirotorApiBase15moveByMotorPWMsEfffff
                (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                float param_5,long *param_6)

{
  long *plVar1;
  long *plVar2;
  char cVar3;
  undefined8 *puVar4;
  long lVar5;
  ulong uVar6;
  undefined1 auVar7 [16];
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined4 uStack_b0;
  undefined4 uStack_ac;
  undefined8 uStack_a8;
  long *plStack_a0;
  long alStack_98 [4];
  long *plStack_78;
  undefined1 auStack_60 [8];
  double dStack_58;
  char cStack_48;
  long lStack_40;
  
  plVar1 = param_6 + 7;
  uStack_b8 = param_4;
  uStack_b4 = param_3;
  uStack_b0 = param_2;
  uStack_ac = param_1;
  cVar3 = _ZNSt3__115recursive_mutex8try_lockEv(plVar1);
  if (cVar3 == '\0') {
    LOCK();
    *(undefined1 *)(param_6 + 6) = 1;
    UNLOCK();
    _ZNSt3__115recursive_mutex4lockEv(plVar1);
  }
  LOCK();
  *(int *)((long)param_6 + 0x34) = *(int *)((long)param_6 + 0x34) + 1;
  UNLOCK();
  if (*(int *)((long)param_6 + 0x34) == 1) {
    LOCK();
    *(undefined1 *)(param_6 + 6) = 0;
    UNLOCK();
    LOCK();
    *(undefined1 *)((long)param_6 + 0x31) = 0;
    UNLOCK();
    LOCK();
    *(undefined4 *)((long)param_6 + 0x34) = 0;
    UNLOCK();
  }
  if (*(int *)((long)param_6 + 0x34) == 1) {
    (**(code **)(*param_6 + 0x188))(param_6);
  }
  uVar6 = CONCAT71((int7)((ulong)plVar1 >> 8),1);
  if (0.0 < param_5) {
    plStack_78 = (long *)0x0;
    plStack_78 = (long *)_Znwm(0x30);
    *plStack_78 = (long)&UNK_070b4fe8;
    plStack_78[1] = (long)param_6;
    plStack_78[2] = (long)&uStack_ac;
    plStack_78[3] = (long)&uStack_b0;
    plStack_78[4] = (long)&uStack_b4;
    plStack_78[5] = (long)&uStack_b8;
    _ZN3msr6airlib17MultirotorApiBase15waitForFunctionENSt3__18functionIFbvEEEf
              (auStack_60,param_6,alStack_98);
    if (cStack_48 == '\0') {
      uStack_a8 = 0;
      plStack_a0 = (long *)0x0;
      puVar4 = (undefined8 *)func_0x02605eb0(&uStack_a8);
      plVar1 = plStack_a0;
      if (plStack_a0 != (long *)0x0) {
        LOCK();
        plVar2 = plStack_a0 + 1;
        lVar5 = *plVar2;
        *plVar2 = *plVar2 + -1;
        UNLOCK();
        if (lVar5 == 0) {
          (**(code **)(*plStack_a0 + 0x10))(plStack_a0);
          _ZNSt3__119__shared_weak_count14__release_weakEv(plVar1);
        }
      }
      lVar5 = (**(code **)*puVar4)(puVar4);
      lVar5 = lVar5 - lStack_40;
      auVar7._8_4_ = (int)((ulong)lVar5 >> 0x20);
      auVar7._0_8_ = lVar5;
      auVar7._12_4_ = _UNK_006c2fa4;
      uVar6 = CONCAT71((int7)((ulong)lStack_40 >> 8),
                       dStack_58 <=
                       ((auVar7._8_8_ - _UNK_006ccff8) +
                       ((double)CONCAT44(_DAT_006c2fa0,(int)lVar5) - _DAT_006ccff0)) / _DAT_006c1f20
                      );
    }
    else {
      uVar6 = 0;
    }
    if (alStack_98 == plStack_78) {
      (**(code **)(*plStack_78 + 0x20))();
    }
    else if (plStack_78 != (long *)0x0) {
      (**(code **)(*plStack_78 + 0x28))();
    }
  }
  if (*(int *)((long)param_6 + 0x34) == 1) {
    (**(code **)(*param_6 + 400))();
  }
  if (*(int *)((long)param_6 + 0x34) == 1) {
    LOCK();
    *(undefined1 *)(param_6 + 6) = 0;
    UNLOCK();
    LOCK();
    *(undefined1 *)((long)param_6 + 0x31) = 0;
    UNLOCK();
    LOCK();
    *(undefined4 *)((long)param_6 + 0x34) = 0;
    UNLOCK();
  }
  _ZNSt3__115recursive_mutex6unlockEv(param_6 + 7);
  if (0 < *(int *)((long)param_6 + 0x34)) {
    LOCK();
    *(int *)((long)param_6 + 0x34) = *(int *)((long)param_6 + 0x34) + -1;
    UNLOCK();
  }
  return uVar6 & 0xffffffff;
}



