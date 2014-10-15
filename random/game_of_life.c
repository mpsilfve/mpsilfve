                                            #include  <stdlib.h>
                                            #include <stdio.h>//
                                            #include <time.h>///
                                            #define A 20//Glidrs
                                            #define B (A*A)//are
                                            //the smallest space  
                                            //ships. NB need c99

int main(void){srand                        (time(0));//mpsilfve
char c[2*B];for( int                        i=0;i < B; ++i) c[i]
= rand() % 2; printf                        ( "Press enter for "
"next iteration and"                        " Ctrl-D to quit.\n"
);for (int i = 0;i==                        0 ||  (i % B)  ||  (
getchar()  !=  EOF);                        ++i) { int n =  i  %
(2*B); int x = (n  %                        B)  % A; int y = (n%

                      B) /A; printf("%s%s"  , (c[n] ? "*" : " ")
                      ,((x+1)% A? "":"|\n"  )); int lc =  (x > 0
                      ?c[n-1]:0) + (x < A-  1 ?  c[n+ 1]:0); for
                      (int z = (x -1 > 0 ?  x-1: 0); z < (A<x+2?
                      A:x+2); ++z)lc+=(y>1  ?c[n-A-x+z]:0)+(y<A-
                      1?c[n+A-x+z]:0);c[(n  +B)%(2*B) ] = (lc ==
                      3|| c[n] &&  lc == 2  ) ; }  return 0 ;  }
