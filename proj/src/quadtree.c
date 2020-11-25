#include "quadtree.h"


Boundary boundaryInit(double  x, double  y , double  width, double  height){
  struct Boundary b;
  Vector vec = v(x,y);
  b.posi = vec;
  b.width = width;
  b.height = height;
  return b;
}

bool contains(Boundary b, Vector point){
  return point.x >= b.posi.x
      && point.x < b.posi.x + b.width
      && point.y >= b.posi.y
      && point.y < b.posi.y + b.height;
}

bool intersect(Boundary b1 , Boundary b2){
  return   !( b2.posi.x > b1.posi.x + b1.width ||
              b2.posi.x + b2.width < b1.posi.x ||
              b2.posi.y > b1.posi.y + b1.height ||
              b2.posi.y + b2.height < b1.posi.y
            );
}


QuadTreeObs quadtreeObsInit(double x, double  y , double  width, double  height , int n_obstacles){
  struct QuadTreeObs qt;

  qt.max_obstacles = n_obstacles;
  qt.n_obstacles = 0;

  qt.qt1 = NULL;
  qt.qt2 = NULL;
  qt.qt3 = NULL;
  qt.qt4 = NULL;

  qt.divided = false;

  qt.obstacles = (Obstacle * )malloc(sizeof(Obstacle ) * n_obstacles);

  qt.boundary = boundaryInit(x,y,width,height);

  return qt;
}

void quadtreeObsInsertObs(QuadTreeObs *qt , Obstacle obs){
  if(!contains(qt->boundary, obs.gameObs.pos))
    return;
  if(qt->n_obstacles < qt->max_obstacles){
    qt->obstacles[qt->n_obstacles] = obs;
    qt->n_obstacles++;
    return;
  }

  if(!qt->divided){
    qt->divided = true;
    qt->qt1 = (QuadTreeObs*)malloc(sizeof(QuadTreeObs));
    qt->qt2 = (QuadTreeObs*)malloc(sizeof(QuadTreeObs));
    qt->qt3 = (QuadTreeObs*)malloc(sizeof(QuadTreeObs));
    qt->qt4 = (QuadTreeObs*)malloc(sizeof(QuadTreeObs));
    *qt->qt1 = quadtreeObsInit(qt->boundary.posi.x , qt->boundary.posi.y , qt->boundary.width/2 , qt->boundary.height/2 , qt->n_obstacles);
    *qt->qt2 = quadtreeObsInit(qt->boundary.posi.x + qt->boundary.width/2, qt->boundary.posi.y , qt->boundary.width/2 , qt->boundary.height/2 , qt->n_obstacles);
    *qt->qt3 = quadtreeObsInit(qt->boundary.posi.x , qt->boundary.posi.y + qt->boundary.height/2 , qt->boundary.width/2 , qt->boundary.height/2 , qt->n_obstacles);
    *qt->qt4 = quadtreeObsInit(qt->boundary.posi.x + qt->boundary.width/2, qt->boundary.posi.y  + qt->boundary.height/2, qt->boundary.width/2 , qt->boundary.height/2 , qt->n_obstacles);
  }
  quadtreeObsInsertObs(qt->qt1 , obs);
  quadtreeObsInsertObs(qt->qt2 , obs);
  quadtreeObsInsertObs(qt->qt3 , obs);
  quadtreeObsInsertObs(qt->qt4 , obs);

}


void quadtreeObsQuery(QuadTreeObs * qt , double x, double y , double width, double height, List * l){

  struct Boundary b =  boundaryInit(x,y,width,height);

  if(!intersect(qt->boundary , b))
    return;
  for(int i = 0; i< qt->n_obstacles;++i){
    if(contains(b,qt->obstacles[i].gameObs.pos)){
      insertFront(l , &qt->obstacles[i]);
    }
  }
  if(qt->divided){
    quadtreeObsQuery(qt->qt1,x,y,width,height,l);
    quadtreeObsQuery(qt->qt2,x,y,width,height,l);
    quadtreeObsQuery(qt->qt3,x,y,width,height,l);
    quadtreeObsQuery(qt->qt4,x,y,width,height,l);
  }



}

void quadtreeObsFree(QuadTreeObs *qt){
  if(qt->obstacles !=NULL){
    free(qt->obstacles);
    qt->obstacles = NULL;
  }
  if(qt->divided){
    if(qt->qt1 != NULL){
      quadtreeObsFree(qt->qt1);
      free(qt->qt1);
      qt->qt1 = NULL;
    }
    if(qt->qt2 != NULL){
      quadtreeObsFree(qt->qt2);
      free(qt->qt2);
      qt->qt2 = NULL;
    }
    if(qt->qt3 != NULL){
      quadtreeObsFree(qt->qt3);
      free(qt->qt3);
      qt->qt3 = NULL;
    }
    if(qt->qt4 != NULL){
      quadtreeObsFree(qt->qt4);
      free(qt->qt4);
      qt->qt4 = NULL;
    }
  }

}










QuadTreePowerUpSpawn quadtreePowerUpSpawnInit(double x, double  y , double  width, double  height , int n_obstacles){
  struct QuadTreePowerUpSpawn qt;

  qt.max_PowerUpSpawns = n_obstacles;
  qt.n_PowerUpSpawns = 0;

  qt.qt1 = NULL;
  qt.qt2 = NULL;
  qt.qt3 = NULL;
  qt.qt4 = NULL;

  qt.divided = false;

  qt.obstacles = (PowerUpSpawn * )malloc(sizeof(PowerUpSpawn ) * n_obstacles);

  qt.boundary = boundaryInit(x,y,width,height);

  return qt;
}

void quadtreePowerUpSpawnInsertPowerUpSpawn(QuadTreePowerUpSpawn *qt , PowerUpSpawn pu){
  if(!contains(qt->boundary, pu.gObj.pos))
    return;
  if(qt->n_PowerUpSpawns < qt->max_PowerUpSpawns){
    qt->obstacles[qt->n_PowerUpSpawns] = pu;
    qt->n_PowerUpSpawns++;
    return;
  }

  if(!qt->divided){
    qt->divided = true;
    qt->qt1 = (QuadTreePowerUpSpawn*)malloc(sizeof(QuadTreePowerUpSpawn));
    qt->qt2 = (QuadTreePowerUpSpawn*)malloc(sizeof(QuadTreePowerUpSpawn));
    qt->qt3 = (QuadTreePowerUpSpawn*)malloc(sizeof(QuadTreePowerUpSpawn));
    qt->qt4 = (QuadTreePowerUpSpawn*)malloc(sizeof(QuadTreePowerUpSpawn));
    *qt->qt1 = quadtreePowerUpSpawnInit(qt->boundary.posi.x , qt->boundary.posi.y , qt->boundary.width/2 , qt->boundary.height/2 , qt->n_PowerUpSpawns);
    *qt->qt2 = quadtreePowerUpSpawnInit(qt->boundary.posi.x + qt->boundary.width/2, qt->boundary.posi.y , qt->boundary.width/2 , qt->boundary.height/2 , qt->n_PowerUpSpawns);
    *qt->qt3 = quadtreePowerUpSpawnInit(qt->boundary.posi.x , qt->boundary.posi.y + qt->boundary.height/2 , qt->boundary.width/2 , qt->boundary.height/2 , qt->n_PowerUpSpawns);
    *qt->qt4 = quadtreePowerUpSpawnInit(qt->boundary.posi.x + qt->boundary.width/2, qt->boundary.posi.y  + qt->boundary.height/2, qt->boundary.width/2 , qt->boundary.height/2 , qt->n_PowerUpSpawns);
  }
  quadtreePowerUpSpawnInsertPowerUpSpawn(qt->qt1 , pu);
  quadtreePowerUpSpawnInsertPowerUpSpawn(qt->qt2 , pu);
  quadtreePowerUpSpawnInsertPowerUpSpawn(qt->qt3 , pu);
  quadtreePowerUpSpawnInsertPowerUpSpawn(qt->qt4 , pu);

}


void quadtreePowerUpSpawnQuery(QuadTreePowerUpSpawn * qt , double x, double y , double width, double height, List * l){

  struct Boundary b =  boundaryInit(x,y,width,height);

  if(!intersect(qt->boundary , b))
    return;
  for(int i = 0; i< qt->n_PowerUpSpawns;++i){
    if(contains(b,qt->obstacles[i].gObj.pos)){
      insertFront(l , &qt->obstacles[i]);
    }
  }
  if(qt->divided){
    quadtreePowerUpSpawnQuery(qt->qt1,x,y,width,height,l);
    quadtreePowerUpSpawnQuery(qt->qt2,x,y,width,height,l);
    quadtreePowerUpSpawnQuery(qt->qt3,x,y,width,height,l);
    quadtreePowerUpSpawnQuery(qt->qt4,x,y,width,height,l);
  }



}





void quadtreePowerUpSpawnFree(QuadTreePowerUpSpawn *qt){
  if(qt->obstacles != NULL){
    free(qt->obstacles);
    qt->obstacles = NULL;
  }
  if(qt->divided){
    if(qt->qt1 != NULL){
      quadtreePowerUpSpawnFree(qt->qt1);
      free(qt->qt1);
      qt->qt1 = NULL;
    }
    if(qt->qt2 != NULL){
      quadtreePowerUpSpawnFree(qt->qt2);
      free(qt->qt2);
      qt->qt2 = NULL;
    }
    if(qt->qt3 != NULL){
      quadtreePowerUpSpawnFree(qt->qt3);
      free(qt->qt3);
      qt->qt3 = NULL;
    }
    if(qt->qt4 != NULL){
      quadtreePowerUpSpawnFree(qt->qt4);
      free(qt->qt4);
      qt->qt4 = NULL;
    }
  }

}
