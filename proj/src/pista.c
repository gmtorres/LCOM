#include "pista.h"

#define RADIANS(x) PI * x/180

Pista *pistaInit(char * filename ,bool mp ,bool server){


    memset(tiles,0,sizeof(tiles));
    initiateTiles();
    initiateItems();
    initiatePowerUps();
    initiateObstaculos();
    carros[0] = sBmpInit("/home/lcom/labs/proj/resources/images/cars/cars2(17).bmp");
    carros[1] = sBmpInit("/home/lcom/labs/proj/resources/images/cars/cars2(7).bmp");
    lights[0] =  sBmpInit("/home/lcom/labs/proj/resources/images/utilities/light_yellow.bmp");
    lights[1] =  sBmpInit("/home/lcom/labs/proj/resources/images/utilities/light_white.bmp");

  init = true;

  struct Pista *p = (Pista *)malloc(sizeof(Pista));
  p->mapa = NULL;

  p->multiplayer = mp;

  p->ticks = 0;
  memset(p->date , 0 , sizeof(p->date));
  p->time = false;


  FILE * fp = fopen(filename , "r");

  if(fp == NULL){
    printf("Unable to open file\n");
    free(p);
    return NULL;
  }

  fscanf(fp,"%hd" ,&p->x_tiles);
  fscanf(fp,"%hd" ,&p->y_tiles);

  p->obstacles = quadtreeObsInit (-129, -129, (p->x_tiles + 2) * 128 + 1, (p->y_tiles + 2) * 128 + 1, 1);
  p->powerUpSpawns = quadtreePowerUpSpawnInit(-129, -129, (p->x_tiles + 2) * 128 + 1, (p->y_tiles + 2) * 128 + 1, 1);

  p->mapa = malloc(sizeof(Tile) * p->x_tiles * p->y_tiles);
  if(p->mapa == NULL){
    free(p);
    printf("Error allocating memory.\n" );
    return NULL;
  }

  char t;
  int i = 0;
  fscanf(fp,"%c" , &t);
  while(i < p->x_tiles * p->y_tiles){
    fscanf(fp,"%c",&t);
    #define WIDTH 128
    #define HEIGHT 128
    p->mapa[i] = tile(t - '!' , (i%(p->x_tiles)) * WIDTH , (i/p->x_tiles) * HEIGHT);
    i++;
    fscanf(fp,"%c" , &t);
  }

  uint16_t m_x , m_y , m_l, m_o;
  fscanf(fp,"%hd" ,&m_x);
  fscanf(fp,"%hd" ,&m_y);
  fscanf(fp,"%hd" ,&m_l);
  fscanf(fp,"%hd" ,&m_o);

  if(m_o == 0){
    p->meta1 = gameObjectRectangle(m_x * WIDTH + WIDTH * m_l/2 , m_y * HEIGHT + HEIGHT/2, 0, 0,WIDTH * (m_l +2)  , HEIGHT  ,PI/2,true,10000);
    p->meta2 = gameObjectRectangle(m_x * WIDTH + WIDTH * m_l/2, m_y* HEIGHT+HEIGHT + HEIGHT/2, 0, 0,WIDTH * (m_l +2) , HEIGHT ,PI/2,true,10000);
  }else if(m_o == 1){
    }

  fscanf(fp,"%hd" , &p->laps);


  uint16_t n_obs, x, y, ang;
  fscanf(fp,"%hd" ,&n_obs);
  fscanf(fp,"%c",&t);
  i = 0;

  while(i < n_obs){
    fscanf(fp,"%c",&t);
    if(t == 'l'){
      fscanf(fp,"%c",&t);
      fscanf(fp,"%c",&t);
      uint16_t x1 , y1;
      fscanf(fp,"%hd" ,&x1);
      fscanf(fp,"%hd" ,&y1);
      fscanf(fp,"%hd" ,&x);
      fscanf(fp,"%hd" ,&y);

      Vector d = v(x-x1,y-y1);
      uint32_t g = ceil(normaVector(&d) / 210.0);
      multVectors(&d , 210/normaVector(&d));

      for(uint32_t f = 0;f < g ;f++, x1 += d.x , y1+=d.y){
        Obstacle obs = obstacleInit(t - 'a' , x1 ,y1 );
        quadtreeObsInsertObs(&p->obstacles, obs);
      }


    }else{
    fscanf(fp,"%hd" ,&x);
    fscanf(fp,"%hd" ,&y);
    Obstacle obs = obstacleInit(t - 'a' , x ,y );
    quadtreeObsInsertObs(&p->obstacles, obs);
    }
    fscanf(fp,"%c",&t);
    i++;
  }


  uint16_t t_n;
  fscanf(fp,"%hd" ,&t_n);
  t_n++;
  if(p->multiplayer)
    p->n_carros = 2;
  else
    p->n_carros = t_n;
  p->carros = (Car*) malloc(sizeof(Car) * p->n_carros);


  for(unsigned int a = 0 ; a < t_n || a < p->n_carros; a++){
    if(a < t_n){
      fscanf(fp,"%hd" ,&x);
      fscanf(fp,"%hd" ,&y);
      fscanf(fp,"%hd" ,&ang);
    }
    SpriteBmp * c;
    if(a == 0)
      c = carros[0];
    else c = carros[1];

    if(a >= p->n_carros)
      continue;
    if(!p->multiplayer)
      p->carros[a] = carInit(x,y,0,0,ang * PI/4, c , lights[1], (a!=0));
    else{
      p->carros[a] = carInit(x,y,0,0,ang * PI/4, c , lights[1], false);
      if(a == 1 && t_n == 1)
      p->carros[a] = carInit(x-80*cos(ang + PI/3),y - 80*sin(ang + PI/3),0,0,ang * PI/4, c , lights[1], false);
    }
  }

  if(p->multiplayer && !server){
    Car cr_p = p->carros[0];
    p->carros[0] = p->carros[1];
    p->carros[1] = cr_p;
  }


  if(!p->multiplayer){
    uint16_t n_spawns;
    fscanf(fp,"%hd" ,&n_spawns);
    for(unsigned int a = 0; a < n_spawns;a++){
      fscanf(fp,"%hd" ,&x);
      fscanf(fp,"%hd" ,&y);
      PowerUpSpawn pus = powerUpSpawnInit(x,y);
      generatePowerUp(&pus);
      quadtreePowerUpSpawnInsertPowerUpSpawn(&p->powerUpSpawns , pus);
    }
  }


  fclose(fp);
  p->cm = cameraInit(&p->carros[0].gO.pos , v(get_hres()/2, get_vres()/2) , true , false); // true has problems
  setBorders(&p->cm , v(0,0) , v(p->x_tiles * 128 , p->y_tiles * 128));
  setSpeed(&p->cm , &p->carros[0].gO.speed);


  return p;
}


void freePista(struct Pista *p){

  if(!init || p == NULL)
    return;
  init = false;
  freeTiles();

  freePowerUps();

  freeItems();

  quadtreeObsFree(&p->obstacles);

  quadtreePowerUpSpawnFree(&p->powerUpSpawns);

  if(p->carros != NULL){
    for(size_t i = 0; i < p->n_carros;++i)
        freeCarro(&p->carros[i]);
    p->carros = NULL;
  }
  p->n_carros = 0;

  if(p->carros != NULL){
    free(p->carros);
    p->carros = NULL;
  }


  if(p->mapa != NULL){
    free(p->mapa);
    p->mapa = NULL;
  }


    for(unsigned int i = 0; i<sizeof(carros) / sizeof(carros[0]);i++)
      if(carros[i] != NULL){
        freeBmp(carros[i]);
        carros[i] = NULL;
      }

    for(unsigned int i = 0; i<sizeof(lights) / sizeof(lights[0]);i++)
      if(lights[i] != NULL){
        freeBmp(lights[i]);
        lights[i] = NULL;
      }
}



void desenharNivel(struct Pista * p){
  Vector desl = cameraDeslocamento(&p->cm);
  int i = (desl.x- 127 );
  if(i < 0) i = 0;
  int j = (desl.y- 128);
  if(j < 0) j = 0;
  for(i/=128; i < p->x_tiles && i < (int)(desl.x + get_hres() + 128 * 2)/128;i++){
    for(j/=128; j < p->y_tiles && j < (int)(desl.y + get_vres() + 128 * 2)/128;j++){
      drawTileCamera(&p->mapa[i + j * p->x_tiles] ,&p->cm);
    }
  }

  List l = listInit();
  quadtreePowerUpSpawnQuery(&p->powerUpSpawns , p->cm.translacao->x - get_hres()/2, p->cm.translacao->y - get_vres()/2 , get_hres() , get_vres() ,&l);
  while(!isEmpty(&l)){
    struct PowerUpSpawn * pu =  ( PowerUpSpawn* )popFront(&l);
    drawPowerUpSpawnCamera(pu,&p->cm);
  }
  freeList(&l);

  quadtreeObsQuery(&p->obstacles ,  p->cm.translacao->x - get_hres()/2 - 200, p->cm.translacao->y - get_vres()/2 -200, get_hres() + 400 , get_vres() + 400, &l);
  while(!isEmpty(&l)){
    struct Obstacle * obs =  (Obstacle * )popFront(&l);
    drawObstacle(obs, &p->cm);
  }
  freeList(&l);
}

void checkCollisionCar(struct Pista *p , struct Car *car){
  List l = listInit();
  quadtreeObsQuery(&p->obstacles , car->gO.pos.x - 130, car->gO.pos.y - 130 , 260,260 ,&l);
  while(!isEmpty(&l)){
    struct Obstacle * obs =  (Obstacle * )popFront(&l);
    checkHandleCollisionRectangles( &car->gO , &obs->gameObs );
  }
  freeList(&l);
}

void getInputKbd(struct Pista * p, uint8_t tecla){
  receberTecla(&p->carros[0] , tecla);
}

void getInputMouse(struct Pista * p, struct Mouse *m){
  receberMouse(&p->carros[0] , m);
}

void getInputSp1(struct Pista *p , uint8_t ch){
  if(!p->multiplayer)
    return;
  receberTecla(&p->carros[1] , ch);
}




void checkAllColisions(struct Pista *p){
  for(unsigned int  a = 0; a < p->n_carros;++a){

    //collisions entre carros e obstaculos
    List l = listInit();
    quadtreeObsQuery(&p->obstacles , p->carros[a].gO.pos.x - 130, p->carros[a].gO.pos.y - 130 , 260,260 ,&l);
    while(!isEmpty(&l)){
      struct Obstacle * obs =  (Obstacle * )popFront(&l);
      checkHandleCollision(  &p->carros[a].gO , &obs->gameObs );
    }
    freeList(&l);

    //percorrer os items dos carros
    for(int i = 0; i < (int)p->carros[a].n_items ; ++i){

      //collision entre items dos carros e obscatulos
      quadtreeObsQuery(&p->obstacles , p->carros[a].items[i].gObj.pos.x - 130, p->carros[a].items[i].gObj.pos.y - 130 , 260,260 ,&l);
      while(!isEmpty(&l)){
        struct Obstacle * obs =  (Obstacle * )popFront(&l);
        checkHandlecollisionRectangleCircle(&obs->gameObs , &p->carros[a].items[i].gObj);
      }
      freeList(&l);

      for(unsigned int b = 0 ; b< p->n_carros;b++){

        if(p->carros[a].items[i].carro == &p->carros[a].gO){
          if(!collisionRectangleCircle(&p->carros[a].gO , &p->carros[a].items[i].gObj)){
            p->carros[a].items[i].carro = NULL;
          }
        }else{

          if(collisionRectangleCircle(&p->carros[b].gO , &p->carros[a].items[i].gObj)) {
            hitItem(&p->carros[b] , p->carros[a].items[i].id);
            p->carros[a].items[i].lifetime = 1;
          }
        }


      }


    }


    //collision entre carros
    for(unsigned int b = a+1 ; b< p->n_carros;b++){
      checkHandleCollisionRectangles( &(p->carros[a].gO) , &(p->carros[b].gO) );
    }
  }
}






void checkmeta(struct Pista *p){

  for(int i = 0; i < p->n_carros ; i++){
    bool m1 = collisionRectangles(&p->meta1 , &p->carros[i].gO);
    bool m2 = collisionRectangles(&p->meta2 , &p->carros[i].gO);

    if(m2){
      if( p->carros[i].laps >= 0)
        p->carros[i].laps = (int)p->carros[i].laps + 0.5;
      else
        p->carros[i].laps = -0.5;

    }
    else if(m1  && !m2 ){
      if(p->carros[i].laps == -0.5){
        p->carros[i].laps = 0.25;
      }
      else if(p->carros[i].laps - (int)p->carros[i].laps == 0.5){
        p->carros[i].laps = (int)p->carros[i].laps + 1.25;

      }else if(p->carros[i].laps - (int)p->carros[i].laps == 0){
        p->carros[i].laps = (int)p->carros[i].max_laps - 1;
      }
    }else if(!m1){
      if( p->carros[i].laps >= 0)
        p->carros[i].laps = (int)p->carros[i].laps;
        else
          p->carros[i].laps = -1;
    }

    if((float)p->carros[i].max_laps < (float)p->carros[i].laps){
      p->carros[i].max_laps = p->carros[i].laps;
      if ((int)p->laps == (int)p->carros[i].max_laps) {
        p->carros[i].ticks = p->ticks;
        printf("%d\n",i );
      }
    }
  }


}

bool checkEndOfRace(struct Pista *p) {
  return (int)p->carros[0].max_laps == (int)p->laps;
}


int * orderPosition(struct Pista *p) {
  static int v[4];
  Car c = p->carros[0];
  int n = 0;
  int prev_i = 0;
  int interc_i = 0;

  while (n < p->n_carros) {
    for (int i = 0; i < p->n_carros; i++) {
      if ((p->carros[i].ticks < c.ticks) && (i != prev_i)) {
        c = p->carros[i];
        interc_i = i;
      }
    }
    prev_i = interc_i;
    v[n] = prev_i;
    n++;
  }
  return v;
}
void orderPosition2(struct Pista * p, int *v){
  for(unsigned int i = 0; i < p->n_carros; i++){
    v[i] = i;
  }
  for(unsigned int a = 0; a < p->n_carros; a++){
    uint8_t min = v[a];
    for(unsigned int b = a + 1; b < p->n_carros;b++){
      if(p->carros[v[b]].ticks < p->carros[v[min]].ticks)
        min = b;
    }
    int t = v[a];
    v[a] = v[min];
    v[min] = t;
  }
}

int getPosition(struct Pista * p){
  int a = 0;
  for(unsigned int i = 0 ; i <p->n_carros;i++){
    if(p->carros[i].ticks == 0)
      continue;
    if(p->carros[0].ticks > p->carros[i].ticks)
      a++;
  }
  return ++a;
}


void checkCollisionCars(struct Pista *p){
  for(unsigned int  a = 0; a < p->n_carros -1;++a){
    for(unsigned int b = a+1 ; b< p->n_carros;b++)
      checkHandleCollisionRectangles( &(p->carros[a].gO) , &(p->carros[b].gO) );
  }
}
void catchPowerUps(struct Pista *p){
  for(unsigned int i = 0; i < p->n_carros ; ++i){

    List l = listInit();

    quadtreePowerUpSpawnQuery(&p->powerUpSpawns , p->cm.translacao->x - get_hres()/2, p->cm.translacao->y - get_vres()/2 , get_hres() , get_vres() ,&l);
    while(!isEmpty(&l)){
      struct PowerUpSpawn * pu =  ( PowerUpSpawn* )popFront(&l);
      bool c = collision(&pu->gObj, &p->carros[i].gO);
      if(c){
        if(p->carros[i].pu == NULL && pu->pu != NULL){
          p->carros[i].pu = pu->pu;
          pu->pu = NULL;
          pu->waiting_time = 300 + ((int)rand() % 300);
        }
      }
    }

    freeList(&l);
  }

}
void desenharPista(struct Pista*p){
  desenharNivel(p);

  for(unsigned int i = 0 ; i < p->n_carros ; i++){
    desenharCarroCamera(&p->carros[i] , &p->cm);
  }

  if(p->time){
    if(p->date[2] >= 21 || p->date[2] < 6){
      applyMask(0x01CC , 140);
    }
    else if(p->date[2] >= 18){
      float m = (((p->date[2] - 18)*60 + p->date[1]) * 140)/(3*60.0);
      applyMask(0x01CC , (uint8_t) m);
    }
    else if(p->date[2] < 8){
      float m = 140 - (((8 - p->date[2])*60 + p->date[1]) * 140)/(2*60.0);
      applyMask(0x01CC , (uint8_t) m);
    }
  }

  //carroLights(&p->carros[0],&p->cm);
  //applyMask(0x01CC , 160);

  PowerUp * pu = p->carros[0].pu;
  if(pu!=NULL){
    if(pu->activated)
      drawBitmapPerct(pu->spr->bmp,get_hres()-50, get_vres()-50 , (float)pu->max_util_time/(float)pu->max_hold_time);
    else
      drawBitmap(pu->spr->bmp,get_hres()-50, get_vres()-50);
  }
  draw_digit(p->carros[0].max_laps , get_hres()-100 , 20 , 1);
  draw_slash(get_hres()-70 , 20);
  draw_digit(p->laps , get_hres()-40 , 20 , 1);

  draw_time_ticks(p->ticks , 500 , 10);
}


void updatePista(struct Pista *p){

  p->ticks++;

  for(unsigned int i = 0 ; i < p->n_carros ; i++){
      readSensores(&p->carros[i],&p->obstacles);
    updateCarro(&p->carros[i]);
  }
  checkmeta(p);
  checkAllColisions(p);

  List l = listInit();

  quadtreePowerUpSpawnQuery(&p->powerUpSpawns , 0,0 , p->x_tiles * 128 , p->y_tiles * 128,&l);
  while(!isEmpty(&l)){
    struct PowerUpSpawn * pu =  ( PowerUpSpawn* )popFront(&l);
    if(pu->waiting_time == 0){
      generatePowerUp(pu);
      pu->waiting_time = -1;
    }else if(pu->waiting_time > 0)
      pu->waiting_time--;

  }

  freeList(&l);

  catchPowerUps(p);

  desenharPista(p);



}

void getDate(struct Pista * p, uint8_t date[6]){
  for(int i = 0; i <6 ;i++)
    p->date[i] = date[i];
}
