#include "CUBE-STRUCT.hpp"


bool cube_cheaker      (face array[6]);         
void printcube         (face array[6]);         
bool tester            (face     temp);             
void l1_s1_bt          (face   * ctrl);           
void l1_s1_frbm        (face * ctrl);
void lu_tl_ld (face *Hctrl);                           
void ru_tr_rd (face *Hctrl);
void l1_s2_corsolver   (face * ctrl, short ele );
void l2_solver(face * ctrl, char flag);
bool isSecondLayerSolved();
bool is_l3_cross_ready ();
bool is_l3_allignment_ready ();
void l3_cross (face * ctrl);
void l3_alignment (face * ctrl);
void l3_corner_align_case_1 (face * ctrl);
void l3_corner_align_case_2 (face * ctrl);
bool is_l3_corner_alligned_with_adj_centers_from_top (face *ctrl, short i);
bool is_l3_corner_alligned_with_adj_centers_from_other(face *ctrl, short i);


int main() 

{
  
  bool tst;
  face * control;

  SETUP_CUBE('D'); //creates the cube and selects the mode of input (I=(INPUT) or D=(DEFAULT))
 
  printcube(side); 

  //                                     !!!!!!!! SOLVING FIRST LAYER !!!!!!!!
  cout<<"LAYER -1 STEP -1\n\n";
  //  STEP 1 (CREATING PLUS WITH MID PIECES)

  do{ 	
    for (short i=0; i<=3; i++)                            //bottom face 
      {

        if (side[bt].mid[i].col == side[bt].center && *side[bt].mid[i].com != side[bt].adj[i]->center)
        {
          control=side[bt].adj[i];
          control->FR();
          control->FR();
          l1_s1_bt(control);
          control=NULL;
        }
      }

    do{
        for (short i=0; i<=3; i++)     //this is for top face only
        {
          if (side[tp].mid[i].col == side[bt].center)
          {
            control = side[tp].adj[i];
            l1_s1_bt (control);
            control = NULL;
          }
        }
        
        tst = 0;
        for (short i=0;i<=3; i++)
        {
          if (side[tp].mid[i].col == side[bt].center)

            {  tst=1;
                break; }
        }
      }while (tst);
      
    for (short i=2; i<=5; i++) {          //other adj faces
    for (short j=0; j<=3; j++)
      {
        if (side[i].mid[j].col == side[bt].center)
        {
          if (*side[i].mid[j].com == side[i].adj[j]->center)
          {
            switch (j)
            {                
              case rm : { side[i].RD(); 
                          break;	      }
                    
              case tm : { side[i].FR();
                          side[i].FR();
                          break;        }
                                              
              case lm : { side[i].LD(); 
                          break;		    }            
            }            
          }
          else
          {
            switch(j)
            {
              case tm : {
                          l1_s1_frbm(&side[i]);
                          break;
                        }
              case rm : {
                          side[i].RU();
                          side[i].TL();
                          side[i].RD();
                          l1_s1_bt(&side[i]);
                          break;                     
                        }
              case bm : {
                          side[i].FR();
                          side[i].FR();
                          l1_s1_frbm(&side[i]);
                          break;
                        }
              case lm : {
                          side[i].LU();
                          side[i].TR();
                          side[i].LD();
                          l1_s1_bt(&side[i]);
                          break;
                        }
            }     
          }
        }
      }
    }  

    tst=0;
    for (short i=0; i<=3; i++)
      {
        if (side[bt].mid[i].col != side[bt].center)
        {
          tst=1;
          break;
        }
      }
    } while (tst);


  printcube (side);




  //                                      STEP -2   (COMPLETING TOP LAYER AND ITS ADJS)
  cout<<"\nLAYER -1 STEP -2 \n\n";

  while ( !tester(side[bt]) )
  {
    for (short i=0; i<=3; i++)
    {
      if (side[bt].cor[i].col == side[bt].center && side[bt].adj[i]->cor[tr].col != side[bt].adj[i]->center)
      {
        control = side[bt].adj[i];
        control->LU();
        control->TR();
        control->LD();
        control = control->adj[RIGHT];
        l1_s2_corsolver (control,tl);
        control = NULL;
      }
    }

    for (short i=0; i<=3; i++)
    {
      if (side[tp].cor[i].col == side[bt].center)
      {
        control = side[tp].adj[i];
        ru_tr_rd(control);
        control = control->adj[LEFT];
        l1_s2_corsolver(control,tl);
        control = NULL;

      }
    }

    for (short j=2; j<=5; j++)
    for (short i=0; i<=3; i++)
    {
      if (side[j].cor[i].col == side[bt].center)
      {
        switch (i)
        {
          case tl : { l1_s2_corsolver ( &side[j], tl) ;
                      break;
                    };      

          case tr : { l1_s2_corsolver ( &side[j], tr) ;
                      break; 
                    };

          case bl :{
                      lu_tl_ld ( &side[j]);
                      l1_s2_corsolver ( &side[j], tl);
                      break;
                  };

          case br :{
                      ru_tr_rd ( &side[j]);
                      l1_s2_corsolver ( &side[j], tr);
                      break;
                  };
          
        }
      }
    }
  }

  printcube (side);  



  //=============================================================================================================

  //                              !!!!!!SOLVING SECOND LAYER!!!!!!

  cout<<"\nLAYER 2 SOLVING\n\n";

  while (isSecondLayerSolved())
  {

    bool no_convinient_piece_on_top = true;

    for (short i=tm;i<=lm;i++)
      {
        if (side[tp].mid[i].col != side[tp].center && *side[tp].mid[i].com != side[tp].center)
        {

          no_convinient_piece_on_top = false;

          control=side[tp].adj[i];

          while(control->mid[tm].col != control->center)
          {
            control->TR();
            control=control->adj[RIGHT];
          }

          if (*control->mid[tp].com == control->adj[RIGHT]->center)
          {
            control->TL();
            l2_solver(control,'r');
            control=NULL;
          }

          else
          {
            control->TR();
            l2_solver(control,'l');
            control=NULL;
          }

        }
      }
    
    if (no_convinient_piece_on_top)
    {
      for (short i=fr;i<=lt;i++)
        {
          if (side[i].mid[rm].col != side[i].center && *side[i].mid[rm].com != side[i].adj[RIGHT]->center)
            {
              l2_solver(&side[i],'r');
              break;
            }

          else if (side[i].mid[lm].col != side[i].center && *side[i].mid[lm].com != side[i].adj[LEFT]->center)
            {
              l2_solver(&side[i],'l');
              break;
            }

            else
              continue;
        }
    }    

  }

  printcube(side);


  //==============================================================================================================

  //                               !!!!!!!!!!!!!!LAYER 3 SOLVING!!!!!!!!!!!!!!!
  //                                              FINAL LAYER

  cout<<"\n FINAL LAYER\n";

  cout<<"\nCROSS\n\n";
  //                                    SOLVING CROSS
  while(is_l3_cross_ready())
  {
    tst=true;   // for cheacking if solved using L OR ---
    for (short i=fr;i<=lt;i++)
      {
        if(*side[i].adj[LEFT]->mid[tp].com == side[tp].center && *side[i].adj[OPPO]->mid[tp].com == side[tp].center)
        {
          tst=false;
          l3_cross(&side[i]);
          l3_cross(&side[i]);
          break;
        }

        else if(*side[i].adj[LEFT]->mid[tp].com == side[tp].center && *side[i].adj[RIGHT]->mid[tp].com == side[tp].center)
        {
          tst=false;
          l3_cross(&side[i]);
          break;
        }

        else 
          continue;

      }

      if (tst)
      {
        l3_cross(&side[fr]);
      }

  } 

  printcube(side);

  //                                        ALIGNMENT

  cout<<"\nallignment\n\n";

  tst=true; // if entered in case 1 and 2 which solves it completely

  while (is_l3_allignment_ready ())
  {
    for (short i=fr;i<=lt;i++)
    {
      if (side[i].mid[tm].col == side[i].center)
      {
        if (side[i].adj[LEFT]->mid[tm].col == side[i].adj[LEFT]->center) //case 1
        {
          control=side[i].adj[RIGHT];
          l3_alignment(control);
          control=NULL;
          tst=false;
          break;
        }

        else if (side[i].adj[RIGHT]->mid[tm].col == side[i].adj[RIGHT]->center) //case 2
        {
          control=side[i].adj[OPPO];
          l3_alignment(control);
          control=NULL;
          tst=false;
          break;
        }

        else if (side[i].adj[OPPO]->mid[tm].col == side[i].adj[OPPO]->center) //case 3
        {
          control=side[i].adj[RIGHT];
          l3_alignment(control);
          control=NULL;
          break;
        }
      }
    }

    if (tst)
      side[tp].FL();

  } 

  printcube (side);

  //                                     CORNER ALLIGNMENT
  //                                 !!!!!!LAST ALGORITHM!!!!!
  cout<<"\n\ncorner allignment\n\n";
  
  while (!cube_cheaker(side))
  {
    for(short i=tl;i<=bl;i++)
    {
      tst=true;  //cheaking if modification needed to achieve any corner alignment

      //checking if elements in corner pice same as of centers
      if (is_l3_corner_alligned_with_adj_centers_from_top(&side[tp],i))
      {
        tst=false;
        control=side[tp].adj[i];

        while(control->cor[tr].col != control->center)
        {
          l3_corner_align_case_1(control);
        }

        while (! is_l3_corner_alligned_with_adj_centers_from_other(control->adj[RIGHT],tr))
        {
          l3_corner_align_case_2(control);
        }

        control=control->adj[RIGHT];

        while(control->cor[tr].col != control->center)
        {
          l3_corner_align_case_1(control);
        }
        control=control->adj[RIGHT];

        while(control->cor[tr].col != control->center)
        {
          l3_corner_align_case_1(control);
        }
        control=NULL;
        break;
      }
      
    }

    if(tst)
    {
      l3_corner_align_case_2 (&side[rt]);
    }

  } 

  printcube (side);

  getchar();
  return 0;
}





















                            //FUNCTIONS!!!!!!


void printcube (face array[6])     //TO PRINT THE CUBE
  {
    for(short i=0;i<6;i++) 
    {
      array[i].output();
    }
  }
  


bool tester (face temp)           //TO CHEAK IF A FACE IS SOLVED
  {
    char array[8];
    short iter=0;

      for(short i=0;i<=3;i++)
      {        
        array[iter]=temp.cor[i].col;
        iter++;
        array[iter]=temp.mid[i].col;
        iter++;        
      }

    bool test=1;
      for(short i=0;i<=7;i++)
      {
        if(array[i]!=temp.center)
        {
            test=0;
            break;            
        }
      }
    return test;
  }



bool cube_cheaker (face array[6])  //TO CHEAK IF THE CUBE IS SOLVED
  {
    
    bool test[6];
    bool test1=1;

      for(short i=0;i<=5;i++)
      {
        test[i]=tester(array[i]);
      }
      for(short i=0;i<=5;i++)
      {
        if (test[i]==0)
        {
          test1=0;
          break;
        }
      }
    return test1;
  }



void l1_s1_bt (face * ctrl)       //layer 1 , step 1, bottom side
  {
    if (ctrl->center != ctrl->mid[tm].col)
      {

          if (ctrl->adj[LEFT]->center==ctrl->mid[tm].col) {
            ctrl->TL();
            ctrl=ctrl->adj[LEFT];
          }
          else if (ctrl->adj[RIGHT]->center==ctrl->mid[tm].col) {
            ctrl->TR();
            ctrl=ctrl->adj[RIGHT];        
          }
          else {
            ctrl->TR();
            ctrl->TR();
            ctrl=ctrl->adj[OPPO];
          }
        ctrl->FR();
        ctrl->FR();
      }
    else
      {       
        ctrl->FR();
        ctrl->FR();
      }
  }



void l1_s1_frbm (face * ctrl)    //1 layer step1 front bottom mid (in ourcase topmids)
  {
    if (*ctrl->mid[tm].com != ctrl->center)
    {
        if (ctrl->adj[RIGHT]->center == *ctrl->mid[tm].com) {
          ctrl->FR();
          ctrl->RD();
          ctrl->FL();
          
        }
        else if (ctrl->adj[LEFT]->center == *ctrl->mid[tm].com) {
          ctrl->FL();
          ctrl->LD();
          ctrl->FR();
         
        }
        else {
          ctrl->TL();
          ctrl->adj[LEFT]->FL();
          ctrl->adj[LEFT]->LD();
          ctrl->adj[LEFT]->FR();
        }
    }
    else
    { 
      ctrl->TR();
      ctrl->RD();
      ctrl->FR();
      ctrl->RU();  
    }  
  }



void l1_s2_corsolver (face * ctrl, short ele )
  {
    char tester = *ctrl->cor[ele].vcom;

    if (ele == tl) {

      if(ctrl->adj[LEFT]->center == tester) {

        ctrl = ctrl->adj[LEFT];
        lu_tl_ld (ctrl);
      }
      else if (ctrl->adj[RIGHT]->center == tester) {

        ctrl = ctrl->adj[RIGHT];
        ctrl->TR();
        ctrl->TR();
        lu_tl_ld (ctrl);
      }
      else if (ctrl->adj[OPPO]->center == tester) {

        ctrl = ctrl->adj[OPPO];
        ctrl->TL();
        lu_tl_ld (ctrl);
      }
      else {

        ctrl->TR();
        lu_tl_ld (ctrl);
      }

    }
    else if (ele == tr) {

      if (ctrl->adj[LEFT]->center == tester) {

        ctrl = ctrl->adj[LEFT];
        ctrl->TL();
        ctrl->TL();
        ru_tr_rd (ctrl);
      }
      else if (ctrl->adj[RIGHT]->center == tester) {

        ctrl = ctrl->adj[RIGHT];
        ru_tr_rd (ctrl); 
      }
      else if (ctrl->adj[OPPO]->center == tester) {

        ctrl = ctrl->adj[OPPO];
        ctrl->TR();
        ru_tr_rd (ctrl); 
      }
      else {
        ctrl->TL();
        ru_tr_rd (ctrl); 
      }
    }
  } 
void lu_tl_ld (face *Hctrl)
{
  Hctrl->LU();
  Hctrl->TL();
  Hctrl->LD();
}
void ru_tr_rd (face *Hctrl)
{
  Hctrl->RU();
  Hctrl->TR();
  Hctrl->RD();
}  


void l2_solver(face * ctrl,char flag)
{
  if( flag == 'r')
  {
    ru_tr_rd(ctrl);
    ctrl=ctrl->adj[RIGHT];
    ctrl->TR();
    ctrl->LU();
    ctrl->TL();
    ctrl->LD();
  }
  else if(flag == 'l')
  {
    lu_tl_ld(ctrl);
    ctrl=ctrl->adj[LEFT];
    ctrl->TL();
    ctrl->RU();
    ctrl->TR();
    ctrl->RD();
  }
}


bool isSecondLayerSolved()
{
  bool flag = false;
  for (short i=fr;i<=lt;i++)  //itering through faces front - left
  {
    if (side[i].mid[rm].col != side[i].center || side[i].mid[lm].col != side[i].center)
    {
      flag = true;
    }
  }
  return flag;
}


void l3_cross (face *ctrl)
{
  ctrl->FR();
  ctrl->RU();
  ctrl->TL();
  ctrl->RD();
  ctrl->TR();
  ctrl->FL();
}


void l3_alignment (face * ctrl)
{
  ctrl->RU();
  ctrl->TL();  
  ctrl->TL();
  ctrl->RD(); 
  ctrl->TR();
  ctrl->RU();
  ctrl->TR();
  ctrl->RD();
  ctrl->TR();
}


void l3_corner_align_case_1 (face * ctrl)
{
  ctrl->RU();
  ctrl->TL();  
  ctrl->TL();
  ctrl->RD(); 
  ctrl->TR();
  ctrl->RU();
  ctrl->TR();
  ctrl->RD();
  ctrl->LU();
  ctrl->TR();
  ctrl->TR();
  ctrl->LD();
  ctrl->TL();
  ctrl->LU();
  ctrl->TL();
  ctrl->LD();
}


void l3_corner_align_case_2 (face * ctrl)
{
  ctrl->LU();
  ctrl->TL();
  ctrl->RU();
  ctrl->TR();
  ctrl->LD();
  ctrl->TL();
  ctrl->RD();
  ctrl->TR();
}


bool is_l3_cross_ready ()
{
  for (short i=tm;i<=lm;i++)
  {
    if(side[tp].mid[i].col != side[tp].center)
    {
      return true;
    }      
  }
  return false;
}


bool is_l3_allignment_ready ()
{
  for (short i=fr;i<=lt;i++)
  {
    if (side[i].mid[tm].col != side[i].center)
    {
      return true;
    }
  }
  return false;
}

bool is_l3_corner_alligned_with_adj_centers_from_top (face* ctrl, short i)
{
  int cornerHash = int(ctrl->cor[i].col)+int(*ctrl->cor[i].vcom)+int(*ctrl->cor[i].hcom);
  int centerHash = int(ctrl->center)+int(ctrl->adj[i]->center)+int(ctrl->adj[i]->adj[RIGHT]->center);

  if (cornerHash==centerHash)
    return true;
  else
    return false;
}

bool is_l3_corner_alligned_with_adj_centers_from_other(face *ctrl, short i)
{
  int cornerHash = int(ctrl->cor[i].col)+int(*ctrl->cor[i].vcom)+int(*ctrl->cor[i].hcom);
  int centerHash = int(ctrl->center)+int(ctrl->adj[UP]->center)+int(ctrl->adj[RIGHT]->center);

  if (cornerHash==centerHash)
    return true;
  else
    return false;
}