#include "Geometry/TrackerGeometryBuilder/interface/trackerHierarchy.h"

#include<string>
#include<vector>

std::string trackerHierarchy(const TrackerTopology* tTopo, unsigned int rawid) {
  DetId id(rawid);
  int subdetid = id.subdetId();
  switch (subdetid) {
    
    // PXB
  case 1:
    {
      char theLayer  = tTopo->layer(id);
      char theLadder = tTopo->pxbLadder(id);
      char theModule = tTopo->module(id);
      char key[] = { 1, theLayer , theLadder, theModule};
      return std::string(key,4);
    }
    
    // P1XF
  case 2:
    {
      char thePanel  = tTopo->pxfPanel(id);
      char theDisk   = tTopo->pxfDisk(id);
      char theBlade  = tTopo->pxfBlade(id);
      char theModule = tTopo->module(id);
      char key[] = { 2,
		     char(tTopo->side(id)),
		     thePanel , theDisk, 
		     theBlade, theModule};
      return std::string(key,6);
    }
  
  // TIB
  case 3:
    {
      char            theLayer  = tTopo->layer(id);
      std::vector<unsigned int> theString = tTopo->tibStringInfo(id);
      char             theModule = tTopo->module(id);
      //side = (theString[0] == 1 ) ? "-" : "+";
      //part = (theString[1] == 1 ) ? "int" : "ext";
      char key[] = { 3, 
		     theLayer, 
		     char(theString[0]),
		     char(theString[1]), 
		     char(theString[2]), 
		     theModule,
		     char(tTopo->tibGlued(id) ? tTopo->isStereo(id)+1 : 0)
      };
      return std::string(key, tTopo->tibGlued(id) ? 7 : 6);
    }
    
    // TID
  case 4:
    {
      unsigned int         theDisk   = tTopo->tidWheel(id);
      unsigned int         theRing   = tTopo->tidRing(id);
      // side = (tTopo->side(id) == 1 ) ? "-" : "+";
      // part = (tTopo->tidOrder(id) == 1 ) ? "back" : "front";
      char key[] = { 4, 
		     char(tTopo->side(id)),
		     char(theDisk) , 
		     char(theRing),
		     char(tTopo->tidOrder(id)), 
		     char(tTopo->module(id)),
		     char(tTopo->tidGlued(id) ? tTopo->isStereo(id)+1 : 0)
      };
      return std::string(key,tTopo->tidGlued(id) ? 7 : 6);
    }
    
    // TOB
  case 5:
    {
      unsigned int              theLayer  = tTopo->layer(id);
      unsigned int              theModule = tTopo->module(id);
      //	side = (tTopo->side(id) == 1 ) ? "-" : "+";
      char key[] = { 5, char(theLayer) , 
		     char(tTopo->side(id)), 
		     char(tTopo->tobRod(id)), 
		     char(theModule),
		     char(tTopo->tobGlued(id) ? tTopo->isStereo(id)+1 : 0)
      };
      return std::string(key, tTopo->tobGlued(id) ?  6 : 5);
    }
    
    // TEC
  case 6:
    {
      unsigned int              theWheel  = tTopo->tecWheel(id);
      unsigned int              theModule = tTopo->module(id);
      unsigned int              theRing   = tTopo->tecRing(id);
      //	side  = (tTopo->side(id) == 1 ) ? "-" : "+";
      //	petal = (tTopo->tecOrder(id) == 1 ) ? "back" : "front";
      // int out_side  = (tTopo->side(id) == 1 ) ? -1 : 1;
      
      char key[] = { 6, 
		     char(tTopo->side(id)),
		     char(theWheel),
		     char(tTopo->tecOrder(id)), 
		     char(tTopo->tecPetalNumber(id)),
		     char(theRing),
		     char(theModule),
		     char(tTopo->tecGlued(id) ? tTopo->isStereo(id)+1 : 0)
      };
      return std::string(key, tTopo->tecGlued(id) ? 8 : 7);
    }
  default:
    return std::string();
  }
}
