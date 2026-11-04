// ADDRESS 02fc5630

void selected_02fc5630(long param_1,int param_2)

{
  long *plVar1;
  long lVar2;
  
  *(int *)(param_1 + 0x270) = param_2;
  if (param_2 == 1) {
    plVar1 = *(long **)(param_1 + 0x278);
    if ((plVar1 != (long *)0x0) && (*(long *)(param_1 + 0x228) != 0)) {
      (**(code **)(*plVar1 + 0x598))(plVar1,2);
    }
    plVar1 = *(long **)(param_1 + 0x280);
    if ((plVar1 != (long *)0x0) && (*(long *)(param_1 + 0x228) != 0)) {
      (**(code **)(*plVar1 + 0x598))(plVar1,0);
    }
    plVar1 = *(long **)(param_1 + 0x288);
    if ((plVar1 != (long *)0x0) && (*(long *)(param_1 + 0x228) != 0)) {
      (**(code **)(*plVar1 + 0x598))(plVar1,0);
    }
    plVar1 = *(long **)(param_1 + 0x290);
    if ((plVar1 != (long *)0x0) && (*(long *)(param_1 + 0x228) != 0)) {
      (**(code **)(*plVar1 + 0x598))(plVar1,0);
    }
    plVar1 = *(long **)(param_1 + 0x298);
    if (plVar1 == (long *)0x0) {
      return;
    }
    lVar2 = *(long *)(param_1 + 0x228);
  }
  else {
    if (param_2 != 0) {
      return;
    }
    plVar1 = *(long **)(param_1 + 0x278);
    if ((plVar1 != (long *)0x0) && (*(long *)(param_1 + 0x220) != 0)) {
      (**(code **)(*plVar1 + 0x598))(plVar1,2);
    }
    plVar1 = *(long **)(param_1 + 0x280);
    if ((plVar1 != (long *)0x0) && (*(long *)(param_1 + 0x220) != 0)) {
      (**(code **)(*plVar1 + 0x598))(plVar1,0);
    }
    plVar1 = *(long **)(param_1 + 0x288);
    if ((plVar1 != (long *)0x0) && (*(long *)(param_1 + 0x220) != 0)) {
      (**(code **)(*plVar1 + 0x598))(plVar1,0);
    }
    plVar1 = *(long **)(param_1 + 0x290);
    if ((plVar1 != (long *)0x0) && (*(long *)(param_1 + 0x220) != 0)) {
      (**(code **)(*plVar1 + 0x598))(plVar1,0);
    }
    plVar1 = *(long **)(param_1 + 0x298);
    if (plVar1 == (long *)0x0) {
      return;
    }
    lVar2 = *(long *)(param_1 + 0x220);
  }
  if (lVar2 == 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x02fc57bd. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*plVar1 + 0x598))(plVar1,0);
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



// ADDRESS 02fc2440

void selected_02fc2440(long param_1,undefined1 param_2)

{
  *(undefined1 *)(param_1 + 0x238) = param_2;
  return;
}



