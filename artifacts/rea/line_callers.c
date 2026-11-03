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
          fVar24 = (float)cosf();
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
          fVar24 = (float)sinf();
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
      cVar4 = selected_02fc4980(uStack_58,*puVar6,uStack_60,uStack_5c,uStack_70);
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



// ADDRESS 02fc7d70

void selected_02fc7d70(long param_1)

{
  int iVar1;
  uint uVar2;
  long *plVar3;
  code *pcVar4;
  bool bVar5;
  ulong uVar6;
  long lVar7;
  long lVar8;
  int *piVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  bool bVar13;
  int iVar14;
  long lVar15;
  int iStack_7c;
  int *piStack_78;
  undefined8 uStack_70;
  long lStack_68;
  int *piStack_60;
  int iStack_54;
  int **ppiStack_50;
  int **ppiStack_48;
  int *piStack_40;
  int *piStack_38;
  
  piStack_78 = (int *)0x0;
  uStack_70 = 0;
  iVar1 = *(int *)(param_1 + 0x2d8);
  lStack_68 = param_1;
  if ((long)iVar1 != 0) {
    lVar8 = *(long *)(param_1 + 0x2d0);
    lVar15 = 0;
    ppiStack_48 = &piStack_78;
    do {
      uVar6 = uStack_70;
      iStack_7c = *(int *)(lVar8 + lVar15);
      if (iStack_7c == *(int *)(lStack_68 + 0x278)) break;
      piStack_60 = &iStack_7c;
      if ((piStack_78 <= &iStack_7c) && (&iStack_7c < piStack_78 + uStack_70._4_4_)) {
        ppiStack_50 = &piStack_60;
        func_0x06f8e390(&ppiStack_50);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      iVar12 = (int)uStack_70;
      lVar7 = (long)(int)uStack_70;
      uStack_70 = CONCAT44(uStack_70._4_4_,(int)uStack_70 + 1);
      if (uStack_70._4_4_ <= iVar12) {
        func_0x025f0330(ppiStack_48,uVar6 & 0xffffffff);
      }
      piStack_78[lVar7] = iStack_7c;
      lVar15 = lVar15 + 4;
    } while ((long)iVar1 * 4 != lVar15);
  }
  ppiStack_48 = (int **)(lStack_68 + 0x2d0);
  lVar8 = lStack_68;
  if ((long)(int)uStack_70 != 0) {
    piStack_40 = piStack_78 + (int)uStack_70;
    piVar9 = piStack_78;
    do {
      piStack_60 = &iStack_7c;
      iStack_7c = *piVar9;
      if ((*ppiStack_48 <= piStack_60) && (piStack_60 < *ppiStack_48 + *(int *)(lVar8 + 0x2dc))) {
        ppiStack_50 = &piStack_60;
        func_0x06f8e390(&ppiStack_50);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      iVar1 = *(int *)(lVar8 + 0x2d8);
      if ((long)iVar1 != 0) {
        iVar12 = 0;
        iVar11 = 0;
        piStack_38 = piVar9;
        bVar13 = **ppiStack_48 == iStack_7c;
        do {
          lVar15 = (long)iVar11;
          lVar8 = lVar15;
          iVar10 = iVar11;
          iVar14 = iVar11 + 1;
          if (iVar11 < iVar1) {
            iVar14 = iVar1;
          }
          do {
            lVar8 = lVar8 + 1;
            if (iVar1 <= lVar8) {
              bVar5 = false;
              if (bVar13 != false) goto LAB_02fc7f7a;
              goto LAB_02fc7f48;
            }
            iVar10 = iVar10 + 1;
          } while (bVar13 == (*(int *)((long)*ppiStack_48 + lVar8 * 4) == iStack_7c));
          bVar5 = true;
          iVar14 = iVar10;
          if (!bVar13) {
LAB_02fc7f48:
            if (iVar12 != iVar11) {
              iStack_54 = iVar12;
              memmove((long)*ppiStack_48 + (long)iVar12 * 4,(long)*ppiStack_48 + lVar15 * 4,
                      (long)(iVar14 - iVar11) << 2);
              iVar12 = iStack_54;
            }
            iVar12 = iVar12 + (iVar14 - iVar11);
          }
LAB_02fc7f7a:
          iVar11 = iVar14;
          bVar13 = (bool)(bVar13 ^ 1);
        } while (bVar5);
        *(int *)(lStack_68 + 0x2d8) = iVar12;
        piVar9 = piStack_38;
        lVar8 = lStack_68;
      }
      piVar9 = piVar9 + 1;
    } while (piVar9 != piStack_40);
  }
  if (*(char *)(lVar8 + 0x250) == '\0') {
    func_0x02977980(ppiStack_48,lVar8 + 0x278);
  }
  if (*(int *)(lVar8 + 0x2d8) < 1) {
    if ((*(int *)(lVar8 + 0x228) != 0) &&
       (plVar3 = *(long **)(lVar8 + 0x220), plVar3 != (long *)0x0)) {
      (**(code **)(*plVar3 + 0x58))
                (plVar3,*(undefined4 *)(lVar8 + 0x230),4,*(undefined1 *)(lVar8 + 0x250));
    }
  }
  else {
    uVar2 = **(uint **)(lVar8 + 0x2d0);
    if ((-1 < (int)uVar2) && ((int)uVar2 < *(int *)(lVar8 + 0x248))) {
      func_0x02fc5630(*(undefined8 *)(*(long *)(lVar8 + 0x240) + (ulong)uVar2 * 8),1);
    }
  }
  if (piStack_78 != (int *)0x0) {
    func_0x03071ea0();
  }
  return;
}



