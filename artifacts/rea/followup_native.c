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



// ADDRESS 02fad000

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fad000(long param_1)

{
  long lVar1;
  float fVar2;
  char cVar3;
  undefined8 uVar4;
  double dVar5;
  double dVar6;
  undefined8 uStack_c8;
  float fStack_c0;
  undefined *puStack_b8;
  undefined8 uStack_b0;
  undefined *puStack_a0;
  undefined *puStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  ulong uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  func_0x050ac080();
  lVar1 = *(long *)(param_1 + 0x130);
  uVar4 = _DAT_071fa860;
  fVar2 = _DAT_071fa868;
  if (lVar1 != 0) {
    uVar4 = *(undefined8 *)(lVar1 + 0x1d0);
    fVar2 = *(float *)(lVar1 + 0x1d8);
  }
  uStack_90 = 0;
  uStack_28 = 0;
  uStack_20 = 0;
  uStack_38 = 0;
  uStack_30 = 0;
  uStack_48 = 0;
  uStack_40 = 0;
  uStack_58 = 0;
  uStack_50 = 0;
  uStack_68 = 0;
  uStack_60 = 0;
  uStack_78 = 0;
  uStack_70 = 0;
  uStack_88 = 0;
  uStack_80 = 0;
  uStack_18 = 0;
  puStack_98 = (undefined *)0x3f80000000000000;
  puStack_b8 = (undefined *)0x0;
  uStack_b0 = 0;
  cVar3 = func_0x054ce020(uVar4,fVar2,(int)uVar4,_DAT_006c0fd8 + fVar2,_DAT_00ce6de0,_DAT_00ce6de8,
                          _DAT_00ce6df0,_DAT_00ce6df8,param_1,0,0,&puStack_b8,0,&puStack_98,1,
                          0x40a00000);
  if (puStack_b8 != (undefined *)0x0) {
    func_0x03071ea0();
  }
  if ((cVar3 != '\0') && ((uStack_40 & 0x10000000000) != 0)) {
    uStack_c8 = uStack_80;
    uVar4 = uStack_c8;
    fStack_c0 = (float)uStack_78 + _DAT_006c1458;
    if (2 < DAT_0722748c) {
      uStack_c8._0_4_ = (float)uStack_80;
      uStack_c8._4_4_ = (float)((ulong)uStack_80 >> 0x20);
      dVar5 = (double)(float)uStack_c8;
      dVar6 = (double)uStack_c8._4_4_;
      uStack_c8 = uVar4;
      func_0x03006490(dVar5,dVar6,SUB84((double)fStack_c0,0),&puStack_b8,&UNK_00916d74);
      puStack_a0 = &UNK_009dcf72;
      if ((int)uStack_b0 != 0) {
        puStack_a0 = puStack_b8;
      }
      func_0x02fad200(&puStack_a0);
      if (puStack_b8 != (undefined *)0x0) {
        func_0x03071ea0();
      }
    }
    func_0x050ac610(param_1,&uStack_c8,0,0,0);
  }
  return;
}



// ADDRESS 026eda60

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_026eda60(long param_1)

{
  long lVar1;
  undefined8 uVar2;
  int iVar3;
  undefined8 uStack_50;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined2 uStack_40;
  undefined2 uStack_3e;
  undefined2 uStack_3c;
  undefined2 uStack_3a;
  undefined2 uStack_38;
  undefined4 uStack_36;
  undefined4 uStack_32;
  undefined4 uStack_2e;
  
  func_0x050ac080();
  lVar1 = *(long *)(param_1 + 0x220);
  if (DAT_0715c450 == '\0') {
    iVar3 = func_0x070a9210(&DAT_0715c450);
    if (iVar3 != 0) {
      uStack_38 = (undefined2)_UNK_00bc0780;
      uStack_36 = (undefined4)((uint6)_UNK_00bc0780 >> 0x10);
      uStack_32 = (undefined4)_UNK_00bc0786;
      uStack_2e = (undefined4)((ulong)_UNK_00bc0786 >> 0x20);
      uStack_48 = _DAT_00bc0770;
      uStack_44 = _UNK_00bc0774;
      uStack_40 = (undefined2)_UNK_00bc0778;
      uStack_3e = (undefined2)((uint)_UNK_00bc0778 >> 0x10);
      uStack_3c = _UNK_00bc077c;
      uStack_3a = _DAT_00bc077e;
      func_0x031d5d00(&uStack_50,&uStack_48,1);
      _DAT_0715c448 = uStack_50;
      func_0x070a9230(&DAT_0715c450);
    }
  }
  uVar2 = _DAT_0715c448;
  func_0x033e4af0(&uStack_48,0);
  uStack_40 = 0;
  uStack_3e = 0;
  uStack_3c = 0;
  uStack_3a = 0;
  func_0x026fc610(&uStack_48,param_1,&UNK_026edc20,0,uVar2);
  func_0x026fc6a0(lVar1 + 0x419,CONCAT44(uStack_44,uStack_48),
                  CONCAT26(uStack_3a,CONCAT24(uStack_3c,CONCAT22(uStack_3e,uStack_40))));
  lVar1 = *(long *)(param_1 + 0x220);
  if (DAT_0715c478 == '\0') {
    iVar3 = func_0x070a9210(&DAT_0715c478);
    if (iVar3 != 0) {
      uStack_38 = _UNK_00bc0600;
      uStack_36 = (undefined4)_UNK_00bc0602;
      uStack_32 = (undefined4)((ulong)_UNK_00bc0602 >> 0x20);
      uStack_48 = _DAT_00bc05f0;
      uStack_44 = _UNK_00bc05f4;
      uStack_40 = _UNK_00bc05f8;
      uStack_3e = _DAT_00bc05fa;
      uStack_3c = (undefined2)_UNK_00bc05fc;
      uStack_3a = (undefined2)((uint)_UNK_00bc05fc >> 0x10);
      func_0x031d5d00(&uStack_50,&uStack_48,1);
      _DAT_0715c470 = uStack_50;
      func_0x070a9230(&DAT_0715c478);
    }
  }
  uVar2 = _DAT_0715c470;
  func_0x033e4af0(&uStack_48,0);
  uStack_40 = 0;
  uStack_3e = 0;
  uStack_3c = 0;
  uStack_3a = 0;
  func_0x026fc740(&uStack_48,param_1,&UNK_026edf00,0,uVar2);
  func_0x026fc7d0(lVar1 + 0x41a,CONCAT44(uStack_44,uStack_48),
                  CONCAT26(uStack_3a,CONCAT24(uStack_3c,CONCAT22(uStack_3e,uStack_40))));
  return;
}



// ADDRESS 026ee1e0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_026ee1e0(long param_1)

{
  long lVar1;
  
  func_0x0509aaf0();
  if (_DAT_07142fa0 == 1) {
    lVar1 = *(long *)(_DAT_0715c440 + 0x18);
    *(undefined4 *)(lVar1 + 0x14) = *(undefined4 *)(param_1 + 0x230);
    *(undefined8 *)(lVar1 + 0xc) = *(undefined8 *)(param_1 + 0x228);
  }
  return;
}



// ADDRESS 026ee220

/* WARNING: Possible PIC construction at 0x0263f7e2: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x0263f7e7) */
/* WARNING: Removing unreachable block (ram,0x02fd4660) */

void selected_026ee220(undefined8 *param_1)

{
  undefined8 *puVar1;
  long *plVar2;
  undefined8 in_RAX;
  undefined8 *unaff_RBX;
  undefined8 *puVar3;
  undefined8 *puVar4;
  undefined8 *unaff_R14;
  
  *param_1 = 0xbbedd8;
  plVar2 = (long *)param_1[1];
  if (plVar2 != (long *)0x0) {
    in_RAX = (**(code **)(*plVar2 + 0x10))(plVar2,1);
    if ((long *)param_1[1] != (long *)0x0) {
      in_RAX = (**(code **)(*(long *)param_1[1] + 0x28))();
    }
  }
  puVar4 = (undefined8 *)param_1[3];
  while (puVar4 != (undefined8 *)0x0) {
    *(undefined8 **)((long)register0x00000020 + -8) = unaff_R14;
    *(undefined8 **)((long)register0x00000020 + -0x10) = unaff_RBX;
    *(undefined8 *)((long)register0x00000020 + -0x18) = in_RAX;
    puVar1 = (undefined8 *)*puVar4;
    puVar3 = (undefined8 *)((long)register0x00000020 + -0x20);
    register0x00000020 = (BADSPACEBASE *)((long)register0x00000020 + -0x20);
    *puVar3 = 0x263f7e7;
    unaff_RBX = puVar4;
    puVar4 = puVar1;
    unaff_R14 = param_1 + 2;
  }
  return;
}



// ADDRESS 026ee290

undefined8 selected_026ee290(undefined8 *param_1)

{
  long *plVar1;
  undefined8 uVar2;
  
  *param_1 = 0xbbedd8;
  plVar1 = (long *)param_1[1];
  if (plVar1 != (long *)0x0) {
    (**(code **)(*plVar1 + 0x10))(plVar1,1);
    if ((long *)param_1[1] != (long *)0x0) {
      (**(code **)(*(long *)param_1[1] + 0x28))();
    }
  }
  uVar2 = func_0x0263f7d0(param_1 + 2,param_1[3]);
  func_0x03071ea0(param_1);
  return uVar2;
}



// ADDRESS 026ee300

undefined1 selected_026ee300(void)

{
  func_0x070a97a0(&UNK_026f47a0);
  func_0x070a97b0(0x28);
  return 1;
}



// ADDRESS 026ee320

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_026ee320(long param_1)

{
  ulong uVar1;
  code *pcVar2;
  int iVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  ulong *puVar6;
  undefined8 *puVar7;
  long lVar8;
  undefined8 *puVar9;
  undefined8 uVar10;
  ulong uVar11;
  undefined8 *puVar12;
  ulong uVar13;
  ulong *puVar14;
  ulong uVar15;
  long lVar16;
  ulong uVar17;
  ulong *puStack_98;
  ulong *puStack_90;
  ulong *puStack_88;
  long lStack_70;
  ulong *puStack_68;
  ulong uStack_60;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined8 uStack_48;
  undefined8 uStack_40;
  
  puVar9 = (undefined8 *)(param_1 + 0x18);
  uVar15 = 0;
  lStack_70 = param_1;
  do {
    while( true ) {
      uVar1 = *(ulong *)(param_1 + 0x28);
      uVar13 = *(ulong *)(param_1 + 0x30);
      puVar12 = puVar9;
      if (uVar13 != uVar1) break;
      if (uVar1 == 0) goto LAB_026ee4f0;
      puVar4 = (undefined8 *)*puVar9;
LAB_026ee373:
      uVar13 = uVar1;
      uVar11 = uVar1 + 1;
LAB_026ee377:
      *(ulong *)(param_1 + 0x28) = uVar11;
      uVar17 = uVar13;
joined_r0x026ee382:
      while (puVar5 = puVar12, puVar4 != (undefined8 *)0x0) {
        while (puVar12 = puVar4, (ulong)puVar12[4] <= uVar17) {
          if (uVar17 <= (ulong)puVar12[4]) goto LAB_026ee491;
          puVar5 = puVar12 + 1;
          puVar4 = (undefined8 *)puVar12[1];
          if ((undefined8 *)puVar12[1] == (undefined8 *)0x0) goto LAB_026ee491;
        }
        puVar4 = (undefined8 *)*puVar12;
      }
LAB_026ee491:
      puVar4 = (undefined8 *)*puVar5;
      if (puVar4 == (undefined8 *)0x0) {
        puVar4 = (undefined8 *)_Znwm(0x30);
        puVar4[4] = uVar17;
        puVar4[5] = 0;
        *puVar4 = 0;
        puVar4[1] = 0;
        puVar4[2] = puVar12;
        *puVar5 = puVar4;
        puVar12 = puVar4;
        if (**(long **)(param_1 + 0x10) != 0) {
          *(long *)(param_1 + 0x10) = **(long **)(param_1 + 0x10);
          puVar12 = (undefined8 *)*puVar5;
        }
        func_0x0263f620(*(undefined8 *)(param_1 + 0x18),puVar12);
        *(long *)(param_1 + 0x20) = *(long *)(param_1 + 0x20) + 1;
      }
      if (uVar15 < uVar17) {
        uVar15 = uVar17;
      }
      puVar14 = (ulong *)puVar4[5];
      if (((*(char *)(param_1 + 0x78) != '\0') || (*(char *)(param_1 + 0x79) != '\0')) &&
         (*(long *)(param_1 + 0x68) != 0)) {
        iVar3 = func_0x070a97c0();
        if (iVar3 < 0) {
          if (1 < DAT_0722748c) {
            func_0x026eea20();
          }
        }
        else {
          uStack_48 = _DAT_006c3bd0;
          uStack_40 = _UNK_006c3bd8;
          uStack_58 = _DAT_00bbee70;
          uStack_54 = _UNK_00bbee74;
          uStack_50 = _UNK_00bbee78;
          uStack_4c = _UNK_00bbee7c;
          puStack_98 = puVar14;
          func_0x070a97d0(*(undefined8 *)(param_1 + 0x38),&puStack_98,&uStack_48,0,0x2d0,
                          *(undefined8 *)(param_1 + 0x50),&uStack_58);
          lVar8 = *(long *)(param_1 + 0x48);
          lVar16 = *(long *)(param_1 + 0x50);
          uVar1 = (long)*(int *)(lVar8 + 0x68) / (long)*(int *)(lVar8 + 100);
          iVar3 = *(int *)(lVar8 + 0x368);
          uVar1 = (ulong)(uint)((int)uVar1 >> 0x1f) << 0x20 | uVar1 & 0xffffffff;
          lVar8 = (long)(int)((long)uVar1 / (long)iVar3) * (long)*(int *)(lVar8 + 0x36c);
          *(long *)(lVar16 + 0x1a8) = lVar8;
          *(long *)(lVar16 + 0x88) = *(int *)(param_1 + 0x60) * lVar8;
          func_0x026eea50(param_1,lVar16,(long)uVar1 % (long)iVar3 & 0xffffffff);
        }
      }
    }
    puVar4 = (undefined8 *)*puVar9;
    for (puVar5 = puVar4; puVar5 != (undefined8 *)0x0; puVar5 = (undefined8 *)*puVar5) {
      if ((ulong)puVar5[4] <= uVar1) {
        uVar17 = uVar1;
        if (uVar1 <= (ulong)puVar5[4]) goto joined_r0x026ee4ea;
        puVar5 = puVar5 + 1;
      }
    }
    if ((uVar1 <= uVar13) && (uVar17 = uVar13, 2 < uVar13 - uVar1)) {
joined_r0x026ee4ea:
      puVar5 = puVar4;
      if (uVar17 != 0) {
        for (; puVar5 != (undefined8 *)0x0; puVar5 = (undefined8 *)*puVar5) {
          if ((ulong)puVar5[4] <= uVar1) {
            if (uVar1 <= (ulong)puVar5[4]) goto LAB_026ee373;
            puVar5 = puVar5 + 1;
          }
        }
        if (uVar13 < uVar1) {
          uVar17 = 0;
        }
        else {
          uVar17 = 0;
          uVar11 = uVar13;
          if (2 < uVar13 - uVar1) goto LAB_026ee377;
        }
        goto joined_r0x026ee382;
      }
    }
LAB_026ee4f0:
    puStack_98 = (ulong *)0x0;
    puStack_90 = (ulong *)0x0;
    puStack_88 = (ulong *)0x0;
    puVar12 = *(undefined8 **)(param_1 + 0x10);
    uStack_60 = uVar15;
    if (*(undefined8 **)(param_1 + 0x10) != puVar9) {
      do {
        uVar1 = puVar12[4];
        if (uVar1 <= uVar15) {
          if (puStack_90 == puStack_88) {
            lVar8 = (long)puStack_90 - (long)puStack_98;
            lVar16 = lVar8 >> 3;
            uVar15 = lVar16 + 1;
            if (0x1fffffffffffffff < uVar15) {
              _ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv(&puStack_98);
LAB_026ee9b0:
              puVar9 = (undefined8 *)__cxa_allocate_exception(0x10);
              _ZNSt11logic_errorC2EPKc(puVar9,&UNK_00a61aa6);
              *puVar9 = 0x7141d10;
              uVar10 = __cxa_throw(puVar9,_ZTISt12length_error,_ZNSt12length_errorD1Ev);
              __cxa_free_exception(puVar9);
              if (puStack_98 != (ulong *)0x0) {
                puStack_90 = puStack_98;
                _ZdlPv();
              }
              _Unwind_Resume(uVar10);
              pcVar2 = (code *)swi(3);
              (*pcVar2)();
              return;
            }
            uVar13 = (long)puStack_88 - (long)puStack_98 >> 2;
            if (uVar13 < uVar15) {
              uVar13 = uVar15;
            }
            if (0xffffffffffffffe < (ulong)((long)puStack_88 - (long)puStack_98 >> 3)) {
              uVar13 = 0x1fffffffffffffff;
            }
            if (uVar13 == 0) {
              puVar6 = (ulong *)0x0;
              puVar14 = puStack_98;
            }
            else {
              if (0x1fffffffffffffff < uVar13) goto LAB_026ee9b0;
              puStack_68 = puStack_98;
              puVar6 = (ulong *)_Znwm(uVar13 * 8);
              puVar14 = puStack_68;
            }
            puVar6[lVar16] = uVar1;
            if (0 < lVar8) {
              memcpy(puVar6,puVar14,lVar8);
            }
            uVar15 = uStack_60;
            puStack_98 = puVar6;
            puStack_90 = puVar6 + lVar16 + 1;
            puStack_88 = puVar6 + uVar13;
            if (puVar14 != (ulong *)0x0) {
              _ZdlPv(puVar14);
            }
          }
          else {
            *puStack_90 = uVar1;
            puStack_90 = puStack_90 + 1;
          }
        }
        puVar14 = puStack_90;
        puVar4 = (undefined8 *)puVar12[1];
        if ((undefined8 *)puVar12[1] == (undefined8 *)0x0) {
          puVar5 = (undefined8 *)puVar12[2];
          if ((undefined8 *)*puVar5 != puVar12) {
            do {
              puVar12 = (undefined8 *)puVar12[2];
              puVar5 = (undefined8 *)puVar12[2];
            } while ((undefined8 *)*puVar5 != puVar12);
          }
        }
        else {
          do {
            puVar5 = puVar4;
            puVar4 = (undefined8 *)*puVar5;
          } while ((undefined8 *)*puVar5 != (undefined8 *)0x0);
        }
        puVar12 = puVar5;
        puVar6 = puStack_98;
      } while (puVar5 != puVar9);
      for (; puVar6 != puVar14; puVar6 = puVar6 + 1) {
        uVar15 = *puVar6;
        puVar4 = (undefined8 *)*puVar9;
        puVar12 = puVar9;
        while (puVar5 = puVar12, puVar4 != (undefined8 *)0x0) {
          while (puVar5 = puVar4, (ulong)puVar5[4] <= uVar15) {
            if (uVar15 <= (ulong)puVar5[4]) goto LAB_026ee753;
            puVar12 = puVar5 + 1;
            puVar4 = (undefined8 *)puVar5[1];
            if ((undefined8 *)puVar5[1] == (undefined8 *)0x0) goto LAB_026ee753;
          }
          puVar12 = puVar5;
          puVar4 = (undefined8 *)*puVar5;
        }
LAB_026ee753:
        puVar4 = (undefined8 *)*puVar12;
        if (puVar4 == (undefined8 *)0x0) {
          puVar4 = (undefined8 *)_Znwm(0x30);
          lVar8 = lStack_70;
          puVar4[4] = uVar15;
          puVar4[5] = 0;
          *puVar4 = 0;
          puVar4[1] = 0;
          puVar4[2] = puVar5;
          *puVar12 = puVar4;
          puVar5 = puVar4;
          if (**(long **)(lStack_70 + 0x10) != 0) {
            *(long *)(lStack_70 + 0x10) = **(long **)(lStack_70 + 0x10);
            puVar5 = (undefined8 *)*puVar12;
          }
          func_0x0263f620(*(undefined8 *)(lStack_70 + 0x18),puVar5);
          *(long *)(lVar8 + 0x20) = *(long *)(lVar8 + 0x20) + 1;
        }
        if (puVar4[5] != 0) {
          _ZdlPv();
        }
        puVar12 = (undefined8 *)*puVar9;
        puVar4 = puVar12;
        puVar5 = puVar9;
        if (puVar12 != (undefined8 *)0x0) {
          do {
            if ((ulong)puVar4[4] >= uVar15) {
              puVar5 = puVar4;
            }
            puVar7 = puVar4 + ((ulong)puVar4[4] < uVar15);
            puVar4 = (undefined8 *)*puVar7;
          } while ((undefined8 *)*puVar7 != (undefined8 *)0x0);
          if ((puVar5 != puVar9) && ((ulong)puVar5[4] <= uVar15)) {
            puVar4 = (undefined8 *)puVar5[1];
            if ((undefined8 *)puVar5[1] == (undefined8 *)0x0) {
              puVar7 = (undefined8 *)puVar5[2];
              puVar4 = puVar5;
              if ((undefined8 *)*puVar7 != puVar5) {
                do {
                  puVar4 = (undefined8 *)puVar4[2];
                  puVar7 = (undefined8 *)puVar4[2];
                } while ((undefined8 *)*puVar7 != puVar4);
              }
            }
            else {
              do {
                puVar7 = puVar4;
                puVar4 = (undefined8 *)*puVar7;
              } while ((undefined8 *)*puVar7 != (undefined8 *)0x0);
            }
            if (*(undefined8 **)(lStack_70 + 0x10) == puVar5) {
              *(undefined8 **)(lStack_70 + 0x10) = puVar7;
            }
            *(long *)(lStack_70 + 0x20) = *(long *)(lStack_70 + 0x20) + -1;
            func_0x0264d6e0(puVar12,puVar5);
            _ZdlPv(puVar5);
          }
        }
      }
    }
    param_1 = lStack_70;
    if (*(char *)(lStack_70 + 0x78) == '\0') {
      *(undefined1 *)(lStack_70 + 0x90) = 1;
    }
    else {
      lVar8 = func_0x070a94b0(0);
      if (*(long *)(param_1 + 0x88) == -1) {
        if (5 < lVar8 - *(long *)(param_1 + 0x80)) goto LAB_026ee881;
      }
      else if (2 < lVar8 - *(long *)(param_1 + 0x88)) {
LAB_026ee881:
        *(undefined1 *)(param_1 + 0x90) = 0;
      }
    }
    if (puStack_98 != (ulong *)0x0) {
      puStack_90 = puStack_98;
      _ZdlPv();
    }
    uVar15 = 0;
  } while( true );
}



// ADDRESS 02faf610

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02faf610(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  undefined *puVar7;
  long lVar8;
  undefined8 uStack_b8;
  undefined2 uStack_b0;
  undefined2 uStack_ae;
  undefined2 uStack_ac;
  undefined2 uStack_aa;
  undefined2 uStack_a8;
  undefined4 uStack_a6;
  undefined4 uStack_a2;
  undefined4 uStack_9e;
  undefined8 uStack_98;
  long lStack_90;
  long lStack_88;
  undefined8 uStack_80;
  undefined *puStack_70;
  long lStack_68;
  undefined *puStack_60;
  undefined8 uStack_58;
  long lStack_50;
  long *plStack_48;
  
  func_0x050ac080();
  uVar4 = func_0x02fcc220();
  lStack_68 = 0xcd4128;
  puStack_60 = &UNK_02fafb70;
  uStack_58 = 0;
  lStack_50 = param_1;
  plStack_48 = &lStack_68;
  func_0x02fcc270(uVar4,0x11,&lStack_68);
  if (&lStack_68 == plStack_48) {
    (**(code **)(*plStack_48 + 0x20))();
  }
  else if (plStack_48 != (long *)0x0) {
    (**(code **)(*plStack_48 + 0x28))();
  }
  if (4 < DAT_0722748c) {
    func_0x02fafbc0();
  }
  lStack_88 = 0;
  uStack_80 = 0;
  lVar5 = func_0x05b9c350();
  if (((lVar5 == 0) || (lVar6 = func_0x059d5e80(), lVar6 == 0)) ||
     (*(int *)(lVar5 + 0x38) < *(int *)(lVar6 + 0x38))) {
    lVar8 = 0;
  }
  else {
    lVar8 = 0;
    if (*(long *)(*(long *)(lVar5 + 0x30) + (long)*(int *)(lVar6 + 0x38) * 8) == lVar6 + 0x30) {
      lVar8 = lVar5;
    }
  }
  lVar6 = func_0x059d5e80();
  if (lVar8 == lVar6) {
    func_0x026a2240(param_1,lVar5,&lStack_88);
  }
  else {
    func_0x026a2710(param_1,lVar5,&lStack_88);
  }
  lVar5 = lStack_88;
  lVar6 = (long)(int)uStack_80;
  lStack_90 = param_1;
  if (lVar6 != 0) {
    lVar8 = 0;
    do {
      lVar2 = *(long *)(lVar5 + lVar8);
      if (lVar2 != 0) {
        uStack_98 = *(undefined8 *)(lVar2 + 0x18);
        func_0x031d6e10(&uStack_b8,&uStack_98);
        puVar7 = &UNK_009dcf72;
        if (CONCAT22(uStack_ae,uStack_b0) != 0) {
          puVar7 = uStack_b8;
        }
        iVar3 = func_0x030318e0(puVar7,&UNK_00a7ea8a);
        if (uStack_b8 != (undefined *)0x0) {
          func_0x03071ea0();
        }
        if (iVar3 == 0) {
          if (4 < DAT_0722748c) {
            func_0x02fafbf0();
          }
          lVar5 = lStack_90;
          *(long *)(lStack_90 + 0x220) = lVar2;
          if ((DAT_0719cb88 == '\0') && (iVar3 = func_0x070a9210(&DAT_0719cb88), iVar3 != 0)) {
            uStack_b0 = (undefined2)_DAT_00728460;
            uStack_ae = (undefined2)((uint)_DAT_00728460 >> 0x10);
            uStack_b8 = _DAT_00728458;
            func_0x031d5d00(&uStack_98,&uStack_b8,1);
            _DAT_0719cb80 = uStack_98;
            func_0x070a9230(&DAT_0719cb88);
            lVar5 = lStack_90;
          }
          uVar4 = _DAT_0719cb80;
          func_0x033e4af0(&uStack_b8,0);
          uStack_b0 = 0;
          uStack_ae = 0;
          uStack_ac = 0;
          uStack_aa = 0;
          func_0x02fb03f0(&uStack_b8,lVar5,&UNK_02fafc20,0,uVar4);
          func_0x02f9fae0(lVar2 + 0x418,uStack_b8,
                          CONCAT26(uStack_aa,CONCAT24(uStack_ac,CONCAT22(uStack_ae,uStack_b0))));
          lVar6 = *(long *)(lVar5 + 0x220);
          if ((DAT_0715c450 == '\0') && (iVar3 = func_0x070a9210(&DAT_0715c450), iVar3 != 0)) {
            uStack_a8 = (undefined2)_UNK_00bc0780;
            uStack_a6 = (undefined4)((uint6)_UNK_00bc0780 >> 0x10);
            uStack_a2 = (undefined4)_UNK_00bc0786;
            uStack_9e = (undefined4)((ulong)_UNK_00bc0786 >> 0x20);
            uStack_b8 = (undefined *)CONCAT44(_UNK_00bc0774,_DAT_00bc0770);
            uStack_b0 = (undefined2)_UNK_00bc0778;
            uStack_ae = (undefined2)((uint)_UNK_00bc0778 >> 0x10);
            uStack_ac = _UNK_00bc077c;
            uStack_aa = _DAT_00bc077e;
            func_0x031d5d00(&uStack_98,&uStack_b8,1);
            _DAT_0715c448 = uStack_98;
            func_0x070a9230(&DAT_0715c450);
            lVar5 = lStack_90;
          }
          uVar4 = _DAT_0715c448;
          func_0x033e4af0(&uStack_b8,0);
          uStack_b0 = 0;
          uStack_ae = 0;
          uStack_ac = 0;
          uStack_aa = 0;
          func_0x02fb0480(&uStack_b8,lVar5,0x641,0,uVar4);
          func_0x026fc6a0(lVar6 + 0x419,uStack_b8,
                          CONCAT26(uStack_aa,CONCAT24(uStack_ac,CONCAT22(uStack_ae,uStack_b0))));
          lVar6 = *(long *)(lVar5 + 0x220);
          if ((DAT_0715c478 == '\0') && (iVar3 = func_0x070a9210(&DAT_0715c478), iVar3 != 0)) {
            uStack_a8 = _UNK_00bc0600;
            uStack_a6 = (undefined4)_UNK_00bc0602;
            uStack_a2 = (undefined4)((ulong)_UNK_00bc0602 >> 0x20);
            uStack_b8 = (undefined *)CONCAT44(_UNK_00bc05f4,_DAT_00bc05f0);
            uStack_b0 = _UNK_00bc05f8;
            uStack_ae = _DAT_00bc05fa;
            uStack_ac = (undefined2)_UNK_00bc05fc;
            uStack_aa = (undefined2)((uint)_UNK_00bc05fc >> 0x10);
            func_0x031d5d00(&uStack_98,&uStack_b8,1);
            _DAT_0715c470 = uStack_98;
            func_0x070a9230(&DAT_0715c478);
            lVar5 = lStack_90;
          }
          uVar4 = _DAT_0715c470;
          func_0x033e4af0(&uStack_b8,0);
          uStack_b0 = 0;
          uStack_ae = 0;
          uStack_ac = 0;
          uStack_aa = 0;
          func_0x02fb0510(&uStack_b8,lVar5,0x649,0,uVar4);
          func_0x026fc7d0(lVar6 + 0x41a,uStack_b8,
                          CONCAT26(uStack_aa,CONCAT24(uStack_ac,CONCAT22(uStack_ae,uStack_b0))));
          break;
        }
      }
      if (4 < DAT_0722748c) {
        uStack_98 = *(undefined8 *)(lVar2 + 0x18);
        func_0x031d6e10(&uStack_b8,&uStack_98);
        puStack_70 = &UNK_009dcf72;
        if (CONCAT22(uStack_ae,uStack_b0) != 0) {
          puStack_70 = uStack_b8;
        }
        func_0x02fafdf0(&puStack_70);
        if (uStack_b8 != (undefined *)0x0) {
          func_0x03071ea0();
        }
      }
      lVar8 = lVar8 + 8;
    } while (lVar6 * 8 != lVar8);
  }
  lVar6 = lStack_90;
  func_0x050ac6c0(_DAT_006cf520,_DAT_006c0b68,lStack_90,0);
  lVar5 = *(long *)(lVar6 + 0x130);
  *(long *)(lVar6 + 0x228) = lVar5;
  uVar1 = *(undefined4 *)(lVar5 + 0x130);
  *(undefined8 *)(lVar6 + 0x238) = *(undefined8 *)(lVar5 + 0x128);
  *(undefined4 *)(lVar6 + 0x240) = uVar1;
  if (lStack_88 != 0) {
    func_0x03071ea0();
  }
  return;
}



// ADDRESS 02faff10

void selected_02faff10(undefined8 param_1)

{
  undefined8 uVar1;
  long lStack_48;
  undefined *puStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  long *plStack_28;
  
  if (4 < DAT_0722748c) {
    func_0x02faffc0();
  }
  uVar1 = func_0x02fcc220();
  lStack_48 = 0xcd4128;
  puStack_40 = &UNK_02fafb70;
  uStack_38 = 0;
  uStack_30 = param_1;
  plStack_28 = &lStack_48;
  func_0x02fcc410(uVar1,0x11,&lStack_48);
  if (&lStack_48 == plStack_28) {
    (**(code **)(*plStack_28 + 0x20))();
  }
  else if (plStack_28 != (long *)0x0) {
    (**(code **)(*plStack_28 + 0x28))();
  }
  return;
}



// ADDRESS 02fafe20

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fafe20(float param_1,long param_2)

{
  long lVar1;
  float fVar2;
  float fVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  float fVar9;
  float fVar11;
  float fVar12;
  undefined1 auVar10 [16];
  undefined1 auVar13 [16];
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar20;
  float fVar21;
  undefined1 auVar17 [16];
  undefined1 auVar18 [16];
  float fVar22;
  undefined1 auVar19 [16];
  undefined1 auVar23 [16];
  undefined1 auVar24 [16];
  undefined8 uStack_58;
  float fStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_20;
  
  uStack_20 = 0x2fafe33;
  uVar4 = func_0x0509aaf0();
  if (*(int *)(param_2 + 0x244) == 1) {
    *(float *)(param_2 + 0x238) =
         param_1 * *(float *)(param_2 + 0x230) + *(float *)(param_2 + 0x238);
    *(float *)(param_2 + 0x23c) = *(float *)(param_2 + 0x23c) + 0.0;
    fStack_50 = *(float *)(param_2 + 0x240) + 0.0;
    *(float *)(param_2 + 0x240) = fStack_50;
    uStack_58 = *(undefined8 *)(param_2 + 0x238);
    lVar1 = *(long *)(param_2 + 0x228);
    fVar9 = (float)uStack_58 - (float)*(undefined8 *)(lVar1 + 0x128);
    fVar11 = (float)((ulong)uStack_58 >> 0x20) -
             (float)((ulong)*(undefined8 *)(lVar1 + 0x128) >> 0x20);
    auVar17._4_4_ = fVar11;
    auVar17._0_4_ = fVar9;
    fVar12 = fStack_50 - *(float *)(lVar1 + 0x130);
    fVar2 = (float)DAT_07417a30;
    fVar3 = DAT_07417a30._12_4_;
    auVar17._8_4_ = fVar12;
    auVar17._12_4_ = 0;
    auVar18 = divps(auVar17,_DAT_07417a30);
    fVar14 = auVar18._0_4_;
    fVar20 = auVar18._4_4_;
    fVar21 = auVar18._8_4_;
    fVar22 = auVar18._12_4_;
    uVar5 = -(uint)(_DAT_07417a50 <= (float)(_DAT_07417940 & (uint)fVar14));
    uVar6 = -(uint)(fRam0000000007417a54 <= (float)(uRam0000000007417944 & (uint)fVar20));
    uVar7 = -(uint)(fRam0000000007417a58 <= (float)(uRam0000000007417948 & (uint)fVar21));
    uVar8 = -(uint)(fRam0000000007417a5c <= (float)(uRam000000000741794c & (uint)fVar22));
    fVar15 = DAT_07417a30._4_4_;
    fVar16 = DAT_07417a30._8_4_;
    auVar18._0_4_ =
         fVar9 - (float)(~uVar5 & (uint)(float)(int)fVar14 | (uint)fVar14 & uVar5) * fVar2;
    auVar18._4_4_ =
         fVar11 - (float)(~uVar6 & (uint)(float)(int)fVar20 | (uint)fVar20 & uVar6) * fVar15;
    auVar18._8_4_ =
         fVar12 - (float)(~uVar7 & (uint)(float)(int)fVar21 | (uint)fVar21 & uVar7) * fVar16;
    auVar18._12_4_ = 0.0 - (float)(~uVar8 & (uint)(float)(int)fVar22 | (uint)fVar22 & uVar8) * fVar3
    ;
    auVar19._0_4_ = (float)(_DAT_07417940 & (uint)fVar2);
    auVar19._4_4_ = (float)(uRam0000000007417944 & (uint)fVar15);
    auVar19._8_4_ = (float)(uRam0000000007417948 & (uint)fVar16);
    auVar19._12_4_ = (float)(uRam000000000741794c & (uint)fVar3);
    auVar23._0_4_ = 0.0 - auVar19._0_4_;
    auVar23._4_4_ = 0.0 - auVar19._4_4_;
    auVar23._8_4_ = 0.0 - auVar19._8_4_;
    auVar23._12_4_ = 0.0 - auVar19._12_4_;
    auVar18 = minps(auVar18,auVar19);
    auVar18 = maxps(auVar23,auVar18);
    fVar9 = auVar18._0_4_;
    fVar11 = auVar18._4_4_;
    fVar12 = auVar18._8_4_;
    fVar14 = auVar18._12_4_;
    auVar24._0_4_ = (uint)fVar9 & -(uint)(0.0 <= fVar9);
    auVar24._4_4_ = (uint)fVar11 & -(uint)(0.0 <= fVar11);
    auVar24._8_4_ = (uint)fVar12 & -(uint)(0.0 <= fVar12);
    auVar24._12_4_ = (uint)fVar14 & -(uint)(0.0 <= fVar14);
    auVar13._0_4_ = ~-(uint)(0.0 <= fVar9) & (uint)(fVar2 + fVar9);
    auVar13._4_4_ = ~-(uint)(0.0 <= fVar11) & (uint)(fVar15 + fVar11);
    auVar13._8_4_ = ~-(uint)(0.0 <= fVar12) & (uint)(fVar16 + fVar12);
    auVar13._12_4_ = ~-(uint)(0.0 <= fVar14) & (uint)(fVar3 + fVar14);
    auVar13 = auVar13 | auVar24;
    fVar9 = auVar13._0_4_;
    fVar11 = auVar13._4_4_;
    fVar12 = auVar13._8_4_;
    fVar14 = auVar13._12_4_;
    auVar10._4_4_ =
         -(uint)(_UNK_006d45b4 <
                (float)((~-(uint)(fRam0000000007417a44 < fVar11) & (uint)fVar11 |
                        (uint)(fVar11 - fVar15) & -(uint)(fRam0000000007417a44 < fVar11)) &
                       uRam0000000007417944));
    auVar10._0_4_ =
         -(uint)(_DAT_006d45b0 <
                (float)((~-(uint)(_DAT_07417a40 < fVar9) & (uint)fVar9 |
                        (uint)(fVar9 - fVar2) & -(uint)(_DAT_07417a40 < fVar9)) & _DAT_07417940));
    auVar10._8_4_ =
         -(uint)(_UNK_006d45b8 <
                (float)((~-(uint)(fRam0000000007417a48 < fVar12) & (uint)fVar12 |
                        (uint)(fVar12 - fVar16) & -(uint)(fRam0000000007417a48 < fVar12)) &
                       uRam0000000007417948));
    auVar10._12_4_ =
         -(uint)(_UNK_006d45bc <
                (float)((~-(uint)(fRam0000000007417a4c < fVar14) & (uint)fVar14 |
                        (uint)(fVar14 - fVar3) & -(uint)(fRam0000000007417a4c < fVar14)) &
                       uRam000000000741794c));
    uVar4 = movmskps(uVar4,auVar10);
    if ((char)uVar4 != '\0') {
      uStack_48 = *(undefined8 *)(lVar1 + 0x11c);
      uStack_40 = 0;
      uVar5 = _DAT_07417940;
      uVar6 = uRam0000000007417944;
      uStack_38 = func_0x03104ed0(&uStack_58);
      uStack_30 = CONCAT44(uVar6,uVar5);
      func_0x052a5520(lVar1,&uStack_38,0,0,0);
    }
    return;
  }
  return;
}



// ADDRESS 02fafff0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fafff0(undefined8 param_1,undefined8 param_2,long param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined4 *puVar7;
  bool bVar8;
  undefined8 *puStack_58;
  undefined8 uStack_50;
  undefined4 *puStack_48;
  undefined8 uStack_40;
  undefined *puStack_30;
  int iStack_28;
  undefined *apuStack_20 [2];
  
  puStack_58 = *(undefined8 **)(param_3 + 0x18);
  func_0x031d6e10(&puStack_30,&puStack_58);
  puStack_58 = (undefined8 *)0x0;
  uStack_50 = 0;
  func_0x025ead80(&puStack_58,6);
  iVar4 = (int)uStack_50 + 6;
  uStack_50 = CONCAT44(uStack_50._4_4_,iVar4);
  if (uStack_50._4_4_ < iVar4) {
    func_0x025eade0(&puStack_58);
  }
  *puStack_58 = 0x6e006f00720064;
  *(undefined4 *)(puStack_58 + 1) = 0x65;
  puVar5 = (undefined8 *)&UNK_009dcf72;
  if ((int)uStack_50 != 0) {
    puVar5 = puStack_58;
  }
  iVar4 = func_0x03000d20(&puStack_30,puVar5,1,0,0xffffffff);
  bVar8 = true;
  if (iVar4 == -1) {
    puStack_48 = *(undefined4 **)(param_3 + 0x18);
    func_0x031d6e10(apuStack_20,&puStack_48);
    puStack_48 = (undefined4 *)0x0;
    uStack_40 = 0;
    func_0x025ead80(&puStack_48,10);
    iVar4 = (int)uStack_40 + 10;
    uStack_40 = CONCAT44(uStack_40._4_4_,iVar4);
    if (uStack_40._4_4_ < iVar4) {
      func_0x025eade0(&puStack_48);
    }
    uVar3 = _UNK_006c8bdc;
    uVar2 = _UNK_006c8bd8;
    uVar1 = _UNK_006c8bd4;
    *puStack_48 = _DAT_006c8bd0;
    puStack_48[1] = uVar1;
    puStack_48[2] = uVar2;
    puStack_48[3] = uVar3;
    puStack_48[4] = 0x74;
    puVar7 = (undefined4 *)&UNK_009dcf72;
    if ((int)uStack_40 != 0) {
      puVar7 = puStack_48;
    }
    iVar4 = func_0x03000d20(apuStack_20,puVar7,1,0,0xffffffff);
    bVar8 = iVar4 != -1;
    if (puStack_48 != (undefined4 *)0x0) {
      func_0x03071ea0();
    }
    if (apuStack_20[0] != (undefined *)0x0) {
      func_0x03071ea0();
    }
  }
  if (puStack_58 != (undefined8 *)0x0) {
    func_0x03071ea0();
  }
  if (puStack_30 != (undefined *)0x0) {
    func_0x03071ea0();
  }
  if (bVar8) {
    if (4 < DAT_0722748c) {
      puStack_58 = *(undefined8 **)(param_3 + 0x18);
      func_0x031d6e10(&puStack_30,&puStack_58);
      apuStack_20[0] = &UNK_009dcf72;
      if (iStack_28 != 0) {
        apuStack_20[0] = puStack_30;
      }
      func_0x02fb0340(apuStack_20);
      if (puStack_30 != (undefined *)0x0) {
        func_0x03071ea0();
      }
    }
    puVar5 = (undefined8 *)_Znwm(0x18);
    puVar5[1] = 0x10;
    *puVar5 = 0xcd4100;
    *(undefined4 *)(puVar5 + 2) = 0;
    uVar6 = func_0x02fcc220();
    func_0x02fcc590(uVar6,puVar5);
  }
  else if (4 < DAT_0722748c) {
    puStack_58 = *(undefined8 **)(param_3 + 0x18);
    func_0x031d6e10(&puStack_30,&puStack_58);
    apuStack_20[0] = &UNK_009dcf72;
    if (iStack_28 != 0) {
      apuStack_20[0] = puStack_30;
    }
    func_0x02fb0370(apuStack_20);
    if (puStack_30 != (undefined *)0x0) {
      func_0x03071ea0();
    }
  }
  return;
}



// ADDRESS 02fb03a0

void selected_02fb03a0(long param_1)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  
  puVar2 = (undefined8 *)_Znwm(0x20);
  *puVar2 = 0xcd4128;
  uVar1 = *(undefined8 *)(param_1 + 0x10);
  puVar2[1] = *(undefined8 *)(param_1 + 8);
  puVar2[2] = uVar1;
  puVar2[3] = *(undefined8 *)(param_1 + 0x18);
  return;
}



// ADDRESS 02fb03d0

void selected_02fb03d0(long param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  
  *param_2 = 0xcd4128;
  uVar1 = *(undefined8 *)(param_1 + 0x10);
  param_2[1] = *(undefined8 *)(param_1 + 8);
  param_2[2] = uVar1;
  param_2[3] = *(undefined8 *)(param_1 + 0x18);
  return;
}



