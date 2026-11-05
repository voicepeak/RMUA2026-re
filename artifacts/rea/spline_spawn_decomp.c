// ADDRESS 02fc7620

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long * selected_02fc7620(float param_1,float param_2,long param_3,byte param_4)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  ulong uVar4;
  long lVar5;
  float fVar6;
  undefined8 uVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  undefined8 uStack_28;
  float fStack_20;
  
  fVar8 = 0.0;
  if (0.0 <= param_1) {
    fVar8 = param_1;
  }
  fVar6 = fVar8;
  uVar7 = func_0x052f01a0(fVar8,*(undefined8 *)(param_3 + 0x238),1);
  if (0 < (int)*(uint *)(param_3 + 0x248)) {
    uVar4 = 0;
    lVar5 = 0;
    fVar9 = _DAT_006c1588;
    do {
      lVar1 = *(long *)(*(long *)(param_3 + 0x240) + uVar4 * 8);
      lVar2 = *(long *)(lVar1 + 0x130);
      fVar12 = (float)_DAT_071fa860;
      fVar11 = (float)((ulong)_DAT_071fa860 >> 0x20);
      fVar10 = _DAT_071fa868;
      if (lVar2 != 0) {
        fVar12 = *(float *)(lVar2 + 0x1d0);
        fVar11 = *(float *)(lVar2 + 0x1d4);
        fVar10 = *(float *)(lVar2 + 0x1d8);
      }
      fVar11 = fVar11 - (float)((ulong)uVar7 >> 0x20);
      fVar12 = fVar12 - (float)uVar7;
      fVar10 = SQRT(fVar11 * fVar11 + fVar12 * fVar12 + (fVar10 - fVar6) * (fVar10 - fVar6));
      if (fVar9 <= fVar10) {
        fVar10 = fVar9;
        lVar1 = lVar5;
      }
      lVar5 = lVar1;
      fVar9 = fVar10;
      uVar4 = uVar4 + 1;
    } while (*(uint *)(param_3 + 0x248) != uVar4);
    if (lVar5 != 0) {
      lVar1 = *(long *)(lVar5 + 0x130);
      uStack_28 = _DAT_071fa860;
      fStack_20 = _DAT_071fa868;
      if (lVar1 != 0) {
        uStack_28 = *(undefined8 *)(lVar1 + 0x1d0);
        fStack_20 = *(float *)(lVar1 + 0x1d8);
      }
      func_0x052f1190(*(undefined8 *)(param_3 + 0x238),&uStack_28);
      fVar6 = (float)func_0x02fc6590(*(undefined8 *)(param_3 + 0x238));
      if ((~(param_1 < param_2 + param_2 | param_4) & fVar8 <= fVar6) == 0) {
        plVar3 = (long *)_Znwm(0x10);
        *plVar3 = lVar5;
        *(undefined1 *)(plVar3 + 1) = 1;
        return plVar3;
      }
      plVar3 = (long *)selected_02fc7620(fVar8 + _DAT_006c12d4 * param_2,param_3,0);
      return plVar3;
    }
  }
  return (long *)0x0;
}



// ADDRESS 02fc4980

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong selected_02fc4980(float param_1,long param_2,ulong param_3,int param_4,int param_5)

{
  ulong *puVar1;
  float ***pppfVar2;
  long lVar3;
  code *pcVar4;
  int iVar5;
  long lVar6;
  ulong uVar7;
  bool bVar8;
  uint uVar9;
  uint uVar10;
  ulong uVar11;
  float ***pppfVar12;
  ulong unaff_R14;
  float fVar16;
  float extraout_XMM0_Dc;
  undefined8 extraout_XMM0_Qb;
  undefined1 auVar13 [16];
  undefined8 extraout_XMM0_Qb_00;
  undefined1 auVar14 [16];
  undefined8 extraout_XMM0_Qb_01;
  undefined8 extraout_XMM0_Qb_02;
  undefined8 extraout_XMM0_Qb_03;
  undefined1 auVar15 [16];
  float fVar17;
  undefined1 auVar18 [12];
  undefined1 auVar19 [12];
  undefined1 auVar20 [16];
  undefined1 auVar21 [4];
  float fVar22;
  float fVar23;
  float fVar24;
  undefined1 auVar25 [12];
  undefined1 auVar26 [12];
  undefined1 auVar27 [12];
  undefined1 auVar28 [12];
  undefined1 auVar29 [12];
  undefined1 auVar30 [12];
  undefined1 auVar31 [12];
  undefined1 auVar32 [12];
  float ***pppfStack_118;
  float fStack_110;
  float **ppfStack_108;
  float fStack_100;
  float ****ppppfStack_f8;
  ulong *puStack_f0;
  undefined1 auStack_e8 [4];
  undefined1 auStack_e4 [8];
  float fStack_dc;
  int iStack_d0;
  int iStack_cc;
  undefined1 auStack_c8 [8];
  float fStack_c0;
  float fStack_bc;
  long *plStack_b8;
  float ****ppppfStack_b0;
  float fStack_a4;
  float fStack_a0;
  uint uStack_9c;
  undefined1 auStack_98 [16];
  float **ppfStack_88;
  float fStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  float fStack_58;
  float fStack_54;
  float fStack_50;
  float fStack_4c;
  float fStack_48;
  float fStack_44;
  float fStack_40;
  float fStack_3c;
  
  iStack_cc = param_4 + (uint)param_3;
  lVar6 = *(long *)(param_2 + 0x130);
  if (lVar6 == 0) {
    fVar23 = 0.0;
    fVar22 = 0.0;
    fVar24 = _DAT_071fa868;
    fVar17 = _DAT_071fa860;
    fVar16 = _DAT_071fa864;
  }
  else {
    fVar23 = *(float *)(lVar6 + 0x1d8);
    fVar22 = *(float *)(lVar6 + 0x1dc);
    fVar24 = *(float *)(lVar6 + 0x1d8);
    fVar17 = *(float *)(lVar6 + 0x1d0);
    fVar16 = *(float *)(lVar6 + 0x1d4);
  }
  plStack_b8 = (long *)(param_2 + 0x250);
  iVar5 = *(int *)(param_2 + 600);
  if (iVar5 < iStack_cc) {
    fStack_a0 = param_1 * param_1;
    puVar1 = (ulong *)(param_2 + 0x260);
    uVar10 = 0;
    unaff_R14 = 0;
    iStack_d0 = param_5;
    uStack_9c = (uint)param_3;
    fStack_58 = fVar16;
    fStack_54 = fVar16;
    fStack_50 = fVar22;
    fStack_4c = fVar22;
    fStack_48 = fVar17;
    fStack_44 = fVar24;
    fStack_40 = fVar23;
    fStack_3c = fVar22;
    do {
      lVar6 = func_0x02fcc220();
      uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
      *(uint *)(lVar6 + 0x1a0) = uVar9;
      auVar26._0_4_ =
           ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0e4c + _DAT_006c0fa8;
      _auStack_e8 = ZEXT416((uint)auVar26._0_4_);
      auVar26._4_8_ = 0;
      if (*(long *)(param_2 + 0x130) == 0) {
        auVar13._8_8_ = 0;
        auVar13._0_8_ = _DAT_071fa8b0;
        fVar24 = _DAT_071fa8b8;
      }
      else {
        auVar25 = func_0x0526cb30();
        auVar13._0_8_ = auVar25._0_8_;
        auVar13._8_8_ = extraout_XMM0_Qb;
        auVar26 = auStack_e8._0_12_;
        fVar24 = auVar25._8_4_;
      }
      fVar23 = auVar13._4_4_;
      auVar20._4_4_ = fVar23;
      auVar20._0_4_ = fVar23;
      fVar22 = auVar13._12_4_;
      auVar20._8_4_ = fVar22;
      auVar20._12_4_ = fVar22;
      fVar17 = auVar26._0_4_;
      register0x000012c4 = auVar20._4_12_;
      auStack_e4._0_4_ = fVar24 * fVar17 + fStack_44;
      auStack_e8 = (undefined1  [4])(auVar13._0_4_ * fVar17 + fStack_48);
      auStack_e4._4_4_ = auVar13._8_4_ * auVar26._8_4_ + fStack_40;
      fStack_dc = fVar22 * auVar26._8_4_ + fStack_3c;
      auStack_c8._0_4_ = fVar23 * fVar17 + fStack_58;
      lVar6 = func_0x02fcc220();
      uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
      *(uint *)(lVar6 + 0x1a0) = uVar9;
      auVar25._0_4_ =
           ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0e4c + _DAT_006c0b6c;
      auVar25._4_8_ = 0;
      if (*(long *)(param_2 + 0x130) == 0) {
        auVar14._8_8_ = 0;
        auVar14._0_8_ = _DAT_071fa8c8;
        lVar6 = 0;
        fVar24 = _DAT_071fa8d0;
      }
      else {
        auStack_98 = ZEXT416((uint)auVar25._0_4_);
        auVar26 = func_0x052b78e0();
        auVar14._0_8_ = auVar26._0_8_;
        auVar14._8_8_ = extraout_XMM0_Qb_00;
        auVar25 = auStack_98._0_12_;
        lVar6 = *(long *)(param_2 + 0x130);
        fVar24 = auVar26._8_4_;
      }
      auVar27._8_4_ = _DAT_071fa8a0;
      auVar27._0_8_ = _DAT_071fa898;
      fVar22 = auVar25._0_4_;
      auVar21 = (undefined1  [4])((float)auStack_e8 + auVar14._0_4_ * fVar22);
      fVar23 = (float)auStack_e4._0_4_ + fVar24 * fVar22;
      fVar24 = (float)auStack_c8._0_4_ + auVar14._4_4_ * fVar22;
      auStack_e4._0_4_ = fVar23;
      auStack_e8 = auVar21;
      auStack_e4._4_4_ = (float)auStack_e4._4_4_ + auVar14._8_4_ * auVar25._8_4_;
      fStack_dc = fStack_dc + auVar14._12_4_ * auVar25._8_4_;
      auStack_c8._0_4_ = fVar24;
      if (lVar6 == 0) {
        lVar6 = 0;
      }
      else {
        auVar27 = func_0x052b7940();
        lVar6 = *(long *)(param_2 + 0x130);
        fVar24 = (float)auStack_c8._0_4_;
        auVar21 = auStack_e8;
        fVar23 = (float)auStack_e4._0_4_;
      }
      auVar28._8_4_ = _DAT_071fa8a0;
      auVar28._0_8_ = _DAT_071fa898;
      ppfStack_108 = (float **)
                     CONCAT44(fVar24 + auVar27._4_4_ * _UNK_006cb4f4,
                              (float)auVar21 + auVar27._0_4_ * _DAT_006cb4f0);
      fStack_100 = fVar23 + auVar27._8_4_ * _DAT_006c1ad8;
      if (lVar6 != 0) {
        auVar28 = func_0x052b7940();
        fVar24 = (float)auStack_c8._0_4_;
        auVar21 = auStack_e8;
        fVar23 = (float)auStack_e4._0_4_;
      }
      fVar24 = fVar24 + auVar28._4_4_ * _DAT_006c0ac4;
      fVar22 = (float)auVar21 + auVar28._0_4_ * _DAT_006d4060;
      fStack_80 = fVar23 + auVar28._8_4_ * _UNK_006d4064;
      ppfStack_88 = (float **)CONCAT44(fVar24,fVar22);
      pppfVar2 = *(float ****)(param_2 + 0x250);
      iVar5 = *(int *)(param_2 + 600);
      if ((long)iVar5 == 0) {
LAB_02fc4cf9:
        pppfStack_118 = &ppfStack_88;
        if ((pppfVar2 <= &ppfStack_88) &&
           (&ppfStack_88 < (float ***)((long)pppfVar2 + (long)*(int *)(param_2 + 0x25c) * 0xc))) {
          ppppfStack_f8 = &pppfStack_118;
          puStack_f0 = (ulong *)plStack_b8;
          func_0x06f8d480(&ppppfStack_f8);
          pcVar4 = (code *)swi(3);
          uVar7 = (*pcVar4)(&ppfStack_88);
          return uVar7;
        }
        *(int *)(param_2 + 600) = iVar5 + 1;
        if (*(int *)(param_2 + 0x25c) <= iVar5) {
          func_0x026af8a0(plStack_b8,iVar5);
        }
        lVar6 = *plStack_b8;
        *(float *)(lVar6 + 8 + (long)iVar5 * 0xc) = fStack_80;
        *(float ***)(lVar6 + (long)iVar5 * 0xc) = ppfStack_88;
        puStack_f0 = puVar1;
        if (param_5 == 3) {
          lVar6 = func_0x02fcc220();
          uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
          *(uint *)(lVar6 + 0x1a0) = uVar9;
          auStack_98._0_4_ =
               (((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c0e50 + _DAT_006c0fa8)
               * _DAT_006c0f10;
          lVar6 = func_0x02fcc220();
          uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
          *(uint *)(lVar6 + 0x1a0) = uVar9;
          auStack_c8._0_4_ = ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1ad0;
          auStack_c8._4_4_ = 0;
          fStack_c0 = 0.0;
          fStack_bc = 0.0;
          if (*(long *)(param_2 + 0x130) == 0) {
            uStack_70 = 0;
            auVar30._8_4_ = _DAT_071fa8b8;
            auVar30._0_8_ = _DAT_071fa8b0;
            lVar6 = 0;
          }
          else {
            _auStack_c8 = ZEXT416((uint)auStack_c8._0_4_);
            auVar30 = func_0x0526cb30();
            lVar6 = *(long *)(param_2 + 0x130);
            uStack_70 = extraout_XMM0_Qb_01;
          }
          auStack_e8 = auVar30._8_4_;
          uStack_78 = auVar30._0_8_;
          auStack_c8._0_4_ = (float)auStack_c8._0_4_ + _DAT_006c1ad0;
          fVar24 = (float)func_0x070a96f0();
          uStack_78 = CONCAT44(fVar24 * uStack_78._4_4_,fVar24 * (float)uStack_78);
          uStack_70 = CONCAT44(extraout_XMM0_Dc * uStack_70._4_4_,
                               extraout_XMM0_Dc * (float)uStack_70);
          auStack_e8 = (undefined1  [4])((float)auStack_e8 * fVar24);
          if (lVar6 == 0) {
            uStack_60 = 0;
            auVar32._8_4_ = _DAT_071fa8d0;
            auVar32._0_8_ = _DAT_071fa8c8;
          }
          else {
            auVar32 = func_0x052b78e0(lVar6);
            uStack_60 = extraout_XMM0_Qb_03;
          }
          fStack_a4 = auVar32._8_4_;
          uStack_68 = auVar32._0_8_;
          fVar24 = (float)func_0x070a9890();
          pppfStack_118 =
               (float ***)
               CONCAT44((float)((ulong)ppfStack_108 >> 0x20) +
                        (float)auStack_c8._0_4_ * (uStack_78._4_4_ + fVar24 * uStack_68._4_4_),
                        SUB84(ppfStack_108,0) +
                        (float)auStack_c8._0_4_ * ((float)uStack_78 + fVar24 * (float)uStack_68));
          fStack_110 = ((float)auStack_e8 + fStack_a4 * fVar24) * (float)auStack_c8._0_4_ +
                       fStack_100;
          ppppfStack_b0 = &pppfStack_118;
          if (((float ****)*puVar1 <= ppppfStack_b0) &&
             (&pppfStack_118 < (float ****)((long)*puVar1 + (long)*(int *)(param_2 + 0x26c) * 0xc)))
          {
            ppppfStack_f8 = (float ****)&ppppfStack_b0;
            func_0x06f8d480(&ppppfStack_f8);
            pcVar4 = (code *)swi(3);
            uVar7 = (*pcVar4)();
            return uVar7;
          }
        }
        else if (param_5 == 2) {
          lVar6 = func_0x02fcc220();
          uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
          *(uint *)(lVar6 + 0x1a0) = uVar9;
          fVar24 = ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1440;
          lVar6 = func_0x02fcc220();
          uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
          *(uint *)(lVar6 + 0x1a0) = uVar9;
          auVar18._0_4_ =
               ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1b34 + _DAT_006c1848;
          auVar18._4_8_ = 0;
          if (*(long *)(param_2 + 0x130) == 0) {
            auVar15._8_8_ = 0;
            auVar15._0_8_ = _DAT_071fa8c8;
            fVar23 = _DAT_071fa8d0;
          }
          else {
            _auStack_e8 = ZEXT416((uint)auVar18._0_4_);
            auVar26 = func_0x052b78e0();
            fVar23 = auVar26._8_4_;
            auVar15._0_8_ = auVar26._0_8_;
            auVar15._8_8_ = extraout_XMM0_Qb_02;
            auVar18 = auStack_e8._0_12_;
          }
          auVar19 = auVar18;
          if ((int)fVar24 < 6) {
            auVar19._0_4_ = auVar18._0_4_ ^ _DAT_006c2f70;
            auVar19._4_4_ = auVar18._4_4_ ^ _UNK_006c2f74;
            auVar19._8_4_ = auVar18._8_4_ ^ _UNK_006c2f78;
          }
          fVar24 = auVar19._0_4_;
          auStack_c8._4_4_ = fVar24 * auVar15._4_4_;
          auStack_c8._0_4_ = fVar24 * auVar15._0_4_;
          fStack_c0 = auVar19._8_4_ * auVar15._8_4_;
          fStack_bc = auVar19._8_4_ * auVar15._12_4_;
          _auStack_e4 = auStack_e8._4_12_;
          auStack_e8 = (undefined1  [4])(fVar23 * fVar24);
          lVar6 = func_0x02fcc220();
          uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
          *(uint *)(lVar6 + 0x1a0) = uVar9;
          auVar31._8_4_ = _DAT_071fa8b8;
          auVar31._0_8_ = _DAT_071fa8b0;
          fVar24 = ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1b34 +
                   _DAT_006c1458;
          if (*(long *)(param_2 + 0x130) != 0) {
            auStack_98 = ZEXT416((uint)fVar24);
            auVar31 = func_0x0526cb30();
            fVar24 = auStack_98._0_4_;
          }
          pppfStack_118 =
               (float ***)
               CONCAT44((float)auStack_c8._4_4_ + (float)((ulong)ppfStack_108 >> 0x20) +
                        fVar24 * auVar31._4_4_,
                        (float)auStack_c8._0_4_ + SUB84(ppfStack_108,0) + fVar24 * auVar31._0_4_);
          fStack_110 = (float)auStack_e8 + fStack_100 + auVar31._8_4_ * fVar24;
          ppppfStack_b0 = &pppfStack_118;
          if (((float ****)*puVar1 <= ppppfStack_b0) &&
             (&pppfStack_118 < (float ****)((long)*puVar1 + (long)*(int *)(param_2 + 0x26c) * 0xc)))
          {
            ppppfStack_f8 = (float ****)&ppppfStack_b0;
            func_0x06f8d480(&ppppfStack_f8);
            pcVar4 = (code *)swi(3);
            uVar7 = (*pcVar4)();
            return uVar7;
          }
        }
        else {
          if (param_5 != 1) {
            pppfStack_118 = &ppfStack_108;
            if (((float ***)*puVar1 <= pppfStack_118) &&
               (&ppfStack_108 < (float ***)((long)*puVar1 + (long)*(int *)(param_2 + 0x26c) * 0xc)))
            {
              ppppfStack_f8 = &pppfStack_118;
              func_0x06f8d480(&ppppfStack_f8);
              pcVar4 = (code *)swi(3);
              uVar7 = (*pcVar4)(&ppfStack_88);
              return uVar7;
            }
            iVar5 = *(int *)(param_2 + 0x268);
            *(int *)(param_2 + 0x268) = iVar5 + 1;
            if (*(int *)(param_2 + 0x26c) <= iVar5) {
              func_0x026af8a0(puVar1,iVar5);
            }
            uVar7 = *puVar1;
            *(float *)(uVar7 + 8 + (long)iVar5 * 0xc) = fStack_100;
            *(float ***)(uVar7 + (long)iVar5 * 0xc) = ppfStack_108;
            param_5 = iStack_d0;
            goto LAB_02fc53a1;
          }
          lVar6 = func_0x02fcc220();
          uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
          *(uint *)(lVar6 + 0x1a0) = uVar9;
          fVar24 = ((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1440;
          lVar6 = func_0x02fcc220();
          uVar9 = *(int *)(lVar6 + 0x1a0) * 0xbb38435 + 0x3619636b;
          *(uint *)(lVar6 + 0x1a0) = uVar9;
          auVar29._8_4_ = _DAT_071fa8d0;
          auVar29._0_8_ = _DAT_071fa8c8;
          auVar21 = (undefined1  [4])
                    (((float)(uVar9 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1b34 +
                    _DAT_006c1848);
          if (*(long *)(param_2 + 0x130) != 0) {
            _auStack_e8 = ZEXT416((uint)auVar21);
            auVar29 = func_0x052b78e0();
            auVar21 = auStack_e8;
          }
          if ((int)fVar24 < 6) {
            auVar21 = (undefined1  [4])((uint)auVar21 ^ _DAT_006c2f70);
          }
          pppfStack_118 =
               (float ***)
               CONCAT44((float)((ulong)ppfStack_108 >> 0x20) + (float)auVar21 * auVar29._4_4_,
                        SUB84(ppfStack_108,0) + (float)auVar21 * auVar29._0_4_);
          fStack_110 = auVar29._8_4_ * (float)auVar21 + fStack_100;
          ppppfStack_b0 = &pppfStack_118;
          if (((float ****)*puVar1 <= ppppfStack_b0) &&
             (&pppfStack_118 < (float ****)((long)*puVar1 + (long)*(int *)(param_2 + 0x26c) * 0xc)))
          {
            ppppfStack_f8 = (float ****)&ppppfStack_b0;
            func_0x06f8d480(&ppppfStack_f8);
            pcVar4 = (code *)swi(3);
            uVar7 = (*pcVar4)();
            return uVar7;
          }
        }
        ppppfStack_b0 = &pppfStack_118;
        iVar5 = *(int *)(param_2 + 0x268);
        *(int *)(param_2 + 0x268) = iVar5 + 1;
        if (*(int *)(param_2 + 0x26c) <= iVar5) {
          func_0x026af8a0(puVar1,iVar5);
        }
        uVar7 = *puVar1;
        *(float *)(uVar7 + 8 + (long)iVar5 * 0xc) = fStack_110;
        *(float ****)(uVar7 + (long)iVar5 * 0xc) = pppfStack_118;
        param_5 = iStack_d0;
      }
      else {
        pppfVar12 = pppfVar2;
        do {
          fVar23 = *(float *)((long)pppfVar12 + 4) - fVar24;
          fVar17 = *(float *)pppfVar12 - fVar22;
          bVar8 = false;
          if ((*(float *)(pppfVar12 + 1) - fStack_80) * (*(float *)(pppfVar12 + 1) - fStack_80) +
              fVar23 * fVar23 + fVar17 * fVar17 < fStack_a0) {
            bVar8 = true;
            break;
          }
          pppfVar12 = (float ***)((long)pppfVar12 + 0xc);
        } while (pppfVar12 != (float ***)((long)pppfVar2 + (long)iVar5 * 0xc));
        if (!bVar8) goto LAB_02fc4cf9;
      }
LAB_02fc53a1:
      iVar5 = *(int *)(param_2 + 600);
    } while ((uVar10 < 999) && (uVar10 = uVar10 + 1, iVar5 < iStack_cc));
    param_3 = (ulong)uStack_9c;
    if (iVar5 < iStack_cc) {
      uVar7 = 0;
      goto LAB_02fc5611;
    }
  }
  uVar10 = (uint)param_3;
  if ((int)uVar10 < 1) {
    uVar10 = 0;
  }
  else {
    unaff_R14 = param_3 & 0xffffffff;
    lVar6 = 8;
    uVar7 = 0;
    do {
      ppfStack_108 = (float **)CONCAT44(ppfStack_108._4_4_,(int)uVar7);
      if ((long)*(int *)(param_2 + 0x268) <= (long)uVar7) {
        ppppfStack_f8 = (float ****)&ppfStack_108;
        puStack_f0 = (ulong *)(param_2 + 0x260);
        func_0x06f8ce70(&ppppfStack_f8);
        pcVar4 = (code *)swi(3);
        uVar7 = (*pcVar4)();
        return uVar7;
      }
      ppfStack_108 = (float **)CONCAT44(ppfStack_108._4_4_,(int)uVar7);
      if ((long)*(int *)(param_2 + 600) <= (long)uVar7) {
        ppppfStack_f8 = (float ****)&ppfStack_108;
        puStack_f0 = (ulong *)plStack_b8;
        stack0xffffffffffffff20 = 0;
        _auStack_e8 = *(ulong *)(*(long *)(param_2 + 0x260) + -8 + lVar6);
        auStack_c8._0_4_ = *(undefined4 *)(*(long *)(param_2 + 0x260) + lVar6);
        func_0x06f8ce70(&ppppfStack_f8);
        pcVar4 = (code *)swi(3);
        uVar7 = (*pcVar4)();
        return uVar7;
      }
      func_0x02fc3db0();
      uVar7 = uVar7 + 1;
      lVar6 = lVar6 + 0xc;
    } while ((param_3 & 0xffffffff) != uVar7);
    iVar5 = *(int *)(param_2 + 600);
  }
  uVar7 = CONCAT71((int7)(unaff_R14 >> 8),1);
  if ((int)uVar10 < iVar5) {
    uVar11 = (ulong)uVar10;
    lVar6 = uVar11 * 0xc + 8;
    do {
      ppfStack_108 = (float **)CONCAT44(ppfStack_108._4_4_,(int)uVar11);
      if ((long)*(int *)(param_2 + 0x268) <= (long)uVar11) {
        ppppfStack_f8 = (float ****)&ppfStack_108;
        puStack_f0 = (ulong *)(param_2 + 0x260);
        func_0x06f8ce70(&ppppfStack_f8);
        pcVar4 = (code *)swi(3);
        uVar7 = (*pcVar4)();
        return uVar7;
      }
      lVar3 = *(long *)(param_2 + 0x260);
      ppfStack_108 = (float **)CONCAT44(ppfStack_108._4_4_,(int)uVar11);
      if ((long)iVar5 <= (long)uVar11) {
        ppppfStack_f8 = (float ****)&ppfStack_108;
        puStack_f0 = (ulong *)plStack_b8;
        stack0xffffffffffffff20 = 0;
        _auStack_e8 = *(ulong *)(lVar3 + -8 + lVar6);
        auStack_c8._0_4_ = *(undefined4 *)(lVar3 + lVar6);
        func_0x06f8ce70(&ppppfStack_f8);
        pcVar4 = (code *)swi(3);
        uVar7 = (*pcVar4)();
        return uVar7;
      }
      func_0x02fc3db0();
      uVar11 = uVar11 + 1;
      iVar5 = *(int *)(param_2 + 600);
      lVar6 = lVar6 + 0xc;
    } while ((int)uVar11 < iVar5);
  }
LAB_02fc5611:
  return uVar7 & 0xffffffff;
}



// ADDRESS 02fc3f70

/* WARNING: Possible PIC construction at 0x02fc3f90: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x026de44a) */
/* WARNING: Removing unreachable block (ram,0x026de476) */
/* WARNING: Removing unreachable block (ram,0x026de479) */
/* WARNING: Removing unreachable block (ram,0x026de47e) */

void selected_02fc3f70(long param_1)

{
  long lVar1;
  undefined8 in_RAX;
  long unaff_RBX;
  long *plVar2;
  undefined8 unaff_RBP;
  undefined8 uStack_10;
  
  *(undefined4 *)(param_1 + 600) = 0;
  if (*(int *)(param_1 + 0x25c) == 0) {
    *(undefined4 *)(param_1 + 0x268) = 0;
    if (*(int *)(param_1 + 0x26c) == 0) {
      return;
    }
    plVar2 = (long *)(param_1 + 0x260);
  }
  else {
    plVar2 = (long *)(param_1 + 0x250);
    register0x00000020 = (BADSPACEBASE *)&uStack_10;
    uStack_10 = 0x2fc3f95;
    unaff_RBX = param_1;
  }
  *(undefined8 *)((long)register0x00000020 + -8) = unaff_RBP;
  *(long *)((long)register0x00000020 + -0x10) = unaff_RBX;
  *(undefined8 *)((long)register0x00000020 + -0x18) = in_RAX;
  if (*(int *)((long)plVar2 + 0xc) != 0) {
    *(undefined4 *)((long)plVar2 + 0xc) = 0;
    lVar1 = *plVar2;
    if (lVar1 != 0) {
      *(undefined8 *)((long)register0x00000020 + -0x20) = 0x26de4a7;
      lVar1 = func_0x03071ed0(lVar1,0,0);
      *plVar2 = lVar1;
    }
  }
  return;
}



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
  selected_02fc93c0(uStack_14,uStack_18,uStack_1c,uStack_20,param_1);
  return;
}



// ADDRESS 02fc0180

void selected_02fc0180(float param_1,undefined4 param_2,long param_3)

{
  long lVar1;
  float fVar2;
  undefined8 uVar3;
  float fVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  func_0x0509aaf0();
  if (*(char *)(param_3 + 0x230) != '\0') {
    fVar2 = *(float *)(param_3 + 0x234);
    lVar1 = param_3 + 0x220;
    uVar3 = func_0x033e4e80(lVar1);
    fVar4 = (float)func_0x052effa0(uVar3);
    if (fVar2 < fVar4) {
      uVar3 = func_0x033e4e80(lVar1);
      uVar5 = func_0x052f01a0(*(undefined4 *)(param_3 + 0x234),uVar3,1);
      uVar7 = param_2;
      uVar3 = func_0x033e4e80(lVar1);
      uVar6 = func_0x052f0390(*(undefined4 *)(param_3 + 0x234),uVar3,1);
      uVar3 = func_0x033e4e80(param_3 + 0x228);
      func_0x052b53b0(uVar5,param_2,uVar6,uVar7,uVar3,0,0,0);
      *(float *)(param_3 + 0x234) =
           param_1 * *(float *)(param_3 + 0x238) + *(float *)(param_3 + 0x234);
    }
  }
  return;
}



// ADDRESS 02fc9f30

void selected_02fc9f30(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                      undefined8 *param_5,long param_6,undefined8 *param_7)

{
  code *pcVar1;
  char cVar2;
  undefined8 uVar3;
  
  *param_5 = 0xcd8938;
  *(undefined4 *)(param_5 + 1) = param_1;
  *(undefined4 *)((long)param_5 + 0xc) = param_2;
  *(undefined4 *)(param_5 + 2) = param_3;
  *(undefined4 *)((long)param_5 + 0x14) = param_4;
  uVar3 = func_0x03008160();
  param_5[3] = uVar3;
  *param_5 = 0xcd88c8;
  param_5[5] = *param_7;
  func_0x033e4af0(param_5 + 6,param_6);
  cVar2 = func_0x031d4210(*(undefined4 *)param_7,0);
  if ((*(int *)((long)param_7 + 4) == 0) && (cVar2 != '\0')) {
    func_0x06fadc36();
    pcVar1 = (code *)swi(3);
    (*pcVar1)();
    return;
  }
  if (param_6 != 0) {
    uVar3 = func_0x033e4e80(param_5 + 6);
    uVar3 = func_0x0339ce50(uVar3,*param_7);
    param_5[4] = uVar3;
  }
  return;
}



// ADDRESS 02fc96b0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 selected_02fc96b0(long param_1)

{
  long lVar1;
  long lVar2;
  undefined1 auVar3 [12];
  float *pfVar4;
  ulong uVar5;
  long lVar6;
  float fVar7;
  undefined8 uVar8;
  float extraout_XMM1_Da;
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  undefined8 uStack_48;
  float fStack_40;
  undefined1 auStack_38 [16];
  undefined8 uStack_28;
  undefined8 uStack_20;
  
  uStack_28 = _DAT_071fa860;
  uStack_20 = 0;
  auStack_38 = ZEXT416((uint)_DAT_071fa868);
  fVar7 = 0.0;
  if (0.0 <= *(float *)(param_1 + 0x264)) {
    fVar7 = *(float *)(param_1 + 0x264);
  }
  uVar8 = func_0x052f01a0(fVar7,*(undefined8 *)(param_1 + 0x238),1);
  if (0 < (int)*(uint *)(param_1 + 0x248)) {
    uVar5 = 0;
    lVar6 = 0;
    fVar7 = _DAT_006c1588;
    do {
      lVar1 = *(long *)(*(long *)(param_1 + 0x240) + uVar5 * 8);
      lVar2 = *(long *)(lVar1 + 0x130);
      fVar19 = (float)_DAT_071fa860;
      fVar16 = (float)((ulong)_DAT_071fa860 >> 0x20);
      fVar14 = _DAT_071fa868;
      if (lVar2 != 0) {
        fVar19 = *(float *)(lVar2 + 0x1d0);
        fVar16 = *(float *)(lVar2 + 0x1d4);
        fVar14 = *(float *)(lVar2 + 0x1d8);
      }
      fVar16 = fVar16 - (float)((ulong)uVar8 >> 0x20);
      fVar19 = fVar19 - (float)uVar8;
      fVar14 = SQRT(fVar16 * fVar16 + fVar19 * fVar19 +
                    (fVar14 - extraout_XMM1_Da) * (fVar14 - extraout_XMM1_Da));
      if (fVar7 <= fVar14) {
        fVar14 = fVar7;
        lVar1 = lVar6;
      }
      lVar6 = lVar1;
      fVar7 = fVar14;
      uVar5 = uVar5 + 1;
    } while (*(uint *)(param_1 + 0x248) != uVar5);
    if (lVar6 != 0) {
      lVar1 = *(long *)(lVar6 + 0x130);
      uStack_48 = _DAT_071fa860;
      fStack_40 = _DAT_071fa868;
      if (lVar1 != 0) {
        uStack_48 = *(undefined8 *)(lVar1 + 0x1d0);
        fStack_40 = *(float *)(lVar1 + 0x1d8);
      }
      func_0x052f1190((float)uStack_28,*(undefined8 *)(param_1 + 0x238),&uStack_48);
      func_0x02fc6590(*(undefined8 *)(param_1 + 0x238));
      pfVar4 = (float *)&DAT_071fa830;
      if (*(long *)(lVar6 + 0x130) != 0) {
        pfVar4 = (float *)(*(long *)(lVar6 + 0x130) + 0x1c0);
      }
      auVar3 = *(undefined1 (*) [12])(param_1 + 0x254);
      auVar9._0_4_ = (float)((uint)(auVar3._0_4_ - pfVar4[4]) & _DAT_071a0f10);
      auVar9._4_4_ = (float)((uint)(auVar3._4_4_ - pfVar4[5]) & uRam00000000071a0f14);
      auVar9._8_4_ = (float)((uint)(auVar3._8_4_ - pfVar4[6]) & uRam00000000071a0f18);
      auVar9._12_4_ = (uint)(0.0 - pfVar4[7]) & uRam00000000071a0f1c;
      fVar11 = _DAT_071a0ea0 * *pfVar4;
      fVar12 = fRam00000000071a0ea4 * pfVar4[1];
      fVar13 = fRam00000000071a0ea8 * pfVar4[2];
      fVar15 = auVar9._4_4_ * fVar11 - fVar12 * auVar9._0_4_;
      fVar17 = auVar9._8_4_ * fVar12 - fVar13 * auVar9._4_4_;
      fVar18 = auVar9._0_4_ * fVar13 - fVar11 * auVar9._8_4_;
      fVar15 = fVar15 + fVar15;
      fVar17 = fVar17 + fVar17;
      fVar18 = fVar18 + fVar18;
      auVar10 = *(undefined1 (*) [16])(pfVar4 + 8);
      fVar7 = auVar10._0_4_;
      fVar14 = auVar10._4_4_;
      auVar10 = rcpps(auVar9,auVar10);
      fVar19 = auVar10._0_4_;
      fVar16 = auVar10._4_4_;
      fVar19 = (fVar19 + fVar19) - fVar19 * fVar19 * fVar7;
      fVar16 = (fVar16 + fVar16) - fVar16 * fVar16 * fVar14;
      uStack_28._0_4_ =
           (float)(-(uint)(_DAT_071a0e40 < (float)((uint)fVar7 & _DAT_071a0f30)) &
                  (uint)((fVar19 + fVar19) - fVar19 * fVar19 * fVar7)) *
           ((fVar15 * fVar12 - fVar18 * fVar13) +
           fRam00000000071a0eac * pfVar4[3] * fVar17 + auVar9._0_4_);
      uStack_28._4_4_ =
           (float)(-(uint)(fRam00000000071a0e44 < (float)((uint)fVar14 & uRam00000000071a0f34)) &
                  (uint)((fVar16 + fVar16) - fVar16 * fVar16 * fVar14)) *
           ((fVar17 * fVar13 - fVar15 * fVar11) +
           fRam00000000071a0eac * pfVar4[3] * fVar18 + auVar9._4_4_);
    }
  }
  return CONCAT44(uStack_28._4_4_,(float)uStack_28);
}



