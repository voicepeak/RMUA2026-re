// ADDRESS 02fb8f40

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fb8f40(undefined8 param_1,undefined4 param_2,long param_3)

{
  int iVar1;
  uint uVar2;
  char cVar3;
  long lVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  float *pfVar7;
  long *plVar8;
  byte bVar9;
  long lVar10;
  bool bVar11;
  float fVar12;
  float fVar13;
  undefined4 uVar14;
  float fStack_b8;
  long *plStack_b0;
  int iStack_a8;
  float fStack_9c;
  undefined8 *puStack_98;
  int iStack_90;
  undefined1 auStack_88 [8];
  undefined8 uStack_80;
  long lStack_78;
  long *plStack_68;
  long alStack_58 [4];
  undefined8 uStack_38;
  
  lVar4 = func_0x02fcc220();
  fVar13 = *(float *)(lVar4 + 0x14);
  iVar1 = *(int *)(param_3 + 0x38);
  if ((long)iVar1 != 0) {
    lVar4 = *(long *)(param_3 + 0x30);
    lVar10 = 0;
    do {
      func_0x02fc8d50(*(undefined8 *)(lVar4 + lVar10),param_3 + 0xc);
      lVar10 = lVar10 + 8;
    } while ((long)iVar1 * 8 != lVar10);
  }
  if (*(char *)(param_3 + 0xbc) == '\0') {
    if ((*(float *)(param_3 + 0xc) == _DAT_071fa860) &&
       (!NAN(*(float *)(param_3 + 0xc)) && !NAN(_DAT_071fa860))) {
      if ((*(float *)(param_3 + 0x10) == _DAT_071fa864) &&
         (!NAN(*(float *)(param_3 + 0x10)) && !NAN(_DAT_071fa864))) {
        if ((*(float *)(param_3 + 0x14) == _DAT_071fa868) &&
           (!NAN(*(float *)(param_3 + 0x14)) && !NAN(_DAT_071fa868))) goto LAB_02fb917c;
      }
    }
    uVar5 = func_0x02fcc220();
    func_0x02fced20(uVar5,0xffffffff);
    lVar4 = func_0x02fcc220();
    func_0x02fb2040(param_3 + 0x200,*(undefined4 *)(lVar4 + 0x1a0));
    func_0x02fba970(param_3);
    *(undefined1 *)(param_3 + 0xbc) = 1;
    uStack_38 = 0;
    plStack_b0 = (long *)0x0;
    iStack_a8 = 0;
    func_0x02fb96b0(&plStack_b0,param_3,&UNK_02fb97d0,0);
    puVar6 = (undefined8 *)(_DAT_0744e0d0 + 0x368);
    if (*(long *)(_DAT_0744e0d0 + 0x180) != 0) {
      puVar6 = (undefined8 *)(*(long *)(_DAT_0744e0d0 + 0x180) + 0xd0);
    }
    uVar5 = *puVar6;
    puStack_98 = (undefined8 *)0x0;
    iStack_90 = 0;
    if ((iStack_a8 != 0) && (plStack_b0 != (long *)0x0)) {
      (**(code **)(*plStack_b0 + 0x48))(plStack_b0,&puStack_98);
    }
    func_0x033e4af0(auStack_88,0);
    plStack_68 = (long *)0x0;
    uStack_80 = 0;
    lStack_78 = 0;
    param_2 = _DAT_006c0d00;
    func_0x05909b10(_DAT_006c0a14,uVar5,&uStack_38,&puStack_98,0);
    if (lStack_78 != 0) {
      plVar8 = alStack_58;
      if (plStack_68 != (long *)0x0) {
        plVar8 = plStack_68;
      }
      (**(code **)(*plVar8 + 0x10))();
    }
    if ((iStack_90 != 0) && (puStack_98 != (undefined8 *)0x0)) {
      (**(code **)*puStack_98)();
      if (puStack_98 != (undefined8 *)0x0) {
        puStack_98 = (undefined8 *)func_0x03071ed0(puStack_98,0,0);
      }
      iStack_90 = 0;
    }
    func_0x0263ea90(&puStack_98);
    if ((iStack_a8 != 0) && (plStack_b0 != (long *)0x0)) {
      (**(code **)*plStack_b0)();
      if (plStack_b0 != (long *)0x0) {
        plStack_b0 = (long *)func_0x03071ed0(plStack_b0,0,0);
      }
      iStack_a8 = 0;
    }
    func_0x0263ea90(&plStack_b0);
  }
LAB_02fb917c:
  cVar3 = func_0x02fb9830(param_3);
  if (*(char *)(param_3 + 0x180) != cVar3) {
    lVar4 = func_0x02fb4e80();
    bVar11 = true;
    if (*(char *)(lVar4 + 0x2d0) == '\0') {
      bVar11 = *(char *)(param_3 + 0x180) == '\0';
    }
    *(bool *)(param_3 + 0x180) = bVar11;
    puVar6 = (undefined8 *)_Znwm(0x18);
    puVar6[1] = 0x1e;
    *puVar6 = 0xcd58a0;
    *(bool *)(puVar6 + 2) = bVar11;
    uVar5 = func_0x02fcc220();
    func_0x02fcc520(uVar5,puVar6);
  }
  iVar1 = *(int *)(param_3 + 0x68);
  if ((long)iVar1 != 0) {
    lVar4 = *(long *)(param_3 + 0x60);
    lVar10 = 0;
    do {
      cVar3 = func_0x02fc0e70(*(undefined8 *)(lVar4 + lVar10));
      if (cVar3 != '\0') {
        bVar9 = 1;
        goto LAB_02fb921c;
      }
      lVar10 = lVar10 + 8;
    } while ((long)iVar1 * 8 != lVar10);
  }
  bVar9 = 0;
LAB_02fb921c:
  if (*(byte *)(param_3 + 0x181) != bVar9) {
    bVar9 = *(byte *)(param_3 + 0x181) ^ 1;
    *(byte *)(param_3 + 0x181) = bVar9;
    puVar6 = (undefined8 *)_Znwm(0x18);
    puVar6[1] = 0x1f;
    *puVar6 = 0xcd58c8;
    *(byte *)(puVar6 + 2) = bVar9;
    uVar5 = func_0x02fcc220();
    func_0x02fcc520(uVar5,puVar6);
  }
  uVar2 = *(uint *)(param_3 + 0xd0);
  if (0xb < uVar2) {
    lVar4 = func_0x02fb4e80();
    *(undefined4 *)(lVar4 + 0x318) = 0;
    *(undefined4 *)(param_3 + 0x1a8) = 0;
    goto LAB_02fb94be;
  }
  if ((0x555U >> (uVar2 & 0x1f) & 1) != 0) {
    fVar12 = *(float *)(param_3 + 0x1a8) + fVar13;
    *(float *)(param_3 + 0x1a8) = fVar12;
    lVar4 = func_0x02fb4e80();
    *(float *)(lVar4 + 0x318) = fVar12;
    fVar12 = *(float *)(param_3 + 0x1a8);
    lVar4 = func_0x02fb4e80();
    if ((*(float *)(lVar4 + 0x2c8) <= fVar12 && fVar12 != *(float *)(lVar4 + 0x2c8)) &&
       (lVar4 = func_0x02fb4e80(), *(char *)(lVar4 + 0x2d4) == '\0')) {
      func_0x02fb9960(param_3,0xc,0);
    }
    goto LAB_02fb94be;
  }
  *(undefined4 *)(param_3 + 0x1a8) = 0;
  switch(uVar2) {
  default:
    lVar4 = func_0x02fb4e80();
    pfVar7 = (float *)(lVar4 + 0x2b0);
    break;
  case 3:
    lVar4 = func_0x02fb4e80();
    pfVar7 = (float *)(lVar4 + 0x2b4);
    break;
  case 5:
    lVar4 = func_0x02fb4e80();
    pfVar7 = (float *)(lVar4 + 0x2b8);
    break;
  case 7:
    lVar4 = func_0x02fb4e80();
    pfVar7 = (float *)(lVar4 + 700);
    break;
  case 9:
    lVar4 = func_0x02fb4e80();
    pfVar7 = (float *)(lVar4 + 0x2c0);
    break;
  case 0xb:
    lVar4 = func_0x02fb4e80();
    pfVar7 = (float *)(lVar4 + 0x2c4);
  }
  fStack_9c = *pfVar7;
  uVar14 = *(undefined4 *)(param_3 + 0xd0);
  fStack_b8 = 0.0;
  lVar4 = _ZNSt3__16chrono12steady_clock3nowEv();
  switch(uVar14) {
  case 1:
    plVar8 = (long *)(param_3 + 0x140);
    break;
  default:
    goto LAB_02fb93f2;
  case 3:
    plVar8 = (long *)(param_3 + 0x148);
    break;
  case 5:
    plVar8 = (long *)(param_3 + 0x150);
    break;
  case 7:
    plVar8 = (long *)(param_3 + 0x158);
    break;
  case 9:
    plVar8 = (long *)(param_3 + 0x160);
    break;
  case 0xb:
    plVar8 = (long *)(param_3 + 0x168);
  }
  fStack_b8 = (float)(lVar4 - *plVar8) / _UNK_006c1238;
LAB_02fb93f2:
  lVar4 = func_0x02fb4e80();
  *(float *)(lVar4 + 0x318) = fStack_b8;
  if ((fStack_9c < fStack_b8) && (lVar4 = func_0x02fb4e80(), *(char *)(lVar4 + 0x2d4) == '\0')) {
    func_0x02fb9960(param_3,0xc,0);
  }
  puVar6 = (undefined8 *)_Znwm(0x20);
  puVar6[1] = 0x23;
  *puVar6 = 0xcd58f0;
  *(ulong *)((long)puVar6 + 0x14) = CONCAT44(_DAT_071fa864,_DAT_071fa860);
  *(float *)((long)puVar6 + 0x1c) = _DAT_071fa868;
  *(undefined1 *)(puVar6 + 2) = *(undefined1 *)(param_3 + 0x181);
  if ((((0xb < *(uint *)(param_3 + 0xd0)) ||
       ((0xa80U >> (*(uint *)(param_3 + 0xd0) & 0x1f) & 1) == 0)) || (fStack_9c < fStack_b8)) ||
     (lVar4 = func_0x02fb4e80(), *(char *)(lVar4 + 0x2d5) != '\0')) {
    *(float *)((long)puVar6 + 0x1c) = _DAT_071fa868;
    *(ulong *)((long)puVar6 + 0x14) = CONCAT44(_DAT_071fa864,_DAT_071fa860);
  }
  else {
    func_0x02fb2050(fStack_b8,param_3 + 0x200);
    uVar5 = func_0x02fb2130(fStack_b8,param_3 + 0x200);
    *(undefined8 *)((long)puVar6 + 0x14) = uVar5;
    *(undefined4 *)((long)puVar6 + 0x1c) = param_2;
  }
  uVar5 = func_0x02fcc220();
  func_0x02fcc520(uVar5,puVar6);
LAB_02fb94be:
  if (*(char *)(param_3 + 0x180) == '\0') {
    fVar13 = fVar13 + *(float *)(param_3 + 0x1ac);
    *(float *)(param_3 + 0x1ac) = fVar13;
    lVar4 = func_0x02fb4e80();
    if ((*(float *)(lVar4 + 0x2cc) <= fVar13 && fVar13 != *(float *)(lVar4 + 0x2cc)) &&
       (lVar4 = func_0x02fb4e80(), *(char *)(lVar4 + 0x2d4) == '\0')) {
      func_0x02fb9960(param_3,0xc,0);
    }
  }
  uVar14 = func_0x02fba830(param_3);
  lVar4 = func_0x02fb4e80();
  *(undefined4 *)(lVar4 + 0x2f8) = uVar14;
  uVar14 = *(undefined4 *)(param_3 + 0x1ac);
  lVar4 = func_0x02fb4e80();
  *(undefined4 *)(lVar4 + 0x300) = uVar14;
  return;
}



// ADDRESS 02fbac30

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fbac30(long param_1,uint param_2,uint param_3,uint param_4)

{
  long *plVar1;
  long *plVar2;
  long *plVar3;
  code *pcVar4;
  int iVar5;
  undefined4 uVar6;
  undefined8 uVar7;
  long lVar8;
  undefined8 uVar9;
  uint uVar10;
  uint uVar11;
  ulong uVar12;
  long lVar13;
  long lVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  uint uStack_94;
  uint *puStack_90;
  long *plStack_88;
  uint uStack_80;
  uint uStack_7c;
  ulong uStack_78;
  long lStack_70;
  ulong uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  long lStack_48;
  undefined8 uStack_40;
  ulong uStack_38;
  
  uStack_7c = param_3 - 1;
  lStack_48 = CONCAT44(lStack_48._4_4_,uStack_7c);
  uStack_58 = CONCAT44(uStack_58._4_4_,-param_3 >> 0x1f);
  if (((uint)((int)param_3 <= *(int *)(param_1 + 0x48)) & -param_3 >> 0x1f) != 1) {
    plStack_88 = (long *)(param_1 + 0x40);
    puStack_90 = (uint *)&lStack_48;
    func_0x06f8ce70(&puStack_90);
    pcVar4 = (code *)swi(3);
    (*pcVar4)();
    return;
  }
  uVar12 = (ulong)(int)uStack_7c;
  lVar8 = *(long *)(*(long *)(param_1 + 0x40) + uVar12 * 8);
  *(uint *)(param_1 + 0xe4) = param_2;
  *(uint *)(param_1 + 0xe8) = param_3;
  if (lVar8 != 0) {
    lVar8 = *(long *)(lVar8 + 0x130);
    if (lVar8 == 0) {
      uVar6 = 0;
      uVar15 = 0;
      uVar9 = _DAT_071fa860;
      uVar16 = _DAT_071fa868;
      uVar7 = _DAT_006d5290;
    }
    else {
      uVar6 = *(undefined4 *)(lVar8 + 0x1c0);
      uVar15 = *(undefined4 *)(lVar8 + 0x1c4);
      uVar9 = *(undefined8 *)(lVar8 + 0x1d0);
      uVar16 = *(undefined4 *)(lVar8 + 0x1d8);
      uVar7 = *(undefined8 *)(lVar8 + 0x1c8);
    }
    *(int *)(param_1 + 0x124) = (int)uVar9;
    *(int *)(param_1 + 0x128) = (int)((ulong)uVar9 >> 0x20);
    *(undefined4 *)(param_1 + 300) = uVar16;
    *(undefined4 *)(param_1 + 0x130) = uVar6;
    *(undefined4 *)(param_1 + 0x134) = uVar15;
    *(int *)(param_1 + 0x138) = (int)uVar7;
    *(int *)(param_1 + 0x13c) = (int)((ulong)uVar7 >> 0x20);
    iVar5 = *(int *)(param_1 + 0x58);
    uStack_38 = (ulong)param_4;
    if ((long)iVar5 == 0) {
      iVar5 = 0;
    }
    else {
      lVar8 = *(long *)(param_1 + 0x50);
      lVar13 = 0;
      uStack_78 = uVar12;
      do {
        func_0x02fc13d0(*(undefined8 *)(lVar8 + lVar13),1);
        lVar13 = lVar13 + 8;
      } while ((long)iVar5 * 8 != lVar13);
      iVar5 = *(int *)(param_1 + 0x58);
      uVar12 = uStack_78;
    }
    plStack_88 = (long *)(param_1 + 0x50);
    uVar10 = param_2 - 1;
    uStack_80 = -param_2 >> 0x1f;
    uStack_78 = (ulong)param_2;
    lStack_48._0_4_ = uVar10;
    if (((int)param_2 <= iVar5 & uStack_80) != 1) {
      puStack_90 = (uint *)&lStack_48;
      func_0x06f8ce70(&puStack_90);
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    lStack_70 = (long)(int)uVar10;
    func_0x02fc13d0(*(undefined8 *)(*(long *)(param_1 + 0x50) + lStack_70 * 8),0);
    lStack_48 = CONCAT44(lStack_48._4_4_,uStack_7c);
    uStack_68 = (ulong)param_3;
    if (((int)param_3 <= *(int *)(param_1 + 0x58) & (uint)uStack_58) != 1) {
      puStack_90 = (uint *)&lStack_48;
      func_0x06f8ce70(&puStack_90);
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    func_0x02fc13d0(*(undefined8 *)(*(long *)(param_1 + 0x50) + uVar12 * 8),0);
    *(undefined2 *)(param_1 + 0xda) = 0;
    *(undefined1 *)(param_1 + 0xe0) = 0;
    uVar9 = _DAT_0744e0d0;
    lStack_48 = 0;
    uStack_40 = 0;
    uVar7 = func_0x02fd7260();
    func_0x053eaf30(uVar9,uVar7,&lStack_48);
    lVar8 = lStack_48;
    lVar13 = (long)(int)uStack_40;
    if (lVar13 != 0) {
      lVar14 = 0;
      do {
        func_0x050a2370(*(undefined8 *)(lVar8 + lVar14),0,1);
        lVar14 = lVar14 + 8;
      } while (lVar13 * 8 != lVar14);
    }
    lVar8 = func_0x02fb4e80();
    (**(code **)(**(long **)(lVar8 + 0x240) + 0x9a8))();
    lVar8 = func_0x02fb4e80();
    (**(code **)(**(long **)(lVar8 + 0x248) + 0x9a8))();
    uVar9 = func_0x02fcc220();
    func_0x03006490(&puStack_90,&UNK_00900140,uStack_78,uStack_68);
    func_0x02fced50(_DAT_006c1438,uVar9,&puStack_90,1);
    if (puStack_90 != (uint *)0x0) {
      func_0x03071ea0();
    }
    iVar5 = (int)uStack_78;
    uStack_94 = uVar10;
    switch((int)uStack_38) {
    case 1:
      plStack_88 = (long *)(param_1 + 0x30);
      if ((iVar5 <= *(int *)(param_1 + 0x38) & uStack_80) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(0,0,0,*(undefined8 *)(*plStack_88 + lStack_70 * 8),0,0,0,0,0,0);
      uStack_94 = uStack_7c;
      if (((int)uStack_68 <= *(int *)(param_1 + 0x38) & (uint)uStack_58) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(0,0,0,*(undefined8 *)(*plStack_88 + uVar12 * 8),1,0,0,0,0,0);
      break;
    case 2:
      plStack_88 = (long *)(param_1 + 0x30);
      if ((iVar5 <= *(int *)(param_1 + 0x38) & uStack_80) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(_UNK_006c1638,_UNK_006c1638,_DAT_006c1438,
                      *(undefined8 *)(*plStack_88 + lStack_70 * 8),0,1,1,3,7,0);
      uStack_94 = uStack_7c;
      if (((int)uStack_68 <= *(int *)(param_1 + 0x38) & (uint)uStack_58) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(_UNK_006c1638,_UNK_006c1638,_DAT_006c1438,
                      *(undefined8 *)(*plStack_88 + uVar12 * 8),1,1,1,3,7,0);
      break;
    case 3:
      plStack_88 = (long *)(param_1 + 0x30);
      if ((iVar5 <= *(int *)(param_1 + 0x38) & uStack_80) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(_UNK_006c145c,_UNK_006c12f8,_DAT_006c1438,
                      *(undefined8 *)(*plStack_88 + lStack_70 * 8),0,1,1,5,10,0);
      uStack_94 = uStack_7c;
      if (((int)uStack_68 <= *(int *)(param_1 + 0x38) & (uint)uStack_58) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(_UNK_006c145c,_UNK_006c12f8,_DAT_006c1438,
                      *(undefined8 *)(*plStack_88 + uVar12 * 8),1,1,1,5,10,0);
      break;
    case 4:
      plStack_88 = (long *)(param_1 + 0x30);
      if ((iVar5 <= *(int *)(param_1 + 0x38) & uStack_80) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(_DAT_006c0da4,_UNK_006c19cc,_UNK_006c16f4,
                      *(undefined8 *)(*plStack_88 + lStack_70 * 8),0,1,1,7,0xf,1);
      uStack_94 = uStack_7c;
      if (((int)uStack_68 <= *(int *)(param_1 + 0x38) & (uint)uStack_58) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(_DAT_006c0da4,_UNK_006c19cc,_UNK_006c16f4,
                      *(undefined8 *)(*plStack_88 + uVar12 * 8),1,0,1,0,0,0);
      uStack_94 = uStack_7c;
      if (((int)uStack_68 <= *(int *)(param_1 + 0x38) & (uint)uStack_58) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc77b0(_UNK_006c1aac,_UNK_006c1aac,*(undefined8 *)(*plStack_88 + uVar12 * 8),0x14,5,
                      0x14,10,1);
      break;
    case 5:
      plStack_88 = (long *)(param_1 + 0x30);
      if ((iVar5 <= *(int *)(param_1 + 0x38) & uStack_80) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(_DAT_006c0da4,_UNK_006c19cc,_UNK_006c16f4,
                      *(undefined8 *)(*plStack_88 + lStack_70 * 8),0,0,1,0,0,1);
      uStack_94 = uStack_7c;
      if (((int)uStack_68 <= *(int *)(param_1 + 0x38) & (uint)uStack_58) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(_DAT_006c0da4,_UNK_006c19cc,_UNK_006c16f4,
                      *(undefined8 *)(*plStack_88 + uVar12 * 8),1,0,1,0,0,1);
      if (((int)uStack_78 <= *(int *)(param_1 + 0x38) & uStack_80) != 1) {
        puStack_90 = &uStack_94;
        uStack_94 = uVar10;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      uStack_94 = uVar10;
      func_0x02fc77b0(_DAT_006c1b34,_UNK_006c1aac,*(undefined8 *)(*plStack_88 + lStack_70 * 8),0x14,
                      0xf,0x18,0x18,2);
      uStack_94 = uStack_7c;
      if (((int)uStack_68 <= *(int *)(param_1 + 0x38) & (uint)uStack_58) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc77b0(_DAT_006c1b34,_UNK_006c1aac,*(undefined8 *)(*plStack_88 + uVar12 * 8),0x14,0xf
                      ,0x18,0x18,2);
      break;
    case 6:
      plStack_88 = (long *)(param_1 + 0x30);
      if ((iVar5 <= *(int *)(param_1 + 0x38) & uStack_80) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(_DAT_006c0da4,_UNK_006c19cc,_UNK_006c16f4,
                      *(undefined8 *)(*plStack_88 + lStack_70 * 8),0,0,1,0,0,1);
      uStack_94 = uStack_7c;
      if (((int)uStack_68 <= *(int *)(param_1 + 0x38) & (uint)uStack_58) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(_DAT_006c0da4,_UNK_006c19cc,_UNK_006c16f4,
                      *(undefined8 *)(*plStack_88 + uVar12 * 8),1,0,1,0,0,1);
      if (((int)uStack_78 <= *(int *)(param_1 + 0x38) & uStack_80) != 1) {
        puStack_90 = &uStack_94;
        uStack_94 = uVar10;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      uStack_94 = uVar10;
      func_0x02fc77b0(_UNK_006c1b38,_UNK_006c1aac,*(undefined8 *)(*plStack_88 + lStack_70 * 8),0,
                      0x2d,0,0x3c,3);
      uStack_94 = uStack_7c;
      if (((int)uStack_68 <= *(int *)(param_1 + 0x38) & (uint)uStack_58) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc77b0(_UNK_006c1b38,_UNK_006c1aac,*(undefined8 *)(*plStack_88 + uVar12 * 8),0,0x2d,0
                      ,0x3c,3);
    }
    plStack_88 = (long *)(param_1 + 0x30);
    if (((int)uStack_78 <= *(int *)(param_1 + 0x38) & uStack_80) != 1) {
      puStack_90 = &uStack_94;
      uStack_94 = uVar10;
      func_0x06f8ce70(&puStack_90);
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    uStack_94 = uVar10;
    func_0x02fc7ce0(*(undefined8 *)(*plStack_88 + lStack_70 * 8));
    if ((int)uStack_38 == 4) {
      if ((uStack_80 & (int)uStack_78 <= *(int *)(param_1 + 0x68)) != 1) {
        plStack_88 = (long *)(param_1 + 0x60);
        puStack_90 = &uStack_94;
        uStack_94 = uVar10;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      *(undefined8 *)(param_1 + 0xa0) = *(undefined8 *)(*(long *)(param_1 + 0x60) + lStack_70 * 8);
      *(undefined8 *)(param_1 + 0xa8) = 0;
      *(undefined2 *)(param_1 + 0xd8) = 0x100;
      plVar1 = (long *)(param_1 + 0x70);
      uStack_94 = 0xe;
      plStack_88 = plVar1;
      if (*(int *)(param_1 + 0x78) < 0xf) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fb8970(*(undefined8 *)(*plVar1 + 0x70),1);
      uStack_94 = 0xf;
      if (*(int *)(param_1 + 0x78) < 0x10) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fb8970(*(undefined8 *)(*plVar1 + 0x78),1);
      plVar2 = (long *)(param_1 + 0x80);
      uStack_94 = 0xe;
      if (*(int *)(param_1 + 0x88) < 0xf) {
        puStack_90 = &uStack_94;
        plStack_88 = plVar2;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plVar3 = *(long **)(*plVar2 + 0x70);
      if (plVar3 != (long *)0x0) {
        (**(code **)(*plVar3 + 0x310))(plVar3,0);
        func_0x0509d4e0(plVar3,1);
        func_0x0509a4b0(plVar3,1);
      }
      uStack_94 = 0xf;
      if (*(int *)(param_1 + 0x88) < 0x10) {
        puStack_90 = &uStack_94;
        plStack_88 = plVar2;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plVar3 = *(long **)(*plVar2 + 0x78);
      if (plVar3 != (long *)0x0) {
        (**(code **)(*plVar3 + 0x310))(plVar3,0);
        func_0x0509d4e0(plVar3,1);
        func_0x0509a4b0(plVar3,1);
      }
      uStack_94 = 0x16;
      if (*(int *)(param_1 + 0x78) < 0x17) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fb8970(*(undefined8 *)(*plVar1 + 0xb0),0);
      uStack_94 = 0x17;
      if (*(int *)(param_1 + 0x78) < 0x18) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fb8970(*(undefined8 *)(*plVar1 + 0xb8),0);
      uStack_94 = 0x16;
      if (*(int *)(param_1 + 0x88) < 0x17) {
        puStack_90 = &uStack_94;
        plStack_88 = plVar2;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plVar1 = *(long **)(*plVar2 + 0xb0);
      if (plVar1 != (long *)0x0) {
        (**(code **)(*plVar1 + 0x310))(plVar1,1);
        func_0x0509d4e0(plVar1,0);
        func_0x0509a4b0(plVar1,0);
      }
      uStack_94 = 0x17;
      if (*(int *)(param_1 + 0x88) < 0x18) {
        puStack_90 = &uStack_94;
        plStack_88 = plVar2;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plVar1 = *(long **)(*plVar2 + 0xb8);
      if (plVar1 != (long *)0x0) {
        (**(code **)(*plVar1 + 0x310))(plVar1,1);
        func_0x0509d4e0(plVar1,0);
        func_0x0509a4b0(plVar1,0);
      }
    }
    else if ((int)uStack_38 - 5U < 2) {
      plStack_88 = (long *)(param_1 + 0x60);
      if ((uStack_80 & (int)uStack_78 <= *(int *)(param_1 + 0x68)) != 1) {
        puStack_90 = &uStack_94;
        uStack_94 = uVar10;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      *(undefined8 *)(param_1 + 0xa0) = *(undefined8 *)(*(long *)(param_1 + 0x60) + lStack_70 * 8);
      uStack_94 = uStack_7c;
      if (((uint)uStack_58 & (int)uStack_68 <= *(int *)(param_1 + 0x68)) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      *(undefined8 *)(param_1 + 0xa8) = *(undefined8 *)(*(long *)(param_1 + 0x60) + uVar12 * 8);
      *(undefined2 *)(param_1 + 0xd8) = 0;
      plStack_88 = (long *)(param_1 + 0x70);
      uStack_94 = 0x16;
      if (*(int *)(param_1 + 0x78) < 0x17) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fb8970(*(undefined8 *)(*plStack_88 + 0xb0),1);
      uStack_94 = 0x17;
      if (*(int *)(param_1 + 0x78) < 0x18) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fb8970(*(undefined8 *)(*plStack_88 + 0xb8),1);
      plVar1 = (long *)(param_1 + 0x80);
      uStack_94 = 0x16;
      plStack_88 = plVar1;
      if (*(int *)(param_1 + 0x88) < 0x17) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plVar2 = *(long **)(*plVar1 + 0xb0);
      if (plVar2 != (long *)0x0) {
        (**(code **)(*plVar2 + 0x310))(plVar2,0);
        func_0x0509d4e0(plVar2,1);
        func_0x0509a4b0(plVar2,1);
      }
      uStack_94 = 0x17;
      if (*(int *)(param_1 + 0x88) < 0x18) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plVar1 = *(long **)(*plVar1 + 0xb8);
      if (plVar1 != (long *)0x0) {
        (**(code **)(*plVar1 + 0x310))(plVar1,0);
        func_0x0509d4e0(plVar1,1);
        func_0x0509a4b0(plVar1,1);
      }
    }
    else {
      *(undefined8 *)(param_1 + 0xa0) = 0;
      *(undefined8 *)(param_1 + 0xa8) = 0;
      *(undefined2 *)(param_1 + 0xd8) = 0x101;
      plStack_88 = (long *)(param_1 + 0x70);
      uStack_94 = 0xe;
      if (*(int *)(param_1 + 0x78) < 0xf) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fb8970(*(undefined8 *)(*plStack_88 + 0x70),0);
      uStack_94 = 0xf;
      if (*(int *)(param_1 + 0x78) < 0x10) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fb8970(*(undefined8 *)(*plStack_88 + 0x78),0);
      plVar1 = (long *)(param_1 + 0x80);
      uStack_94 = 0xe;
      plStack_88 = plVar1;
      if (*(int *)(param_1 + 0x88) < 0xf) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plVar2 = *(long **)(*plVar1 + 0x70);
      if (plVar2 != (long *)0x0) {
        (**(code **)(*plVar2 + 0x310))(plVar2,1);
        func_0x0509d4e0(plVar2,0);
        func_0x0509a4b0(plVar2,0);
      }
      uStack_94 = 0xf;
      if (*(int *)(param_1 + 0x88) < 0x10) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plVar1 = *(long **)(*plVar1 + 0x78);
      if (plVar1 != (long *)0x0) {
        (**(code **)(*plVar1 + 0x310))(plVar1,1);
        func_0x0509d4e0(plVar1,0);
        func_0x0509a4b0(plVar1,0);
      }
    }
    uVar11 = (int)uStack_78 * 2 - 2;
    uVar10 = ~uVar11 >> 0x1f;
    plVar1 = (long *)(param_1 + 0x70);
    uStack_94 = uVar11;
    plStack_88 = plVar1;
    if (((int)uVar11 < *(int *)(param_1 + 0x78) & uVar10) != 1) {
      puStack_90 = &uStack_94;
      func_0x06f8ce70(&puStack_90);
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    lVar13 = (long)(int)uVar11;
    lVar8 = *(long *)(*plVar1 + lVar13 * 8);
    if ((*(uint *)(lVar8 + 0x58) & 0x20) == 0) {
      if (((int)uVar11 < *(int *)(param_1 + 0x78) & uVar10) != 1) {
        puStack_90 = &uStack_94;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(0,0,0,lVar8,0,0,0,0,0,0);
      if (((int)uVar11 < *(int *)(param_1 + 0x88) & uVar10) != 1) {
        plStack_88 = (long *)(param_1 + 0x80);
        puStack_90 = &uStack_94;
        uStack_94 = uVar11;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plStack_88 = (long *)(param_1 + 0x90);
      if ((uVar10 & (int)uVar11 < *(int *)(param_1 + 0x98)) != 1) {
        puStack_90 = &uStack_94;
        uStack_94 = uVar11;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      uStack_94 = uVar11;
      func_0x050ac6c0((int)*(undefined8 *)(*plStack_88 + lVar13 * 0xc),
                      *(undefined4 *)(*plStack_88 + 8 + lVar13 * 0xc),
                      *(undefined8 *)(*(long *)(param_1 + 0x80) + lVar13 * 8),0);
    }
    iVar5 = (int)uStack_78 * 2;
    uVar11 = iVar5 - 1;
    uVar10 = (uint)((int)uStack_78 * -2) >> 0x1f;
    uStack_94 = uVar11;
    if ((iVar5 <= *(int *)(param_1 + 0x78) & uVar10) != 1) {
      puStack_90 = &uStack_94;
      plStack_88 = plVar1;
      func_0x06f8ce70(&puStack_90);
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    lVar13 = (long)(int)uVar11;
    lVar8 = *(long *)(*plVar1 + lVar13 * 8);
    if ((*(uint *)(lVar8 + 0x58) & 0x20) == 0) {
      if ((iVar5 <= *(int *)(param_1 + 0x78) & uVar10) != 1) {
        puStack_90 = &uStack_94;
        plStack_88 = plVar1;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(0,0,0,lVar8,0,0,0,0,0,0);
      if ((iVar5 <= *(int *)(param_1 + 0x88) & uVar10) != 1) {
        plStack_88 = (long *)(param_1 + 0x80);
        puStack_90 = &uStack_94;
        uStack_94 = uVar11;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plStack_88 = (long *)(param_1 + 0x90);
      if ((uVar10 & iVar5 <= *(int *)(param_1 + 0x98)) != 1) {
        puStack_90 = &uStack_94;
        uStack_94 = uVar11;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      uStack_94 = uVar11;
      func_0x050ac6c0((int)*(undefined8 *)(*plStack_88 + lVar13 * 0xc),
                      *(undefined4 *)(*plStack_88 + 8 + lVar13 * 0xc),
                      *(undefined8 *)(*(long *)(param_1 + 0x80) + lVar13 * 8),0);
    }
    uVar11 = (int)uStack_68 * 2 - 2;
    uVar10 = ~uVar11 >> 0x1f;
    uStack_94 = uVar11;
    if (((int)uVar11 < *(int *)(param_1 + 0x78) & uVar10) != 1) {
      puStack_90 = &uStack_94;
      plStack_88 = plVar1;
      func_0x06f8ce70(&puStack_90);
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    lVar13 = (long)(int)uVar11;
    lVar8 = *(long *)(*plVar1 + lVar13 * 8);
    if ((*(uint *)(lVar8 + 0x58) & 0x20) == 0) {
      if (((int)uVar11 < *(int *)(param_1 + 0x78) & uVar10) != 1) {
        puStack_90 = &uStack_94;
        plStack_88 = plVar1;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(0,0,0,lVar8,1,0,0,0,0,0);
      if (((int)uVar11 < *(int *)(param_1 + 0x98) & uVar10) != 1) {
        plStack_88 = (long *)(param_1 + 0x90);
        puStack_90 = &uStack_94;
        uStack_94 = uVar11;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      uVar9 = *(undefined8 *)(*(long *)(param_1 + 0x90) + lVar13 * 0xc);
      plStack_88 = (long *)(param_1 + 0x80);
      if ((uVar10 & (int)uVar11 < *(int *)(param_1 + 0x88)) != 1) {
        uStack_50 = 0;
        uStack_78 = CONCAT44(uStack_78._4_4_,
                             *(undefined4 *)(*(long *)(param_1 + 0x90) + 8 + lVar13 * 0xc));
        puStack_90 = &uStack_94;
        uStack_94 = uVar11;
        uStack_58 = uVar9;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      uStack_94 = uVar11;
      func_0x050ac6c0((int)uVar9,*(undefined8 *)(*plStack_88 + lVar13 * 8),0);
    }
    iVar5 = (int)uStack_68 * 2;
    uVar11 = iVar5 - 1;
    uVar10 = (uint)((int)uStack_68 * -2) >> 0x1f;
    uStack_94 = uVar11;
    if ((iVar5 <= *(int *)(param_1 + 0x78) & uVar10) != 1) {
      puStack_90 = &uStack_94;
      plStack_88 = plVar1;
      func_0x06f8ce70(&puStack_90);
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    lVar13 = (long)(int)uVar11;
    lVar8 = *(long *)(*plVar1 + lVar13 * 8);
    if ((*(uint *)(lVar8 + 0x58) & 0x20) == 0) {
      if ((iVar5 <= *(int *)(param_1 + 0x78) & uVar10) != 1) {
        puStack_90 = &uStack_94;
        plStack_88 = plVar1;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      func_0x02fc6cc0(0,0,0,lVar8,1,0,0,0,0,0);
      if ((iVar5 <= *(int *)(param_1 + 0x98) & uVar10) != 1) {
        plStack_88 = (long *)(param_1 + 0x90);
        puStack_90 = &uStack_94;
        uStack_94 = uVar11;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      uVar9 = *(undefined8 *)(*(long *)(param_1 + 0x90) + lVar13 * 0xc);
      plStack_88 = (long *)(param_1 + 0x80);
      if ((uVar10 & iVar5 <= *(int *)(param_1 + 0x88)) != 1) {
        uStack_60 = 0;
        uStack_78 = CONCAT44(uStack_78._4_4_,
                             *(undefined4 *)(*(long *)(param_1 + 0x90) + 8 + lVar13 * 0xc));
        puStack_90 = &uStack_94;
        uStack_94 = uVar11;
        uStack_68 = uVar9;
        func_0x06f8ce70(&puStack_90);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      uStack_94 = uVar11;
      func_0x050ac6c0((int)uVar9,*(undefined8 *)(*plStack_88 + lVar13 * 8),0);
    }
    if (lStack_48 != 0) {
      func_0x03071ea0();
    }
    param_4 = (uint)uStack_38;
  }
  if (*(long *)(param_1 + 0xa0) == 0) {
    if (4 < DAT_0722748c) {
      func_0x02fbc6d0();
    }
  }
  else if (4 < DAT_0722748c) {
    uVar6 = func_0x02fc0e90();
    puStack_90 = (uint *)CONCAT44(puStack_90._4_4_,uVar6);
    func_0x02fbc700(&puStack_90);
  }
  if (*(long *)(param_1 + 0xa8) == 0) {
    if (4 < DAT_0722748c) {
      func_0x02fbc730();
    }
  }
  else if (4 < DAT_0722748c) {
    uVar6 = func_0x02fc0e90();
    puStack_90 = (uint *)CONCAT44(puStack_90._4_4_,uVar6);
    func_0x02fbc760(&puStack_90);
  }
  if (param_4 == 6) {
    lVar8 = func_0x02fb4e80();
    uVar6 = *(undefined4 *)(lVar8 + 0x2c4);
    uVar15 = _DAT_006c1228;
    uVar16 = _DAT_006c1860;
    uVar17 = _DAT_006c0da4;
  }
  else if (param_4 == 5) {
    lVar8 = func_0x02fb4e80();
    uVar6 = *(undefined4 *)(lVar8 + 0x2c0);
    uVar15 = _DAT_006c0da4;
    uVar16 = _DAT_006c17bc;
    uVar17 = _DAT_006c14d4;
  }
  else {
    if (param_4 != 4) {
      return;
    }
    lVar8 = func_0x02fb4e80();
    uVar6 = *(undefined4 *)(lVar8 + 700);
    uVar15 = _DAT_006c14d4;
    uVar16 = _DAT_006c0924;
    uVar17 = _DAT_006c0fbc;
  }
  func_0x02fb23b0(uVar6,uVar16,uVar15,uVar17,param_1 + 0x200);
  return;
}



// ADDRESS 02fc77b0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fc77b0(undefined4 param_1,undefined4 param_2,long param_3,undefined4 param_4,
                      undefined4 param_5,int param_6,int param_7,undefined4 param_8)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  char cVar4;
  uint uVar5;
  undefined8 *puVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  long lVar11;
  undefined *puVar12;
  int iVar13;
  float fVar14;
  float fVar15;
  undefined1 auVar16 [16];
  float fVar17;
  undefined4 *puStack_88;
  undefined8 uStack_80;
  int iStack_78;
  int iStack_74;
  undefined4 uStack_70;
  float fStack_6c;
  float fStack_68;
  float fStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  int iStack_54;
  undefined4 uStack_50;
  int iStack_4c;
  undefined1 auStack_48 [16];
  
  iStack_78 = param_6;
  iStack_74 = param_7;
  uStack_70 = param_8;
  uStack_60 = param_4;
  uStack_5c = param_5;
  uStack_58 = param_1;
  uStack_50 = param_2;
  if (0 < *(int *)(param_3 + 0x248)) {
    lVar11 = 0;
    do {
      func_0x02fc3f70(*(undefined8 *)(*(long *)(param_3 + 0x240) + lVar11 * 8));
      lVar11 = lVar11 + 1;
    } while (lVar11 < *(int *)(param_3 + 0x248));
  }
  fStack_64 = (float)func_0x052effa0(*(undefined8 *)(param_3 + 0x238));
  cVar4 = *(char *)(param_3 + 0x250);
  fVar17 = _DAT_006c1b34;
  if (cVar4 != '\0') {
    fVar17 = _DAT_006c17b8 + fStack_64;
  }
  fStack_68 = (float)iStack_78;
  fStack_6c = (float)iStack_74;
  do {
    if (cVar4 == '\0') {
      fVar14 = (float)func_0x052effa0(*(undefined8 *)(param_3 + 0x238));
      if (fVar14 <= fVar17) {
        return;
      }
    }
    else if (fVar17 <= _DAT_006c0fa8) {
      return;
    }
    puVar6 = (undefined8 *)func_0x02fc7620(fVar17,param_3,0);
    if (puVar6 != (undefined8 *)0x0) {
      cVar4 = func_0x02fc4980(uStack_58,*puVar6,uStack_60,uStack_5c,uStack_70);
      _ZdlPv(puVar6);
      if (cVar4 == '\0') {
        if (1 < DAT_0722748c) {
          func_0x02fc7cb0(param_3 + 0x230);
        }
        lVar11 = func_0x02fb4e80();
        puVar12 = &UNK_009dcf72;
        if (*(int *)(lVar11 + 0x2e0) != 0) {
          puVar12 = *(undefined **)(lVar11 + 0x2d8);
        }
        iVar9 = func_0x030318e0(puVar12,&UNK_00ab470e);
        if (iVar9 == 0) {
          puStack_88 = (undefined4 *)0x0;
          uStack_80 = 0;
          func_0x025ead80(&puStack_88,9);
          iVar9 = (int)uStack_80 + 9;
          uStack_80 = CONCAT44(uStack_80._4_4_,iVar9);
          if (uStack_80._4_4_ < iVar9) {
            func_0x025eade0(&puStack_88);
          }
          uVar3 = _UNK_006d209c;
          uVar2 = _UNK_006d2098;
          uVar1 = _UNK_006d2094;
          *puStack_88 = _DAT_006d2090;
          puStack_88[1] = uVar1;
          puStack_88[2] = uVar2;
          puStack_88[3] = uVar3;
          *(undefined2 *)(puStack_88 + 4) = 0;
          lVar11 = func_0x02fb4e80();
          if ((undefined4 **)(lVar11 + 0x2d8) == &puStack_88) {
            if (puStack_88 != (undefined4 *)0x0) {
              func_0x03071ea0();
            }
          }
          else {
            if (*(long *)(lVar11 + 0x2d8) != 0) {
              func_0x03071ea0();
            }
            *(undefined4 **)(lVar11 + 0x2d8) = puStack_88;
            puStack_88 = (undefined4 *)0x0;
            *(int *)(lVar11 + 0x2e0) = (int)uStack_80;
            *(int *)(lVar11 + 0x2e4) = uStack_80._4_4_;
            uStack_80 = 0;
          }
          func_0x03006490(&puStack_88,&UNK_007d7bba,*(undefined4 *)(param_3 + 0x230));
          lVar11 = func_0x02fb4e80();
          if ((undefined4 **)(lVar11 + 0x2e8) == &puStack_88) {
            if (puStack_88 != (undefined4 *)0x0) {
              func_0x03071ea0();
            }
          }
          else {
            if (*(long *)(lVar11 + 0x2e8) != 0) {
              func_0x03071ea0();
            }
            *(undefined4 **)(lVar11 + 0x2e8) = puStack_88;
            *(int *)(lVar11 + 0x2f0) = (int)uStack_80;
            *(int *)(lVar11 + 0x2f4) = uStack_80._4_4_;
          }
        }
        return;
      }
    }
    cVar4 = *(char *)(param_3 + 0x250);
    fVar14 = fVar17;
    if (cVar4 == '\0') {
      fVar14 = fStack_64 - fVar17;
    }
    iVar9 = iStack_74;
    iVar7 = iStack_78;
    if (fVar14 < _DAT_006c110c) {
      fVar15 = (fVar14 * fStack_68) / _DAT_006c110c;
      iVar7 = -((int)ROUND(_DAT_006c12d4 - (fVar15 + fVar15)) >> 1);
      fVar14 = (fVar14 * fStack_6c) / _DAT_006c110c;
      iVar9 = -((int)ROUND(_DAT_006c12d4 - (fVar14 + fVar14)) >> 1);
    }
    if (0 < iVar9 + iVar7) {
      iVar13 = 0;
      iStack_54 = iVar9 + iVar7;
      iStack_4c = iVar7;
      do {
        uVar5 = 0;
        uVar10 = 0;
        do {
          cVar4 = *(char *)(param_3 + 0x250);
          lVar11 = func_0x02fcc220();
          uVar8 = *(int *)(lVar11 + 0x1a0) * 0xbb38435 + 0x3619636b;
          fVar14 = ((float)(uVar8 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1240 +
                   _DAT_006c123c;
          auVar16 = ZEXT416((uint)fVar14);
          if (cVar4 != '\0') {
            auVar16._0_4_ = (uint)fVar14 ^ _DAT_006c2f70;
            auVar16._4_4_ = _UNK_006c2f74;
            auVar16._8_4_ = _UNK_006c2f78;
            auVar16._12_4_ = _UNK_006c2f7c;
          }
          *(uint *)(lVar11 + 0x1a0) = uVar8;
          auStack_48._4_12_ = auVar16._4_12_;
          auStack_48._0_4_ = auVar16._0_4_ + fVar17;
          fVar15 = (float)func_0x052effa0(*(undefined8 *)(param_3 + 0x238));
          fVar14 = (float)auStack_48._0_4_;
          if (fVar15 <= (float)auStack_48._0_4_) {
            fVar14 = fVar15;
          }
          puVar6 = (undefined8 *)
                   func_0x02fc7620(~-(uint)((float)auStack_48._0_4_ < _DAT_006c0fa8) & (uint)fVar14,
                                   param_3,0);
          if (puVar6 != (undefined8 *)0x0) {
            uVar5 = func_0x02fc3fc0(uStack_50,*puVar6,iVar13 < iStack_4c,uStack_70);
            _ZdlPv(puVar6);
          }
        } while ((uVar10 < 1999) && (uVar10 = uVar10 + 1, (uVar5 & 1) == 0));
        iVar13 = iVar13 + 1;
      } while (iVar13 != iStack_54);
      cVar4 = *(char *)(param_3 + 0x250);
    }
    fVar17 = fVar17 + *(float *)(&UNK_006c2a50 + (ulong)(cVar4 == '\0') * 4);
  } while( true );
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



// ADDRESS 02fa1d70

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fa1d70(long param_1,long param_2)

{
  long *plVar1;
  int *piVar2;
  code *pcVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  char cVar7;
  int iVar8;
  undefined8 *puVar9;
  long lVar10;
  long *plVar11;
  undefined8 uVar12;
  int iVar13;
  undefined *puVar14;
  int iVar15;
  undefined8 *puStack_78;
  undefined8 uStack_70;
  undefined8 *puStack_60;
  long *plStack_58;
  undefined *puStack_50;
  int iStack_48;
  double dStack_40;
  double dStack_38;
  
  if (4 < DAT_0722748c) {
    func_0x02fa8930();
  }
  puVar9 = (undefined8 *)_Znwm(0x50);
  puVar9[4] = 0;
  puVar9[5] = 0;
  puVar9[6] = 0;
  puVar9[7] = 0;
  puVar9[8] = 0;
  puVar9[9] = 0;
  *puVar9 = 0;
  puVar9[1] = 0;
  puVar9[2] = 0;
  puVar9[3] = 0;
  *(undefined4 *)((long)puVar9 + 0x2c) = 0x80;
  puVar9[6] = 0xffffffff;
  puVar9[8] = 0;
  *(undefined4 *)(puVar9 + 9) = 0;
  plStack_58 = (long *)_Znwm(0x18);
  plStack_58[1] = 0x100000001;
  *plStack_58 = 0xbbf4e8;
  plStack_58[2] = (long)puVar9;
  puStack_60 = puVar9;
  lVar10 = _Znwm(0x88);
  func_0x026f4b40(lVar10,param_2 + 0x18);
  plVar11 = (long *)_Znwm(0x18);
  *plVar11 = 0xbbf510;
  plVar11[2] = lVar10;
  plVar11[1] = 0x100000001;
  cVar7 = func_0x026f5290(lVar10,&puStack_60,0);
  puVar9 = puStack_60;
  if (cVar7 != '\0') {
    if (puStack_60 == (undefined8 *)0x0) {
      func_0x06f8d412();
      pcVar3 = (code *)swi(3);
      (*pcVar3)();
      return;
    }
    puStack_78 = (undefined8 *)0x0;
    uStack_70 = 0;
    func_0x025ead80(&puStack_78,8);
    iVar8 = (int)uStack_70 + 8;
    uStack_70 = CONCAT44(uStack_70._4_4_,iVar8);
    if (uStack_70._4_4_ < iVar8) {
      func_0x025eade0(&puStack_78);
    }
    uVar6 = _UNK_006cd31c;
    uVar5 = _UNK_006cd318;
    uVar4 = _UNK_006cd314;
    *(undefined4 *)puStack_78 = _DAT_006cd310;
    *(undefined4 *)((long)puStack_78 + 4) = uVar4;
    *(undefined4 *)(puStack_78 + 1) = uVar5;
    *(undefined4 *)((long)puStack_78 + 0xc) = uVar6;
    func_0x031ddf70(&puStack_50,puVar9,&puStack_78);
    if (puStack_78 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    puVar14 = &UNK_009dcf72;
    if (iStack_48 != 0) {
      puVar14 = puStack_50;
    }
    iVar8 = func_0x030318e0(puVar14,&UNK_00b51166);
    if (iVar8 == 0) {
      if (4 < DAT_0722748c) {
        func_0x02fa8960();
      }
      puVar9 = puStack_60;
      if (puStack_60 == (undefined8 *)0x0) {
        func_0x06f8d412();
        pcVar3 = (code *)swi(3);
        (*pcVar3)();
        return;
      }
      puStack_78 = (undefined8 *)0x0;
      uStack_70 = 0;
      func_0x025ead80(&puStack_78,6);
      iVar8 = (int)uStack_70 + 6;
      uStack_70 = CONCAT44(uStack_70._4_4_,iVar8);
      if (uStack_70._4_4_ < iVar8) {
        func_0x025eade0(&puStack_78);
      }
      *puStack_78 = 0x67006100740073;
      *(undefined4 *)(puStack_78 + 1) = 0x65;
      dStack_38 = (double)func_0x031ddad0(puVar9,&puStack_78);
      if (puStack_78 != (undefined8 *)0x0) {
        func_0x03071ea0();
      }
      puVar9 = puStack_60;
      if (puStack_60 == (undefined8 *)0x0) {
        func_0x06f8d412();
        pcVar3 = (code *)swi(3);
        (*pcVar3)();
        return;
      }
      puStack_78 = (undefined8 *)0x0;
      uStack_70 = 0;
      func_0x025ead80(&puStack_78,0xc);
      iVar8 = (int)uStack_70 + 0xc;
      uStack_70 = CONCAT44(uStack_70._4_4_,iVar8);
      if (uStack_70._4_4_ < iVar8) {
        func_0x025eade0(&puStack_78);
      }
      uVar6 = _UNK_006d75fc;
      uVar5 = _UNK_006d75f8;
      uVar4 = _UNK_006d75f4;
      *(undefined4 *)puStack_78 = _DAT_006d75f0;
      *(undefined4 *)((long)puStack_78 + 4) = uVar4;
      *(undefined4 *)(puStack_78 + 1) = uVar5;
      *(undefined4 *)((long)puStack_78 + 0xc) = uVar6;
      puStack_78[2] = 0x74006e0075;
      dStack_40 = (double)func_0x031ddad0(puVar9,&puStack_78);
      if (puStack_78 != (undefined8 *)0x0) {
        func_0x03071ea0();
      }
      iVar13 = (int)dStack_38;
      iVar15 = (int)dStack_40;
      iVar8 = iVar13 * 10000 + iVar15;
      *(int *)(param_1 + 0x50) = iVar8;
      *(ulong *)(param_1 + 0xc) = CONCAT44(iVar8,iVar8);
      func_0x02fa6eb0(param_1);
      func_0x02fa24a0(param_1);
      func_0x02fa3190(param_1);
      func_0x02fa3dd0(param_1);
      puVar9 = (undefined8 *)_Znwm(0x20);
      puVar9[1] = 0xb;
      *puVar9 = 0xbbfff8;
      *(undefined4 *)(puVar9 + 2) = 2;
      *(int *)((long)puVar9 + 0x14) = iVar13;
      *(int *)(puVar9 + 3) = iVar15;
      uVar12 = func_0x02fcc220();
      func_0x02fcc520(uVar12,puVar9);
      *(undefined1 *)(param_1 + 0x130) = 1;
      *(undefined4 *)(param_1 + 0x134) = 0;
      *(undefined8 *)(param_1 + 0x138) = *(undefined8 *)(param_2 + 0x10);
    }
    else {
      puVar14 = &UNK_009dcf72;
      if (iStack_48 != 0) {
        puVar14 = puStack_50;
      }
      iVar8 = func_0x030318e0(puVar14,&UNK_009f7da9);
      if (iVar8 == 0) {
        if (4 < DAT_0722748c) {
          func_0x02fa8990();
        }
        func_0x02fa89c0(param_1,*(undefined8 *)(param_2 + 0x10));
      }
    }
    if (puStack_50 != (undefined *)0x0) {
      func_0x03071ea0();
    }
  }
  plVar1 = plVar11 + 1;
  *(int *)plVar1 = (int)*plVar1 + -1;
  if ((int)*plVar1 == 0) {
    (**(code **)*plVar11)(plVar11);
    piVar2 = (int *)((long)plVar11 + 0xc);
    *piVar2 = *piVar2 + -1;
    if (*piVar2 == 0) {
      (**(code **)(*plVar11 + 0x10))(plVar11);
    }
  }
  plVar11 = plStack_58;
  if (plStack_58 != (long *)0x0) {
    plVar1 = plStack_58 + 1;
    *(int *)plVar1 = (int)*plVar1 + -1;
    if ((int)*plVar1 == 0) {
      (**(code **)*plStack_58)(plStack_58);
      piVar2 = (int *)((long)plVar11 + 0xc);
      *piVar2 = *piVar2 + -1;
      if (*piVar2 == 0) {
        (**(code **)(*plVar11 + 0x10))(plVar11);
      }
    }
  }
  return;
}



// ADDRESS 02fa22d0

/* WARNING: Switch with 1 destination removed at 0x070a92a0 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fa22d0(long param_1,long param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined8 *puVar4;
  long lVar5;
  long lVar6;
  ulong uVar7;
  long lVar8;
  ulong uVar9;
  ulong uVar10;
  
  if (*(long *)(param_2 + 0x10) != _DAT_072d0ac0) {
    return;
  }
  *(ulong *)(param_1 + 0xc) =
       CONCAT44(*(undefined4 *)(param_1 + 0x50),*(undefined4 *)(param_1 + 0x50));
  if (4 < DAT_0722748c) {
    func_0x02faa9b0(param_1 + 0x50);
  }
  func_0x02fa6eb0(param_1);
  func_0x02fa24a0(param_1);
  func_0x02fa3190(param_1);
  func_0x02fa3dd0(param_1);
  puVar4 = (undefined8 *)_Znwm(0x20);
  *puVar4 = 0xbbfff8;
  *(undefined4 *)(puVar4 + 3) = 0;
  uVar3 = _UNK_006d56ac;
  uVar2 = _UNK_006d56a8;
  uVar1 = _UNK_006d56a4;
  *(undefined4 *)(puVar4 + 1) = _DAT_006d56a0;
  *(undefined4 *)((long)puVar4 + 0xc) = uVar1;
  *(undefined4 *)(puVar4 + 2) = uVar2;
  *(undefined4 *)((long)puVar4 + 0x14) = uVar3;
  lVar5 = func_0x02fcc220();
  lVar5 = *(long *)(lVar5 + 0xa0);
  _ZNSt3__15mutex4lockEv();
  uVar7 = *(ulong *)(lVar5 + 0x50);
  if (0x13ff < uVar7) {
    _ZNSt3__15mutex4lockEv(lVar5);
    if (*(long *)(lVar5 + 0x50) != 0) {
      *(long *)(lVar5 + 0x50) = *(long *)(lVar5 + 0x50) + -1;
      uVar7 = *(long *)(lVar5 + 0x48) + 1;
      *(ulong *)(lVar5 + 0x48) = uVar7;
      if (0x3ff < uVar7) {
        _ZdlPv(**(undefined8 **)(lVar5 + 0x30));
        *(long *)(lVar5 + 0x30) = *(long *)(lVar5 + 0x30) + 8;
        *(long *)(lVar5 + 0x48) = *(long *)(lVar5 + 0x48) + -0x200;
      }
    }
    _ZNSt3__15mutex6unlockEv(lVar5);
    uVar7 = *(ulong *)(lVar5 + 0x50);
  }
  lVar6 = *(long *)(lVar5 + 0x30);
  lVar8 = *(long *)(lVar5 + 0x38) - lVar6;
  uVar10 = lVar8 * 0x40 - 1;
  if (lVar8 == 0) {
    uVar10 = 0;
  }
  uVar9 = *(long *)(lVar5 + 0x48) + uVar7;
  if (uVar10 == uVar9) {
    func_0x02fd1b90(lVar5 + 0x28);
    lVar6 = *(long *)(lVar5 + 0x30);
    uVar7 = *(ulong *)(lVar5 + 0x50);
    uVar9 = *(long *)(lVar5 + 0x48) + uVar7;
  }
  *(undefined8 **)(*(long *)(lVar6 + (uVar9 >> 9) * 8) + (ulong)((uint)uVar9 & 0x1ff) * 8) = puVar4;
  *(ulong *)(lVar5 + 0x50) = uVar7 + 1;
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



