#include "colors.inc"
#include "textures.inc"
#include "shapes.inc"
#include "metals.inc"
#include "glass.inc"
#include "woods.inc"

light_source {
<20,30,40>
White
}

camera {
location  <5,5,5>
look_at <0,0,0>  

}  

union {

plane { <0,1,0>, 0   // normal vector, distance to zero ----

        texture{ pigment{ color rgb<0.35,0.60, 0.00> }
	         normal { bumps 0.25 scale 0.05 }
               } 
      } // end of plane ------------------------------------------

sphere { <0,0,0>, 0.5 

        texture { pigment{ color rgb<1.00, 0.55, 0.00>}
                  finish { phong 1.0}
                } // end of texture

          scale<2,2,2>  rotate<0,0,0>  translate<0,0.5,0>  
       }  // end of sphere ----------------------------------- 

}  

intersection {

cylinder { <4,0,0>,<0,2.00,0>, 0.30 

           texture { pigment { color rgb<1,1,1> }
                   //normal  { bumps 0.5 scale <0.005,0.25,0.005>}
                     finish  { phong 0.5 reflection{ 0.00 metallic 0.00} } 
                   } // end of texture

           scale <1,1,1> rotate<0,0,0> translate<0,0,0>
         } // end of cylinder -------------------------------------

box { <0,0,0>,< 1.00, 1.00, 1.00>   

      texture { pigment{ color rgb<1.00, 1.00, 1.00>*1.1}  
                finish { phong 1 reflection{ 0.00 metallic 0.00} } 
              } // end of texture

      scale <2,2,2> rotate<0,0,0> translate<0,0,0> 
    } // end of box --------------------------------------
         
}         