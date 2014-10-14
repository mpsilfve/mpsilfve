                                            #include   <stdio.h>
                                            #include    <time.h>
                                            #include  <stdlib.h>
                                            #define A 15 /*GOL*/
                                            #define B (A*A)// IN
                                            /*C99 in the shape*/
                                            /* of  a  glider. */

int main(void){srand                        /* 2014 */(time(0));
char c[2*B];for( int                        i=0;i < B; ++i) c[i]
= rand() % 2; printf                        (  "Press enter for"
"next iteration and"                        "Ctrl-D to quit.\n")
;for (int i = 0; i==                        0 ||  (i % B)  ||  (
getchar()  !=  EOF);                        ++i) { int n =  i  %
(2*B); int x = (n  %                        B)  % A; int y = (n%

                      B) /A; printf("%s%s"  , (c[n] ? "*" : " ")
                      ,((x+1)% A? "":"|\n"  )); int lc =  (x > 0
                      ?c[n-1]:0) + (x < A-  1 ?  c[n+ 1]:0); for
                      (int z = (x -1 > 0 ?  x-1: 0); z < (A<x+2?
                      A:x+2); ++z)lc+=(y>1  ?c[n-A-x+z]:0)+(y<A-
                      1?c[n+A-x+z]:0);c[(n  +B)%(2*B) ] = (lc ==
                      3|| c[n] &&  lc == 2  ) ; }  return 0 ;  }
