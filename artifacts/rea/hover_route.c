// ADDRESS 02fc93c0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fc93c0(float param_1,float param_2,undefined8 param_3,float param_4,long param_5)

{
  long lVar1;
  code *pcVar2;
  int iVar3;
  long lVar4;
  long *plVar5;
  long lVar6;
  ulong uVar7;
  undefined8 *puVar8;
  float fVar9;
  undefined8 uVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  undefined8 *puStack_58;
  undefined8 uStack_50;
  float fStack_40;
  float fStack_3c;
  float fStack_38;
  undefined4 uStack_34;
  long alStack_30 [2];
  
  fStack_40 = param_1;
  fStack_3c = param_4;
  fStack_38 = param_2;
  lVar4 = func_0x02fb4e80();
  puStack_58 = (undefined8 *)0x0;
  uStack_50 = (undefined8 *)0x0;
  func_0x025ead80(&puStack_58,6);
  iVar3 = (int)uStack_50 + 6;
  uStack_50 = (undefined8 *)CONCAT44(uStack_50._4_4_,iVar3);
  if (uStack_50._4_4_ < iVar3) {
    func_0x025eade0(&puStack_58);
  }
  *puStack_58 = 0x74006100740053;
  *(undefined4 *)(puStack_58 + 1) = 0x65;
  puVar8 = (undefined8 *)&UNK_009dcf72;
  if ((int)uStack_50 != 0) {
    puVar8 = puStack_58;
  }
  iVar3 = func_0x03000d20(lVar4 + 0x308,puVar8,1,0,0xffffffff);
  if (puStack_58 != (undefined8 *)0x0) {
    func_0x03071ea0();
  }
  if ((0.0 < fStack_3c) && (iVar3 != -1)) {
    fVar9 = 0.0;
    if (0.0 <= fStack_40) {
      fVar9 = fStack_40;
    }
    uVar10 = func_0x052f01a0(fVar9,*(undefined8 *)(param_5 + 0x238),1);
    if (0 < (int)*(uint *)(param_5 + 0x248)) {
      uVar7 = 0;
      lVar4 = 0;
      fVar9 = _DAT_006c1588;
      do {
        lVar6 = *(long *)(*(long *)(param_5 + 0x240) + uVar7 * 8);
        lVar1 = *(long *)(lVar6 + 0x130);
        fVar13 = SUB84(_DAT_071fa860,0);
        fVar12 = (float)((ulong)_DAT_071fa860 >> 0x20);
        fVar11 = _DAT_071fa868;
        if (lVar1 != 0) {
          fVar13 = *(float *)(lVar1 + 0x1d0);
          fVar12 = *(float *)(lVar1 + 0x1d4);
          fVar11 = *(float *)(lVar1 + 0x1d8);
        }
        fVar12 = fVar12 - (float)((ulong)uVar10 >> 0x20);
        fVar13 = fVar13 - (float)uVar10;
        fVar11 = SQRT(fVar12 * fVar12 + fVar13 * fVar13 + (fVar11 - param_2) * (fVar11 - param_2));
        if (fVar9 <= fVar11) {
          fVar11 = fVar9;
          lVar6 = lVar4;
        }
        lVar4 = lVar6;
        fVar9 = fVar11;
        uVar7 = uVar7 + 1;
      } while (*(uint *)(param_5 + 0x248) != uVar7);
      if (lVar4 != 0) {
        lVar6 = *(long *)(lVar4 + 0x130);
        puStack_58 = _DAT_071fa860;
        fVar9 = _DAT_071fa868;
        if (lVar6 != 0) {
          puStack_58 = *(undefined8 **)(lVar6 + 0x1d0);
          fVar9 = *(float *)(lVar6 + 0x1d8);
        }
        uStack_50 = (undefined8 *)CONCAT44(uStack_50._4_4_,fVar9);
        func_0x052f1190(*(undefined8 *)(param_5 + 0x238),&puStack_58);
        func_0x02fc6590(*(undefined8 *)(param_5 + 0x238));
        plVar5 = (long *)_Znwm(0x10);
        *plVar5 = lVar4;
        *(undefined1 *)(plVar5 + 1) = 1;
        lVar6 = func_0x02fb4e80();
        uStack_34 = 0;
        if (*(int *)(lVar6 + 0x228) < 1) {
          puStack_58 = (undefined8 *)&uStack_34;
          uStack_50 = (undefined8 *)(lVar6 + 0x220);
          func_0x06f8ce70(&puStack_58);
          pcVar2 = (code *)swi(3);
          (*pcVar2)();
          return;
        }
        uVar10 = func_0x02fc3270(lVar4,**(undefined8 **)(lVar6 + 0x220),1);
        func_0x02fc98c0(fStack_40,fStack_38,alStack_30,param_5);
        func_0x02fbff60(fStack_3c,uVar10,alStack_30);
        if (alStack_30[0] != 0) {
          func_0x03071ea0();
        }
        _ZdlPv(plVar5);
      }
    }
  }
  return;
}



// ADDRESS 02fc98c0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong * selected_02fc98c0(float param_1,float param_2,ulong *param_3,long param_4)

{
  float fVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  code *pcVar5;
  bool bVar6;
  float *pfVar7;
  ulong *puVar8;
  ulong uVar9;
  long lVar10;
  long lVar11;
  float fVar12;
  undefined8 uVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  undefined8 uStack_68;
  float fStack_60;
  undefined8 **ppuStack_58;
  ulong *puStack_50;
  undefined1 auStack_48 [8];
  float fStack_40;
  undefined8 *puStack_38;
  
  fVar20 = param_2;
  uVar13 = func_0x02fc96b0(param_4);
  *param_3 = 0;
  param_3[1] = 0;
  if (param_1 != param_2) {
    fVar1 = *(float *)(&UNK_006c2238 + (ulong)(param_2 < param_1) * 4);
    fStack_40 = fVar20;
    auStack_48 = uVar13;
    unique0x100000b3 = 0;
    lVar10 = 0;
    fVar16 = (float)uVar13;
    fVar20 = param_1;
    do {
      fVar12 = 0.0;
      if (0.0 <= fVar20) {
        fVar12 = fVar20;
      }
      uVar13 = func_0x052f01a0(fVar12,*(undefined8 *)(param_4 + 0x238),1);
      if (0 < (int)*(uint *)(param_4 + 0x248)) {
        uVar9 = 0;
        lVar11 = 0;
        fVar12 = _DAT_006c1588;
        do {
          lVar3 = *(long *)(*(long *)(param_4 + 0x240) + uVar9 * 8);
          lVar4 = *(long *)(lVar3 + 0x130);
          fVar23 = SUB84(_DAT_071fa860,0);
          fVar22 = (float)((ulong)_DAT_071fa860 >> 0x20);
          fVar21 = _DAT_071fa868;
          if (lVar4 != 0) {
            fVar23 = *(float *)(lVar4 + 0x1d0);
            fVar22 = *(float *)(lVar4 + 0x1d4);
            fVar21 = *(float *)(lVar4 + 0x1d8);
          }
          fVar22 = fVar22 - (float)((ulong)uVar13 >> 0x20);
          fVar23 = fVar23 - (float)uVar13;
          fVar21 = SQRT(fVar22 * fVar22 + fVar23 * fVar23 + (fVar21 - fVar16) * (fVar21 - fVar16));
          if (fVar12 <= fVar21) {
            fVar21 = fVar12;
            lVar3 = lVar11;
          }
          lVar11 = lVar3;
          fVar12 = fVar21;
          uVar9 = uVar9 + 1;
        } while (*(uint *)(param_4 + 0x248) != uVar9);
        if (lVar11 != 0) {
          lVar3 = *(long *)(lVar11 + 0x130);
          ppuStack_58 = _DAT_071fa860;
          fVar16 = _DAT_071fa868;
          if (lVar3 != 0) {
            ppuStack_58 = *(undefined8 ***)(lVar3 + 0x1d0);
            fVar16 = *(float *)(lVar3 + 0x1d8);
          }
          puStack_50 = (ulong *)CONCAT44(puStack_50._4_4_,fVar16);
          func_0x052f1190(*(undefined8 *)(param_4 + 0x238),&ppuStack_58);
          func_0x02fc6590(*(undefined8 *)(param_4 + 0x238));
          if ((lVar10 == 0) || (lVar10 != lVar11)) {
            pfVar7 = (float *)(*(long *)(lVar11 + 0x130) + 0x1c0);
            if (*(long *)(lVar11 + 0x130) == 0) {
              pfVar7 = (float *)&DAT_071fa830;
            }
            fVar17 = pfVar7[8] * (float)auStack_48._0_4_;
            fVar18 = pfVar7[9] * (float)auStack_48._4_4_;
            fVar19 = pfVar7[10] * fStack_40;
            fVar16 = *pfVar7;
            fVar12 = pfVar7[1];
            fVar21 = pfVar7[2];
            fVar23 = pfVar7[3];
            fVar22 = fVar18 * fVar16 - fVar17 * fVar12;
            fVar14 = fVar19 * fVar12 - fVar18 * fVar21;
            fVar15 = fVar17 * fVar21 - fVar19 * fVar16;
            fVar22 = fVar22 + fVar22;
            fVar14 = fVar14 + fVar14;
            fVar15 = fVar15 + fVar15;
            fStack_60 = (fVar15 * fVar16 - fVar14 * fVar12) + fVar23 * fVar22 + fVar19 + pfVar7[6];
            uStack_68 = CONCAT44((fVar14 * fVar21 - fVar22 * fVar16) + fVar23 * fVar15 + fVar18 +
                                 pfVar7[5],
                                 (fVar22 * fVar12 - fVar15 * fVar21) + fVar23 * fVar14 + fVar17 +
                                 pfVar7[4]);
            puStack_38 = &uStack_68;
            if (((undefined8 *)*param_3 <= &uStack_68) &&
               (&uStack_68 <
                (undefined8 *)((long)*param_3 + (long)*(int *)((long)param_3 + 0xc) * 0xc))) {
              ppuStack_58 = &puStack_38;
              puStack_50 = param_3;
              func_0x06f8d480(&ppuStack_58);
              pcVar5 = (code *)swi(3);
              puVar8 = (ulong *)(*pcVar5)();
              return puVar8;
            }
            iVar2 = (int)param_3[1];
            *(int *)(param_3 + 1) = iVar2 + 1;
            if (*(int *)((long)param_3 + 0xc) <= iVar2) {
              func_0x026af8a0(param_3,iVar2);
            }
            uVar9 = *param_3;
            *(float *)(uVar9 + 8 + (long)iVar2 * 0xc) = fStack_60;
            *(undefined8 *)(uVar9 + (long)iVar2 * 0xc) = uStack_68;
            lVar10 = lVar11;
          }
        }
      }
      fVar20 = fVar20 + fVar1;
      bVar6 = fVar20 < param_2;
      if (param_2 < param_1) {
        bVar6 = param_2 < fVar20;
      }
      fVar16 = param_1;
    } while (bVar6);
  }
  return param_3;
}



// ADDRESS 02fbff60

void selected_02fbff60(undefined4 param_1,long param_2,long *param_3)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  undefined8 uStack_38;
  undefined4 uStack_30;
  
  *(undefined1 *)(param_2 + 0x230) = 1;
  *(undefined4 *)(param_2 + 0x234) = 0;
  *(undefined4 *)(param_2 + 0x238) = param_1;
  uVar3 = func_0x033e4e80(param_2 + 0x220);
  func_0x052ecf50(uVar3,1);
  lVar2 = param_3[1];
  if ((long)(int)lVar2 != 0) {
    lVar1 = *param_3;
    lVar4 = 0;
    do {
      uStack_30 = *(undefined4 *)(lVar1 + 8 + lVar4);
      uStack_38 = *(undefined8 *)(lVar1 + lVar4);
      uVar3 = func_0x033e4e80(param_2 + 0x220);
      func_0x052ed580(uVar3,&uStack_38,1,1);
      lVar4 = lVar4 + 0xc;
    } while ((long)(int)lVar2 * 0xc != lVar4);
  }
  return;
}



// ADDRESS 02fc3270

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long selected_02fc3270(undefined8 param_1,float param_2,long *param_3,long param_4,char param_5)

{
  float fVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  uint uVar7;
  float fVar8;
  float fVar9;
  undefined8 extraout_XMM0_Qa;
  undefined8 extraout_XMM0_Qa_00;
  ulong extraout_XMM0_Qa_01;
  ulong extraout_XMM0_Qa_02;
  ulong uVar10;
  undefined8 extraout_XMM0_Qa_03;
  undefined8 extraout_XMM0_Qa_04;
  undefined8 extraout_XMM0_Qb;
  undefined8 extraout_XMM0_Qb_00;
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  undefined8 extraout_XMM0_Qb_01;
  undefined8 extraout_XMM0_Qb_02;
  undefined8 extraout_XMM0_Qb_03;
  undefined8 extraout_XMM0_Qb_04;
  undefined8 extraout_XMM0_Qb_05;
  undefined8 extraout_XMM0_Qb_06;
  float fVar13;
  float fVar20;
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  undefined1 auVar17 [16];
  undefined1 auVar18 [16];
  undefined1 auVar19 [16];
  float fVar21;
  undefined4 uVar22;
  undefined8 uStack_d8;
  float fStack_d0;
  undefined8 uStack_c8;
  float fStack_c0;
  undefined1 auStack_b8 [8];
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  float fStack_90;
  float fStack_8c;
  undefined8 uStack_88;
  float fStack_80;
  float fStack_78;
  float fStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  float fStack_68;
  float fStack_64;
  float fStack_60;
  float fStack_5c;
  undefined1 uStack_40;
  
  if (param_5 == '\0') {
    lVar4 = param_3[0x26];
    if (lVar4 == 0) {
      uStack_c8 = _DAT_071fa860;
      fStack_c0 = _DAT_071fa868;
      auVar16._8_8_ = 0;
      auVar16._0_8_ = _DAT_071fa8c8;
      lVar4 = 0;
      uVar3 = _DAT_071fa860;
      fVar13 = _DAT_071fa8d0;
      fVar1 = _DAT_071fa868;
    }
    else {
      uVar3 = *(undefined8 *)(lVar4 + 0x1d0);
      param_2 = *(float *)(lVar4 + 0x1d8);
      uStack_c8 = uVar3;
      fStack_c0 = param_2;
      fVar1 = (float)func_0x052b78e0();
      auVar16._8_8_ = extraout_XMM0_Qb_00;
      auVar16._0_8_ = extraout_XMM0_Qa_00;
      lVar4 = param_3[0x26];
      fVar13 = param_2;
    }
    fVar20 = auVar16._4_4_;
    uVar2 = auVar16._12_4_;
    auVar17._4_12_ = auVar16._4_12_;
    auVar17._0_4_ = auVar16._0_4_ * 0.0;
    fVar21 = fVar20 * 0.0;
    fVar13 = fVar13 * 0.0;
    if (lVar4 == 0) {
      lVar4 = 0;
      param_2 = _DAT_071fa8b8;
      uVar22 = uVar2;
      uVar10 = _DAT_071fa8b0;
    }
    else {
      uStack_a8 = CONCAT44(uStack_a8._4_4_,fVar13);
      _auStack_b8 = auVar17;
      fStack_78 = fVar21;
      fStack_74 = fVar20;
      uStack_70 = uVar2;
      uStack_6c = uVar2;
      fVar1 = (float)func_0x0526cb30();
      lVar4 = param_3[0x26];
      fVar13 = (float)uStack_a8;
      auVar17 = _auStack_b8;
      fVar21 = fStack_78;
      fVar20 = fStack_74;
      uVar2 = uStack_70;
      uVar22 = uStack_6c;
      uVar10 = extraout_XMM0_Qa_02;
    }
    fVar9 = (float)uVar3;
    fVar8 = (float)((ulong)uVar3 >> 0x20);
    auVar19._4_12_ = auVar17._4_12_;
    auVar19._0_4_ = auVar17._0_4_ + (float)uVar10 * _DAT_006c123c;
    fVar21 = fVar21 + (float)(uVar10 >> 0x20) * _DAT_006c123c;
    param_2 = param_2 * _DAT_006c123c;
    fVar13 = fVar13 + param_2;
    if (lVar4 == 0) {
      lVar4 = 0;
      param_2 = _DAT_071fa8a0;
      uVar3 = _DAT_071fa898;
    }
    else {
      uStack_a8 = CONCAT44(uStack_a8._4_4_,fVar13);
      _auStack_b8 = auVar19;
      fStack_90 = fVar9;
      fStack_8c = fVar8;
      fStack_78 = fVar21;
      fStack_74 = fVar20;
      uStack_70 = uVar2;
      uStack_6c = uVar22;
      fVar1 = (float)func_0x052b7940();
      lVar4 = param_3[0x26];
      fVar13 = (float)uStack_a8;
      auVar19 = _auStack_b8;
      fVar21 = fStack_78;
      fVar8 = fStack_8c;
      fVar9 = fStack_90;
      uVar3 = extraout_XMM0_Qa_04;
    }
    param_2 = param_2 * _DAT_006c1ad0;
    uStack_c8 = CONCAT44(fVar21 + (float)((ulong)uVar3 >> 0x20) * _DAT_006c1ad0 + fVar8,
                         auVar19._0_4_ + (float)uVar3 * _DAT_006c1ad0 + fVar9);
    fStack_c0 = fVar13 + param_2 + fStack_c0;
    fVar13 = _DAT_071fa874;
    uStack_88 = _DAT_071fa86c;
    if (lVar4 != 0) {
      fStack_68 = *(float *)(lVar4 + 0x1c0);
      fStack_64 = *(float *)(lVar4 + 0x1c4);
      fStack_60 = *(float *)(lVar4 + 0x1c8);
      fStack_5c = *(float *)(lVar4 + 0x1cc);
      auVar12._4_4_ = -(uint)(fStack_64 != *(float *)(lVar4 + 0x184));
      auVar12._0_4_ = -(uint)(fStack_68 != *(float *)(lVar4 + 0x180));
      auVar12._8_4_ = -(uint)(fStack_60 != *(float *)(lVar4 + 0x188));
      auVar12._12_4_ = -(uint)(fStack_5c != *(float *)(lVar4 + 0x18c));
      uVar2 = movmskps(fVar1,auVar12);
      if ((char)uVar2 == '\0') {
        fVar13 = *(float *)(lVar4 + 0x198);
        uStack_88 = *(undefined8 *)(lVar4 + 400);
      }
      else {
        *(undefined8 *)(lVar4 + 0x180) = *(undefined8 *)(lVar4 + 0x1c0);
        *(undefined8 *)(lVar4 + 0x188) = *(undefined8 *)(lVar4 + 0x1c8);
        uStack_88 = func_0x031049d0(&fStack_68);
        *(undefined8 *)(lVar4 + 400) = uStack_88;
        *(float *)(lVar4 + 0x198) = param_2;
        fVar13 = param_2;
      }
    }
    fStack_80 = fVar13;
    func_0x059a5c60(&fStack_68);
    uStack_40 = 4;
    uVar3 = (**(code **)(*param_3 + 0x168))(param_3);
    if (((param_4 == 0) || (lVar4 = func_0x059d3fa0(), lVar4 == 0)) ||
       (*(int *)(param_4 + 0x38) < *(int *)(lVar4 + 0x38))) {
      lVar5 = 0;
    }
    else {
      lVar5 = 0;
      if (*(long *)(*(long *)(param_4 + 0x30) + (long)*(int *)(lVar4 + 0x38) * 8) == lVar4 + 0x30) {
        lVar5 = param_4;
      }
    }
    lVar4 = func_0x054e1610(uVar3,lVar5,&uStack_c8,&uStack_88,&fStack_68);
    if (lVar4 == 0) {
      do {
        fStack_d0 = fStack_c0;
        uStack_d8 = uStack_c8;
        if (param_3[0x26] == 0) {
          uStack_b0 = 0;
          auStack_b8 = (undefined1  [8])_DAT_071fa8c8;
          uStack_a8._0_4_ = _DAT_071fa8d0;
        }
        else {
          auStack_b8 = (undefined1  [8])func_0x052b78e0();
          uStack_b0 = extraout_XMM0_Qb_04;
          uStack_a8._0_4_ = fVar13;
        }
        lVar4 = func_0x02fcc220();
        uVar7 = *(int *)(lVar4 + 0x1a0) * 0xbb38435 + 0x3619636b;
        *(uint *)(lVar4 + 0x1a0) = uVar7;
        fVar13 = ((float)(uVar7 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0e4c + _DAT_006c0b6c;
        fVar21 = (float)auStack_b8._0_4_ * fVar13 + (float)uStack_d8;
        fVar1 = (float)auStack_b8._4_4_ * fVar13 + uStack_d8._4_4_;
        fStack_d0 = fVar13 * (float)uStack_a8 + fStack_d0;
        uStack_d8._0_4_ = fVar21;
        uStack_d8._4_4_ = fVar1;
        if (param_3[0x26] == 0) {
          uStack_b0 = 0;
          auStack_b8 = (undefined1  [8])_DAT_071fa8b0;
          uStack_a8 = CONCAT44(uStack_a8._4_4_,_DAT_071fa8b8);
        }
        else {
          auStack_b8 = (undefined1  [8])func_0x0526cb30();
          uStack_b0 = extraout_XMM0_Qb_05;
          uStack_a8 = CONCAT44(uStack_a8._4_4_,fVar1);
        }
        lVar4 = func_0x02fcc220();
        uVar7 = *(int *)(lVar4 + 0x1a0) * 0xbb38435 + 0x3619636b;
        *(uint *)(lVar4 + 0x1a0) = uVar7;
        fVar13 = ((float)(uVar7 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0e4c + _DAT_006c0b6c;
        uStack_d8._0_4_ = (float)auStack_b8._0_4_ * fVar13 + (float)uStack_d8;
        fVar1 = (float)auStack_b8._4_4_ * fVar13 + uStack_d8._4_4_;
        fStack_d0 = fVar13 * (float)uStack_a8 + fStack_d0;
        uStack_d8._4_4_ = fVar1;
        if (param_3[0x26] == 0) {
          uStack_a8 = _DAT_071fa898;
          uStack_a0 = 0;
          auStack_b8._0_4_ = _DAT_071fa8a0;
        }
        else {
          uStack_a8 = func_0x052b7940();
          auStack_b8._0_4_ = fVar1;
          uStack_a0 = extraout_XMM0_Qb_06;
        }
        lVar4 = func_0x02fcc220();
        uVar7 = *(int *)(lVar4 + 0x1a0) * 0xbb38435 + 0x3619636b;
        *(uint *)(lVar4 + 0x1a0) = uVar7;
        fVar1 = ((float)(uVar7 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0c88 + _DAT_006c190c;
        fVar13 = uStack_a8._4_4_ * fVar1 + uStack_d8._4_4_;
        uStack_d8 = CONCAT44(fVar13,(float)uStack_a8 * fVar1 + (float)uStack_d8);
        fStack_d0 = fVar1 * (float)auStack_b8._0_4_ + fStack_d0;
        uVar3 = (**(code **)(*param_3 + 0x168))(param_3);
        if (((param_4 == 0) || (lVar4 = func_0x059d3fa0(), lVar4 == 0)) ||
           (*(int *)(param_4 + 0x38) < *(int *)(lVar4 + 0x38))) {
          lVar5 = 0;
        }
        else {
          lVar5 = 0;
          if (*(long *)(*(long *)(param_4 + 0x30) + (long)*(int *)(lVar4 + 0x38) * 8) ==
              lVar4 + 0x30) {
            lVar5 = param_4;
          }
        }
        lVar4 = func_0x054e1610(uVar3,lVar5,&uStack_d8,&uStack_88,&fStack_68);
      } while (lVar4 == 0);
    }
  }
  else {
    lVar4 = param_3[0x26];
    if (lVar4 == 0) {
      uStack_c8 = _DAT_071fa860;
      fStack_c0 = _DAT_071fa868;
      auVar14._8_8_ = 0;
      auVar14._0_8_ = _DAT_071fa8c8;
      lVar4 = 0;
      uVar3 = _DAT_071fa860;
      fVar13 = _DAT_071fa8d0;
      fVar1 = _DAT_071fa868;
    }
    else {
      uVar3 = *(undefined8 *)(lVar4 + 0x1d0);
      param_2 = *(float *)(lVar4 + 0x1d8);
      uStack_c8 = uVar3;
      fStack_c0 = param_2;
      fVar1 = (float)func_0x052b78e0();
      auVar14._8_8_ = extraout_XMM0_Qb;
      auVar14._0_8_ = extraout_XMM0_Qa;
      lVar4 = param_3[0x26];
      fVar13 = param_2;
    }
    fVar20 = auVar14._4_4_;
    uVar2 = auVar14._12_4_;
    auVar15._4_12_ = auVar14._4_12_;
    auVar15._0_4_ = auVar14._0_4_ * 0.0;
    fVar21 = fVar20 * 0.0;
    fVar13 = fVar13 * 0.0;
    if (lVar4 == 0) {
      lVar4 = 0;
      param_2 = _DAT_071fa8b8;
      uVar22 = uVar2;
      uVar10 = _DAT_071fa8b0;
    }
    else {
      uStack_a8 = CONCAT44(uStack_a8._4_4_,fVar13);
      _auStack_b8 = auVar15;
      fStack_78 = fVar21;
      fStack_74 = fVar20;
      uStack_70 = uVar2;
      uStack_6c = uVar2;
      fVar1 = (float)func_0x0526cb30();
      lVar4 = param_3[0x26];
      fVar13 = (float)uStack_a8;
      auVar15 = _auStack_b8;
      fVar21 = fStack_78;
      fVar20 = fStack_74;
      uVar2 = uStack_70;
      uVar22 = uStack_6c;
      uVar10 = extraout_XMM0_Qa_01;
    }
    fVar9 = (float)uVar3;
    fVar8 = (float)((ulong)uVar3 >> 0x20);
    auVar18._4_12_ = auVar15._4_12_;
    auVar18._0_4_ = auVar15._0_4_ + (float)uVar10 * _DAT_006c14d8;
    fVar21 = fVar21 + (float)(uVar10 >> 0x20) * _DAT_006c14d8;
    param_2 = param_2 * _DAT_006c14d8;
    fVar13 = fVar13 + param_2;
    if (lVar4 == 0) {
      lVar4 = 0;
      param_2 = _DAT_071fa8a0;
      uVar3 = _DAT_071fa898;
    }
    else {
      uStack_a8 = CONCAT44(uStack_a8._4_4_,fVar13);
      _auStack_b8 = auVar18;
      fStack_90 = fVar9;
      fStack_8c = fVar8;
      fStack_78 = fVar21;
      fStack_74 = fVar20;
      uStack_70 = uVar2;
      uStack_6c = uVar22;
      fVar1 = (float)func_0x052b7940();
      lVar4 = param_3[0x26];
      fVar13 = (float)uStack_a8;
      auVar18 = _auStack_b8;
      fVar21 = fStack_78;
      fVar8 = fStack_8c;
      fVar9 = fStack_90;
      uVar3 = extraout_XMM0_Qa_03;
    }
    param_2 = param_2 * _DAT_006c1ad0;
    uStack_c8 = CONCAT44(fVar21 + (float)((ulong)uVar3 >> 0x20) * _DAT_006c1ad0 + fVar8,
                         auVar18._0_4_ + (float)uVar3 * _DAT_006c1ad0 + fVar9);
    fStack_c0 = fVar13 + param_2 + fStack_c0;
    fVar13 = _DAT_071fa874;
    uStack_88 = _DAT_071fa86c;
    if (lVar4 != 0) {
      fStack_68 = *(float *)(lVar4 + 0x1c0);
      fStack_64 = *(float *)(lVar4 + 0x1c4);
      fStack_60 = *(float *)(lVar4 + 0x1c8);
      fStack_5c = *(float *)(lVar4 + 0x1cc);
      auVar11._4_4_ = -(uint)(fStack_64 != *(float *)(lVar4 + 0x184));
      auVar11._0_4_ = -(uint)(fStack_68 != *(float *)(lVar4 + 0x180));
      auVar11._8_4_ = -(uint)(fStack_60 != *(float *)(lVar4 + 0x188));
      auVar11._12_4_ = -(uint)(fStack_5c != *(float *)(lVar4 + 0x18c));
      uVar2 = movmskps(fVar1,auVar11);
      if ((char)uVar2 == '\0') {
        fVar13 = *(float *)(lVar4 + 0x198);
        uStack_88 = *(undefined8 *)(lVar4 + 400);
      }
      else {
        *(undefined8 *)(lVar4 + 0x180) = *(undefined8 *)(lVar4 + 0x1c0);
        *(undefined8 *)(lVar4 + 0x188) = *(undefined8 *)(lVar4 + 0x1c8);
        uStack_88 = func_0x031049d0(&fStack_68);
        *(undefined8 *)(lVar4 + 400) = uStack_88;
        *(float *)(lVar4 + 0x198) = param_2;
        fVar13 = param_2;
      }
    }
    fStack_80 = fVar13;
    func_0x059a5c60(&fStack_68);
    uStack_40 = 4;
    uVar3 = (**(code **)(*param_3 + 0x168))(param_3);
    if (((param_4 == 0) || (lVar4 = func_0x059d3fa0(), lVar4 == 0)) ||
       (*(int *)(param_4 + 0x38) < *(int *)(lVar4 + 0x38))) {
      lVar5 = 0;
    }
    else {
      lVar5 = 0;
      if (*(long *)(*(long *)(param_4 + 0x30) + (long)*(int *)(lVar4 + 0x38) * 8) == lVar4 + 0x30) {
        lVar5 = param_4;
      }
    }
    lVar4 = func_0x054e1610(uVar3,lVar5,&uStack_c8,&uStack_88,&fStack_68);
    if (lVar4 == 0) {
      do {
        fStack_d0 = fStack_c0;
        uStack_d8 = uStack_c8;
        if (param_3[0x26] == 0) {
          uStack_b0 = 0;
          auStack_b8 = (undefined1  [8])_DAT_071fa8c8;
          uStack_a8._0_4_ = _DAT_071fa8d0;
        }
        else {
          auStack_b8 = (undefined1  [8])func_0x052b78e0();
          uStack_b0 = extraout_XMM0_Qb_01;
          uStack_a8._0_4_ = fVar13;
        }
        lVar4 = func_0x02fcc220();
        uVar7 = *(int *)(lVar4 + 0x1a0) * 0xbb38435 + 0x3619636b;
        *(uint *)(lVar4 + 0x1a0) = uVar7;
        fVar13 = ((float)(uVar7 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0e4c + _DAT_006c0b6c;
        fVar21 = (float)auStack_b8._0_4_ * fVar13 + (float)uStack_d8;
        fVar1 = (float)auStack_b8._4_4_ * fVar13 + uStack_d8._4_4_;
        fStack_d0 = fVar13 * (float)uStack_a8 + fStack_d0;
        uStack_d8._0_4_ = fVar21;
        uStack_d8._4_4_ = fVar1;
        if (param_3[0x26] == 0) {
          uStack_b0 = 0;
          auStack_b8 = (undefined1  [8])_DAT_071fa8b0;
          uStack_a8 = CONCAT44(uStack_a8._4_4_,_DAT_071fa8b8);
        }
        else {
          auStack_b8 = (undefined1  [8])func_0x0526cb30();
          uStack_b0 = extraout_XMM0_Qb_02;
          uStack_a8 = CONCAT44(uStack_a8._4_4_,fVar1);
        }
        lVar4 = func_0x02fcc220();
        uVar7 = *(int *)(lVar4 + 0x1a0) * 0xbb38435 + 0x3619636b;
        *(uint *)(lVar4 + 0x1a0) = uVar7;
        fVar13 = ((float)(uVar7 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0e4c + _DAT_006c0b6c;
        uStack_d8._0_4_ = (float)auStack_b8._0_4_ * fVar13 + (float)uStack_d8;
        fVar1 = (float)auStack_b8._4_4_ * fVar13 + uStack_d8._4_4_;
        fStack_d0 = fVar13 * (float)uStack_a8 + fStack_d0;
        uStack_d8._4_4_ = fVar1;
        if (param_3[0x26] == 0) {
          uStack_a8 = _DAT_071fa898;
          uStack_a0 = 0;
          auStack_b8._0_4_ = _DAT_071fa8a0;
        }
        else {
          uStack_a8 = func_0x052b7940();
          auStack_b8._0_4_ = fVar1;
          uStack_a0 = extraout_XMM0_Qb_03;
        }
        lVar4 = func_0x02fcc220();
        uVar7 = *(int *)(lVar4 + 0x1a0) * 0xbb38435 + 0x3619636b;
        *(uint *)(lVar4 + 0x1a0) = uVar7;
        fVar1 = ((float)(uVar7 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c14d8 + _DAT_006c0ac4;
        fVar13 = uStack_a8._4_4_ * fVar1 + uStack_d8._4_4_;
        uStack_d8 = CONCAT44(fVar13,(float)uStack_a8 * fVar1 + (float)uStack_d8);
        fStack_d0 = fVar1 * (float)auStack_b8._0_4_ + fStack_d0;
        uVar3 = (**(code **)(*param_3 + 0x168))(param_3);
        if (((param_4 == 0) || (lVar4 = func_0x059d3fa0(), lVar4 == 0)) ||
           (*(int *)(param_4 + 0x38) < *(int *)(lVar4 + 0x38))) {
          lVar5 = 0;
        }
        else {
          lVar5 = 0;
          if (*(long *)(*(long *)(param_4 + 0x30) + (long)*(int *)(lVar4 + 0x38) * 8) ==
              lVar4 + 0x30) {
            lVar5 = param_4;
          }
        }
        lVar4 = func_0x054e1610(uVar3,lVar5,&uStack_d8,&uStack_88,&fStack_68);
      } while (lVar4 == 0);
    }
  }
  lVar5 = func_0x02fd7260();
  if (*(int *)(*(long *)(lVar4 + 0x10) + 0x38) < *(int *)(lVar5 + 0x38)) {
    lVar6 = 0;
  }
  else {
    lVar6 = 0;
    if (*(long *)(*(long *)(*(long *)(lVar4 + 0x10) + 0x30) + (long)*(int *)(lVar5 + 0x38) * 8) ==
        lVar5 + 0x30) {
      lVar6 = lVar4;
    }
  }
  return lVar6;
}



// ADDRESS 02fc6590

ulong selected_02fc6590(float param_1,long param_2)

{
  float fVar1;
  long lVar2;
  code *pcVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  float fVar8;
  ulong uVar9;
  uint uStack_50;
  float fStack_4c;
  uint *puStack_48;
  long *plStack_40;
  long lStack_38;
  
  plStack_40 = (long *)(param_2 + 0x490);
  uVar4 = *(uint *)(param_2 + 0x498);
  if ((int)uVar4 < 1) {
    if (uVar4 != 0) {
LAB_02fc6704:
      uStack_50 = 0;
      puStack_48 = &uStack_50;
      func_0x06f8ce70(&puStack_48);
      pcVar3 = (code *)swi(3);
      uVar9 = (*pcVar3)();
      return uVar9;
    }
    uVar4 = 0;
  }
  else {
    uVar7 = 0;
    puStack_48 = &uStack_50;
    uVar6 = uVar4;
    do {
      uVar5 = uVar6 >> 1;
      uStack_50 = uVar7 + uVar5;
      fStack_4c = param_1;
      if (((uint)((int)uStack_50 < (int)uVar4) & ~(uStack_50 >> 0x1f)) != 1) {
        lStack_38 = param_2;
        func_0x06f8ce70(&puStack_48);
        pcVar3 = (code *)swi(3);
        uVar9 = (*pcVar3)(lStack_38,&puStack_48);
        return uVar9;
      }
      lVar2 = *plStack_40;
      if (*(float *)(lVar2 + 4 + (long)(int)uStack_50 * 0x14) <= param_1) {
        uVar7 = uStack_50 + 1;
        uVar5 = uVar6 + ~uVar5;
      }
      uVar6 = uVar5;
    } while (0 < (int)uVar6);
    if (uVar7 != uVar4) {
      if (uVar7 == 0) {
        if ((int)uVar4 < 1) goto LAB_02fc6704;
        fVar8 = *(float *)*plStack_40;
      }
      else {
        uStack_50 = uVar7 - 1;
        if (((uint)((int)uVar7 <= (int)uVar4) & ~(uStack_50 >> 0x1f)) != 1) {
          func_0x06f8ce70(&puStack_48);
          pcVar3 = (code *)swi(3);
          uVar9 = (*pcVar3)();
          return uVar9;
        }
        if (((uint)((int)uVar7 < (int)uVar4) & ~(uVar7 >> 0x1f)) != 1) {
          uStack_50 = uVar7;
          func_0x06f8ce70(&puStack_48);
          pcVar3 = (code *)swi(3);
          uVar9 = (*pcVar3)();
          return uVar9;
        }
        fVar8 = *(float *)(lVar2 + (long)(int)uStack_50 * 0x14);
        fVar1 = *(float *)(lVar2 + 4 + (long)(int)uStack_50 * 0x14);
        fVar8 = (*(float *)(lVar2 + (long)(int)uVar7 * 0x14) - fVar8) *
                ((param_1 - fVar1) / (*(float *)(lVar2 + 4 + (long)(int)uVar7 * 0x14) - fVar1)) +
                fVar8;
      }
      goto LAB_02fc6768;
    }
  }
  uStack_50 = uVar4 - 1;
  if (((uint)(uVar4 != 0x80000000) & ~(uStack_50 >> 0x1f)) != 1) {
    puStack_48 = &uStack_50;
    func_0x06f8ce70(&puStack_48);
    pcVar3 = (code *)swi(3);
    uVar9 = (*pcVar3)();
    return uVar9;
  }
  fVar8 = *(float *)(*plStack_40 + (long)(int)uStack_50 * 0x14);
LAB_02fc6768:
  return (ulong)(uint)fVar8;
}



