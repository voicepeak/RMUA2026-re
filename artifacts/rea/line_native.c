// ADDRESS 02fc3db0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fc3db0(undefined8 param_1,undefined8 param_2,undefined8 param_3,float param_4,
                      undefined8 param_5,char param_6)

{
  long lVar1;
  undefined8 *puVar2;
  float fVar3;
  undefined4 uVar4;
  undefined4 in_XMM1_Dc;
  undefined4 in_XMM1_Dd;
  float fVar5;
  float fVar6;
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  float fVar9;
  float fVar10;
  undefined8 uStack_a8;
  float fStack_a0;
  undefined1 auStack_98 [16];
  undefined8 uStack_88;
  float fStack_80;
  float fStack_78;
  float fStack_68;
  float fStack_64;
  float fStack_60;
  float fStack_5c;
  undefined8 uStack_58;
  float fStack_50;
  undefined4 uStack_4c;
  undefined1 auStack_48 [16];
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  
  fVar3 = (float)((ulong)param_3 >> 0x20);
  uVar4 = (undefined4)((ulong)param_2 >> 0x20);
  fStack_60 = (float)((ulong)param_1 >> 0x20);
  fVar9 = (float)param_1 - (float)param_3;
  fVar10 = fStack_60 - fVar3;
  fStack_a0 = (float)param_2 - param_4;
  uStack_a8 = CONCAT44(fVar10,fVar9);
  fVar5 = fStack_a0 * fStack_a0 + fVar10 * fVar10 + fVar9 * fVar9;
  auStack_98 = ZEXT416((uint)SQRT(fVar5));
  if (_DAT_006c1640 < fVar5) {
    auVar7._4_4_ = uVar4;
    auVar7._0_4_ = fVar5;
    auVar7._8_4_ = in_XMM1_Dc;
    auVar7._12_4_ = in_XMM1_Dd;
    auVar8._4_4_ = uVar4;
    auVar8._0_4_ = fVar5;
    auVar8._8_4_ = in_XMM1_Dc;
    auVar8._12_4_ = in_XMM1_Dd;
    auVar8 = rsqrtss(auVar7,auVar8);
    fVar6 = auVar8._0_4_;
    fVar6 = (_DAT_006c1394 - fVar6 * fVar6 * fVar5 * _DAT_006c1394) * fVar6 + fVar6;
    fVar6 = (_DAT_006c1394 - fVar6 * fVar6 * fVar5 * _DAT_006c1394) * fVar6 + fVar6;
    uStack_a8 = CONCAT44(fVar10 * fVar6,fVar9 * fVar6);
    fStack_a0 = fStack_a0 * fVar6;
  }
  fVar5 = ((float)param_2 + param_4) * _DAT_006c1394;
  fStack_60 = fStack_60 + fVar3;
  fStack_64 = fStack_60 * _DAT_006c1394;
  fStack_68 = ((float)param_1 + (float)param_3) * _DAT_006c1394;
  fStack_78 = fVar5;
  fStack_5c = fStack_60;
  func_0x03106220(&uStack_58,&uStack_a8);
  uStack_88 = func_0x031055f0(&uStack_58);
  if ((float)auStack_98._0_4_ <= _DAT_006c0c88) {
    auStack_98._0_4_ = _DAT_006c0c88;
  }
  fVar3 = ((float)auStack_98._0_4_ / _DAT_006c0c88) * _DAT_006c0f14;
  auStack_98._0_4_ = fVar3;
  uVar4 = auStack_98._4_4_;
  fStack_80 = fVar5;
  uStack_58 = func_0x03104ed0(&uStack_88);
  auStack_48._4_4_ = fStack_64;
  auStack_48._0_4_ = fStack_68;
  auStack_48._8_4_ = fStack_78;
  auStack_48._12_4_ = 0;
  uStack_38 = _DAT_006c5200;
  uStack_34 = _UNK_006c5204;
  uStack_30 = auStack_98._0_4_;
  uStack_2c = _UNK_006c520c;
  fStack_50 = fVar3;
  uStack_4c = uVar4;
  lVar1 = func_0x02fb4e80();
  puVar2 = (undefined8 *)(lVar1 + 0x240);
  if (param_6 == '\0') {
    puVar2 = (undefined8 *)(lVar1 + 0x248);
  }
  func_0x054775d0(*puVar2,&uStack_58);
  return;
}



// ADDRESS 02fc3fc0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 selected_02fc3fc0(float param_1,long param_2,undefined8 param_3,int param_4)

{
  long *plVar1;
  int iVar2;
  float **ppfVar3;
  ulong uVar4;
  code *pcVar5;
  long lVar6;
  undefined8 uVar7;
  uint uVar8;
  uint uVar9;
  float **ppfVar10;
  ulong *puVar11;
  bool bVar12;
  float fVar13;
  float extraout_XMM0_Dc;
  undefined8 extraout_XMM0_Qb;
  undefined1 auVar14 [16];
  undefined8 extraout_XMM0_Qb_00;
  undefined8 extraout_XMM0_Qb_01;
  undefined8 extraout_XMM0_Qb_02;
  undefined8 extraout_XMM0_Qb_03;
  undefined1 auVar15 [16];
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  undefined1 auVar24 [12];
  undefined1 auVar25 [12];
  undefined1 auVar26 [12];
  undefined1 auVar27 [12];
  undefined1 auVar28 [12];
  undefined1 auVar29 [12];
  float fStack_ec;
  float **ppfStack_b0;
  float fStack_a8;
  ulong *puStack_a0;
  undefined1 auStack_98 [8];
  undefined8 uStack_90;
  float *pfStack_88;
  float fStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  float fStack_68;
  float fStack_64;
  float fStack_60;
  float fStack_5c;
  undefined8 uStack_58;
  undefined8 uStack_50;
  float ****ppppfStack_48;
  ulong *puStack_40;
  float ***pppfStack_38;
  
  lVar6 = *(long *)(param_2 + 0x130);
  if (lVar6 == 0) {
    uStack_78 = _DAT_071fa860;
    uStack_70 = 0;
    fVar13 = _DAT_071fa868;
  }
  else {
    uStack_78 = *(undefined8 *)(lVar6 + 0x1d0);
    uStack_70 = *(undefined8 *)(lVar6 + 0x1d8);
    fVar13 = *(float *)(lVar6 + 0x1d8);
  }
  plVar1 = (long *)(param_2 + 0x250);
  puStack_a0 = (ulong *)(param_2 + 0x260);
  uStack_58 = CONCAT44(uStack_58._4_4_,param_1 * param_1);
  uVar9 = 0;
  while( true ) {
    lVar6 = func_0x02fcc220();
    uVar8 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
    *(uint *)(lVar6 + 0x1a0) = uVar8;
    auVar24._8_4_ = _DAT_071fa8b8;
    auVar24._0_8_ = _DAT_071fa8b0;
    fVar19 = ((float)(uVar8 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0f30 + _DAT_006c0fa8;
    if (*(long *)(param_2 + 0x130) != 0) {
      auVar24 = func_0x0526cb30();
    }
    lVar6 = func_0x02fcc220();
    uVar8 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
    *(uint *)(lVar6 + 0x1a0) = uVar8;
    fVar16 = ((float)(uVar8 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0e4c + _DAT_006c0b6c;
    if (*(long *)(param_2 + 0x130) == 0) {
      auVar14._8_8_ = 0;
      auVar14._0_8_ = _DAT_071fa8c8;
      lVar6 = 0;
      fVar18 = _DAT_071fa8d0;
    }
    else {
      auVar25 = func_0x052b78e0();
      auVar14._0_8_ = auVar25._0_8_;
      auVar14._8_8_ = extraout_XMM0_Qb;
      lVar6 = *(long *)(param_2 + 0x130);
      fVar18 = auVar25._8_4_;
    }
    fVar22 = fVar19 * auVar24._0_4_ + (float)uStack_78;
    fVar23 = fVar19 * auVar24._4_4_ + uStack_78._4_4_;
    fVar20 = fVar16 * auVar14._0_4_;
    fVar21 = fVar16 * auVar14._4_4_;
    if (lVar6 == 0) {
      uStack_90 = 0;
      auStack_98 = (undefined1  [8])_DAT_071fa898;
      lVar6 = 0;
      fVar17 = _DAT_071fa8a0;
    }
    else {
      fStack_68 = fVar20;
      fStack_64 = fVar21;
      fStack_60 = auVar14._8_4_ * 0.0;
      fStack_5c = auVar14._12_4_ * 0.0;
      auVar25 = func_0x052b7940();
      uStack_90 = extraout_XMM0_Qb_00;
      auStack_98 = (undefined1  [8])auVar25._0_8_;
      lVar6 = *(long *)(param_2 + 0x130);
      fVar17 = auVar25._8_4_;
      fVar20 = fStack_68;
      fVar21 = fStack_64;
    }
    auVar25._8_4_ = _DAT_071fa8a0;
    auVar25._0_8_ = _DAT_071fa898;
    fVar22 = fVar22 + fVar20;
    fVar23 = fVar23 + fVar21;
    fVar19 = auVar24._8_4_ * fVar19 + fVar13 + fVar18 * fVar16;
    if (lVar6 != 0) {
      auVar25 = func_0x052b7940();
    }
    fVar16 = fVar23 + auVar25._4_4_ * _DAT_006c0ac4;
    fVar18 = fVar22 + auVar25._0_4_ * _DAT_006d4060;
    fStack_80 = fVar19 + auVar25._8_4_ * _UNK_006d4064;
    pfStack_88 = (float *)CONCAT44(fVar16,fVar18);
    ppfVar3 = *(float ***)(param_2 + 0x250);
    iVar2 = *(int *)(param_2 + 600);
    if ((long)iVar2 == 0) break;
    ppfVar10 = ppfVar3;
    do {
      fVar20 = *(float *)((long)ppfVar10 + 4) - fVar16;
      fVar21 = *(float *)ppfVar10 - fVar18;
      bVar12 = false;
      if ((*(float *)(ppfVar10 + 1) - fStack_80) * (*(float *)(ppfVar10 + 1) - fStack_80) +
          fVar20 * fVar20 + fVar21 * fVar21 < (float)uStack_58) {
        bVar12 = true;
        break;
      }
      ppfVar10 = (float **)((long)ppfVar10 + 0xc);
    } while (ppfVar10 != (float **)((long)ppfVar3 + (long)iVar2 * 0xc));
    if (!bVar12) break;
    bVar12 = 0x7ce < uVar9;
    uVar9 = uVar9 + 1;
    if (bVar12) {
      return 0;
    }
  }
  ppfStack_b0 = &pfStack_88;
  if ((ppfVar3 <= ppfStack_b0) &&
     (ppfStack_b0 < (float **)((long)ppfVar3 + (long)*(int *)(param_2 + 0x25c) * 0xc))) {
    ppppfStack_48 = (float ****)&ppfStack_b0;
    puStack_40 = (ulong *)plVar1;
    func_0x06f8d480(&ppppfStack_48);
    pcVar5 = (code *)swi(3);
    uVar7 = (*pcVar5)();
    return uVar7;
  }
  fVar13 = (float)auStack_98._0_4_ * _DAT_006cb4f0;
  fVar16 = (float)auStack_98._4_4_ * _UNK_006cb4f4;
  fVar18 = (float)uStack_90 * _UNK_006cb4f8;
  fVar20 = uStack_90._4_4_ * _UNK_006cb4fc;
  fVar17 = fVar17 * _DAT_006c1ad8;
  *(int *)(param_2 + 600) = iVar2 + 1;
  if (*(int *)(param_2 + 0x25c) <= iVar2) {
    auStack_98._4_4_ = fVar16;
    auStack_98._0_4_ = fVar13;
    uStack_90._0_4_ = fVar18;
    uStack_90._4_4_ = fVar20;
    func_0x026af8a0(plVar1,iVar2);
    fVar13 = (float)auStack_98._0_4_;
    fVar16 = (float)auStack_98._4_4_;
  }
  fVar22 = fVar22 + fVar13;
  fVar23 = fVar23 + fVar16;
  fVar19 = fVar19 + fVar17;
  lVar6 = *plVar1;
  *(float *)(lVar6 + 8 + (long)iVar2 * 0xc) = fStack_80;
  *(float **)(lVar6 + (long)iVar2 * 0xc) = pfStack_88;
  if (param_4 == 3) {
    lVar6 = func_0x02fcc220();
    uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
    *(uint *)(lVar6 + 0x1a0) = uVar9;
    auStack_98._0_4_ =
         (((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0e50 + 0.0) * _DAT_006c0f10;
    lVar6 = func_0x02fcc220();
    uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
    *(uint *)(lVar6 + 0x1a0) = uVar9;
    fStack_68 = ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1ad0;
    fStack_64 = 0.0;
    fStack_60 = 0.0;
    fStack_5c = 0.0;
    if (*(long *)(param_2 + 0x130) == 0) {
      auVar27._8_4_ = _DAT_071fa8b8;
      auVar27._0_8_ = _DAT_071fa8b0;
      uStack_70 = 0;
      lVar6 = 0;
    }
    else {
      fStack_64 = 0.0;
      fStack_60 = 0.0;
      fStack_5c = 0.0;
      auVar27 = func_0x0526cb30();
      lVar6 = *(long *)(param_2 + 0x130);
      uStack_70 = extraout_XMM0_Qb_01;
    }
    puVar11 = puStack_a0;
    uStack_78 = auVar27._0_8_;
    fStack_68 = fStack_68 + _DAT_006c1ad0;
    fVar13 = (float)cosf();
    uStack_78 = CONCAT44(fVar13 * uStack_78._4_4_,fVar13 * (float)uStack_78);
    uStack_70 = CONCAT44(extraout_XMM0_Dc * uStack_70._4_4_,extraout_XMM0_Dc * (float)uStack_70);
    if (lVar6 == 0) {
      uStack_50 = 0;
      auVar28._8_4_ = _DAT_071fa8d0;
      auVar28._0_8_ = _DAT_071fa8c8;
    }
    else {
      auVar28 = func_0x052b78e0(lVar6);
      uStack_50 = extraout_XMM0_Qb_03;
    }
    fStack_ec = auVar28._8_4_;
    uStack_58 = auVar28._0_8_;
    fVar16 = (float)sinf();
    ppfStack_b0 = (float **)
                  CONCAT44(fVar23 + fStack_68 * (uStack_78._4_4_ + fVar16 * uStack_58._4_4_),
                           fVar22 + fStack_68 * ((float)uStack_78 + fVar16 * (float)uStack_58));
    fVar19 = fVar19 + (auVar27._8_4_ * fVar13 + fStack_ec * fVar16) * fStack_68;
  }
  else if (param_4 == 2) {
    lVar6 = func_0x02fcc220();
    uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
    *(uint *)(lVar6 + 0x1a0) = uVar9;
    fVar13 = ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1440;
    lVar6 = func_0x02fcc220();
    uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
    *(uint *)(lVar6 + 0x1a0) = uVar9;
    fVar16 = ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1b34;
    if (*(long *)(param_2 + 0x130) == 0) {
      auVar15._8_8_ = 0;
      auVar15._0_8_ = _DAT_071fa8c8;
      fVar18 = _DAT_071fa8d0;
    }
    else {
      auVar24 = func_0x052b78e0();
      auVar15._0_8_ = auVar24._0_8_;
      auVar15._8_8_ = extraout_XMM0_Qb_02;
      fVar18 = auVar24._8_4_;
    }
    puVar11 = puStack_a0;
    fVar16 = fVar16 + _DAT_006c1848;
    fStack_5c = 0.0;
    if ((int)fVar13 < 6) {
      fVar16 = (float)((uint)fVar16 ^ _DAT_006c2f70);
      fStack_5c = _UNK_006c2f78;
    }
    fStack_68 = fVar16 * auVar15._0_4_;
    fStack_64 = fVar16 * auVar15._4_4_;
    fStack_60 = fStack_5c * auVar15._8_4_;
    fStack_5c = fStack_5c * auVar15._12_4_;
    lVar6 = func_0x02fcc220();
    uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
    *(uint *)(lVar6 + 0x1a0) = uVar9;
    auVar29._8_4_ = _DAT_071fa8b8;
    auVar29._0_8_ = _DAT_071fa8b0;
    fVar13 = ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1b34 + _DAT_006c1458;
    if (*(long *)(param_2 + 0x130) != 0) {
      _auStack_98 = ZEXT416((uint)fVar13);
      auVar29 = func_0x0526cb30();
      fVar13 = auStack_98._0_4_;
    }
    ppfStack_b0 = (float **)
                  CONCAT44(fVar23 + fStack_64 + fVar13 * auVar29._4_4_,
                           fVar22 + fStack_68 + fVar13 * auVar29._0_4_);
    fVar19 = fVar19 + fVar18 * fVar16 + auVar29._8_4_ * fVar13;
  }
  else if (param_4 == 1) {
    lVar6 = func_0x02fcc220();
    uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
    *(uint *)(lVar6 + 0x1a0) = uVar9;
    fVar13 = ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1440;
    lVar6 = func_0x02fcc220();
    puVar11 = puStack_a0;
    uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
    *(uint *)(lVar6 + 0x1a0) = uVar9;
    auVar26._8_4_ = _DAT_071fa8d0;
    auVar26._0_8_ = _DAT_071fa8c8;
    fVar16 = ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1b34;
    if (*(long *)(param_2 + 0x130) != 0) {
      auVar26 = func_0x052b78e0();
    }
    fVar16 = fVar16 + _DAT_006c1848;
    if ((int)fVar13 < 6) {
      fVar16 = (float)((uint)fVar16 ^ _DAT_006c2f70);
    }
    ppfStack_b0 = (float **)
                  CONCAT44(fVar23 + fVar16 * auVar26._4_4_,fVar22 + fVar16 * auVar26._0_4_);
    fVar19 = fVar19 + auVar26._8_4_ * fVar16;
  }
  else {
    ppfStack_b0 = (float **)CONCAT44(fVar23,fVar22);
    puVar11 = puStack_a0;
  }
  pppfStack_38 = &ppfStack_b0;
  fStack_a8 = fVar19;
  if (((float ***)*puVar11 <= pppfStack_38) &&
     (pppfStack_38 < (float ***)((long)*puVar11 + (long)*(int *)(param_2 + 0x26c) * 0xc))) {
    ppppfStack_48 = &pppfStack_38;
    puStack_40 = puVar11;
    func_0x06f8d480(&ppppfStack_48);
    pcVar5 = (code *)swi(3);
    uVar7 = (*pcVar5)();
    return uVar7;
  }
  iVar2 = *(int *)(param_2 + 0x268);
  *(int *)(param_2 + 0x268) = iVar2 + 1;
  if (*(int *)(param_2 + 0x26c) <= iVar2) {
    func_0x026af8a0(puVar11,iVar2);
  }
  uVar4 = *puVar11;
  *(float *)(uVar4 + 8 + (long)iVar2 * 0xc) = fStack_a8;
  *(float ***)(uVar4 + (long)iVar2 * 0xc) = ppfStack_b0;
  selected_02fc3db0(ppfStack_b0);
  return 1;
}



// ADDRESS 02fc9220

void selected_02fc9220(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                      long *param_5,undefined8 param_6,undefined8 param_7)

{
  long lVar1;
  long *plStack_38;
  int iStack_30;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  
  plStack_38 = (long *)0x0;
  iStack_30 = 0;
  uStack_28 = param_1;
  uStack_24 = param_2;
  uStack_20 = param_3;
  uStack_1c = param_4;
  plStack_38 = (long *)func_0x03071ed0(0,0x40,0);
  iStack_30 = 4;
  func_0x02fc9f30(uStack_28,uStack_24,uStack_20,uStack_1c,plStack_38,param_6,param_7);
  if (&plStack_38 != (long **)param_5) {
    if ((iStack_30 == 0) || (plStack_38 == (long *)0x0)) {
      if (((int)param_5[1] != 0) && ((undefined8 *)*param_5 != (undefined8 *)0x0)) {
        (*(code *)**(undefined8 **)*param_5)();
        if (*param_5 != 0) {
          lVar1 = func_0x03071ed0(*param_5,0,0);
          *param_5 = lVar1;
        }
        *(undefined4 *)(param_5 + 1) = 0;
      }
    }
    else {
      (**(code **)(*plStack_38 + 0x48))(plStack_38,param_5);
    }
  }
  if ((iStack_30 != 0) && (plStack_38 != (long *)0x0)) {
    (**(code **)*plStack_38)();
    if (plStack_38 != (long *)0x0) {
      plStack_38 = (long *)func_0x03071ed0(plStack_38,0,0);
    }
    iStack_30 = 0;
  }
  func_0x0263ea90(&plStack_38);
  return;
}



