// ADDRESS 02fb23b0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fb23b0(undefined4 param_1,float param_2,float param_3,float param_4,ulong *param_5)

{
  ulong *puVar1;
  int iVar2;
  ulong uVar3;
  code *pcVar4;
  uint3 uVar5;
  uint uVar6;
  long lVar7;
  uint uVar8;
  float fVar9;
  long lVar10;
  undefined4 uVar11;
  ulong uVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  undefined8 **ppuStack_e0;
  undefined8 ***pppuStack_d8;
  long lStack_d0;
  undefined8 **ppuStack_c8;
  undefined8 uStack_c0;
  undefined4 uStack_b8;
  uint uStack_b4;
  float fStack_b0;
  float fStack_ac;
  undefined8 **ppuStack_a8;
  undefined8 uStack_a0;
  undefined8 **ppuStack_98;
  undefined8 uStack_90;
  undefined8 **ppuStack_88;
  undefined8 uStack_80;
  long lStack_70;
  float fStack_64;
  float *pfStack_60;
  float fStack_58;
  float fStack_54;
  ulong uStack_50;
  float fStack_48;
  
  lStack_70 = CONCAT44(lStack_70._4_4_,param_1);
  *(undefined4 *)(param_5 + 1) = 0;
  fStack_ac = param_3;
  fStack_64 = param_2;
  fStack_48 = param_4;
  if (*(int *)((long)param_5 + 0xc) != 0) {
    func_0x025eceb0(param_5,0);
  }
  *(undefined4 *)((long)param_5 + 0x14) = 0;
  ppuStack_88 = (undefined8 ***)0x0;
  uStack_80 = 0;
  ppuStack_98 = (undefined8 ***)0x0;
  uStack_90 = 0;
  ppuStack_a8 = (undefined8 ***)0x0;
  uStack_a0 = 0;
  if (_DAT_006c1440 <= (float)lStack_70) {
    pfStack_60 = (float *)((long)param_5 + 0x14);
    fVar14 = (float)lStack_70;
    do {
      lStack_d0 = CONCAT44(lStack_d0._4_4_,fVar14);
      uVar5 = rand();
      uVar12 = uStack_80;
      ppuStack_c8 = (undefined8 **)
                    CONCAT44(ppuStack_c8._4_4_,
                             (float)uVar5 / _DAT_006c163c + (float)uVar5 / _DAT_006c163c +
                             _DAT_006c0d20);
      pppuStack_d8 = &ppuStack_88;
      uStack_c0 = &ppuStack_c8;
      if ((ppuStack_88 <= &ppuStack_c8) &&
         (&ppuStack_c8 < (undefined8 ***)((long)ppuStack_88 + (long)uStack_80._4_4_ * 4))) {
        ppuStack_e0 = (undefined8 **)&uStack_c0;
        func_0x06f8e390(&ppuStack_e0);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      iVar2 = (int)uStack_80;
      lVar7 = (long)(int)uStack_80;
      uStack_80 = CONCAT44(uStack_80._4_4_,(int)uStack_80 + 1);
      if (uStack_80._4_4_ <= iVar2) {
        func_0x025f0330(&ppuStack_88,uVar12 & 0xffffffff);
      }
      *(float *)((long)ppuStack_88 + lVar7 * 4) = ppuStack_c8._0_4_;
      fVar14 = (float)lStack_d0 - (ppuStack_c8._0_4_ + _DAT_006c14d4);
    } while (_DAT_006c1440 <= fVar14);
    iVar2 = (int)uStack_80;
    lVar7 = (long)(int)uStack_80;
    if (1 < lVar7) {
      uVar12 = uStack_80 & 0xffffffff;
      lStack_d0 = lVar7;
      uVar5 = rand();
      uVar6 = (uint)((float)iVar2 * ((float)uVar5 / _DAT_006c163c));
      uVar8 = (uint)(lVar7 + -1);
      if (iVar2 <= (int)uVar6) {
        uVar6 = uVar8;
      }
      uStack_c0 = (undefined8 ***)CONCAT44(uStack_c0._4_4_,uVar6);
      uStack_50 = uVar12;
      if (((uint)((int)uVar6 < (int)uStack_80) & ~(uVar6 >> 0x1f)) != 1) {
        ppuStack_e0 = (undefined8 **)&uStack_c0;
        func_0x06f8ce70(&ppuStack_e0);
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      fVar14 = *(float *)((long)ppuStack_88 + (long)(int)uVar6 * 4);
      fVar15 = (float)(int)uVar8;
      fStack_54 = (fStack_64 - fVar14 * fStack_ac) /
                  (((float)lStack_70 - fVar14) - (fVar15 + fVar15));
      fStack_58 = _DAT_006c0fdc + fStack_ac;
      uVar8 = 0;
      lStack_70 = lVar7 + -1;
      do {
        fVar14 = fStack_ac;
        if (uVar6 != uVar8) {
          uVar5 = rand();
          fVar15 = ((float)uVar5 / _DAT_006c163c) * _DAT_006c1a40 + _DAT_006c0c84 + fStack_54;
          ppuStack_c8 = (undefined8 **)((ulong)ppuStack_c8 & 0xffffffff00000000);
          fVar14 = fVar15;
          if (fStack_58 <= fVar15) {
            fVar14 = fStack_58;
          }
          fVar14 = (float)(-(uint)(fVar15 < fStack_48) & (uint)fStack_48 |
                          ~-(uint)(fVar15 < fStack_48) & (uint)fVar14);
        }
        uVar12 = uStack_90;
        ppuStack_c8 = (undefined8 **)CONCAT44(ppuStack_c8._4_4_,fVar14);
        if ((ppuStack_98 <= &ppuStack_c8) &&
           (&ppuStack_c8 < (undefined8 ***)((long)ppuStack_98 + (long)uStack_90._4_4_ * 4))) {
          ppuStack_e0 = (undefined8 **)&uStack_c0;
          pppuStack_d8 = &ppuStack_98;
          uStack_c0 = &ppuStack_c8;
          func_0x06f8e390(&ppuStack_e0);
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
        iVar2 = (int)uStack_90;
        lVar7 = (long)(int)uStack_90;
        uStack_90 = CONCAT44(uStack_90._4_4_,(int)uStack_90 + 1);
        uStack_c0 = &ppuStack_c8;
        if (uStack_90._4_4_ <= iVar2) {
          func_0x025f0330(&ppuStack_98,uVar12 & 0xffffffff);
        }
        *(float *)((long)ppuStack_98 + lVar7 * 4) = ppuStack_c8._0_4_;
        uVar5 = rand();
        uVar12 = uStack_a0;
        fStack_b0 = ((float)uVar5 / _DAT_006c163c) * _DAT_006c0e50 + _DAT_006c0fa8;
        uStack_c0 = (undefined8 ***)&fStack_b0;
        if ((ppuStack_a8 <= &fStack_b0) &&
           (&fStack_b0 < (undefined8 ***)((long)ppuStack_a8 + (long)uStack_a0._4_4_ * 4))) {
          ppuStack_e0 = (undefined8 **)&uStack_c0;
          pppuStack_d8 = &ppuStack_a8;
          func_0x06f8e390(&ppuStack_e0);
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
        iVar2 = (int)uStack_a0;
        lVar7 = (long)(int)uStack_a0;
        uStack_a0 = CONCAT44(uStack_a0._4_4_,(int)uStack_a0 + 1);
        if (uStack_a0._4_4_ <= iVar2) {
          func_0x025f0330(&ppuStack_a8,uVar12 & 0xffffffff);
        }
        *(float *)((long)ppuStack_a8 + lVar7 * 4) = fStack_b0;
        uVar8 = uVar8 + 1;
      } while ((float)lStack_d0 != (float)uVar8);
      fVar14 = 0.0;
      uVar12 = 0;
      do {
        uVar11 = (undefined4)uVar12;
        uStack_c0 = (undefined8 ***)CONCAT44(uStack_c0._4_4_,uVar11);
        lStack_d0 = CONCAT44(lStack_d0._4_4_,fVar14);
        if ((long)(int)uStack_80 <= (long)uVar12) {
          pppuStack_d8 = &ppuStack_88;
          ppuStack_e0 = (undefined8 **)&uStack_c0;
          func_0x06f8ce70(&ppuStack_e0);
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
        fStack_ac = *(float *)((long)ppuStack_88 + uVar12 * 4);
        ppuStack_c8 = (undefined8 **)CONCAT44(ppuStack_c8._4_4_,uVar11);
        if ((long)(int)uStack_90 <= (long)uVar12) {
          pppuStack_d8 = &ppuStack_98;
          ppuStack_e0 = &ppuStack_c8;
          func_0x06f8ce70(&ppuStack_e0);
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
        ppuStack_c8 = (undefined8 **)CONCAT44(ppuStack_c8._4_4_,uVar11);
        if ((long)(int)uStack_a0 <= (long)uVar12) {
          pppuStack_d8 = &ppuStack_a8;
          ppuStack_e0 = &ppuStack_c8;
          func_0x06f8ce70(&ppuStack_e0);
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
        uStack_b8 = *(undefined4 *)((long)ppuStack_a8 + uVar12 * 4);
        uStack_c0 = (undefined8 ***)CONCAT44(*(undefined4 *)((long)ppuStack_98 + uVar12 * 4),fVar14)
        ;
        uStack_b4 = uStack_b4 & 0xffffff00;
        pppuStack_d8 = (undefined8 ***)param_5;
        ppuStack_c8 = (undefined8 **)&uStack_c0;
        if (((undefined8 **)*param_5 <= &uStack_c0) &&
           (&uStack_c0 < (undefined8 **)*param_5 + (long)*(int *)((long)param_5 + 0xc) * 2)) {
          ppuStack_e0 = &ppuStack_c8;
          func_0x06f8cea0(&ppuStack_e0);
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
        iVar2 = (int)param_5[1];
        *(int *)(param_5 + 1) = iVar2 + 1;
        if (*(int *)((long)param_5 + 0xc) <= iVar2) {
          func_0x025eaee0(param_5,iVar2);
        }
        puVar1 = (ulong *)(*param_5 + (long)iVar2 * 0x10);
        *puVar1 = (ulong)uStack_c0;
        puVar1[1] = CONCAT44(uStack_b4,uStack_b8);
        fVar14 = (float)lStack_d0 + fStack_ac;
        if ((long)uVar12 < lStack_70) {
          ppuStack_c8 = (undefined8 **)CONCAT44(ppuStack_c8._4_4_,uVar11);
          lStack_d0 = CONCAT44(lStack_d0._4_4_,fVar14);
          if ((long)(int)uStack_90 <= (long)uVar12) {
            pppuStack_d8 = &ppuStack_98;
            ppuStack_e0 = &ppuStack_c8;
            func_0x06f8ce70(&ppuStack_e0);
            pcVar4 = (code *)swi(3);
            (*pcVar4)();
            return;
          }
          ppuStack_c8 = (undefined8 **)CONCAT44(ppuStack_c8._4_4_,uVar11);
          if ((long)(int)uStack_a0 <= (long)uVar12) {
            pppuStack_d8 = &ppuStack_a8;
            ppuStack_e0 = &ppuStack_c8;
            func_0x06f8ce70(&ppuStack_e0);
            pcVar4 = (code *)swi(3);
            (*pcVar4)();
            return;
          }
          uStack_b8 = *(undefined4 *)((long)ppuStack_a8 + uVar12 * 4);
          uStack_c0 = (undefined8 ***)
                      CONCAT44(*(undefined4 *)((long)ppuStack_98 + uVar12 * 4),fVar14);
          uStack_b4 = CONCAT31(uStack_b4._1_3_,1);
          if (((undefined8 **)*param_5 <= &uStack_c0) &&
             (&uStack_c0 < (undefined8 **)*param_5 + (long)*(int *)((long)param_5 + 0xc) * 2)) {
            ppuStack_e0 = &ppuStack_c8;
            ppuStack_c8 = (undefined8 **)&uStack_c0;
            func_0x06f8cea0(&ppuStack_e0);
            pcVar4 = (code *)swi(3);
            (*pcVar4)();
            return;
          }
          iVar2 = (int)param_5[1];
          *(int *)(param_5 + 1) = iVar2 + 1;
          ppuStack_c8 = (undefined8 **)&uStack_c0;
          if (*(int *)((long)param_5 + 0xc) <= iVar2) {
            func_0x025eaee0(param_5,iVar2);
          }
          puVar1 = (ulong *)(*param_5 + (long)iVar2 * 0x10);
          *puVar1 = (ulong)uStack_c0;
          puVar1[1] = CONCAT44(uStack_b4,uStack_b8);
          fVar14 = (float)lStack_d0 + _DAT_006c14d4;
        }
        uVar12 = uVar12 + 1;
      } while (uStack_50 != uVar12);
      *(undefined4 *)((long)param_5 + 0x14) = 0;
      ppuStack_c8 = (undefined8 **)((ulong)ppuStack_c8 & 0xffffffff00000000);
      fStack_b0 = 0.0;
      uVar12 = (ulong)(uint)param_5[1];
      fVar14 = 0.0;
      if (1 < (int)(uint)param_5[1]) {
        fVar14 = 0.0;
        lVar7 = 0xc;
        lVar10 = 0;
        fVar15 = _DAT_006c1394;
        do {
          uStack_c0 = (undefined8 ***)CONCAT44(uStack_c0._4_4_,(int)lVar10);
          if ((int)uVar12 <= lVar10) {
            lStack_d0 = CONCAT44(lStack_d0._4_4_,fVar14);
            ppuStack_e0 = (undefined8 **)&uStack_c0;
            func_0x06f8ce70(&ppuStack_e0);
            pcVar4 = (code *)swi(3);
            (*pcVar4)();
            return;
          }
          uVar3 = *param_5;
          lVar10 = lVar10 + 1;
          fVar9 = (float)lVar10;
          uStack_c0 = (undefined8 ***)CONCAT44(uStack_c0._4_4_,fVar9);
          if ((int)uVar12 <= lVar10) {
            lStack_d0 = CONCAT44(lStack_d0._4_4_,fVar14);
            ppuStack_e0 = (undefined8 **)&uStack_c0;
            func_0x06f8ce70(&ppuStack_e0);
            pcVar4 = (code *)swi(3);
            (*pcVar4)();
            return;
          }
          fVar13 = *(float *)((uVar3 - 8) + lVar7);
          if (*(char *)(uVar3 + lVar7) != '\0') {
            fVar13 = (*(float *)(uVar3 + 8 + lVar7) + fVar13) * fVar15;
          }
          fVar13 = (*(float *)(uVar3 + 4 + lVar7) - *(float *)((uVar3 - 0xc) + lVar7)) * fVar13;
          ppuStack_e0 = (undefined8 **)CONCAT44(ppuStack_e0._4_4_,fVar13);
          fVar14 = fVar14 + fVar13;
          ppuStack_c8 = (undefined8 **)CONCAT44(ppuStack_c8._4_4_,fVar14);
          *pfStack_60 = fVar13 + *pfStack_60;
          if (4 < DAT_0719ee4c) {
            lStack_d0 = CONCAT44(lStack_d0._4_4_,fVar14);
            func_0x02fb2d20(&fStack_b0,(uVar3 - 0xc) + lVar7,uVar3 + lVar7 + 4,(uVar3 - 8) + lVar7,
                            (uVar3 - 4) + lVar7,uVar3 + lVar7,&ppuStack_e0,&ppuStack_c8);
            fVar14 = (float)lStack_d0;
            fVar15 = _DAT_006c1394;
          }
          uVar12 = (ulong)(int)param_5[1];
          lVar7 = lVar7 + 0x10;
          fStack_b0 = fVar9;
        } while (lVar10 < (long)(uVar12 - 1));
        fVar14 = *pfStack_60;
      }
      ppuStack_e0 = (undefined8 **)
                    CONCAT44(ppuStack_e0._4_4_,
                             ((float)((uint)(fVar14 - fStack_64) & _DAT_006c6790) / fStack_64) *
                             _DAT_006c1b34);
      if (2 < DAT_0719ee4c) {
        func_0x02fb2d90(pfStack_60,&ppuStack_e0);
      }
      goto LAB_02fb2c12;
    }
  }
  if (1 < DAT_0719ee4c) {
    func_0x02fb2cf0();
  }
LAB_02fb2c12:
  if ((undefined8 ***)ppuStack_a8 != (undefined8 ***)0x0) {
    func_0x03071ea0();
  }
  if ((undefined8 ***)ppuStack_98 != (undefined8 ***)0x0) {
    func_0x03071ea0();
  }
  if ((undefined8 ***)ppuStack_88 != (undefined8 ***)0x0) {
    func_0x03071ea0();
  }
  return;
}



// ADDRESS 02fb2040

void selected_02fb2040(undefined8 param_1,undefined4 param_2)

{
  (*(code *)PTR_srand_07158928)(param_2);
  return;
}



// ADDRESS 02fb2050

void selected_02fb2050(float param_1,long *param_2)

{
  code *pcVar1;
  ulong uVar2;
  long lVar3;
  long lVar4;
  int iStack_44;
  int *piStack_40;
  long *plStack_38;
  
  uVar2 = (ulong)*(uint *)(param_2 + 1);
  if (1 < (int)*(uint *)(param_2 + 1)) {
    lVar4 = 0;
    piStack_40 = &iStack_44;
    lVar3 = 0;
    do {
      iStack_44 = (int)lVar3;
      plStack_38 = param_2;
      if ((int)uVar2 <= lVar3) {
        func_0x06f8ce70(&piStack_40);
        pcVar1 = (code *)swi(3);
        (*pcVar1)(param_1);
        return;
      }
      lVar3 = lVar3 + 1;
      if (*(float *)(*param_2 + lVar4) <= param_1) {
        if ((int)param_2[1] <= lVar3) {
          iStack_44 = iStack_44 + 1;
          func_0x06f8ce70(&piStack_40);
          pcVar1 = (code *)swi(3);
          (*pcVar1)(param_1);
          return;
        }
        if (param_1 < *(float *)(*param_2 + 0x10 + lVar4)) {
          *(int *)(param_2 + 2) = iStack_44;
          return;
        }
      }
      uVar2 = (ulong)(int)param_2[1];
      lVar4 = lVar4 + 0x10;
    } while (lVar3 < (long)(uVar2 - 1));
  }
  return;
}



// ADDRESS 02fb2130

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 selected_02fb2130(undefined8 param_1,long *param_2)

{
  float fVar1;
  int iVar2;
  long lVar3;
  code *pcVar4;
  long lVar5;
  long lVar6;
  undefined8 uVar7;
  undefined4 in_XMM0_Dc;
  undefined4 in_XMM0_Dd;
  float fVar8;
  float fVar9;
  uint uStack_5c;
  undefined8 uStack_58;
  long *plStack_50;
  undefined1 auStack_48 [8];
  float fStack_40;
  float fStack_3c;
  float fStack_38;
  float fStack_34;
  float fStack_30;
  float fStack_2c;
  float fStack_28;
  float fStack_24;
  
  fStack_24 = (float)param_1;
  iVar2 = (int)param_2[1];
  if (iVar2 < 2) {
    auStack_48._0_4_ = (undefined4)_DAT_071fa860;
    auStack_48._4_4_ = (undefined4)((ulong)_DAT_071fa860 >> 0x20);
  }
  else {
    uStack_5c = *(uint *)(param_2 + 2);
    plStack_50 = param_2;
    if (((uint)((int)uStack_5c < iVar2) & ~(uStack_5c >> 0x1f)) != 1) {
      uStack_58 = &uStack_5c;
      fStack_40 = (float)in_XMM0_Dc;
      auStack_48 = (undefined1  [8])param_1;
      fStack_3c = (float)in_XMM0_Dd;
      func_0x06f8ce70(&uStack_58);
      pcVar4 = (code *)swi(3);
      uVar7 = (*pcVar4)();
      return uVar7;
    }
    lVar6 = (long)(int)uStack_5c;
    lVar3 = *param_2;
    uStack_5c = uStack_5c + 1;
    if (((uint)((int)uStack_5c < iVar2) & ~(uStack_5c >> 0x1f)) != 1) {
      uStack_58 = &uStack_5c;
      fStack_40 = (float)in_XMM0_Dc;
      auStack_48 = (undefined1  [8])param_1;
      fStack_3c = (float)in_XMM0_Dd;
      func_0x06f8ce70(&uStack_58);
      pcVar4 = (code *)swi(3);
      uVar7 = (*pcVar4)();
      return uVar7;
    }
    lVar6 = lVar6 * 0x10;
    if (*(char *)(lVar3 + 0xc + lVar6) == '\0') {
      _auStack_48 = ZEXT416(*(uint *)(lVar3 + 4 + lVar6));
      func_0x070a93a0(*(float *)(lVar3 + 8 + lVar6) * _DAT_006c0f10,&fStack_30,&fStack_34);
      fStack_2c = fStack_34;
      fStack_28 = fStack_30;
      fStack_38 = (float)auStack_48._0_4_;
    }
    else {
      lVar5 = (long)(int)uStack_5c * 0x10;
      fVar8 = (fStack_24 - *(float *)(lVar3 + lVar6)) /
              (*(float *)(lVar3 + lVar5) - *(float *)(lVar3 + lVar6));
      fVar9 = fVar8;
      if (_DAT_006c0a14 <= fVar8) {
        fVar9 = _DAT_006c0a14;
      }
      fVar9 = (float)(~-(uint)(fVar8 < 0.0) & (uint)fVar9);
      fVar8 = *(float *)(lVar3 + 4 + lVar6);
      fVar1 = *(float *)(lVar3 + 8 + lVar6);
      _auStack_48 = ZEXT416((uint)((*(float *)(lVar3 + 4 + lVar5) - fVar8) * fVar9 + fVar8));
      func_0x070a93a0(((*(float *)(lVar3 + 8 + lVar5) - fVar1) * fVar9 + fVar1) * _DAT_006c0f10,
                      &fStack_28,&fStack_2c);
      fStack_38 = (float)auStack_48._0_4_;
    }
    auStack_48._0_4_ = fStack_2c * fStack_38;
    auStack_48._4_4_ = fStack_28 * fStack_38;
    fStack_3c = fStack_40 * 0.0;
    uStack_58 = (uint *)CONCAT44(auStack_48._4_4_,auStack_48._0_4_);
    fStack_38 = fStack_38 * 0.0;
    plStack_50 = (long *)CONCAT44(plStack_50._4_4_,fStack_38);
    if (5 < DAT_0719ee4c) {
      fStack_40 = fStack_40 * 0.0;
      func_0x02fb2360(&fStack_24,param_2 + 2,lVar3 + lVar6 + 0xc,&uStack_58,(long)&uStack_58 + 4);
    }
  }
  return CONCAT44(auStack_48._4_4_,auStack_48._0_4_);
}



