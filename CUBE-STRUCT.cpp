#include <iostream>
using namespace std;

void SETUP_CUBE(); //MASTER FUNCTION (CREATES THE CUBE USING 6 FACES)

enum {UP=0,RIGHT,DOWN,LEFT,OPPO};        //FOR ADJS
enum {tl=0,tr,br,bl};            		 //FOR CORNERS
enum {tm=0,rm,bm,lm};              		 //FOR MIDDLE
enum {tp=0,bt,fr,rt,bk,lt};              //FOR SIDES

int counter=1;  //for counting number of steps completed while solving the cube
 

/*@@@@@    @     @@@@  @@@@@      @@@@  @@@@@  @@@@   @   @   @@@@  @@@@@
  @       @ @   @      @         @        @    @   @  @   @  @        @
  @@@    @@@@@  @      @@@       @@@@@    @    @@@@   @   @  @        @
  @      @   @  @      @             @    @    @ @    @   @  @        @
  @      @   @   @@@@  @@@@@     @@@@     @    @  @@   @@@    @@@@    @  */

//a face of the cube is created by the face class

class face  
{

    private:

        struct cornor        //for corner elements
        {  

            char col;        //color of the element
            char* vcom=NULL; //address of its vertical companion
            char* hcom=NULL; //address of its horizontal companion
        };

        struct middle        //for middle elements   
        {   

            char col;
            char*com=NULL;
        };  

    public:

    cornor cor[4]; //creating 4 corner and 4 middle element
    middle mid[4];
    char center;
    face* adj[5]; //address of 4 adjecent faces and opposite face


    face (const char array[10]) //construcor function for initialising elements of a face (10th for \0)
    { 
        
		short iter=0;
        for(short i=0;i<=3;i++) 
        {
           cor[i].col=array[iter];
           iter++;
           mid[i].col=array[iter];
           iter++;
        }
        center=array[8];
    }   


    void setadj (face *_1,face *_2,face *_3,face *_4,face *_5)   //setting adj faces
    {     
        
       adj[UP]    = _1;
       adj[RIGHT] = _2;
       adj[DOWN]  = _3;
       adj[LEFT]  = _4;
       adj[OPPO]  = _5;
    } 


    void output() //output of a face
    {
		const char* array;
		switch (center) 
		{
			case 'R':array="RED";
			break;
			case 'O':array="ORANGE";
			break;
			case 'Y':array="YELLOW";
			break;
			case 'W':array="WHITE";
			break;
			case 'B':array="BLUE";
			break;
			case 'G':array="GREEN";
			break;
		}	
        cout<<"\n\n\n"<<array<<" FACE\n"<<"  "<<cor[tl].col<<" "<<mid[tm].col<<" "<<cor[tr].col<<"\n"<<"  "<<mid[lm].col<<" "<<center<<" "<<mid[rm].col<<"\n"<<"  "<<cor[bl].col<<" "<<mid[bm].col<<" "<<cor[br].col<<"\n\n";
    } 


    void setcompanions (char *_1,char *_2,char *_3,char *_4,char *_5,char *_6,char *_7,char *_8,char *_9,char *_10,char *_11,char *_12)  //vcom    hcom   com  all next by next in continuous order
                             
    {
         char* comp[12]={_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12};
         short iter=0;
            for (short i=0;i<=3;i++)
            {                           
                cor[i].vcom=comp[iter];
                iter++;
                cor[i].hcom=comp[iter];
                iter++;
                mid[i].com=comp[iter];
                iter++;
            }
    }


    void FL() //rotate the face to left (FL -> "FRONT LEFT")
    {
        cout<<counter++<<" "<<center<<"-FRONT LEFT\n";
        char buf=cor[0].col , buf2=mid[0].col;
        char vbuf=*cor[0].vcom , hbuf=*cor[0].hcom , mbuf=*mid[0].com; 

        for (short i=0;i<=3;i++) 
        {
            if(i==3)
            {
                cor[i].col=buf;
                mid[i].col=buf2;
                *cor[i].vcom=hbuf;
                *cor[i].hcom=vbuf;
                *mid[i].com=mbuf;
            }
            else
            {
                cor[i].col=cor[i+1].col;
                mid[i].col=mid[i+1].col;
                *cor[i].vcom=*cor[i+1].hcom;
                *cor[i].hcom=*cor[i+1].vcom;
                *mid[i].com=*mid[i+1].com;
            }
        }      
    }   


    void FR() //rotate the face to right (FR -> "FRONT RIGHT")
    {
        cout<<counter++<<" "<<center<<"-FRONT RIGHT\n";
        char buf=cor[3].col , buf2=mid[3].col;
        char vbuf=*cor[3].vcom , hbuf=*cor[3].hcom , mbuf=*mid[3].com; 

        for (short i=3;i>=0;i--) 
        {
            if(i==0)
            {
                cor[i].col=buf;
                mid[i].col=buf2;
                *cor[i].vcom=hbuf;
                *cor[i].hcom=vbuf;
                *mid[i].com=mbuf;
            }
            else
            {
                cor[i].col=cor[i-1].col;
                mid[i].col=mid[i-1].col;
                *cor[i].vcom=*cor[i-1].hcom;
                *cor[i].hcom=*cor[i-1].vcom;
                *mid[i].com=*mid[i-1].com;
            }
        }         
    }   


    void TL() {             //top left
        adj[UP]->FR();
    }   

    void TR() {             //top right
        adj[UP]->FL();
    } 

    void BL() {             //bottom left
        adj[DOWN]->FL();
    } 

    void BR() {             //bottom right
        adj[DOWN]->FR();
    } 

    void LU() {             //left up
        adj[LEFT]->FL();
    } 

    void LD() {             //left down
        adj[LEFT]->FR();
    }  

    void RU() {             //right up
        adj[RIGHT]->FR();
    }  

    void RD() {             //right down
        adj[RIGHT]->FL();
    }

};



/* @@@@  @   @  @@@@   @@@@@      @@@@  @@@@@  @@@@   @   @   @@@@  @@@@@
  @      @   @  @   @  @         @        @    @   @  @   @  @        @
  @      @   @  @@@@   @@@       @@@@@    @    @@@@   @   @  @        @
  @      @   @  @   @  @             @    @    @ @    @   @  @        @
   @@@@   @@@   @@@@   @@@@@     @@@@     @    @  @@   @@@    @@@@    @  */




face side[6] //creating six faces and initialising  its elements
  { 
  ("YOGRBYWRR"),//TOP FACE ELEMENTS 
  ("GOOWBWWRO"),//BOTTOM FACE ELEMENTS (oppo of top face)   |    1 2 3
  ("RGRROBWOW"),//FRONT FACE ELEMENTS                       |    8 9 4
  ("YBYYOGBWG"),//RIGHT FACE ELEMENTS                       |    7 6 5
  ("RGOYGBWOY"),//BACK FACE ELEMENTS
  ("BYGWRGYBB") //LEFT FACE ELEMENTS
  };




void SETUP_CUBE() //a function that compines the above faces to produce the cube

{
    //setting the companions of each element in a face

    side[tp].setcompanions (&side[bk].cor[tr].col, &side[lt].cor[tl].col, &side[bk].mid[tm].col,     
                            &side[bk].cor[tl].col, &side[rt].cor[tr].col, &side[rt].mid[tm].col,     
                            &side[fr].cor[tr].col, &side[rt].cor[tl].col, &side[fr].mid[tm].col,    
                            &side[fr].cor[tl].col, &side[lt].cor[tr].col, &side[lt].mid[tm].col );  

    side[bt].setcompanions (&side[bk].cor[bl].col, &side[rt].cor[br].col, &side[bk].mid[bm].col,  
                            &side[bk].cor[br].col, &side[lt].cor[bl].col, &side[lt].mid[bm].col,  
                            &side[fr].cor[bl].col, &side[lt].cor[br].col, &side[fr].mid[bm].col,
                            &side[fr].cor[br].col, &side[rt].cor[bl].col, &side[rt].mid[bm].col );

    side[fr].setcompanions ( &side[tp].cor[bl].col, &side[lt].cor[tr].col, &side[tp].mid[bm].col,  
                            &side[tp].cor[br].col, &side[rt].cor[tl].col, &side[rt].mid[lm].col,  
                            &side[bt].cor[bl].col, &side[rt].cor[bl].col, &side[bt].mid[bm].col,
                            &side[bt].cor[br].col, &side[lt].cor[br].col, &side[lt].mid[rm].col );

    side[rt].setcompanions (&side[tp].cor[br].col, &side[fr].cor[tr].col, &side[tp].mid[rm].col,     
                            &side[tp].cor[tr].col, &side[bk].cor[tl].col, &side[bk].mid[lm].col,
                            &side[bt].cor[tl].col, &side[bk].cor[bl].col, &side[bt].mid[lm].col,   
                            &side[bt].cor[bl].col, &side[fr].cor[br].col, &side[fr].mid[rm].col );  

    side[bk].setcompanions (&side[tp].cor[tr].col, &side[rt].cor[tr].col, &side[tp].mid[tm].col,  
                            &side[tp].cor[tl].col, &side[lt].cor[tl].col, &side[lt].mid[lm].col,  
                            &side[bt].cor[tr].col, &side[lt].cor[bl].col, &side[bt].mid[tm].col,
                            &side[bt].cor[tl].col, &side[rt].cor[br].col, &side[rt].mid[rm].col );

    side[lt].setcompanions (&side[tp].cor[tl].col, &side[bk].cor[tr].col, &side[tp].mid[lm].col,  
                            &side[tp].cor[bl].col, &side[fr].cor[tl].col, &side[fr].mid[lm].col,  
                            &side[bt].cor[br].col, &side[fr].cor[bl].col, &side[bt].mid[rm].col,
                            &side[bt].cor[tr].col, &side[bk].cor[br].col, &side[bk].mid[rm].col );

    //connecting the adjecent sides and opposite side to a face (ORDER-TOP,RIGHT,BOTTOM,LEFT,BACK) 

    side[tp].setadj (&side[bk],&side[rt],&side[fr],&side[lt],&side[bt]);   
    side[bt].setadj (&side[bk],&side[lt],&side[fr],&side[rt],&side[tp]);
    side[fr].setadj (&side[tp],&side[rt],&side[bt],&side[lt],&side[bk]);
    side[rt].setadj (&side[tp],&side[bk],&side[bt],&side[fr],&side[lt]);
    side[bk].setadj (&side[tp],&side[lt],&side[bt],&side[rt],&side[fr]);
    side[lt].setadj (&side[tp],&side[fr],&side[bt],&side[bk],&side[rt]);  

}
       
