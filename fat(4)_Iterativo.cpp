// Joao P S L Pereira
// p-code.cpp :  cpp.sh
//
// C conventions
// 0 == FALSE
//
#include <stdio.h>

#define levmax 3
#define amax 2047

enum fct {LIT, OPR, LOD, STO, CAL, INT, JMP, JPC};

typedef struct tinstruction {
     fct    f;
     int    l; // l: 0..levmax; // Level buffer
     int    a; // a: 0..amax;   // Address buffer
}instruction;

instruction code[2048];

//procedure interpret;
// const stacksize = 500;

#define stacksize 5012

 // p, b, t: integer; {program-, base-, topstack-registers}
 int p, // program-register
     b, // base-register
     t; // topstack-register

instruction i;            //: instruction; {instruction register}
int         s[stacksize]; //: array [1..stacksize] of integer; {datastore}

int base(int l){ //l: integer)
 int b1; //

 b1 = b; // {find base l levels down}
 while (l > 0) {
    b1 = s[b1];
    l  = l - 1;
 }
 return b1;
}//end-int-base

// Determina se seu argumento é impar
int odd(int x){ return (  ((x%2)==1) ); }

void pcodevhw(){ // begin
 printf("\n start pl/0");
 printf("\n p   b   t     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14");
 printf("\n === === ===   === === ===             === === === === === === === === === === === === === === ===");
 t = -1; // topstack-register
 b = 0;  // base-register
 p = 0;  // program-register

 // OBS: s[0] must not be used
 s[1] = 0;
 s[2] = 0;
 s[3] = 0;




 do {
  i = code[p];
  //printf("\n %3d %3d %3d - %3d %3d %3d", p, b, t, i.f, i.l, i.a);
  printf("\n %3d %3d %3d   ", p, b, t);
  p = p + 1;
 // switch case i.f-BEGIN
  switch  (i.f) { // with i do case f of
    case LIT : { t = t + 1; s[t] = i.a; printf("LIT %3d %3d", i.l, i.a); } break;
    case OPR :
               printf("OPR %3d %3d", i.l, i.a);
//             switch case i.a-BEGIN
               switch (i.a) { // case a of {operator}
                 case  0: { // return
                            t = b - 1;
                            p = s[t + 3];
                            b = s[t + 2];
                          }
                          break;

                 case  1: { //
                            s[t] = -s[t];
                          }
                          break;

                 case  2: { //  ADD     : PUSH( POP()+POP() )
                            t    = t - 1;
                            s[t] = s[t] + s[t + 1];
                          }
                          break;

                 case  3: { // SUBSTRACT: PUSH( POP()-POP() )
                            t    = t - 1;
                            s[t] = s[t] - s[t + 1];
                          }
                          break;

                 case  4: { // MULTIPLY: PUSH( POP()*POP() )
                            t    = t - 1;
                            s[t] = s[t] * s[t + 1];
                          }
                          break;

                 case  5: { // DIVIDE  : PUSH( POP()/POP() )
                            t    = t - 1;
                            s[t] = s[t] / s[t + 1];
                          }
                          break;

                 case  6: { //
                            s[t] = odd(s[t]); //s[t] = ord(odd(s[t]));
                          }
                          break;

                 case  8: { // EQUAL
                            t    = t - 1;
                            s[t] = (s[t] == s[t + 1]);
                          }
                          break;

                 case  9: { // NOT.EQUAL
                            t    = t - 1;
                            s[t] = (s[t] != s[t + 1]);
                          }
                          break;

                 case 10: { // LESS THAN
                            t    = t - 1;
                            s[t] = (s[t] < s[t + 1]);
                          }
                          break;

                 case 11: { // GREATER EQUAL THAN
                            t    = t - 1;
                            s[t] = (s[t] >= s[t + 1]);
                          }
                          break;

                 case 12: { // GREATER THAN
                            t = t - 1;
                            s[t] = (s[t] > s[t + 1]);
                          }
                          break;

                 case 13: { // LESS EQUAL THAN
                            t    = t - 1;
                            s[t] = (s[t] <= s[t + 1]);
                          }
                          break;
               } // end;
//         switch case i.a-END
           break;

   case LOD : { // LOAD
                printf("LOD %3d %3d", i.l, i.a);
                t    = t + 1;
                s[t] = s[base(i.l) + i.a];
              }
              break;

   case STO : { // STORE
                printf("STO %3d %3d", i.l, i.a);
                s[base(i.l)+i.a] = s[t];
                t            = t - 1;
              }
              break;

   case CAL :
              { // {generate new block mark}
                printf("CAL %3d %3d", i.l, i.a);
                s[t + 1] = base(i.l);
                s[t + 2] = b;
                s[t + 3] = p;
                b        = t + 1;
                p        = i.a;
              }
              break;

   case INT : t = t + i.a;printf("INT %3d %3d", i.l, i.a); break;
   case JMP : p = i.a;    printf("JMP %3d %3d", i.l, i.a); break;
   case JPC : if (s[t] != 0) { p = i.a; }  t = t - 1; printf("JPC %3d %3d", i.l, i.a); break;
  } // end {with, case}
  // switch case i.f-END

  // print stack
  printf("      s[] : ");
  for (int h=0; h<=t; h++) { printf(" %3d", s[h]); }
 } while ( p != 0 );

 printf("\n === === ===   === === ===             === === === === === === === === === === === === === === ===");
 printf("\n p   b   t     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14");
 printf("\n end pl/0");
}//end-void-pcmachine() {interpret};

int main() {
    code[0].f = INT; code[0].l = 0; code[0].a = 7;   // main-INICIO
    code[1].f = LIT; code[1].l = 0; code[1].a = 4;   // n = 4
    code[2].f = STO; code[2].l = 0; code[2].a = 3;   // ARI(3)<-n
    code[3].f = LOD; code[3].l = 0; code[3].a = 3;   // load n
    code[4].f = STO; code[4].l = 0; code[4].a = 10;   // store n in another variable
    code[5].f = LOD; code[5].l = 0; code[5].a = 3;   // load n
    code[6].f = STO; code[6].l = 0; code[6].a = 11;   // store n in another variable
    code[7].f = CAL; code[7].l = 0; code[7].a = 8; // Call fat(n)

    // Função fat(n)
    code[8].f = INT; code[8].l = 0; code[8].a = 5;   // fat-INICIO
    code[9].f = LOD; code[9].l = 0; code[9].a = 3;   // load n
    code[10].f = LIT; code[10].l = 0; code[10].a = 1;   // load 1
    code[11].f = OPR; code[11].l = 0; code[11].a = 8;   // check if n == 1
    code[12].f = JPC; code[12].l = 0; code[12].a = 22;  // jump to the end if n == 1

    // fat(n-1)
    code[13].f = LOD; code[13].l = 0; code[13].a = 3; // load n
    code[14].f = LIT; code[14].l = 0; code[14].a = 1; // load 1
    code[15].f = OPR; code[15].l = 0; code[15].a = 3; // n-1
    code[16].f = STO; code[16].l = 0; code[16].a = 3; // store n-1 for the next recursive call

    // fat(n) = n * fat(n-1)
    code[17].f = LOD; code[17].l = 0; code[17].a = 3; // load n
    code[18].f = LOD; code[18].l = 0; code[18].a = 4; // load n
    code[19].f = OPR; code[19].l = 0; code[19].a = 4; // n * (n-1)
    code[20].f = STO; code[20].l = 0; code[20].a = 4; // store the result back to n
    code[21].f = JMP; code[21].l = 0; code[21].a = 9; // jump back to the beginning of the loop

    // Return to the main function
    code[22].f = LOD; code[22].l = 0; code[22].a = 11; // load n (result)
    code[23].f = STO; code[23].l = 2; code[23].a = 4; // store the result in the variable t+3 (n)
    code[24].f = OPR; code[24].l = 0; code[24].a = 0; // RETURN // main-FIM

    // Chamada da P-code machine para interpretar essas instruções
    pcodevhw();
    return 0;
}

