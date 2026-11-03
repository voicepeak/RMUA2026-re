// ADDRESS 02fa65b0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fa65b0(long *param_1,ulong *param_2,undefined8 *param_3,undefined8 *param_4)

{
  int iVar1;
  long lVar2;
  ulong uVar3;
  undefined1 *puVar4;
  code *pcVar5;
  uint uVar6;
  uint uVar7;
  long lVar8;
  int iVar9;
  float fVar10;
  float fVar12;
  undefined1 auVar11 [16];
  float fVar13;
  float fVar14;
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  undefined1 auVar17 [16];
  float fVar18;
  undefined1 auVar19 [16];
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  float fVar27;
  float fVar28;
  float fVar29;
  float fVar30;
  undefined8 uStack_58;
  float fStack_50;
  undefined1 *puStack_48;
  undefined1 **ppuStack_40;
  ulong *puStack_38;
  
  lVar2 = *param_1;
  iVar9 = 0;
  auVar19._8_8_ = _UNK_006c5208;
  auVar19._0_8_ = _DAT_006c5200;
  fVar20 = _DAT_006d1c50;
  fVar21 = _UNK_006d1c54;
  fVar22 = _UNK_006d1c5c;
  fVar23 = _DAT_006c0d00;
  fVar24 = _DAT_006c0a14;
  fVar25 = _DAT_006c0910;
  fVar26 = _DAT_006c1394;
  fVar27 = _DAT_006c1640;
  fVar28 = _DAT_006c1438;
  fVar29 = _DAT_006c0f1c;
  fVar30 = _DAT_006c1b34;
  do {
    uVar6 = *(uint *)(lVar2 + 0x10);
    do {
      do {
        uVar6 = uVar6 * 0xbb38435 + 0x3619636b;
        uVar7 = uVar6 * 0xbb38435 + 0x3619636b;
        auVar11._0_4_ = uVar6 >> 9;
        auVar11._4_4_ = uVar7 >> 9;
        auVar11._8_8_ = 0;
        auVar11 = auVar11 | auVar19;
        fVar10 = auVar11._0_4_ + fVar20;
        fVar12 = auVar11._4_4_ + fVar21;
        fVar13 = auVar11._12_4_ + fVar22;
        fVar10 = fVar10 + fVar10 + fVar20;
        fVar12 = fVar12 + fVar12 + fVar21;
        uVar6 = uVar7 * 0xbb38435 + 0x3619636b;
        fVar14 = (float)(uVar6 >> 9 | 0x3f800000) + fVar23;
        fVar14 = fVar14 + fVar14 + fVar23;
        fVar14 = fVar14 * fVar14 + fVar12 * fVar12 + fVar10 * fVar10;
      } while (fVar24 < fVar14);
    } while (fVar14 < fVar25);
    *(uint *)(lVar2 + 0x10) = uVar6;
    auVar11 = rsqrtss(ZEXT416((uint)fVar14),ZEXT416((uint)fVar14));
    fVar18 = auVar11._0_4_;
    fVar18 = (fVar26 - fVar18 * fVar18 * fVar14 * fVar26) * fVar18 + fVar18;
    fVar18 = (fVar26 - fVar18 * fVar18 * fVar14 * fVar26) * fVar18 + fVar18;
    fVar10 = fVar10 * fVar18;
    fVar12 = fVar12 * fVar18;
    fVar13 = (fVar13 + fVar13 + fVar22) * 0.0;
    auVar15._4_4_ = fVar12 * fVar12;
    auVar15._12_4_ = fVar13 * fVar13;
    auVar16._4_4_ = auVar15._4_4_;
    auVar16._0_4_ = auVar15._4_4_;
    auVar16._8_4_ = auVar15._12_4_;
    auVar16._12_4_ = auVar15._12_4_;
    auVar17._4_12_ = auVar16._4_12_;
    fVar13 = 0.0;
    auVar17._0_4_ = auVar15._4_4_ + fVar10 * fVar10 + 0.0;
    if (fVar27 < auVar17._0_4_) {
      auVar15._0_4_ = auVar17._0_4_;
      auVar15._8_4_ = auVar15._12_4_;
      auVar11 = rsqrtss(auVar15,auVar17);
      fVar13 = auVar11._0_4_;
      fVar13 = (fVar26 - fVar13 * fVar13 * auVar17._0_4_ * fVar26) * fVar13 + fVar13;
      fVar13 = (fVar26 - fVar13 * fVar13 * auVar17._0_4_ * fVar26) * fVar13 + fVar13;
      fVar10 = fVar10 * fVar13;
      fVar12 = fVar12 * fVar13;
      fVar13 = fVar13 * 0.0;
    }
    uVar6 = uVar6 * 0xbb38435 + 0x3619636b;
    *(uint *)(lVar2 + 0x10) = uVar6;
    fVar14 = (((float)(uVar6 >> 9 | 0x3f800000) + fVar23) * fVar28 + fVar29) * fVar30;
    fVar10 = (float)*param_3 + fVar14 * fVar10;
    fVar12 = (float)((ulong)*param_3 >> 0x20) + fVar14 * fVar12;
    uStack_58 = CONCAT44(fVar12,fVar10);
    fStack_50 = fVar14 * fVar13 + *(float *)(param_3 + 1);
    puVar4 = (undefined1 *)*param_2;
    iVar1 = (int)param_2[1];
    if ((long)iVar1 != 0) {
      lVar8 = 0;
      do {
        fVar13 = (float)*(undefined8 *)(puVar4 + lVar8) - fVar10;
        fVar14 = (float)((ulong)*(undefined8 *)(puVar4 + lVar8) >> 0x20) - fVar12;
        if (SQRT(fVar14 * fVar14 + fVar13 * fVar13) < _DAT_006c14d8) goto LAB_02fa664f;
        lVar8 = lVar8 + 0xc;
      } while ((long)iVar1 * 0xc != lVar8);
    }
    fVar10 = (float)*param_4 - fVar10;
    fVar12 = (float)((ulong)*param_4 >> 0x20) - fVar12;
    if (_DAT_006c14d8 < SQRT(fVar12 * fVar12 + fVar10 * fVar10)) {
      if ((puVar4 <= &uStack_58) &&
         (&uStack_58 < puVar4 + (long)*(int *)((long)param_2 + 0xc) * 0xc)) {
        ppuStack_40 = &puStack_48;
        puStack_48 = (undefined1 *)&uStack_58;
        puStack_38 = param_2;
        func_0x06f8d480(&ppuStack_40);
        pcVar5 = (code *)swi(3);
        (*pcVar5)();
        return;
      }
      *(int *)(param_2 + 1) = iVar1 + 1;
      if (*(int *)((long)param_2 + 0xc) <= iVar1) {
        puStack_48 = (undefined1 *)&uStack_58;
        func_0x026af8a0(param_2,iVar1);
        auVar19._8_8_ = _UNK_006c5208;
        auVar19._0_8_ = _DAT_006c5200;
        fVar20 = _DAT_006d1c50;
        fVar21 = _UNK_006d1c54;
        fVar22 = _UNK_006d1c5c;
        fVar23 = _DAT_006c0d00;
        fVar24 = _DAT_006c0a14;
        fVar25 = _DAT_006c0910;
        fVar26 = _DAT_006c1394;
        fVar27 = _DAT_006c1640;
        fVar28 = _DAT_006c1438;
        fVar29 = _DAT_006c0f1c;
        fVar30 = _DAT_006c1b34;
      }
      uVar3 = *param_2;
      *(float *)(uVar3 + 8 + (long)iVar1 * 0xc) = fStack_50;
      *(undefined8 *)(uVar3 + (long)iVar1 * 0xc) = uStack_58;
      iVar9 = iVar9 + 1;
    }
LAB_02fa664f:
    if (2 < iVar9) {
      return;
    }
  } while( true );
}



// ADDRESS 02faafa0

long selected_02faafa0(long param_1,int param_2)

{
  code *pcVar1;
  long lVar2;
  undefined8 uVar3;
  undefined *puVar4;
  long lVar5;
  undefined *puVar6;
  undefined *puStack_30;
  int iStack_28;
  undefined *puStack_20;
  int iStack_18;
  undefined8 uStack_10;
  
  if (param_1 == 0) {
    if (param_2 != 1) {
      return 0;
    }
  }
  else {
    lVar2 = func_0x02fd9270();
    if ((*(int *)(lVar2 + 0x38) <= *(int *)(*(long *)(param_1 + 0x10) + 0x38)) &&
       (*(long *)(*(long *)(*(long *)(param_1 + 0x10) + 0x30) + (long)*(int *)(lVar2 + 0x38) * 8) ==
        lVar2 + 0x30)) {
      return param_1;
    }
    lVar2 = func_0x033e9190();
    if (*(int *)(*(long *)(param_1 + 0x10) + 0x38) < *(int *)(lVar2 + 0x38)) {
      lVar5 = 0;
    }
    else {
      lVar5 = 0;
      if (*(long *)(*(long *)(*(long *)(param_1 + 0x10) + 0x30) + (long)*(int *)(lVar2 + 0x38) * 8)
          == lVar2 + 0x30) {
        lVar5 = param_1;
      }
    }
    func_0x0339ada0(&puStack_30,lVar5,0,0);
    puVar4 = &UNK_009dcf72;
    if (iStack_28 != 0) {
      puVar4 = puStack_30;
    }
    lVar2 = func_0x02fd9270();
    uStack_10 = *(undefined8 *)(lVar2 + 0x18);
    func_0x031d6e10(&puStack_20,&uStack_10);
    puVar6 = &UNK_009dcf72;
    if (iStack_18 != 0) {
      puVar6 = puStack_20;
    }
    func_0x0327d6b0(puVar4,puVar6);
  }
  lVar2 = func_0x02fd9270();
  puStack_20 = *(undefined **)(lVar2 + 0x18);
  func_0x031d6e10(&puStack_30,&puStack_20);
  puVar4 = &UNK_009dcf72;
  if (iStack_28 != 0) {
    puVar4 = puStack_30;
  }
  uVar3 = func_0x0327d6b0(&UNK_0081d726,puVar4);
  if (puStack_30 != (undefined *)0x0) {
    func_0x03071ea0();
  }
  uVar3 = _Unwind_Resume(uVar3);
  func_0x0260f540(uVar3);
  pcVar1 = (code *)swi(3);
  lVar2 = (*pcVar1)();
  return lVar2;
}



// ADDRESS 02fae0b0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 selected_02fae0b0(void)

{
  return _DAT_0719e080;
}



// ADDRESS 02fd9270

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long selected_02fd9270(void)

{
  if (_DAT_071a4f40 != 0) {
    return _DAT_071a4f40;
  }
  func_0x0327dc50(&UNK_0091a3a8,&UNK_00775d0e,&DAT_071a4f40,&UNK_02fd9150,0x230,8,0x10000004,0,
                  &UNK_007d3fa8,&UNK_02fd92e0,&UNK_02fd92f0,&UNK_05098920,&UNK_026c73d0,
                  &UNK_026c6e30,0,0);
  return _DAT_071a4f40;
}



// ADDRESS 02fa3dd0

/* WARNING: Removing unreachable block (ram,0x02fa4331) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fa3dd0(long param_1)

{
  int iVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *puVar4;
  uint uVar5;
  long *plVar6;
  undefined8 uVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  undefined8 uVar11;
  long lVar12;
  float fVar13;
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  undefined8 *puStack_118;
  undefined8 uStack_110;
  undefined1 auStack_108 [8];
  long lStack_100;
  undefined8 *puStack_f8;
  float fStack_f0;
  undefined8 uStack_e8;
  undefined4 uStack_e0;
  float fStack_dc;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  float fStack_c0;
  undefined8 *puStack_b8;
  float fStack_ac;
  undefined8 **ppuStack_a8;
  ulong *puStack_a0;
  undefined8 *puStack_78;
  undefined4 uStack_70;
  float fStack_68;
  float fStack_64;
  float fStack_60;
  undefined1 auStack_58 [8];
  float fStack_50;
  float fStack_4c;
  long lStack_40;
  undefined1 *puStack_38;
  
  puStack_38 = auStack_108;
  puStack_118 = (undefined8 *)0x0;
  uStack_110 = 0;
  puStack_a0 = (ulong *)(param_1 + 0x20);
  uStack_c8._0_4_ = 1.82169e-44;
  lStack_100 = param_1;
  lStack_40 = param_1;
  if (*(int *)(param_1 + 0x28) < 0xe) {
    ppuStack_a8 = (undefined8 **)&uStack_c8;
    func_0x06f8ce70(&ppuStack_a8);
    pcVar3 = (code *)swi(3);
    (*pcVar3)();
    return;
  }
  lVar10 = *(long *)(*(long *)(*puStack_a0 + 0x68) + 0x130);
  if (lVar10 == 0) {
    uStack_e0 = 0;
    fStack_dc = 0.0;
    fStack_60 = _DAT_071fa868;
    fStack_68 = _DAT_071fa860;
    fStack_64 = fRam00000000071fa864;
  }
  else {
    uStack_e0 = *(undefined4 *)(lVar10 + 0x1d8);
    fStack_dc = *(float *)(lVar10 + 0x1dc);
    fStack_60 = *(float *)(lVar10 + 0x1d8);
    fStack_68 = *(float *)(lVar10 + 0x1d0);
    fStack_64 = *(float *)(lVar10 + 0x1d4);
  }
  puStack_f8 = (undefined8 *)CONCAT44(puStack_f8._4_4_,0xe);
  if (0xe < *(int *)(param_1 + 0x28)) {
    lVar10 = *(long *)(*(long *)(*puStack_a0 + 0x70) + 0x130);
    if (lVar10 == 0) {
      fStack_50 = 0.0;
      fStack_c0 = _DAT_071fa868;
      uStack_c8._0_4_ = _DAT_071fa860;
      uStack_c8._4_4_ = fRam00000000071fa864;
    }
    else {
      fStack_50 = *(float *)(lVar10 + 0x1dc);
      fStack_c0 = *(float *)(lVar10 + 0x1d8);
      uStack_c8._0_4_ = *(float *)(lVar10 + 0x1d0);
      uStack_c8._4_4_ = *(float *)(lVar10 + 0x1d4);
    }
    uVar5 = *(int *)(param_1 + 0x10) * 0xbb38435 + 0x3619636b;
    *(uint *)(param_1 + 0x10) = uVar5;
    auStack_58._4_4_ = uStack_c8._4_4_ - fStack_64;
    fStack_50 = fStack_50 - fStack_dc;
    fVar19 = (float)uStack_c8 - fStack_68;
    fStack_dc = uStack_c8._4_4_ - fStack_64;
    fVar16 = (fStack_c0 - fStack_60) * 0.0;
    fVar13 = (float)auStack_58._4_4_ - fVar16;
    uStack_e8 = (ulong *)CONCAT44(fVar19,fStack_c0 - fStack_60);
    uStack_e0 = 0;
    fVar16 = fVar16 - fVar19;
    fVar19 = fVar19 * 0.0 - (float)auStack_58._4_4_ * 0.0;
    fVar17 = fStack_dc * 0.0 - fStack_dc;
    fVar20 = fVar19 * fVar19;
    fVar17 = fVar17 * fVar17;
    fVar18 = fVar20 + fVar13 * fVar13 + fVar16 * fVar16;
    auStack_58._0_4_ = auStack_58._4_4_;
    fStack_4c = fStack_50;
    if (_DAT_006c1640 < fVar18) {
      auVar14._4_4_ = fVar20;
      auVar14._0_4_ = fVar18;
      auVar14._8_4_ = fVar17;
      auVar14._12_4_ = fVar17;
      auVar15._4_4_ = fVar20;
      auVar15._0_4_ = fVar18;
      auVar15._8_4_ = fVar17;
      auVar15._12_4_ = fVar17;
      auVar15 = rsqrtss(auVar14,auVar15);
      fVar17 = auVar15._0_4_;
      fVar17 = (_DAT_006c1394 - fVar17 * fVar17 * fVar18 * _DAT_006c1394) * fVar17 + fVar17;
      fVar17 = (_DAT_006c1394 - fVar17 * fVar17 * fVar18 * _DAT_006c1394) * fVar17 + fVar17;
      fVar13 = fVar13 * fVar17;
      fVar16 = fVar16 * fVar17;
      fVar19 = fVar19 * fVar17;
    }
    fVar17 = (float)(int)(((float)(uVar5 >> 9 | 0x3f800000) + _DAT_006c0d00 + _DAT_006c14d4) *
                         _DAT_006c1b34);
    fStack_f0 = (fStack_60 + fStack_c0) * _DAT_006c1394 + fVar19 * fVar17;
    puStack_f8 = (undefined8 *)
                 CONCAT44((fStack_64 + uStack_c8._4_4_) * _UNK_006d7b64 + fVar17 * fVar16,
                          (fStack_68 + (float)uStack_c8) * _DAT_006d7b60 + fVar17 * fVar13);
    puStack_78 = &puStack_f8;
    uStack_110 = 1;
    func_0x026af8a0(&puStack_118,0);
    *(float *)(puStack_118 + 1) = fStack_f0;
    *puStack_118 = puStack_f8;
    fStack_ac = SQRT((float)auStack_58._0_4_ * (float)auStack_58._0_4_ +
                     uStack_e8._4_4_ * uStack_e8._4_4_);
    if (4 < DAT_0722748c) {
      func_0x02fa6580(&fStack_ac);
    }
    selected_02fa65b0(&lStack_40,&puStack_118,&fStack_68,&uStack_c8);
    selected_02fa65b0(&lStack_40,&puStack_118,&uStack_c8,&fStack_68);
    if (4 < DAT_0722748c) {
      ppuStack_a8 = (undefined8 **)CONCAT44(ppuStack_a8._4_4_,(int)uStack_110);
      func_0x02fa6990(&ppuStack_a8);
    }
    puStack_78 = (undefined8 *)0x0;
    uStack_70 = 0;
    if (0 < (int)uStack_110) {
      uStack_e8 = (ulong *)(lStack_100 + 0x30);
      lVar10 = 0;
      lVar12 = 0;
      do {
        plVar6 = (long *)selected_02fae0b0();
        uVar7 = (**(code **)(*plVar6 + 0x168))(plVar6);
        lVar8 = selected_02fae0b0();
        lVar2 = *(long *)(lVar8 + 0x240);
        if ((((lVar2 == 0) || (lVar9 = selected_02fd9270(), lVar9 == 0)) ||
            (*(int *)(lVar2 + 0x38) < *(int *)(lVar9 + 0x38))) ||
           (*(long *)(*(long *)(lVar2 + 0x30) + (long)*(int *)(lVar9 + 0x38) * 8) != lVar9 + 0x30))
        {
          uVar11 = 0;
        }
        else {
          uVar11 = *(undefined8 *)(lVar8 + 0x240);
        }
        puVar4 = puStack_118;
        puStack_b8 = (undefined8 *)CONCAT44(puStack_b8._4_4_,(int)lVar12);
        if ((int)uStack_110 <= lVar12) {
          ppuStack_a8 = &puStack_b8;
          puStack_a0 = (ulong *)&puStack_118;
          func_0x06f8ce70(&ppuStack_a8);
          pcVar3 = (code *)swi(3);
          (*pcVar3)();
          return;
        }
        func_0x059a5c60(&ppuStack_a8);
        uVar7 = func_0x054e1610(uVar7,uVar11,(long)puVar4 + lVar10,&puStack_78,&ppuStack_a8);
        uStack_d0 = selected_02faafa0(uVar7,0);
        puStack_b8 = &uStack_d0;
        if (((undefined8 *)*uStack_e8 <= puStack_b8) &&
           (&uStack_d0 < (undefined8 *)*uStack_e8 + *(int *)(lStack_100 + 0x3c))) {
          ppuStack_a8 = &puStack_b8;
          puStack_a0 = uStack_e8;
          func_0x06f8cfe0(&ppuStack_a8);
          pcVar3 = (code *)swi(3);
          (*pcVar3)();
          return;
        }
        iVar1 = *(int *)(lStack_100 + 0x38);
        *(int *)(lStack_100 + 0x38) = iVar1 + 1;
        if (*(int *)(lStack_100 + 0x3c) <= iVar1) {
          func_0x025ee150(uStack_e8,iVar1);
        }
        *(undefined8 *)(*uStack_e8 + (long)iVar1 * 8) = uStack_d0;
        lVar12 = lVar12 + 1;
        lVar10 = lVar10 + 0xc;
      } while (lVar12 < (int)uStack_110);
    }
    if (4 < DAT_0722748c) {
      ppuStack_a8 = (undefined8 **)CONCAT44(ppuStack_a8._4_4_,*(undefined4 *)(lStack_100 + 0x38));
      func_0x02fa69c0(&ppuStack_a8);
    }
    if (puStack_118 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    return;
  }
  _auStack_58 = ZEXT416((uint)fStack_60);
  uStack_e8 = (ulong *)CONCAT44(fStack_64,fStack_68);
  ppuStack_a8 = &puStack_f8;
  func_0x06f8ce70(&ppuStack_a8);
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}



