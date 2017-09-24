/*****************************************************************************/
/*                 G E N E R A T E D       C    C O D E                      */
/*****************************************************************************/
/* KIELER - Kiel Integrated Environment for Layout Eclipse RichClient        */
/*                                                                           */
/* http://www.informatik.uni-kiel.de/rtsys/kieler/                           */
/* Copyright 2014 by                                                         */
/* + Kiel University                                                         */
/*   + Department of Computer Science                                        */
/*     + Real-Time and Embedded Systems Group                                */
/*                                                                           */
/* This code is provided under the terms of the Eclipse Public License (EPL).*/
/*****************************************************************************/
char URITO;
char VPace;
char URI_start;
char LRI_stop;
char ASense;
char VSense;
char AVITO;
char PVARPTO;
char VRPTO;
char AEITO;
char LRITO;
char APace;
char AVI_start;
char PVARP_start;
char VRP_start;
char AEI_start;
char LRI_start;
char AVI_stop;
char AEI_stop;
char _DDDPacemaker_local_URI_ex;
char _DDDPacemaker_local_PVARP_ex;
char _DDDPacemaker_local_VRP_ex;
char g0;
char g1;
char g2;
char g3;
char PRE_g3;
char g4;
char g4b;
char g5;
char g6;
char PRE_g6;
char g7;
char g8;
char g9;
char PRE_g9;
char g10;
char g10b;
char g11;
char g12;
char g13;
char PRE_g13;
char g14;
char g14b;
char g15;
char g16;
char g17;
char g18;
char g19;
char PRE_g19;
char g20;
char g21;
char g22;
char PRE_g22;
char g23;
char g24;
char g25;
char g26;
char g27;
char g28;
char PRE_g28;
char g29;
char g30;
char g31;
char PRE_g31;
char g32;
char g33;
char g34;
char g35;
char g36;
char g37;
char PRE_g37;
char g38;
char g39;
char g40;
char PRE_g40;
char g41;
char g42;
char g43;
char PRE_g43;
char g44;
char g44b;
char g45;
char g46;
char g47;
char g48;
char g49;
char g50;
char g51;
char PRE_g51;
char g52;
char g53;
char g54;
char PRE_g54;
char g55;
char g55b;
char g56;
char PRE_g56;
char g57;
char g57b;
char g58;
char g59;
char g60;
char g61;
char g62;
char g63;
char PRE_g63;
char g64;
char g65;
char g66;
char PRE_g66;
char g67;
char g68;
char g69;
char g70;
char g71;
char g72;
char PRE_g72;
char g73;
char g74;
char g75;
char g76;
char g77;
char PRE_g77;
char g78;
char g79;
char g80;
char _GO;
char _cg10;
char _cg16;
char _cg4;
char _cg12;
char _cg7;
char _cg11;
char _cg14;
char _cg25;
char _cg20;
char _cg23;
char _cg34;
char _cg29;
char _cg32;
char _cg44;
char _cg48;
char _cg38;
char _cg46;
char _cg41;
char _cg45;
char _cg57;
char _cg59;
char _cg52;
char _cg58;
char _cg69;
char _cg64;
char _cg67;
char g17_e1;
char g26_e2;
char g35_e3;
char g49_e4;
char g61_e5;
char g70_e6;
char g74_e7;
char g75_e1_fix;
char g75_e1;
char g79_e2;
int _PRE_GO;
void reset(){
   _GO = 1;
   _PRE_GO = 0;
   PRE_g3 = 0;
   PRE_g6 = 0;
   PRE_g9 = 0;
   PRE_g13 = 0;
   PRE_g19 = 0;
   PRE_g22 = 0;
   PRE_g28 = 0;
   PRE_g31 = 0;
   PRE_g37 = 0;
   PRE_g40 = 0;
   PRE_g43 = 0;
   PRE_g51 = 0;
   PRE_g54 = 0;
   PRE_g56 = 0;
   PRE_g63 = 0;
   PRE_g66 = 0;
   PRE_g72 = 0;
   PRE_g77 = 0;
   return;
}
void tick(){
   if(_PRE_GO == 1){
      _GO = 0;
   }
   {
      g0 = _GO;
      g1 = g0;
      g2 = g1;
      g76 = g0;
      g78 =(PRE_g77);
      g77 =(g76||g78);
      if(g77){
         APace = 0;
         VPace = 0;
         AVI_start = 0;
         PVARP_start = 0;
         VRP_start = 0;
         AEI_start = 0;
         LRI_start = 0;
         URI_start = 0;
         AVI_stop = 0;
         AEI_stop = 0;
         LRI_stop = 0;
      }
      g73 =(PRE_g72);
      g71 = g1;
      g72 =(g73||g71);
      if(g72){
         _DDDPacemaker_local_PVARP_ex = 0;
         _DDDPacemaker_local_VRP_ex = 0;
         _DDDPacemaker_local_URI_ex = 0;
      }
      g67 =(PRE_g66);
      _cg67 = URITO;
      g68 =(g67&&_cg67);
      if(g68){
         _DDDPacemaker_local_URI_ex =(_DDDPacemaker_local_URI_ex||1);
      }
      g14 =(PRE_g13);
      g14b = g14;
      _cg14 = _DDDPacemaker_local_URI_ex;
      g15 =(g14b&&_cg14);
      if(g15){
         VPace =(VPace||1);
      }
      g23 =(PRE_g22);
      _cg23 = PVARPTO;
      g24 =(g23&&_cg23);
      if(g24){
         _DDDPacemaker_local_PVARP_ex =(_DDDPacemaker_local_PVARP_ex||1);
      }
      g10 =(PRE_g9);
      g10b = g10;
      _cg10 = _DDDPacemaker_local_PVARP_ex;
      g41 =(PRE_g40);
      _cg41 = ASense;
      g45 =(g41&&(!(_cg41)));
      _cg45 = APace;
      g46 =(g45&&(!(_cg45)));
      _cg46 = AEITO;
      g47 =(g46&&_cg46);
      if(g47){
         APace =(APace||1);
      }
      g4 =(PRE_g3);
      g4b = g4;
      _cg4 = APace;
      g16 =(g4b&&(!(_cg4)));
      _cg16 = ASense;
      g3 =(g15||g2||(g10b&&_cg10)||(g16&&(!(_cg16))));
      g5 =((g4b&&_cg4)||(g16&&_cg16));
      if(g5){
         AVI_start =(AVI_start||1);
      }
      g52 =(PRE_g51);
      _cg52 = VSense;
      g58 =(g52&&(!(_cg52)));
      _cg58 = VPace;
      g59 =(g58&&(!(_cg58)));
      _cg59 = LRITO;
      g60 =(g59&&_cg59);
      if(g60){
         VPace =(VPace||1);
      }
      g7 =(PRE_g6);
      _cg7 = VSense;
      g11 =(g7&&(!(_cg7)));
      _cg11 = VPace;
      g12 =(g11&&(!(_cg11)));
      _cg12 = AVITO;
      g6 =((g12&&(!(_cg12)))||g5);
      g8 =((g11&&_cg11)||(g7&&_cg7));
      if(g8){
         AVI_stop =(AVI_stop||1);
      }
      g9 =(g8||(g10b&&(!(_cg10))));
      g13 =((g12&&_cg12)||(g14b&&(!(_cg14))));
      g18 = g1;
      g20 =(PRE_g19);
      _cg20 = VSense;
      g25 =(g20&&(!(_cg20)));
      _cg25 = VPace;
      g19 =(g24||(g25&&(!(_cg25)))||g18);
      g21 =((g25&&_cg25)||(g20&&_cg20));
      if(g21){
         PVARP_start =(PVARP_start||1);
      }
      g22 =(g21||(g23&&(!(_cg23))));
      g27 = g1;
      g29 =(PRE_g28);
      _cg29 = VSense;
      g34 =(g29&&(!(_cg29)));
      _cg34 = VPace;
      g32 =(PRE_g31);
      _cg32 = VRPTO;
      g33 =(g32&&_cg32);
      if(g33){
         _DDDPacemaker_local_VRP_ex =(_DDDPacemaker_local_VRP_ex||1);
      }
      g28 =(g27||(g34&&(!(_cg34)))||g33);
      g30 =((g29&&_cg29)||(g34&&_cg34));
      if(g30){
         VRP_start =(VRP_start||1);
      }
      g31 =(g30||(g32&&(!(_cg32))));
      g36 = g1;
      g44 =(PRE_g43);
      g44b = g44;
      _cg44 = _DDDPacemaker_local_VRP_ex;
      g38 =(PRE_g37);
      _cg38 = VSense;
      g48 =(g38&&(!(_cg38)));
      _cg48 = VPace;
      g37 =(g36||(g44b&&_cg44)||g47||(g48&&(!(_cg48))));
      g39 =((g38&&_cg38)||(g48&&_cg48));
      if(g39){
         AEI_start =(AEI_start||1);
      }
      g40 =(g39||(g46&&(!(_cg46))));
      g42 =((g41&&_cg41)||(g45&&_cg45));
      if(g42){
         AEI_stop =(AEI_stop||1);
      }
      g43 =((g44b&&(!(_cg44)))||g42);
      g50 = g1;
      g57 =(PRE_g56);
      g57b = g57;
      _cg57 = _DDDPacemaker_local_VRP_ex;
      g51 =((g57b&&_cg57)||g50||(g59&&(!(_cg59))));
      g53 =((g52&&_cg52)||(g58&&_cg58));
      if(g53){
         LRI_stop =(LRI_stop||1);
      }
      g54 =(g53||g60);
      g55 =(PRE_g54);
      g55b = g55;
      if(g55b){
         LRI_start =(LRI_start||1);
      }
      g56 =(g55b||(g57b&&(!(_cg57))));
      g62 = g1;
      g64 =(PRE_g63);
      _cg64 = VSense;
      g69 =(g64&&(!(_cg64)));
      _cg69 = VPace;
      g63 =(g68||(g69&&(!(_cg69)))||g62);
      g65 =((g64&&_cg64)||(g69&&_cg69));
      if(g65){
         URI_start =(URI_start||1);
      }
      g66 =(g65||(g67&&(!(_cg67))));
      g17_e1 =(!((g4||g7||g10||g14)));
      g26_e2 =(!((g20||g23)));
      g35_e3 =(!((g29||g32)));
      g49_e4 =(!((g38||g41||g44)));
      g61_e5 =(!((g52||g55||g57)));
      g70_e6 =(!((g64||g67)));
      g74_e7 =(!(g73));
      g75_e1_fix =(g32||g41||g55||g67||g73||g57||g10||g44);
      g75_e1 =(!((g4||g20||g29||g38||g52||g64||g7||g23||g14||g75_e1_fix)));
      g79_e2 =(!(g78));
   }
   PRE_g3 = g3;
   PRE_g6 = g6;
   PRE_g9 = g9;
   PRE_g13 = g13;
   PRE_g19 = g19;
   PRE_g22 = g22;
   PRE_g28 = g28;
   PRE_g31 = g31;
   PRE_g37 = g37;
   PRE_g40 = g40;
   PRE_g43 = g43;
   PRE_g51 = g51;
   PRE_g54 = g54;
   PRE_g56 = g56;
   PRE_g63 = g63;
   PRE_g66 = g66;
   PRE_g72 = g72;
   PRE_g77 = g77;
   _PRE_GO = _GO;
   return;
}
