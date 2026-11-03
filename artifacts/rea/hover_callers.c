// ADDRESS 02fc8900

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fc8900(float param_1,long *param_2)

{
  undefined8 uVar1;
  code *pcVar2;
  undefined8 *puVar3;
  long lVar4;
  uint uVar5;
  long *plVar6;
  bool bVar7;
  float fVar8;
  long *plStack_a8;
  int iStack_a0;
  float fStack_94;
  float fStack_90;
  float fStack_8c;
  undefined8 *puStack_88;
  undefined8 uStack_80;
  undefined1 auStack_78 [8];
  undefined8 uStack_70;
  long lStack_68;
  long *plStack_58;
  long alStack_48 [4];
  undefined8 uStack_28;
  
  fStack_90 = _DAT_006c1864 + param_1;
  fStack_8c = _DAT_006c12fc + param_1;
  fStack_94 = param_1;
  puVar3 = (undefined8 *)func_0x02fc7620(param_2,0);
  if (puVar3 != (undefined8 *)0x0) {
    uVar1 = *puVar3;
    lVar4 = func_0x02fb4e80();
    plStack_a8 = (long *)((ulong)plStack_a8 & 0xffffffff00000000);
    if (*(int *)(lVar4 + 0x228) < 1) {
      puStack_88 = &plStack_a8;
      uStack_80 = (undefined8 *)(lVar4 + 0x220);
      func_0x06f8ce70(&puStack_88);
      pcVar2 = (code *)swi(3);
      (*pcVar2)();
      return;
    }
    func_0x02fc3270(uVar1,**(undefined8 **)(lVar4 + 0x220),*(undefined1 *)(puVar3 + 1));
    _ZdlPv(puVar3);
  }
  lVar4 = func_0x02fcc220();
  uVar5 = *(int *)(lVar4 + 0x1a0) * 0xbb38435 + 0x3619636b;
  *(uint *)(lVar4 + 0x1a0) = uVar5;
  if ((int)(((float)(uVar5 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1440) < 6) {
    func_0x02fc93c0(param_2);
  }
  else {
    uStack_28 = 0;
    plStack_a8 = (long *)0x0;
    iStack_a0 = 0;
    func_0x031d6030(&puStack_88,&UNK_00b52ba4,1);
    bVar7 = (char)param_2[0x4a] == '\0';
    fVar8 = fStack_8c;
    if (!bVar7) {
      fVar8 = fStack_90;
    }
    func_0x02fc9220(fStack_94 + *(float *)(&UNK_006c2d28 + (ulong)bVar7 * 4),fVar8,_DAT_006c0f30,
                    *(float *)((long)param_2 + 0x26c) * _DAT_006c1b34,&plStack_a8,param_2,
                    &puStack_88);
    lVar4 = (**(code **)(*param_2 + 0x168))(param_2);
    puVar3 = (undefined8 *)(lVar4 + 0x368);
    if (*(long *)(lVar4 + 0x180) != 0) {
      puVar3 = (undefined8 *)(*(long *)(lVar4 + 0x180) + 0xd0);
    }
    uVar1 = *puVar3;
    fStack_94 = (float)param_2[0x4d];
    puStack_88 = (undefined8 *)0x0;
    uStack_80 = (undefined8 *)((ulong)uStack_80._4_4_ << 0x20);
    if ((iStack_a0 != 0) && (plStack_a8 != (long *)0x0)) {
      (**(code **)(*plStack_a8 + 0x48))(plStack_a8,&puStack_88);
    }
    func_0x033e4af0(auStack_78,0);
    plStack_58 = (long *)0x0;
    uStack_70 = 0;
    lStack_68 = 0;
    func_0x05909b10(uVar1,&uStack_28,&puStack_88,0);
    if (lStack_68 != 0) {
      plVar6 = alStack_48;
      if (plStack_58 != (long *)0x0) {
        plVar6 = plStack_58;
      }
      (**(code **)(*plVar6 + 0x10))();
    }
    if (((int)uStack_80 != 0) && (puStack_88 != (undefined8 *)0x0)) {
      (**(code **)*puStack_88)();
      if (puStack_88 != (undefined8 *)0x0) {
        puStack_88 = (undefined8 *)func_0x03071ed0(puStack_88,0,0);
      }
      uStack_80 = (undefined8 *)((ulong)uStack_80 & 0xffffffff00000000);
    }
    func_0x0263ea90(&puStack_88);
    if ((iStack_a0 != 0) && (plStack_a8 != (long *)0x0)) {
      (**(code **)*plStack_a8)();
      if (plStack_a8 != (long *)0x0) {
        plStack_a8 = (long *)func_0x03071ed0(plStack_a8,0,0);
      }
      iStack_a0 = 0;
    }
    func_0x0263ea90(&plStack_a8);
  }
  return;
}



// ADDRESS 02fdc2f0

void selected_02fdc2f0(undefined8 param_1,long param_2)

{
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  
  uStack_14 = 0;
  func_0x03363830();
  if (*(long *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(long *)(param_2 + 0x80) + 0x20);
    func_0x0339aaa0(param_2,&uStack_14);
  }
  else {
    func_0x0339aa70(param_2,*(undefined8 *)(param_2 + 0x18),&uStack_14);
  }
  uStack_18 = 0;
  func_0x03363830();
  if (*(long *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(long *)(param_2 + 0x80) + 0x20);
    func_0x0339aaa0(param_2,&uStack_18);
  }
  else {
    func_0x0339aa70(param_2,*(undefined8 *)(param_2 + 0x18),&uStack_18);
  }
  uStack_1c = 0;
  func_0x03363830();
  if (*(long *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(long *)(param_2 + 0x80) + 0x20);
    func_0x0339aaa0(param_2,&uStack_1c);
  }
  else {
    func_0x0339aa70(param_2,*(undefined8 *)(param_2 + 0x18),&uStack_1c);
  }
  uStack_20 = 0;
  func_0x03363830();
  if (*(long *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(long *)(param_2 + 0x80) + 0x20);
    func_0x0339aaa0(param_2,&uStack_20);
  }
  else {
    func_0x0339aa70(param_2,*(undefined8 *)(param_2 + 0x18),&uStack_20);
  }
  *(ulong *)(param_2 + 0x20) =
       (*(long *)(param_2 + 0x20) + 1) - (ulong)(*(long *)(param_2 + 0x20) == 0);
  func_0x02fc93c0(uStack_14,uStack_18,uStack_1c,uStack_20,param_1);
  return;
}



