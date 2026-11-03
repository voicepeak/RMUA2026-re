// ADDRESS 02fc6cc0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fc6cc0(undefined4 param_1,undefined4 param_2,undefined4 param_3,long param_4,
                      undefined1 param_5,undefined4 param_6,undefined4 param_7,int param_8,
                      int param_9,undefined1 param_10)

{
  ulong *puVar1;
  ulong *puVar2;
  char cVar3;
  undefined8 uVar4;
  bool bVar5;
  code *pcVar6;
  undefined4 uVar7;
  char cVar8;
  int iVar9;
  undefined8 *puVar10;
  long lVar11;
  uint uVar12;
  undefined4 uVar13;
  ulong uVar14;
  float *pfVar15;
  undefined8 **ppuVar16;
  ulong uVar17;
  ulong uVar18;
  float fVar19;
  undefined1 auStack_ac [4];
  undefined8 **ppuStack_a8;
  undefined8 uStack_a0;
  undefined1 uStack_89;
  float *pfStack_88;
  undefined8 uStack_80;
  float fStack_70;
  undefined4 uStack_6c;
  undefined1 auStack_68 [16];
  int iStack_4c;
  int iStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  ulong *puStack_38;
  
  auStack_68._0_4_ = param_3;
  *(undefined1 *)(param_4 + 0x250) = param_5;
  *(undefined1 *)(param_4 + 0x251) = param_10;
  uStack_89 = param_5;
  uStack_6c = param_6;
  iStack_4c = param_8;
  iStack_48 = param_9;
  uStack_44 = param_7;
  uStack_40 = param_2;
  uStack_3c = param_1;
  fStack_70 = (float)func_0x052effa0(*(undefined8 *)(param_4 + 0x238));
  fVar19 = 0.0;
  if (*(char *)(param_4 + 0x250) != '\0') {
    fVar19 = fStack_70;
  }
  *(float *)(param_4 + 0x260) = fVar19;
  *(float *)(param_4 + 0x264) = fVar19;
  *(undefined4 *)(param_4 + 0x274) = 0xffffffff;
  *(undefined8 *)(param_4 + 0x254) = _DAT_071fa860;
  *(undefined4 *)(param_4 + 0x25c) = _DAT_071fa868;
  puVar1 = (ulong *)(param_4 + 0x2e0);
  *(undefined4 *)(param_4 + 0x2e8) = 0;
  if (*(int *)(param_4 + 0x2ec) != 0) {
    func_0x026b5760(puVar1,0);
  }
  puVar2 = (ulong *)(param_4 + 0x2d0);
  *(undefined4 *)(param_4 + 0x2d8) = 0;
  puStack_38 = puVar1;
  if (*(int *)(param_4 + 0x2dc) != 0) {
    func_0x026b5760(puVar2,0);
  }
  *(undefined4 *)(param_4 + 0x26c) = uStack_40;
  *(undefined4 *)(param_4 + 0x268) = uStack_3c;
  *(undefined4 *)(param_4 + 0x270) = auStack_68._0_4_;
  if (0 < *(int *)(param_4 + 0x248)) {
    auStack_68._0_8_ = param_4 + 0x240;
    uVar18 = 0xffffffff;
    uVar14 = 0;
    uVar17 = 0xffffffff;
    do {
      pfStack_88 = *(float **)(*(long *)(*(long *)auStack_68._0_8_ + uVar14 * 8) + 0x18);
      func_0x031d6e10(&ppuStack_a8,&pfStack_88);
      pfStack_88 = (float *)0x0;
      uStack_80 = 0;
      func_0x025ead80(&pfStack_88,8);
      iVar9 = (int)uStack_80 + 8;
      uStack_80 = CONCAT44(uStack_80._4_4_,iVar9);
      if (uStack_80._4_4_ < iVar9) {
        func_0x025eade0(&pfStack_88);
      }
      uVar4 = _UNK_006d61b8;
      *(undefined8 *)pfStack_88 = _DAT_006d61b0;
      *(undefined8 *)(pfStack_88 + 2) = uVar4;
      pfVar15 = (float *)&UNK_009dcf72;
      if ((int)uStack_80 != 0) {
        pfVar15 = pfStack_88;
      }
      iVar9 = func_0x03000d20(&ppuStack_a8,pfVar15,1,0,0xffffffff);
      if (pfStack_88 != (float *)0x0) {
        func_0x03071ea0();
      }
      if (ppuStack_a8 != (undefined8 **)0x0) {
        func_0x03071ea0();
      }
      fVar19 = (float)uVar14;
      auStack_ac = (undefined1  [4])fVar19;
      if (iVar9 == -1) {
        if ((long)*(int *)(param_4 + 0x248) <= (long)uVar14) {
          ppuStack_a8 = (undefined8 **)auStack_ac;
          uStack_a0 = (ulong *)auStack_68._0_8_;
          func_0x06f8ce70(&ppuStack_a8);
          pcVar6 = (code *)swi(3);
          (*pcVar6)();
          return;
        }
        ppuStack_a8 = *(undefined8 ***)(*(long *)(*(long *)auStack_68._0_8_ + uVar14 * 8) + 0x18);
        func_0x031d6e10(&pfStack_88,&ppuStack_a8);
        ppuStack_a8 = (undefined8 **)0x0;
        uStack_a0 = (ulong *)0x0;
        func_0x025ead80(&ppuStack_a8,9);
        iVar9 = (int)uStack_a0 + 9;
        uStack_a0 = (ulong *)CONCAT44(uStack_a0._4_4_,iVar9);
        if (uStack_a0._4_4_ < iVar9) {
          func_0x025eade0(&ppuStack_a8);
        }
        puVar10 = _UNK_006cdf88;
        *ppuStack_a8 = _DAT_006cdf80;
        ppuStack_a8[1] = puVar10;
        *(undefined2 *)(ppuStack_a8 + 2) = 0;
        ppuVar16 = (undefined8 **)&UNK_009dcf72;
        if ((int)uStack_a0 != 0) {
          ppuVar16 = ppuStack_a8;
        }
        iVar9 = func_0x03000d20(&pfStack_88,ppuVar16,1,0,0xffffffff);
        if (ppuStack_a8 != (undefined8 **)0x0) {
          func_0x03071ea0();
        }
        if (pfStack_88 != (float *)0x0) {
          func_0x03071ea0();
        }
        if (iVar9 != -1) {
          uVar17 = uVar14 & 0xffffffff;
        }
      }
      else {
        uVar18 = uVar14 & 0xffffffff;
      }
      cVar3 = *(char *)(param_4 + 0x250);
      uVar7 = (int)uVar17;
      uVar13 = (int)uVar18;
      if (cVar3 == '\0') {
        uVar7 = (int)uVar18;
        uVar13 = (int)uVar17;
      }
      *(undefined4 *)(param_4 + 0x27c) = uVar7;
      *(undefined4 *)(param_4 + 0x278) = uVar13;
      if (((*(char *)(param_4 + 0x234) == '\0') || (0x18 < *(uint *)(param_4 + 0x230))) ||
         ((0x1aa02b0U >> (*(uint *)(param_4 + 0x230) & 0x1f) & 1) == 0)) {
        auStack_ac = (undefined1  [4])fVar19;
        if (cVar3 == '\0') {
          auStack_ac = (undefined1  [4])(~(uint)fVar19 + *(int *)(param_4 + 0x248));
        }
        pfStack_88 = (float *)auStack_ac;
        if (((float *)*puVar2 <= pfStack_88) &&
           (auStack_ac < (float *)*puVar2 + *(int *)(param_4 + 0x2dc))) {
          ppuStack_a8 = (undefined8 **)&pfStack_88;
          uStack_a0 = puVar2;
          func_0x06f8e390(&ppuStack_a8);
          pcVar6 = (code *)swi(3);
          (*pcVar6)();
          return;
        }
      }
      else {
        auStack_ac = (undefined1  [4])fVar19;
        if (cVar3 != '\0') {
          auStack_ac = (undefined1  [4])(~(uint)fVar19 + *(int *)(param_4 + 0x248));
        }
        pfStack_88 = (float *)auStack_ac;
        if (((float *)*puVar2 <= pfStack_88) &&
           (auStack_ac < (float *)*puVar2 + *(int *)(param_4 + 0x2dc))) {
          ppuStack_a8 = (undefined8 **)&pfStack_88;
          uStack_a0 = puVar2;
          func_0x06f8e390(&ppuStack_a8);
          pcVar6 = (code *)swi(3);
          (*pcVar6)();
          return;
        }
      }
      pfStack_88 = (float *)auStack_ac;
      iVar9 = *(int *)(param_4 + 0x2d8);
      *(int *)(param_4 + 0x2d8) = iVar9 + 1;
      if (*(int *)(param_4 + 0x2dc) <= iVar9) {
        func_0x025f0330(puVar2,iVar9);
      }
      *(undefined1 (*) [4])(*puVar2 + (long)iVar9 * 4) = auStack_ac;
      pfStack_88 = (float *)CONCAT44(pfStack_88._4_4_,fVar19);
      if ((long)*(int *)(param_4 + 0x248) <= (long)uVar14) {
        ppuStack_a8 = (undefined8 **)&pfStack_88;
        uStack_a0 = (ulong *)auStack_68._0_8_;
        func_0x06f8ce70(&ppuStack_a8);
        pcVar6 = (code *)swi(3);
        (*pcVar6)();
        return;
      }
      func_0x02fc5630(*(undefined8 *)(*(long *)(param_4 + 0x240) + uVar14 * 8),0);
      uVar14 = uVar14 + 1;
    } while ((long)uVar14 < (long)*(int *)(param_4 + 0x248));
  }
  if (2 < DAT_0722748c) {
    ppuStack_a8 = (undefined8 **)
                  CONCAT44(ppuStack_a8._4_4_,*(int *)(param_4 + 0x288) - *(int *)(param_4 + 0x2b4));
    pfStack_88 = (float *)CONCAT44(pfStack_88._4_4_,*(undefined4 *)(param_4 + 0x2d8));
    func_0x02fc75e0(param_4 + 0x230,&ppuStack_a8,&pfStack_88,&uStack_89);
  }
  cVar3 = (char)uStack_6c;
  if (((char)uStack_6c == '\0') && ((char)uStack_44 == '\0')) {
    return;
  }
  cVar8 = *(char *)(param_4 + 0x250);
  auStack_ac = (undefined1  [4])_DAT_006c1b34;
  if (cVar8 != '\0') {
    auStack_ac = (undefined1  [4])(_DAT_006c17b8 + fStack_70);
  }
  fStack_70 = fStack_70 + _DAT_006c0aa8;
  bVar5 = 0 < iStack_48;
  do {
    if (cVar8 == '\0') {
      auStack_68._0_4_ = auStack_ac;
      fVar19 = (float)func_0x052effa0(*(undefined8 *)(param_4 + 0x238));
      if (fVar19 <= (float)auStack_68._0_4_) {
        return;
      }
      if (*(char *)(param_4 + 0x250) != '\0') goto LAB_02fc72a7;
      if (_DAT_006c110c < (float)auStack_ac) goto LAB_02fc72b0;
LAB_02fc73d0:
      iVar9 = iStack_48;
      if (cVar3 == '\x01' && bVar5) {
        do {
          cVar8 = *(char *)(param_4 + 0x250);
          auStack_68 = ZEXT416((uint)auStack_ac);
          lVar11 = func_0x02fcc220();
          uVar12 = *(int *)(lVar11 + 0x1a0) * 0xbb38435 + 0x3619636b;
          *(uint *)(lVar11 + 0x1a0) = uVar12;
          fVar19 = ((float)(uVar12 >> 9 | 0x3f800000) + _DAT_006c0d00) * _DAT_006c1240 +
                   _DAT_006c0e4c;
          if (cVar8 == '\0') {
            fVar19 = (float)((uint)fVar19 ^ _DAT_006c2f70);
          }
          auStack_68._0_4_ = (float)auStack_68._0_4_ + fVar19;
          func_0x052effa0(*(undefined8 *)(param_4 + 0x238));
          puVar10 = (undefined8 *)func_0x02fc7620(param_4,0);
          if (puVar10 != (undefined8 *)0x0) {
            uVar4 = *puVar10;
            lVar11 = func_0x02fb4e80();
            pfStack_88 = (float *)((ulong)pfStack_88 & 0xffffffff00000000);
            if (*(int *)(lVar11 + 0x228) < 1) {
              ppuStack_a8 = (undefined8 **)&pfStack_88;
              uStack_a0 = (undefined8 *)(lVar11 + 0x220);
              func_0x06f8ce70(&ppuStack_a8);
              pcVar6 = (code *)swi(3);
              (*pcVar6)();
              return;
            }
            func_0x02fc3270(uVar4,**(undefined8 **)(lVar11 + 0x220),*(undefined1 *)(puVar10 + 1));
            _ZdlPv(puVar10);
          }
          iVar9 = iVar9 + -1;
        } while (iVar9 != 0);
      }
    }
    else {
      if ((float)auStack_ac <= _DAT_006c0fa8) {
        return;
      }
LAB_02fc72a7:
      if (fStack_70 <= (float)auStack_ac) goto LAB_02fc73d0;
LAB_02fc72b0:
      puVar1 = puStack_38;
      if ((char)uStack_44 != '\0') {
        pfStack_88 = (float *)auStack_ac;
        if (((float *)*puStack_38 <= pfStack_88) &&
           (auStack_ac < (float *)*puStack_38 + *(int *)(param_4 + 0x2ec))) {
          ppuStack_a8 = (undefined8 **)&pfStack_88;
          uStack_a0 = puStack_38;
          func_0x06f8e390(&ppuStack_a8);
          pcVar6 = (code *)swi(3);
          (*pcVar6)();
          return;
        }
        iVar9 = *(int *)(param_4 + 0x2e8);
        *(int *)(param_4 + 0x2e8) = iVar9 + 1;
        if (*(int *)(param_4 + 0x2ec) <= iVar9) {
          func_0x025f0330(puStack_38,iVar9);
        }
        *(undefined1 (*) [4])(*puVar1 + (long)iVar9 * 4) = auStack_ac;
      }
      if ((char)uStack_6c != '\0') {
        puVar10 = (undefined8 *)func_0x02fc7620(param_4,0);
        if (puVar10 != (undefined8 *)0x0) {
          iVar9 = iStack_4c;
          if (0 < iStack_4c) {
            do {
              uVar4 = *puVar10;
              lVar11 = func_0x02fb4e80();
              pfStack_88 = (float *)((ulong)pfStack_88 & 0xffffffff00000000);
              if (*(int *)(lVar11 + 0x228) < 1) {
                ppuStack_a8 = (undefined8 **)&pfStack_88;
                uStack_a0 = (undefined8 *)(lVar11 + 0x220);
                func_0x06f8ce70(&ppuStack_a8);
                pcVar6 = (code *)swi(3);
                (*pcVar6)();
                return;
              }
              func_0x02fc3270(uVar4,**(undefined8 **)(lVar11 + 0x220),*(undefined1 *)(puVar10 + 1));
              iVar9 = iVar9 + -1;
            } while (iVar9 != 0);
          }
          _ZdlPv(puVar10);
        }
        goto LAB_02fc73d0;
      }
    }
    cVar8 = *(char *)(param_4 + 0x250);
    auStack_ac = (undefined1  [4])
                 ((float)auStack_ac + *(float *)(&UNK_006c2a50 + (ulong)(cVar8 == '\0') * 4));
  } while( true );
}



// ADDRESS 02fc13d0

void selected_02fc13d0(long param_1,int param_2)

{
  long *plVar1;
  undefined8 uVar2;
  undefined8 uStack_28;
  undefined8 uStack_20;
  
  plVar1 = (long *)func_0x033e4e80(param_1 + 0x220);
  if (param_2 == 0) {
    func_0x031d5d00(&uStack_20,&UNK_00820f7c,1);
    (**(code **)(*plVar1 + 0x668))(plVar1,uStack_20,1);
  }
  else {
    func_0x031d5d00(&uStack_28,&UNK_0074213e,1);
    (**(code **)(*plVar1 + 0x668))(plVar1,uStack_28,1);
  }
  uVar2 = func_0x033e4e80(param_1 + 0x220);
  func_0x052bb600(uVar2,param_2 != 0,1);
  return;
}



// ADDRESS 02fb9960

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fb9960(long param_1,undefined4 param_2,char param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined8 *puVar5;
  long lVar6;
  undefined8 uVar7;
  undefined8 *puStack_38;
  undefined8 uStack_30;
  
  if (4 < DAT_0722748c) {
    puStack_38 = (undefined8 *)CONCAT44(puStack_38._4_4_,param_2);
    func_0x02fbf100(&puStack_38);
  }
  puVar5 = (undefined8 *)_Znwm(0x18);
  puVar5[1] = 0x11;
  *puVar5 = 0xcd1e48;
  *(undefined4 *)(puVar5 + 2) = 0;
  lVar6 = func_0x02fcc220();
  *(undefined4 *)((long)puVar5 + 0x14) = *(undefined4 *)(lVar6 + 0x1a0);
  switch(param_2) {
  case 0:
    *(undefined4 *)(param_1 + 0xd0) = 0;
    *(undefined4 *)(puVar5 + 2) = 0;
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,9);
    iVar4 = (int)uStack_30 + 9;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    uVar3 = _UNK_006caaac;
    uVar2 = _UNK_006caaa8;
    uVar1 = _UNK_006caaa4;
    *(undefined4 *)puStack_38 = _UNK_006caaa0;
    *(undefined4 *)((long)puStack_38 + 4) = uVar1;
    *(undefined4 *)(puStack_38 + 1) = uVar2;
    *(undefined4 *)((long)puStack_38 + 0xc) = uVar3;
    *(undefined2 *)(puStack_38 + 2) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) == &puStack_38) {
      if (puStack_38 != (undefined8 *)0x0) {
        func_0x03071ea0();
      }
    }
    else {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
code_r0x02fba321:
      *(undefined8 **)(lVar6 + 0x308) = puStack_38;
      puStack_38 = (undefined8 *)0x0;
      *(int *)(lVar6 + 0x310) = (int)uStack_30;
      *(int *)(lVar6 + 0x314) = uStack_30._4_4_;
      uStack_30 = 0;
    }
    break;
  case 1:
    *(undefined4 *)(param_1 + 0xd0) = 1;
    *(undefined4 *)(puVar5 + 2) = 1;
    uVar7 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(undefined8 *)(param_1 + 0x140) = uVar7;
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,7);
    iVar4 = (int)uStack_30 + 7;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    *puStack_38 = 0x74006100740053;
    *(undefined4 *)(puStack_38 + 1) = 0x310065;
    *(undefined2 *)((long)puStack_38 + 0xc) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 2:
    if (*(int *)(param_1 + 0xd0) != 2) {
      lVar6 = func_0x02fb4e80();
      *(int *)(lVar6 + 0x2fc) = *(int *)(lVar6 + 0x2fc) + 1;
    }
    *(undefined4 *)(param_1 + 0xd0) = 2;
    *(undefined4 *)(puVar5 + 2) = 2;
    lVar6 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(float *)(param_1 + 400) = (float)(lVar6 - *(long *)(param_1 + 0x140)) / _UNK_006c1238;
    func_0x02fbac30(param_1,3,5,2);
    func_0x02fbc850(param_1);
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,9);
    iVar4 = (int)uStack_30 + 9;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    uVar3 = _UNK_006c4a6c;
    uVar2 = _UNK_006c4a68;
    uVar1 = _UNK_006c4a64;
    *(undefined4 *)puStack_38 = _UNK_006c4a60;
    *(undefined4 *)((long)puStack_38 + 4) = uVar1;
    *(undefined4 *)(puStack_38 + 1) = uVar2;
    *(undefined4 *)((long)puStack_38 + 0xc) = uVar3;
    *(undefined2 *)(puStack_38 + 2) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 3:
    *(undefined4 *)(param_1 + 0xd0) = 3;
    *(undefined4 *)(puVar5 + 2) = 2;
    uVar7 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(undefined8 *)(param_1 + 0x148) = uVar7;
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,7);
    iVar4 = (int)uStack_30 + 7;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    *puStack_38 = 0x74006100740053;
    *(undefined4 *)(puStack_38 + 1) = 0x320065;
    *(undefined2 *)((long)puStack_38 + 0xc) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 4:
    if (*(int *)(param_1 + 0xd0) != 4) {
      lVar6 = func_0x02fb4e80();
      *(int *)(lVar6 + 0x2fc) = *(int *)(lVar6 + 0x2fc) + 1;
    }
    *(undefined4 *)(param_1 + 0xd0) = 4;
    *(undefined4 *)(puVar5 + 2) = 2;
    lVar6 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(float *)(param_1 + 0x194) = (float)(lVar6 - *(long *)(param_1 + 0x148)) / _UNK_006c1238;
    func_0x02fbac30(param_1,5,8,3);
    func_0x02fbc850(param_1);
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,9);
    iVar4 = (int)uStack_30 + 9;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    uVar3 = _UNK_006d0bbc;
    uVar2 = _UNK_006d0bb8;
    uVar1 = _UNK_006d0bb4;
    *(undefined4 *)puStack_38 = _UNK_006d0bb0;
    *(undefined4 *)((long)puStack_38 + 4) = uVar1;
    *(undefined4 *)(puStack_38 + 1) = uVar2;
    *(undefined4 *)((long)puStack_38 + 0xc) = uVar3;
    *(undefined2 *)(puStack_38 + 2) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 5:
    *(undefined4 *)(param_1 + 0xd0) = 5;
    *(undefined4 *)(puVar5 + 2) = 2;
    uVar7 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(undefined8 *)(param_1 + 0x150) = uVar7;
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,7);
    iVar4 = (int)uStack_30 + 7;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    *puStack_38 = 0x74006100740053;
    *(undefined4 *)(puStack_38 + 1) = 0x330065;
    *(undefined2 *)((long)puStack_38 + 0xc) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 6:
    if (*(int *)(param_1 + 0xd0) != 6) {
      lVar6 = func_0x02fb4e80();
      *(int *)(lVar6 + 0x2fc) = *(int *)(lVar6 + 0x2fc) + 1;
    }
    *(undefined4 *)(param_1 + 0xd0) = 6;
    *(undefined4 *)(puVar5 + 2) = 2;
    lVar6 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(float *)(param_1 + 0x198) = (float)(lVar6 - *(long *)(param_1 + 0x150)) / _UNK_006c1238;
    func_0x02fbac30(param_1,8,0xc,4);
    func_0x02fbc850(param_1);
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,9);
    iVar4 = (int)uStack_30 + 9;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    uVar3 = _UNK_006c4a5c;
    uVar2 = _UNK_006c4a58;
    uVar1 = _UNK_006c4a54;
    *(undefined4 *)puStack_38 = _UNK_006c4a50;
    *(undefined4 *)((long)puStack_38 + 4) = uVar1;
    *(undefined4 *)(puStack_38 + 1) = uVar2;
    *(undefined4 *)((long)puStack_38 + 0xc) = uVar3;
    *(undefined2 *)(puStack_38 + 2) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 7:
    *(undefined4 *)(param_1 + 0xd0) = 7;
    *(undefined4 *)(puVar5 + 2) = 2;
    uVar7 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(undefined8 *)(param_1 + 0x158) = uVar7;
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,7);
    iVar4 = (int)uStack_30 + 7;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    *puStack_38 = 0x74006100740053;
    *(undefined4 *)(puStack_38 + 1) = 0x340065;
    *(undefined2 *)((long)puStack_38 + 0xc) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 8:
    if (*(int *)(param_1 + 0xd0) != 8) {
      lVar6 = func_0x02fb4e80();
      *(int *)(lVar6 + 0x2fc) = *(int *)(lVar6 + 0x2fc) + 1;
    }
    *(undefined4 *)(param_1 + 0xd0) = 8;
    *(undefined4 *)(puVar5 + 2) = 2;
    lVar6 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(float *)(param_1 + 0x19c) = (float)(lVar6 - *(long *)(param_1 + 0x158)) / _UNK_006c1238;
    func_0x02fbac30(param_1,0xc,10,5);
    func_0x02fbc850(param_1);
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,9);
    iVar4 = (int)uStack_30 + 9;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    uVar3 = _UNK_006d4b4c;
    uVar2 = _UNK_006d4b48;
    uVar1 = _UNK_006d4b44;
    *(undefined4 *)puStack_38 = _UNK_006d4b40;
    *(undefined4 *)((long)puStack_38 + 4) = uVar1;
    *(undefined4 *)(puStack_38 + 1) = uVar2;
    *(undefined4 *)((long)puStack_38 + 0xc) = uVar3;
    *(undefined2 *)(puStack_38 + 2) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 9:
    *(undefined4 *)(param_1 + 0xd0) = 9;
    *(undefined4 *)(puVar5 + 2) = 2;
    uVar7 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(undefined8 *)(param_1 + 0x160) = uVar7;
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,7);
    iVar4 = (int)uStack_30 + 7;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    *puStack_38 = 0x74006100740053;
    *(undefined4 *)(puStack_38 + 1) = 0x350065;
    *(undefined2 *)((long)puStack_38 + 0xc) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 10:
    if (*(int *)(param_1 + 0xd0) != 10) {
      lVar6 = func_0x02fb4e80();
      *(int *)(lVar6 + 0x2fc) = *(int *)(lVar6 + 0x2fc) + 1;
    }
    *(undefined4 *)(param_1 + 0xd0) = 10;
    *(undefined4 *)(puVar5 + 2) = 2;
    lVar6 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(float *)(param_1 + 0x1a0) = (float)(lVar6 - *(long *)(param_1 + 0x160)) / _UNK_006c1238;
    func_0x02fbac30(param_1,10,7,6);
    func_0x02fbc850(param_1);
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,9);
    iVar4 = (int)uStack_30 + 9;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    uVar3 = _UNK_006d20cc;
    uVar2 = _UNK_006d20c8;
    uVar1 = _UNK_006d20c4;
    *(undefined4 *)puStack_38 = _UNK_006d20c0;
    *(undefined4 *)((long)puStack_38 + 4) = uVar1;
    *(undefined4 *)(puStack_38 + 1) = uVar2;
    *(undefined4 *)((long)puStack_38 + 0xc) = uVar3;
    *(undefined2 *)(puStack_38 + 2) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 0xb:
    *(undefined4 *)(param_1 + 0xd0) = 0xb;
    *(undefined4 *)(puVar5 + 2) = 2;
    uVar7 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(undefined8 *)(param_1 + 0x168) = uVar7;
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,7);
    iVar4 = (int)uStack_30 + 7;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    *puStack_38 = 0x74006100740053;
    *(undefined4 *)(puStack_38 + 1) = 0x360065;
    *(undefined2 *)((long)puStack_38 + 0xc) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  case 0xc:
    *(undefined4 *)(param_1 + 0xd4) = *(undefined4 *)(param_1 + 0xd0);
    *(undefined4 *)(param_1 + 0xd0) = 0xc;
    *(undefined4 *)(puVar5 + 2) = 3;
    lVar6 = _ZNSt3__16chrono12steady_clock3nowEv();
    *(long *)(param_1 + 0x170) = lVar6;
    if (param_3 == '\0') {
      *(undefined4 *)(param_1 + 0x1a4) = 0;
    }
    else {
      *(float *)(param_1 + 0x1a4) = (float)(lVar6 - *(long *)(param_1 + 0x168)) / _UNK_006c1238;
      lVar6 = func_0x02fb4e80();
      *(int *)(lVar6 + 0x2fc) = *(int *)(lVar6 + 0x2fc) + 1;
    }
    *(char *)(param_1 + 0xe1) = param_3;
    puStack_38 = (undefined8 *)0x0;
    uStack_30 = 0;
    func_0x025ead80(&puStack_38,9);
    iVar4 = (int)uStack_30 + 9;
    uStack_30 = CONCAT44(uStack_30._4_4_,iVar4);
    if (uStack_30._4_4_ < iVar4) {
      func_0x025eade0(&puStack_38);
    }
    uVar3 = _UNK_006cc9fc;
    uVar2 = _UNK_006cc9f8;
    uVar1 = _UNK_006cc9f4;
    *(undefined4 *)puStack_38 = _UNK_006cc9f0;
    *(undefined4 *)((long)puStack_38 + 4) = uVar1;
    *(undefined4 *)(puStack_38 + 1) = uVar2;
    *(undefined4 *)((long)puStack_38 + 0xc) = uVar3;
    *(undefined2 *)(puStack_38 + 2) = 0;
    lVar6 = func_0x02fb4e80();
    if ((undefined8 **)(lVar6 + 0x308) != &puStack_38) {
      if (*(long *)(lVar6 + 0x308) != 0) {
        func_0x03071ea0();
      }
      goto code_r0x02fba321;
    }
    if (puStack_38 != (undefined8 *)0x0) {
      func_0x03071ea0();
    }
    break;
  default:
    goto LAB_02fba361;
  }
  uVar7 = func_0x02fcc220();
  func_0x02fcc520(uVar7,puVar5);
LAB_02fba361:
  return;
}



// ADDRESS 02fba970

void selected_02fba970(long param_1)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  long lVar6;
  undefined8 *puStack_38;
  undefined8 uStack_30;
  undefined4 uStack_24;
  
  func_0x02fbac30(param_1,1,3,1);
  iVar2 = *(int *)(param_1 + 0x48);
  if ((long)iVar2 != 0) {
    lVar3 = *(long *)(param_1 + 0x40);
    lVar6 = 0;
    do {
      func_0x02fc2440(*(undefined8 *)(lVar3 + lVar6),0);
      lVar6 = lVar6 + 8;
    } while ((long)iVar2 * 8 != lVar6);
    uStack_24 = 0;
    if (0 < *(int *)(param_1 + 0x48)) {
      func_0x02fc2440(**(undefined8 **)(param_1 + 0x40),1);
      selected_02fb9960(param_1,0,0);
      lVar3 = func_0x02fb4e80();
      *(undefined4 *)(lVar3 + 0x2fc) = 0;
      *(undefined4 *)(param_1 + 0x1ac) = 0;
      *(undefined8 *)(param_1 + 400) = 0;
      *(undefined8 *)(param_1 + 0x198) = 0;
      *(undefined8 *)(param_1 + 0x1a0) = 0;
      puStack_38 = (undefined8 *)0x0;
      uStack_30 = 0;
      func_0x025ead80(&puStack_38,7);
      iVar2 = (int)uStack_30 + 7;
      uStack_30 = CONCAT44(uStack_30._4_4_,iVar2);
      if (uStack_30._4_4_ < iVar2) {
        func_0x025eade0(&puStack_38);
      }
      *puStack_38 = 0x6d0072006f006e;
      *(undefined4 *)(puStack_38 + 1) = 0x6c0061;
      *(undefined2 *)((long)puStack_38 + 0xc) = 0;
      lVar3 = func_0x02fb4e80();
      if ((undefined8 **)(lVar3 + 0x2d8) == &puStack_38) {
        if (puStack_38 != (undefined8 *)0x0) {
          func_0x03071ea0();
        }
      }
      else {
        if (*(long *)(lVar3 + 0x2d8) != 0) {
          func_0x03071ea0();
        }
        *(undefined8 **)(lVar3 + 0x2d8) = puStack_38;
        puStack_38 = (undefined8 *)0x0;
        *(int *)(lVar3 + 0x2e0) = (int)uStack_30;
        *(int *)(lVar3 + 0x2e4) = uStack_30._4_4_;
        uStack_30 = 0;
      }
      lVar3 = func_0x02fb4e80();
      if (*(long *)(lVar3 + 0x2e8) != 0) {
        func_0x03071ea0();
      }
      *(undefined8 *)(lVar3 + 0x2e8) = 0;
      *(undefined8 *)(lVar3 + 0x2f0) = 0;
      *(undefined8 *)(param_1 + 0x1cc) = 0;
      *(undefined4 *)(param_1 + 0x1d8) = 0;
      *(undefined2 *)(param_1 + 0x180) = 1;
      puVar4 = (undefined8 *)_Znwm(0x18);
      puVar4[1] = 0x1e;
      *puVar4 = 0xcd58a0;
      *(undefined1 *)(puVar4 + 2) = 1;
      uVar5 = func_0x02fcc220();
      func_0x02fcc520(uVar5,puVar4);
      puVar4 = (undefined8 *)_Znwm(0x18);
      puVar4[1] = 0x1f;
      *puVar4 = 0xcd58c8;
      *(undefined1 *)(puVar4 + 2) = *(undefined1 *)(param_1 + 0x181);
      uVar5 = func_0x02fcc220();
      func_0x02fcc520(uVar5,puVar4);
      iVar2 = *(int *)(param_1 + 0x68);
      if ((long)iVar2 != 0) {
        lVar3 = *(long *)(param_1 + 0x60);
        lVar6 = 0;
        do {
          func_0x02fc05c0(*(undefined8 *)(lVar3 + lVar6));
          lVar6 = lVar6 + 8;
        } while ((long)iVar2 * 8 != lVar6);
      }
      func_0x02fb8650(param_1,1,1);
      return;
    }
  }
  uStack_24 = 0;
  uStack_30 = param_1 + 0x40;
  puStack_38 = (undefined8 *)&uStack_24;
  func_0x06f8ce70(&puStack_38);
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



