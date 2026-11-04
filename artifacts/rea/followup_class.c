// ADDRESS 02fd7260

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long selected_02fd7260(void)

{
  if (_DAT_071a3940 != 0) {
    return _DAT_071a3940;
  }
  func_0x0327dc50(&UNK_0091a3a8,&UNK_007a7c94,&DAT_071a3940,&UNK_02fd71b0,0x240,8,0x10000004,0,
                  &UNK_007d3fa8,&UNK_02fd72d0,&UNK_02fd72e0,&UNK_05098920,&UNK_026c73d0,
                  &UNK_026c6e30,0,0);
  return _DAT_071a3940;
}



// ADDRESS 02fd7000

/* WARNING: Control flow encountered bad instruction data */

void selected_02fd7000(undefined8 param_1,undefined8 param_2,undefined2 param_3)

{
  byte in_AL;
  byte bVar1;
  undefined7 in_register_00000001;
  long unaff_RBP;
  
  bVar1 = in_AL & (&UNK_01ba009c)[unaff_RBP];
  *(byte *)CONCAT71(in_register_00000001,bVar1) =
       *(char *)CONCAT71(in_register_00000001,bVar1) + bVar1;
  in(param_3);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



// ADDRESS 02fd7100

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void thunk_FUN_02fd7122(void)

{
  undefined4 *in_RAX;
  
  *in_RAX = 0xcdb170;
  func_0x032874a0();
  _DAT_071a3570 = 800;
  _DAT_071a3578 = 0xa4c83d7d;
  _DAT_071a3580 = 0;
  DAT_071a3588 = 0;
  _DAT_071a3568 = 0xcdb2c0;
  func_0x033b1870(&DAT_071a3568,&UNK_009648ea,800,0xa4c83d7d);
  func_0x033b1a30(&UNK_02fd6990,&UNK_02fd6920,&UNK_009648ea,&UNK_0091a3a8,0,0);
  return;
}



// ADDRESS 02fd7300

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 selected_02fd7300(undefined8 param_1,undefined8 param_2)

{
  undefined4 in_XMM0_Dc;
  undefined4 in_XMM0_Dd;
  undefined4 in_XMM1_Dc;
  undefined4 in_XMM1_Dd;
  
  uRam00000000071a35b4 = (undefined4)((ulong)param_2 >> 0x20);
  _DAT_071a35b0 = (undefined4)param_2;
  uRam00000000071a38a4 = (undefined4)((ulong)param_1 >> 0x20);
  _DAT_071a38a0 = (undefined4)param_1;
  _DAT_071a35c0 = _DAT_006cb080;
  uRam00000000071a35c8 = _UNK_006cb088;
  _DAT_071a35d0 = _DAT_006d5280;
  uRam00000000071a35d8 = _UNK_006d5288;
  _DAT_071a35e0 = ZEXT416(_DAT_006c0a14);
  _DAT_071a35f0 = _DAT_006cfc30;
  uRam00000000071a35f8 = _UNK_006cfc38;
  _DAT_071a3600 = _DAT_006c72d0;
  uRam00000000071a3608 = _UNK_006c72d8;
  _DAT_071a3610 = _DAT_006cfc40;
  uRam00000000071a3618 = _UNK_006cfc48;
  _DAT_071a3620 = _DAT_006c7c70;
  uRam00000000071a3628 = _UNK_006c7c78;
  _DAT_071a3630 = _DAT_006cb090;
  uRam00000000071a3638 = _UNK_006cb098;
  _DAT_071a3640 = _DAT_006c7c80;
  uRam00000000071a3648 = _UNK_006c7c88;
  _DAT_071a3650 = _DAT_006d45b0;
  uRam00000000071a3654 = _UNK_006d45b4;
  uRam00000000071a3658 = _UNK_006d45b8;
  uRam00000000071a365c = _UNK_006d45bc;
  _DAT_071a3660 = _DAT_006cfc30;
  uRam00000000071a3668 = _UNK_006cfc38;
  _DAT_071a3670 = _DAT_006c72d0;
  uRam00000000071a3678 = _UNK_006c72d8;
  _DAT_071a3680 = _DAT_006d45c0;
  uRam00000000071a3688 = _UNK_006d45c8;
  _DAT_071a3690 = _DAT_006d5d00;
  uRam00000000071a3698 = _UNK_006d5d08;
  _DAT_071a36b0 = 0xffffffff;
  uRam00000000071a36b4 = 0xffffffff;
  uRam00000000071a36b8 = 0xffffffff;
  uRam00000000071a36bc = 0xffffffff;
  _DAT_071a36c0 = _DAT_006cfc50;
  uRam00000000071a36c8 = _UNK_006cfc58;
  _DAT_071a36d0 = _DAT_006d3bd0;
  uRam00000000071a36d8 = _UNK_006d3bd8;
  _DAT_071a36e0 = _DAT_006c6780;
  uRam00000000071a36e8 = _UNK_006c6788;
  _DAT_071a36f0 = _DAT_006d70f0;
  uRam00000000071a36f8 = _UNK_006d70f8;
  _DAT_071a3700 = _DAT_006c45c0;
  uRam00000000071a3708 = _UNK_006c45c8;
  _DAT_071a3710 = _DAT_006d45d0;
  uRam00000000071a3718 = _UNK_006d45d8;
  _DAT_071a3720 = _DAT_006cb0a0;
  uRam00000000071a3728 = _UNK_006cb0a8;
  _DAT_071a3730 = _DAT_006ccfe0;
  uRam00000000071a3738 = _UNK_006ccfe8;
  _DAT_071a3740 = _DAT_006c2f70;
  uRam00000000071a3744 = _UNK_006c2f74;
  uRam00000000071a3748 = _UNK_006c2f78;
  uRam00000000071a374c = _UNK_006c2f7c;
  _DAT_071a3750 = _DAT_006c6790;
  uRam00000000071a3754 = _UNK_006c6794;
  uRam00000000071a3758 = _UNK_006c6798;
  uRam00000000071a375c = _UNK_006c679c;
  _DAT_071a3760 = _DAT_006c2f70;
  uRam00000000071a3764 = _UNK_006c2f74;
  uRam00000000071a3768 = _UNK_006c2f78;
  uRam00000000071a376c = _UNK_006c2f7c;
  _DAT_071a3770 = _DAT_006c6790;
  uRam00000000071a3774 = _UNK_006c6794;
  uRam00000000071a3778 = _UNK_006c6798;
  uRam00000000071a377c = _UNK_006c679c;
  _DAT_071a3780 = 0xffffffff;
  uRam00000000071a3784 = 0xffffffff;
  uRam00000000071a3788 = 0xffffffff;
  uRam00000000071a378c = 0xffffffff;
  _DAT_071a3790 = 0xffffffff;
  uRam00000000071a3794 = 0xffffffff;
  uRam00000000071a3798 = 0xffffffff;
  uRam00000000071a379c = 0xffffffff;
  _DAT_071a37a0 = _DAT_006d7b40;
  uRam00000000071a37a8 = _UNK_006d7b48;
  _DAT_071a37b0 = _DAT_006c9140;
  uRam00000000071a37b8 = _UNK_006c9148;
  _DAT_071a37c0 = _DAT_006d7b50;
  uRam00000000071a37c8 = _UNK_006d7b58;
  _DAT_071a37d0 = _DAT_006cd9c0;
  uRam00000000071a37d8 = _UNK_006cd9c8;
  _DAT_071a37e0 = _DAT_006d3be0;
  uRam00000000071a37e8 = _UNK_006d3be8;
  _DAT_071a37f0 = _DAT_006c3b00;
  uRam00000000071a37f8 = _UNK_006c3b08;
  _DAT_071a3800 = _DAT_006c45d0;
  uRam00000000071a3808 = _UNK_006c45d8;
  _DAT_071a3810 = _DAT_006c3b10;
  uRam00000000071a3818 = _UNK_006c3b18;
  _DAT_071a3820 = _DAT_006d45e0;
  uRam00000000071a3828 = _UNK_006d45e8;
  _DAT_071a3830 = _DAT_006c72e0;
  uRam00000000071a3838 = _UNK_006c72e8;
  _DAT_071a3840 = _DAT_006d5d10;
  uRam00000000071a3848 = _UNK_006d5d18;
  _DAT_071a3850 = _DAT_006c8710;
  uRam00000000071a3858 = _UNK_006c8718;
  _DAT_071a3860 = _DAT_006ca640;
  uRam00000000071a3868 = _UNK_006ca648;
  _DAT_071a3870 = _DAT_006d1c00;
  uRam00000000071a3878 = _UNK_006d1c08;
  _DAT_071a3880 = _DAT_006cbb20;
  uRam00000000071a3888 = _UNK_006cbb28;
  _DAT_071a3890 = _DAT_006d0670;
  uRam00000000071a3894 = _UNK_006d0674;
  uRam00000000071a3898 = _UNK_006d0678;
  uRam00000000071a389c = _UNK_006d067c;
  _DAT_071a38c0 = 0x40f2dce8;
  _DAT_071a38b0 = _DAT_006ce570;
  uRam00000000071a38b8 = _UNK_006ce578;
  _DAT_071a38e0 = 0x3fd121c8;
  _DAT_071a38d0 = _DAT_006d7100;
  uRam00000000071a38d4 = _UNK_006d7104;
  uRam00000000071a38d8 = _UNK_006d7108;
  uRam00000000071a38dc = _UNK_006d710c;
  _DAT_071a38f8 = _DAT_071fa868;
  _DAT_071a38f0 = _DAT_071fa860;
  _DAT_071a38ec = _DAT_071fa868;
  _DAT_071a38e4 = _DAT_071fa860;
  DAT_071a38fc = 0;
  _DAT_071a3900 = 0x7f7fffff7f7fffff;
  _DAT_071a3908 = 0x7f7fffff;
  _DAT_071a3920 = 0x240;
  _DAT_071a3928 = 0xd2639628;
  _DAT_071a3930 = 0;
  DAT_071a3938 = 0;
  _DAT_071a3918 = 0xcdb348;
  uRam00000000071a35b8 = in_XMM1_Dc;
  uRam00000000071a35bc = in_XMM1_Dd;
  _DAT_071a36a0 = _DAT_071a35b0;
  uRam00000000071a36a4 = uRam00000000071a35b4;
  uRam00000000071a36a8 = in_XMM1_Dc;
  uRam00000000071a36ac = in_XMM1_Dd;
  uRam00000000071a38a8 = in_XMM0_Dc;
  uRam00000000071a38ac = in_XMM0_Dd;
  func_0x033b1870(&DAT_071a3918,&UNK_007a7c92,0x240,0xd2639628);
  func_0x033b1a30(&UNK_02fd7230,&UNK_02fd71c0,&UNK_007a7c92,&UNK_0091a3a8,0,0);
  return 0;
}



