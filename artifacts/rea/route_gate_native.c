// ADDRESS 02fc8090

void selected_02fc8090(long param_1)

{
  float *pfVar1;
  float fVar2;
  code *pcVar3;
  uint uVar4;
  bool bVar5;
  int iVar6;
  int iVar7;
  undefined8 uVar8;
  ulong uVar9;
  long lVar10;
  long lVar11;
  long *plVar12;
  int iVar13;
  uint uVar14;
  float fVar15;
  int iVar16;
  int iVar17;
  long lVar18;
  long lVar19;
  undefined4 uVar20;
  float **ppfStack_80;
  float **ppfStack_78;
  long *plStack_70;
  float *apfStack_68 [2];
  float *pfStack_58;
  undefined8 uStack_50;
  float fStack_40;
  undefined4 uStack_3c;
  long lStack_38;
  
  func_0x0509aaf0();
  if (*(int *)(param_1 + 0x2b4) < *(int *)(param_1 + 0x288)) {
    func_0x052f1190(*(undefined8 *)(param_1 + 0x238),param_1 + 0x254);
    uVar20 = func_0x02fc6590(*(undefined8 *)(param_1 + 0x238));
    fVar15 = *(float *)(param_1 + 0x264);
    *(float *)(param_1 + 0x260) = fVar15;
    *(undefined4 *)(param_1 + 0x264) = uVar20;
    pfStack_58 = (float *)0x0;
    uStack_50 = 0;
    plStack_70 = (long *)(param_1 + 0x2e0);
    uVar9 = (ulong)*(uint *)(param_1 + 0x2e8);
    lStack_38 = param_1;
    if (0 < (int)*(uint *)(param_1 + 0x2e8)) {
      lVar18 = 0;
      lVar19 = 0;
      do {
        lVar10 = *(long *)(param_1 + 0x2e0);
        fVar2 = *(float *)(lVar10 + lVar19 * 4);
        uVar20 = (undefined4)lVar19;
        apfStack_68[0]._0_4_ = uVar20;
        if (*(char *)(param_1 + 0x250) == '\0') {
          if (fVar15 < fVar2) {
            fVar15 = *(float *)(param_1 + 0x264);
            apfStack_68[0] = (float *)CONCAT44(apfStack_68[0]._4_4_,uVar20);
            if ((int)uVar9 <= lVar19) {
              ppfStack_78 = (float **)plStack_70;
              ppfStack_80 = apfStack_68;
              fStack_40 = fVar15;
              func_0x06f8ce70(&ppfStack_80);
              pcVar3 = (code *)swi(3);
              (*pcVar3)();
              return;
            }
            pfVar1 = (float *)(lVar10 + lVar19 * 4);
            if (*pfVar1 <= fVar15 && fVar15 != *pfVar1) {
              uVar8 = func_0x02fcc220();
              if (*(int *)(param_1 + 0x2e8) <= lVar19) {
                ppfStack_80 = (float **)&uStack_3c;
                ppfStack_78 = (float **)plStack_70;
                uStack_3c = uVar20;
                func_0x06f8ce70(&ppfStack_80);
                pcVar3 = (code *)swi(3);
                (*pcVar3)();
                return;
              }
              uStack_3c = uVar20;
              func_0x03006490((double)*(float *)(*plStack_70 + lVar19 * 4),apfStack_68,&UNK_0072849a
                              ,*(undefined4 *)(param_1 + 0x230));
              func_0x02fced50(uVar8,apfStack_68,1);
              if (apfStack_68[0] != (float *)0x0) {
                func_0x03071ea0();
              }
              uVar8 = func_0x02fcc220();
              func_0x03006490((double)*(float *)(param_1 + 0x260),
                              (double)*(float *)(param_1 + 0x264),&ppfStack_80,&UNK_00885090);
              func_0x02fced50(uVar8,&ppfStack_80,1);
              if (ppfStack_80 != (float **)0x0) {
                func_0x03071ea0();
              }
              if (*(int *)(param_1 + 0x2e8) <= lVar19) {
                ppfStack_80 = apfStack_68;
                ppfStack_78 = (float **)plStack_70;
                func_0x06f8ce70(&ppfStack_80);
                pcVar3 = (code *)swi(3);
                (*pcVar3)();
                return;
              }
              func_0x02fc8900(param_1);
              uVar9 = uStack_50;
              apfStack_68[0] = (float *)CONCAT44(apfStack_68[0]._4_4_,uVar20);
              if (*(int *)(param_1 + 0x2e8) <= lVar19) {
                ppfStack_80 = apfStack_68;
                ppfStack_78 = (float **)plStack_70;
                func_0x06f8ce70(&ppfStack_80);
                pcVar3 = (code *)swi(3);
                (*pcVar3)();
                return;
              }
              lVar10 = *plStack_70;
              apfStack_68[0] = (float *)(lVar10 + lVar18);
              if ((pfStack_58 <= apfStack_68[0]) && (apfStack_68[0] < pfStack_58 + uStack_50._4_4_))
              {
                ppfStack_80 = apfStack_68;
                ppfStack_78 = &pfStack_58;
                func_0x06f8e390(&ppfStack_80);
                pcVar3 = (code *)swi(3);
                (*pcVar3)();
                return;
              }
              iVar7 = (int)uStack_50;
              lVar11 = (long)(int)uStack_50;
              uStack_50 = CONCAT44(uStack_50._4_4_,(int)uStack_50 + 1);
              if (uStack_50._4_4_ <= iVar7) {
                func_0x025f0330(&pfStack_58,uVar9 & 0xffffffff);
              }
              goto LAB_02fc8490;
            }
          }
        }
        else if (fVar2 < fVar15) {
          apfStack_68[0] = (float *)CONCAT44(apfStack_68[0]._4_4_,uVar20);
          if ((int)uVar9 <= lVar19) {
            ppfStack_78 = (float **)plStack_70;
            ppfStack_80 = apfStack_68;
            fStack_40 = *(float *)(param_1 + 0x264);
            func_0x06f8ce70(&ppfStack_80);
            pcVar3 = (code *)swi(3);
            (*pcVar3)();
            return;
          }
          if (*(float *)(param_1 + 0x264) < *(float *)(lVar10 + lVar19 * 4)) {
            uVar8 = func_0x02fcc220();
            if (*(int *)(param_1 + 0x2e8) <= lVar19) {
              ppfStack_80 = (float **)&uStack_3c;
              ppfStack_78 = (float **)plStack_70;
              uStack_3c = uVar20;
              func_0x06f8ce70(&ppfStack_80);
              pcVar3 = (code *)swi(3);
              (*pcVar3)();
              return;
            }
            uStack_3c = uVar20;
            func_0x03006490((double)*(float *)(*plStack_70 + lVar19 * 4),apfStack_68,&UNK_008d0188,
                            *(undefined4 *)(param_1 + 0x230));
            func_0x02fced50(uVar8,apfStack_68,1);
            if (apfStack_68[0] != (float *)0x0) {
              func_0x03071ea0();
            }
            uVar8 = func_0x02fcc220();
            func_0x03006490((double)*(float *)(param_1 + 0x260),(double)*(float *)(param_1 + 0x264),
                            &ppfStack_80,&UNK_00885090);
            func_0x02fced50(uVar8,&ppfStack_80,1);
            if (ppfStack_80 != (float **)0x0) {
              func_0x03071ea0();
            }
            if (*(int *)(param_1 + 0x2e8) <= lVar19) {
              ppfStack_80 = apfStack_68;
              ppfStack_78 = (float **)plStack_70;
              func_0x06f8ce70(&ppfStack_80);
              pcVar3 = (code *)swi(3);
              (*pcVar3)();
              return;
            }
            func_0x02fc8900(param_1);
            uVar9 = uStack_50;
            apfStack_68[0] = (float *)CONCAT44(apfStack_68[0]._4_4_,uVar20);
            if (*(int *)(param_1 + 0x2e8) <= lVar19) {
              ppfStack_80 = apfStack_68;
              ppfStack_78 = (float **)plStack_70;
              func_0x06f8ce70(&ppfStack_80);
              pcVar3 = (code *)swi(3);
              (*pcVar3)();
              return;
            }
            lVar10 = *plStack_70;
            apfStack_68[0] = (float *)(lVar10 + lVar18);
            if ((pfStack_58 <= apfStack_68[0]) && (apfStack_68[0] < pfStack_58 + uStack_50._4_4_)) {
              ppfStack_80 = apfStack_68;
              ppfStack_78 = &pfStack_58;
              func_0x06f8e390(&ppfStack_80);
              pcVar3 = (code *)swi(3);
              (*pcVar3)();
              return;
            }
            iVar7 = (int)uStack_50;
            lVar11 = (long)(int)uStack_50;
            uStack_50 = CONCAT44(uStack_50._4_4_,(int)uStack_50 + 1);
            if (uStack_50._4_4_ <= iVar7) {
              func_0x025f0330(&pfStack_58,uVar9 & 0xffffffff);
            }
LAB_02fc8490:
            pfStack_58[lVar11] = *(float *)(lVar10 + lVar19 * 4);
          }
        }
        lVar19 = lVar19 + 1;
        uVar9 = (ulong)*(int *)(param_1 + 0x2e8);
        if ((long)uVar9 <= lVar19) break;
        fVar15 = *(float *)(param_1 + 0x260);
        lVar18 = lVar18 + 4;
      } while( true );
    }
    while (pfVar1 = pfStack_58, 0 < (int)uStack_50) {
      apfStack_68[0] = pfStack_58;
      if (((float *)*plStack_70 <= pfStack_58) &&
         (pfStack_58 < (float *)*plStack_70 + *(int *)(param_1 + 0x2ec))) {
        ppfStack_80 = apfStack_68;
        ppfStack_78 = (float **)plStack_70;
        func_0x06f8e390(&ppfStack_80);
        pcVar3 = (code *)swi(3);
        (*pcVar3)();
        return;
      }
      iVar7 = *(int *)(param_1 + 0x2e8);
      if ((long)iVar7 != 0) {
        uVar14 = -(uint)(*pfStack_58 == *(float *)*plStack_70) & 1;
        iVar16 = 0;
        iVar17 = 0;
        plVar12 = plStack_70;
        do {
          fVar15 = (float)(uVar14 ^ 1);
          lVar19 = (long)iVar17;
          lVar18 = lVar19;
          iVar13 = iVar17;
          iVar6 = iVar17 + 1;
          if (iVar17 < iVar7) {
            iVar6 = iVar7;
          }
          do {
            lVar18 = lVar18 + 1;
            if (iVar7 <= lVar18) {
              bVar5 = false;
              uVar4 = uVar14 & 1;
              iVar13 = iVar6;
              uVar14 = (uint)fVar15;
              if (uVar4 != 0) goto LAB_02fc8718;
              goto LAB_02fc8788;
            }
            iVar13 = iVar13 + 1;
          } while ((bool)((byte)uVar14 & 1) == (*(float *)(*plVar12 + lVar18 * 4) == *pfVar1));
          bVar5 = true;
          uVar4 = uVar14 & 1;
          iVar6 = iVar13;
          uVar14 = (uint)fVar15;
          if (uVar4 == 0) {
LAB_02fc8788:
            if (iVar16 != iVar17) {
              fStack_40 = fVar15;
              memmove(*plStack_70 + (long)iVar16 * 4,*plStack_70 + lVar19 * 4,
                      (long)(iVar13 - iVar17) << 2);
              fVar15 = fStack_40;
            }
            iVar16 = iVar16 + (iVar13 - iVar17);
            plVar12 = plStack_70;
            iVar6 = iVar13;
            uVar14 = (uint)fVar15;
          }
LAB_02fc8718:
          iVar17 = iVar6;
        } while (bVar5);
        *(int *)(lStack_38 + 0x2e8) = iVar16;
        param_1 = lStack_38;
      }
      if ((int)uStack_50 + -1 == 0) {
        iVar7 = 1;
      }
      else {
        memmove(pfStack_58,pfStack_58 + 1,(long)((int)uStack_50 + -1) << 2);
        iVar7 = (int)uStack_50;
      }
      uStack_50 = CONCAT44(uStack_50._4_4_,iVar7 + -1);
      func_0x026b0070(&pfStack_58);
    }
    if (pfStack_58 != (float *)0x0) {
      func_0x03071ea0();
    }
  }
  return;
}



// ADDRESS 02fc1360

void selected_02fc1360(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  func_0x050ac080();
  func_0x05b9c350();
  lVar2 = func_0x050a7430(param_1);
  if (lVar2 != 0) {
    lVar3 = func_0x05b9c350();
    if (*(int *)(lVar3 + 0x38) <= *(int *)(*(long *)(lVar2 + 0x10) + 0x38)) {
      lVar4 = 0;
      if (*(long *)(*(long *)(*(long *)(lVar2 + 0x10) + 0x30) + (long)*(int *)(lVar3 + 0x38) * 8) ==
          lVar3 + 0x30) {
        lVar4 = lVar2;
      }
      goto LAB_02fc13ac;
    }
  }
  lVar4 = 0;
LAB_02fc13ac:
  if (lVar4 == 0) {
    *(undefined4 *)(param_1 + 0x220) = 0xffffffff;
    uVar1 = 0;
  }
  else {
    *(undefined4 *)(param_1 + 0x220) = *(undefined4 *)(lVar4 + 0xc);
    uVar1 = func_0x033afdb0(0x72cf780);
  }
  *(undefined4 *)(param_1 + 0x224) = uVar1;
  return;
}



// ADDRESS 02fc8d80

void selected_02fc8d80(long param_1,int param_2,char param_3,int param_4)

{
  long *plVar1;
  uint uVar2;
  int *piVar3;
  code *pcVar4;
  int iVar5;
  bool bVar6;
  bool bVar7;
  undefined4 uVar8;
  int iVar9;
  long lVar10;
  undefined4 *puVar11;
  uint uVar12;
  int *extraout_RDX;
  long lVar13;
  long lVar14;
  int iVar15;
  ulong uVar16;
  int iVar17;
  long lVar18;
  int iVar19;
  bool bVar20;
  int iStack_64;
  int **ppiStack_60;
  int **ppiStack_58;
  int *piStack_50;
  int iStack_48;
  int iStack_40;
  undefined4 uStack_3c;
  long lStack_38;
  
  iStack_64 = param_2;
  if (param_4 != 0) {
    plVar1 = (long *)(param_1 + 0x2d0);
    piStack_50 = (int *)((ulong)piStack_50 & 0xffffffff00000000);
    ppiStack_58 = (int **)plVar1;
    if (*(int *)(param_1 + 0x2d8) < 1) {
      ppiStack_60 = &piStack_50;
      func_0x06f8ce70(&ppiStack_60);
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    piVar3 = (int *)*plVar1;
    if (*piVar3 != param_2) {
      return;
    }
    piStack_50 = &iStack_64;
    if ((piVar3 <= piStack_50) && (piStack_50 < piVar3 + *(int *)(param_1 + 0x2dc))) {
      ppiStack_60 = &piStack_50;
      func_0x06f8e390(&ppiStack_60);
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    iVar9 = *(int *)(param_1 + 0x2d8);
    if ((long)iVar9 != 0) {
      iVar19 = 0;
      bVar20 = *(int *)*plVar1 == param_2;
      lStack_38 = param_1;
      iVar17 = 0;
      do {
        lVar18 = (long)iVar17;
        lVar10 = lVar18;
        iVar15 = iVar17;
        iVar5 = iVar17 + 1;
        if (iVar17 < iVar9) {
          iVar5 = iVar9;
        }
        do {
          lVar10 = lVar10 + 1;
          if (iVar9 <= lVar10) {
            bVar7 = false;
            bVar6 = false;
            iVar15 = iVar5;
            goto joined_r0x02fc8e76;
          }
          iVar15 = iVar15 + 1;
        } while (bVar20 == (*(int *)(*plVar1 + lVar10 * 4) == iStack_64));
        bVar7 = true;
        bVar6 = true;
joined_r0x02fc8e76:
        if (bVar20 == false) {
          if (iVar19 != iVar17) {
            iStack_40 = iVar19;
            memmove(*plVar1 + (long)iVar19 * 4,*plVar1 + lVar18 * 4,(long)(iVar15 - iVar17) << 2);
            iVar19 = iStack_40;
          }
          iVar19 = iVar19 + (iVar15 - iVar17);
          bVar6 = bVar7;
        }
        bVar20 = (bool)(bVar20 ^ 1);
        iVar17 = iVar15;
      } while (bVar6);
      *(int *)(lStack_38 + 0x2d8) = iVar19;
      param_1 = lStack_38;
    }
    lVar10 = func_0x02fb4e80();
    *(int *)(lVar10 + 0x2fc) = *(int *)(lVar10 + 0x2fc) + 1;
    if (*(char *)(param_1 + 0x234) == '\0') {
      if ((*(char *)(param_1 + 0x251) != '\0') &&
         (iStack_64 + (uint)*(byte *)(param_1 + 0x250) == *(int *)(param_1 + 0x27c))) {
        if (*(int *)(param_1 + 0x228) == 0) {
          return;
        }
        plVar1 = *(long **)(param_1 + 0x220);
        if (plVar1 != (long *)0x0) {
          (**(code **)(*plVar1 + 0x58))
                    (plVar1,*(undefined4 *)(param_1 + 0x230),1,*(byte *)(param_1 + 0x250) != 0);
          return;
        }
        return;
      }
      if ((*(int *)(param_1 + 0x2d8) < 1) || (iVar9 = *(int *)(param_1 + 0x248), iVar9 < 1)) {
        if (*(int *)(param_1 + 0x228) == 0) {
          return;
        }
        plVar1 = *(long **)(param_1 + 0x220);
        if (plVar1 != (long *)0x0) {
          (**(code **)(*plVar1 + 0x58))
                    (plVar1,*(undefined4 *)(param_1 + 0x230),4,*(undefined1 *)(param_1 + 0x250));
          return;
        }
        return;
      }
    }
    else if ((*(int *)(param_1 + 0x2d8) < 1) || (iVar9 = *(int *)(param_1 + 0x248), iVar9 < 1)) {
      uVar12 = *(uint *)(param_1 + 0x230) & 1;
      uVar2 = uVar12 ^ 3;
      if (*(char *)(param_1 + 0x250) != '\0') {
        uVar2 = uVar12 + 2;
      }
      if (*(int *)(param_1 + 0x228) != 0) {
        plVar1 = *(long **)(param_1 + 0x220);
        if (plVar1 != (long *)0x0) {
          (**(code **)(*plVar1 + 0x58))
                    (plVar1,*(uint *)(param_1 + 0x230),uVar2,*(char *)(param_1 + 0x250));
          return;
        }
        return;
      }
      return;
    }
    uVar2 = *(uint *)*plVar1;
    if ((int)uVar2 < 0) {
      return;
    }
    if ((int)uVar2 < iVar9) {
      func_0x02fc5630(*(undefined8 *)(*(long *)(param_1 + 0x240) + (ulong)uVar2 * 8),1);
      return;
    }
    return;
  }
  plVar1 = (long *)(param_1 + 0x280);
  if (param_3 != '\0') {
    uVar8 = func_0x027e1a20(plVar1);
    *extraout_RDX = param_2;
    extraout_RDX[1] = -1;
    func_0x027e1ac0(plVar1,param_2,extraout_RDX,uVar8,0);
    *(int *)(param_1 + 0x274) = iStack_64;
    return;
  }
  if (*(int *)(param_1 + 0x288) != *(int *)(param_1 + 0x2b4)) {
    lVar10 = param_1 + 0x2b8;
    if (*(long *)(param_1 + 0x2c0) != 0) {
      lVar10 = *(long *)(param_1 + 0x2c0);
    }
    uVar16 = (long)*(int *)(param_1 + 0x2c8) - 1;
    iVar9 = *(int *)(lVar10 + ((long)param_2 & uVar16) * 4);
    if (iVar9 != -1) {
      lVar18 = *plVar1;
      do {
        lVar14 = (long)iVar9;
        if (*(int *)(lVar18 + lVar14 * 0xc) == param_2) {
          lVar13 = (long)(int)((uint)uVar16 & *(uint *)(lVar18 + 8 + lVar14 * 0xc));
          iVar19 = *(int *)(lVar10 + lVar13 * 4);
          if (iVar19 == -1) goto LAB_02fc9121;
          if (iVar19 != iVar9) goto LAB_02fc9100;
          puVar11 = (undefined4 *)(lVar10 + lVar13 * 4);
          goto LAB_02fc911b;
        }
        iVar9 = *(int *)(lVar18 + 4 + lVar14 * 0xc);
      } while (iVar9 != -1);
    }
    if (*(int *)(param_1 + 0x2b4) < *(int *)(param_1 + 0x288)) goto LAB_02fc8f71;
  }
  goto LAB_02fc9144;
  while (iVar19 != iVar9) {
LAB_02fc9100:
    lVar10 = (long)iVar19;
    iVar19 = *(int *)(lVar18 + 4 + lVar10 * 0xc);
    if (iVar19 == -1) goto LAB_02fc9121;
  }
  puVar11 = (undefined4 *)(lVar18 + lVar10 * 0xc + 4);
LAB_02fc911b:
  *puVar11 = *(undefined4 *)(lVar18 + 4 + lVar14 * 0xc);
LAB_02fc9121:
  func_0x027e1c90(plVar1,iVar9,1);
  if (*(int *)(param_1 + 0x2b4) < *(int *)(param_1 + 0x288)) {
LAB_02fc8f71:
    func_0x028bac20(&piStack_50,plVar1);
    uStack_3c = 0;
    if (iStack_48 < 1) {
      ppiStack_60 = (int **)&uStack_3c;
      ppiStack_58 = &piStack_50;
      func_0x06f8ce70(&ppiStack_60);
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    *(int *)(param_1 + 0x274) = *piStack_50;
    func_0x03071ea0();
    return;
  }
LAB_02fc9144:
  *(undefined4 *)(param_1 + 0x274) = 0xffffffff;
  return;
}



