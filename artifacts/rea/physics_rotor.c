// ADDRESS 0269ace0

void selected_0269ace0(long param_1,undefined8 param_2)

{
  undefined4 uStack_a4;
  byte abStack_a0 [16];
  undefined8 uStack_90;
  byte abStack_88 [16];
  undefined8 uStack_78;
  byte abStack_70 [16];
  undefined8 uStack_60;
  byte abStack_58 [16];
  undefined8 uStack_48;
  byte abStack_40 [16];
  undefined8 uStack_30;
  byte abStack_28 [16];
  undefined8 uStack_18;
  
  abStack_28[0] = 6;
  abStack_28[1] = 0x44;
  abStack_28[2] = 0x69;
  abStack_28[3] = 0x72;
  abStack_28[4] = 0;
  uStack_a4 = *(undefined4 *)(param_1 + 0x80);
  func_0x0266b7a0(param_2,abStack_28,&uStack_a4);
  if ((abStack_28[0] & 1) != 0) {
    _ZdlPv(uStack_18);
  }
  abStack_58[0] = 0xe;
  abStack_58[1] = 0x43;
  abStack_58[2] = 0x74;
  abStack_58[3] = 0x72;
  abStack_58[4] = 0x6c;
  abStack_58[5] = 0x2d;
  abStack_58[6] = 0x69;
  abStack_58[7] = 0x6e;
  abStack_58[8] = 0;
  func_0x02670c20(param_2,abStack_58,param_1 + 300);
  if ((abStack_58[0] & 1) != 0) {
    _ZdlPv(uStack_48);
  }
  abStack_70[0] = 0xe;
  abStack_70[1] = 0x43;
  abStack_70[2] = 0x74;
  abStack_70[3] = 0x72;
  abStack_70[4] = 0x6c;
  abStack_70[5] = 0x2d;
  abStack_70[6] = 0x66;
  abStack_70[7] = 0x6c;
  abStack_70[8] = 0;
  func_0x02670c20(param_2,abStack_70,param_1 + 0x128);
  if ((abStack_70[0] & 1) != 0) {
    _ZdlPv(uStack_60);
  }
  abStack_40[0] = 10;
  abStack_40[1] = 0x73;
  abStack_40[2] = 0x70;
  abStack_40[3] = 0x65;
  abStack_40[4] = 0x65;
  abStack_40[5] = 100;
  abStack_40[6] = 0;
  func_0x02670c20(param_2,abStack_40,param_1 + 0x120);
  if ((abStack_40[0] & 1) != 0) {
    _ZdlPv(uStack_30);
  }
  abStack_88[0] = 0xc;
  abStack_88[1] = 0x74;
  abStack_88[2] = 0x68;
  abStack_88[3] = 0x72;
  abStack_88[4] = 0x75;
  abStack_88[5] = 0x73;
  abStack_88[6] = 0x74;
  abStack_88[7] = 0;
  func_0x02670c20(param_2,abStack_88,param_1 + 0x118);
  if ((abStack_88[0] & 1) != 0) {
    _ZdlPv(uStack_78);
  }
  abStack_a0[0] = 0xc;
  abStack_a0[1] = 0x74;
  abStack_a0[2] = 0x6f;
  abStack_a0[3] = 0x72;
  abStack_a0[4] = 0x71;
  abStack_a0[5] = 0x75;
  abStack_a0[6] = 0x65;
  abStack_a0[7] = 0;
  func_0x02670c20(param_2,abStack_a0,param_1 + 0x11c);
  if ((abStack_a0[0] & 1) != 0) {
    _ZdlPv(uStack_90);
  }
  return;
}



// ADDRESS 0269af00

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_0269af00(long param_1)

{
  float fVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  int iVar4;
  
  *(undefined4 *)(param_1 + 0x3c) = *(undefined4 *)(param_1 + 0x30);
  *(undefined8 *)(param_1 + 0x40) = *(undefined8 *)(param_1 + 0x34);
  *(undefined4 *)(param_1 + 0x54) = *(undefined4 *)(param_1 + 0x48);
  *(undefined8 *)(param_1 + 0x58) = *(undefined8 *)(param_1 + 0x4c);
  if (DAT_0715a878 == '\0') {
    iVar4 = func_0x070a9210(&DAT_0715a878);
    if (iVar4 != 0) {
      _DAT_0715a860 = 0;
      uRam000000000715a868 = 0;
      _DAT_0715a870 = 0;
      func_0x070a9230(&DAT_0715a878);
    }
  }
  uVar3 = _DAT_0715a870;
  uVar2 = uRam000000000715a868;
  *(undefined8 *)(param_1 + 0x60) = _DAT_0715a860;
  *(undefined8 *)(param_1 + 0x68) = uVar2;
  *(undefined8 *)(param_1 + 0x70) = uVar3;
  *(float *)(param_1 + 0x114) =
       *(float *)(*(long *)(param_1 + 0x108) + 0xe4) / *(float *)(param_1 + 0x110);
  func_0x02606a00(param_1 + 0xb8);
  *(undefined4 *)(param_1 + 300) = *(undefined4 *)(param_1 + 0xf4);
  fVar1 = *(float *)(param_1 + 0xec);
  *(float *)(param_1 + 0x128) = fVar1;
  *(float *)(param_1 + 0x120) = SQRT(*(float *)(param_1 + 0xac) * fVar1);
  *(float *)(param_1 + 0x118) = *(float *)(param_1 + 0xb0) * fVar1;
  *(float *)(param_1 + 0x11c) = (float)*(int *)(param_1 + 0x80) * fVar1 * *(float *)(param_1 + 0xb4)
  ;
  *(int *)(param_1 + 0x124) = *(int *)(param_1 + 0x80);
  return;
}



// ADDRESS 0269b020

void selected_0269b020(long param_1,float *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  undefined8 uVar4;
  float fStack_10;
  float fStack_c;
  
  fVar3 = *(float *)(param_1 + 0x5c);
  uVar4 = *(undefined8 *)(param_1 + 0x54);
  fVar1 = *(float *)(param_1 + 0x114);
  fVar2 = *(float *)(param_1 + 0x118);
  fStack_10 = (float)uVar4;
  *param_2 = fVar2 * fStack_10 * fVar1;
  fStack_c = (float)((ulong)uVar4 >> 0x20);
  param_2[1] = fVar2 * fStack_c * fVar1;
  param_2[2] = fVar2 * fVar3 * fVar1;
  fVar1 = *(float *)(param_1 + 0x114);
  fVar2 = *(float *)(param_1 + 0x11c);
  param_2[3] = fStack_10 * fVar2 * fVar1;
  param_2[4] = fStack_c * fVar2 * fVar1;
  param_2[5] = fVar2 * fVar3 * fVar1;
  return;
}



// ADDRESS 0268ce60

undefined8 * selected_0268ce60(undefined8 *param_1,long param_2)

{
  undefined8 uVar1;
  
  uVar1 = _Znwm(0x438);
  func_0x0268cf60(uVar1,param_2,*(undefined8 *)(param_2 + 0x110));
  *param_1 = uVar1;
  return param_1;
}



// ADDRESS 0268ceb0

long selected_0268ceb0(undefined8 *param_1)

{
  long *plVar1;
  long lVar2;
  undefined8 *puVar3;
  long *plVar4;
  undefined8 *puVar5;
  long in_RAX;
  long lVar6;
  long lVar7;
  
  *param_1 = 0xbae320;
  plVar4 = (long *)param_1[0x24];
  if (plVar4 != (long *)0x0) {
    LOCK();
    plVar1 = plVar4 + 1;
    in_RAX = *plVar1;
    *plVar1 = *plVar1 + -1;
    UNLOCK();
    if (in_RAX == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      in_RAX = _ZNSt3__119__shared_weak_count14__release_weakEv(plVar4);
    }
  }
  *param_1 = 0xbacfc8;
  lVar2 = param_1[0x1f];
  if (lVar2 != 0) {
    lVar7 = param_1[0x20];
    lVar6 = lVar2;
    if (lVar7 != lVar2) {
      do {
        plVar4 = *(long **)(lVar7 + -8);
        lVar7 = lVar7 + -0x10;
        if (plVar4 != (long *)0x0) {
          LOCK();
          plVar1 = plVar4 + 1;
          lVar6 = *plVar1;
          *plVar1 = *plVar1 + -1;
          UNLOCK();
          if (lVar6 == 0) {
            (**(code **)(*plVar4 + 0x10))(plVar4);
            _ZNSt3__119__shared_weak_count14__release_weakEv(plVar4);
          }
        }
      } while (lVar2 != lVar7);
      lVar6 = param_1[0x1f];
    }
    param_1[0x20] = lVar2;
    in_RAX = _ZdlPv(lVar6);
  }
  param_1[0x14] = 0xbacdc8;
  puVar5 = (undefined8 *)param_1[0x1c];
  while (puVar5 != (undefined8 *)0x0) {
    puVar3 = (undefined8 *)*puVar5;
    plVar4 = (long *)puVar5[3];
    puVar5[3] = 0;
    if (plVar4 != (long *)0x0) {
      (**(code **)(*plVar4 + 0x10))();
    }
    in_RAX = _ZdlPv(puVar5);
    puVar5 = puVar3;
  }
  lVar2 = param_1[0x1a];
  param_1[0x1a] = 0;
  if (lVar2 != 0) {
    in_RAX = _ZdlPv();
  }
  param_1[0x14] = 0xbaace8;
  if ((*(byte *)(param_1 + 0x17) & 1) != 0) {
    in_RAX = _ZdlPv(param_1[0x19]);
  }
  if (param_1[2] != 0) {
    param_1[3] = param_1[2];
    func_0x03071ea0();
    return in_RAX;
  }
  return in_RAX;
}



// ADDRESS 0268cf00

undefined8 selected_0268cf00(undefined8 *param_1)

{
  long *plVar1;
  long lVar2;
  long *plVar3;
  undefined8 uVar4;
  
  *param_1 = 0xbae320;
  plVar3 = (long *)param_1[0x24];
  if (plVar3 != (long *)0x0) {
    LOCK();
    plVar1 = plVar3 + 1;
    lVar2 = *plVar1;
    *plVar1 = *plVar1 + -1;
    UNLOCK();
    if (lVar2 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      _ZNSt3__119__shared_weak_count14__release_weakEv(plVar3);
    }
  }
  uVar4 = func_0x026765b0(param_1);
  func_0x03071ea0(param_1);
  return uVar4;
}



// ADDRESS 02690140

undefined8 selected_02690140(undefined8 *param_1)

{
  undefined8 in_RAX;
  
  *param_1 = 0xbae760;
  param_1[2] = 0xbae7d8;
  param_1[3] = 0xbae808;
  param_1[4] = 0xbae840;
  param_1[5] = 0xbae870;
  if (param_1[9] != 0) {
    param_1[10] = param_1[9];
    in_RAX = _ZdlPv();
  }
  if (param_1[6] != 0) {
    param_1[7] = param_1[6];
    in_RAX = _ZdlPv();
  }
  func_0x03071ea0(param_1);
  return in_RAX;
}



// ADDRESS 02690250

ulong selected_02690250(void)

{
  long *plVar1;
  long lVar2;
  long *plVar3;
  undefined8 *puVar4;
  ulong uVar5;
  undefined8 uStack_28;
  long *plStack_20;
  
  uStack_28 = 0;
  plStack_20 = (long *)0x0;
  puVar4 = (undefined8 *)func_0x02605eb0(&uStack_28);
  plVar3 = plStack_20;
  if (plStack_20 != (long *)0x0) {
    LOCK();
    plVar1 = plStack_20 + 1;
    lVar2 = *plVar1;
    *plVar1 = *plVar1 + -1;
    UNLOCK();
    if (lVar2 == 0) {
      (**(code **)(*plStack_20 + 0x10))(plStack_20);
      _ZNSt3__119__shared_weak_count14__release_weakEv(plVar3);
    }
  }
  uVar5 = (**(code **)*puVar4)(puVar4);
  return uVar5 / 1000000;
}



// ADDRESS 02690340

void selected_02690340(undefined4 param_1,long param_2,uint param_3)

{
  *(undefined4 *)(*(long *)(param_2 + 0x30) + (ulong)param_3 * 4) = param_1;
  return;
}



// ADDRESS 02690350

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02690350(long param_1,float *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  long lVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  
  lVar4 = *(long *)(param_1 + 0x70);
  fVar5 = (float)(*(uint *)(lVar4 + 0xc) ^ _DAT_006c2fc0);
  fVar6 = (float)(*(uint *)(lVar4 + 0x10) ^ _UNK_006c2fc4);
  fVar7 = (float)(*(uint *)(lVar4 + 0x14) ^ _UNK_006c2fc8);
  fVar8 = (float)(*(uint *)(lVar4 + 0x18) ^ _UNK_006c2fcc);
  fVar1 = *(float *)(lVar4 + 0x3c);
  fVar2 = *(float *)(lVar4 + 0x38);
  fVar3 = *(float *)(lVar4 + 0x34);
  fVar10 = fVar2 * fVar5 - fVar6 * fVar3;
  fVar10 = fVar10 + fVar10;
  fVar11 = fVar7 * fVar3 - fVar1 * fVar5;
  fVar11 = fVar11 + fVar11;
  fVar9 = fVar1 * fVar6 - fVar2 * fVar7;
  fVar9 = fVar9 + fVar9;
  *param_2 = fVar9 * fVar8 + fVar3 + (fVar6 * fVar10 - fVar7 * fVar11);
  param_2[1] = fVar11 * fVar8 + fVar2 + (fVar7 * fVar9 - fVar5 * fVar10);
  param_2[2] = fVar10 * fVar8 + fVar1 + (fVar5 * fVar11 - fVar6 * fVar9);
  return;
}



// ADDRESS 02690440

void selected_02690440(long param_1,undefined8 *param_2)

{
  undefined4 uVar1;
  
  uVar1 = *(undefined4 *)(*(long *)(param_1 + 0x70) + 0x30);
  *param_2 = *(undefined8 *)(*(long *)(param_1 + 0x70) + 0x28);
  *(undefined4 *)(param_2 + 1) = uVar1;
  return;
}



