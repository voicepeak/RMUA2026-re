// ADDRESS 02fbfde0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fbfde0(undefined8 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  
  func_0x050976b0();
  *param_1 = 0xcd5b90;
  uVar3 = _UNK_006cf12c;
  uVar2 = _UNK_006cf128;
  uVar1 = _UNK_006cf124;
  *(undefined4 *)(param_1 + 0x44) = _DAT_006cf120;
  *(undefined4 *)((long)param_1 + 0x224) = uVar1;
  *(undefined4 *)(param_1 + 0x45) = uVar2;
  *(undefined4 *)((long)param_1 + 0x22c) = uVar3;
  *(undefined1 *)(param_1 + 0x46) = 0;
  *(undefined8 *)((long)param_1 + 0x234) = 0;
  *(byte *)((long)param_1 + 0x32) = *(byte *)((long)param_1 + 0x32) | 2;
  func_0x031d5d00(&uStack_20,&UNK_008e434a,1);
  uVar4 = func_0x05b696b0();
  uVar4 = func_0x033358a0(param_1,uStack_20,uVar4,uVar4,1,0);
  func_0x050ad3d0(param_1,uVar4);
  func_0x031d5d00(&uStack_28,&UNK_0089cce6,1);
  uVar5 = func_0x05b696b0();
  uVar5 = func_0x033358a0(param_1,uStack_28,uVar5,uVar5,1,0);
  func_0x033e4af0(param_1 + 0x45,uVar5);
  uVar5 = func_0x033e4e80(param_1 + 0x45);
  func_0x052b1f60(uVar5,uVar4,0x741d1f8,0);
  func_0x031d5d00(&uStack_30,&UNK_007bff80,1);
  uVar5 = func_0x05b95860();
  uVar5 = func_0x033358a0(param_1,uStack_30,uVar5,uVar5,1,0);
  param_1 = param_1 + 0x44;
  func_0x033e4af0(param_1,uVar5);
  uVar5 = func_0x033e4e80(param_1);
  func_0x052ecf50(uVar5,1);
  uVar5 = func_0x033e4e80(param_1);
  func_0x052b1f60(uVar5,uVar4,0x741d1f8,0);
  return;
}



// ADDRESS 02fc0020

void selected_02fc0020(undefined8 param_1)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lStack_48;
  ulong uStack_40;
  undefined8 uStack_30;
  
  func_0x050ac080();
  lVar2 = func_0x02fb4e80();
  if (*(char *)(lVar2 + 0x2d1) != '\0') {
    lStack_48 = 0;
    uStack_40 = 0;
    lVar2 = func_0x05b9c350();
    uStack_40 = uStack_40 & 0xffffffff00000000;
    if (((lVar2 == 0) || (lVar3 = func_0x059d5e80(), lVar3 == 0)) ||
       (*(int *)(lVar2 + 0x38) < *(int *)(lVar3 + 0x38))) {
      lVar5 = 0;
    }
    else {
      lVar5 = 0;
      if (*(long *)(*(long *)(lVar2 + 0x30) + (long)*(int *)(lVar3 + 0x38) * 8) == lVar3 + 0x30) {
        lVar5 = lVar2;
      }
    }
    lVar3 = func_0x059d5e80();
    if (lVar5 == lVar3) {
      func_0x02d930f0(param_1,lVar2,&lStack_48);
    }
    else {
      func_0x02d935c0(param_1,lVar2,&lStack_48);
    }
    lVar2 = lStack_48;
    lVar3 = (long)(int)uStack_40;
    if (lVar3 != 0) {
      lVar5 = 0;
      do {
        plVar1 = *(long **)(lVar2 + lVar5);
        if (plVar1 != (long *)0x0) {
          lVar4 = func_0x05b9c350();
          if ((*(int *)(lVar4 + 0x38) <= *(int *)(plVar1[2] + 0x38)) &&
             (*(long *)(*(long *)(plVar1[2] + 0x30) + (long)*(int *)(lVar4 + 0x38) * 8) ==
              lVar4 + 0x30)) {
            func_0x031d5d00(&uStack_30,&UNK_00820f7c,1);
            (**(code **)(*plVar1 + 0x668))(plVar1,uStack_30,1);
          }
        }
        lVar5 = lVar5 + 8;
      } while (lVar3 * 8 != lVar5);
    }
    if (lStack_48 != 0) {
      func_0x03071ea0(lStack_48);
    }
  }
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



// ADDRESS 02fd71c0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long selected_02fd71c0(void)

{
  if (_DAT_071a3940 != 0) {
    return _DAT_071a3940;
  }
  func_0x0327dc50(&UNK_0091a3a8,&UNK_007a7c94,&DAT_071a3940,&UNK_02fd71b0,0x240,8,0x10000004,0,
                  &UNK_007d3fa8,&UNK_02fd72d0,&UNK_02fd72e0,&UNK_05098920,&UNK_026c73d0,
                  &UNK_026c6e30,0,0);
  return _DAT_071a3940;
}



// ADDRESS 02fd72d0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void selected_02fd72d0(undefined8 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  
  param_1 = (undefined8 *)*param_1;
  func_0x050976b0();
  *param_1 = 0xcd5b90;
  uVar3 = _UNK_006cf12c;
  uVar2 = _UNK_006cf128;
  uVar1 = _UNK_006cf124;
  *(undefined4 *)(param_1 + 0x44) = _DAT_006cf120;
  *(undefined4 *)((long)param_1 + 0x224) = uVar1;
  *(undefined4 *)(param_1 + 0x45) = uVar2;
  *(undefined4 *)((long)param_1 + 0x22c) = uVar3;
  *(undefined1 *)(param_1 + 0x46) = 0;
  *(undefined8 *)((long)param_1 + 0x234) = 0;
  *(byte *)((long)param_1 + 0x32) = *(byte *)((long)param_1 + 0x32) | 2;
  func_0x031d5d00(&uStack_20,&UNK_008e434a,1);
  uVar4 = func_0x05b696b0();
  uVar4 = func_0x033358a0(param_1,uStack_20,uVar4,uVar4,1,0);
  func_0x050ad3d0(param_1,uVar4);
  func_0x031d5d00(&uStack_28,&UNK_0089cce6,1);
  uVar5 = func_0x05b696b0();
  uVar5 = func_0x033358a0(param_1,uStack_28,uVar5,uVar5,1,0);
  func_0x033e4af0(param_1 + 0x45,uVar5);
  uVar5 = func_0x033e4e80(param_1 + 0x45);
  func_0x052b1f60(uVar5,uVar4,0x741d1f8,0);
  func_0x031d5d00(&uStack_30,&UNK_007bff80,1);
  uVar5 = func_0x05b95860();
  uVar5 = func_0x033358a0(param_1,uStack_30,uVar5,uVar5,1,0);
  param_1 = param_1 + 0x44;
  func_0x033e4af0(param_1,uVar5);
  uVar5 = func_0x033e4e80(param_1);
  func_0x052ecf50(uVar5,1);
  uVar5 = func_0x033e4e80(param_1);
  func_0x052b1f60(uVar5,uVar4,0x741d1f8,0);
  return;
}



