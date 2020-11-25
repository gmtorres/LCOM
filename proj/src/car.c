#include "car.h"

Car carInit (double x, double y, double vx, double vy,double ang, struct SpriteBmp *spr , struct SpriteBmp * l, bool ai) {
  struct Car c;
  struct gameObject gOb = gameObjectRectangle (x,y,vx,vy,spr->bmp->bitmapInfoHeader.width,spr->bmp->bitmapInfoHeader.height,ang,false , 0.0011);
  c.sensores[0] = 0;
  c.sensores[1] = 0;
  c.sensores[2] = 0;
  c.sensores[3] = 0;
  c.sensores[4] = 0;
  //struct gameObject gObprev = gameObjectRectangle (x,y,vx,vy,spr->bmp->bitmapInfoHeader.width,spr->bmp->bitmapInfoHeader.height,ang);


  c.gO = gOb;
  //c.prev = gObprev;
  c.sp =  spr;
  c.light = l;
  c.l = false;
  c.acelerar = 0;
  c.travar = 0;
  c.virar_direita = 0;
  c.virar_esquerda = 0;
  c.applyPowerUp = false;
  c.tecla = 0x00;

  c.items = NULL;
  c.n_items = 0;
  c.ai = ai;

  c.laps = -1;
  c.max_laps = 0;

  c.ticks = 0;

  c.turtle_hit = 0;
  c.banana_hit = 0;

  c.dir = v(1,0);

  c.pu = NULL;

  c.hold_pu = 0;

  return c;
}
/*
void desenharCarro(struct Car *carro) {
  //drawSprite(carro->sp, carro->gO.pos.x, carro->gO.pos.y);
  drawSpriteBmpAng(carro->sp, carro->gO.pos.x, carro->gO.pos.y, carro->gO.ang + PI/2);
}*/

void desenharCarroCamera(struct Car *carro , struct Camera * cm){
  struct Vector desl = cameraDeslocamento(cm);
  //desl.x = 0;
  //drawSpriteBmp(carro->sp , carro->gO.pos.x - desl.x - carro->sp->bmp->bitmapInfoHeader.width/2,carro->gO.pos.y -desl.y - carro->sp->bmp->bitmapInfoHeader.height/2);
  //drawSpriteBmp(carro->sp, carro->gO.pos.x  - desl.x - *carro->sp->width/2, carro->gO.pos.y - desl.y - *carro->sp->height/2);
  drawSpriteBmpAng(carro->sp, carro->gO.pos.x  - desl.x - *carro->sp->width/2, carro->gO.pos.y - desl.y - *carro->sp->height/2, carro->gO.ang + PI/2);

  for(size_t i = 0;i<carro->n_items ; ++i){
    //drawSpriteBmp(carro->items[i].spr , carro->items[i].gObj.pos.x - desl.x , carro->items[i].gObj.pos.y - desl.y);
    drawItem(&carro->items[i],cm);
  }

  if(carro->l){
    float ang = carro->gO.ang;
    drawBitmapFade(carro->light->bmp ,carro->gO.pos.x  - desl.x - *carro->light->width/2 + (*carro->light->height/2 + *carro->sp->height/2) * cos(ang)  , carro->gO.pos.y - desl.y - *carro->light->height/2 + (*carro->light->height/2 + *carro->sp->height/2) * sin(ang), carro->gO.ang +PI/2  , 0, 200);
    drawBitmapFade(carro->light->bmp ,carro->gO.pos.x  - desl.x - *carro->light->width/2 + (*carro->light->height/2 + *carro->sp->height/2) * cos(ang)  , carro->gO.pos.y - desl.y - *carro->light->height/2 + (*carro->light->height/2 + *carro->sp->height/2) * sin(ang), carro->gO.ang +PI/2  , 0, 200);

    }

}

void receberTecla(struct Car *carro , uint8_t tecla){
  carro->tecla = tecla;
  switch(tecla){
    case 0x11:
    carro->acelerar = 2;
    break;
    case 0x91:
    carro->acelerar = 0;
    break;
    case 0x1f:
    carro->travar = 2;
    break;
    case 0x9f:
    carro->travar = 0;
    break;
    case 0x1e:
    carro->virar_esquerda = 2;
    break;
    case 0x9e:
    carro->virar_esquerda = 0;
    break;
    case 0x20:
    carro->virar_direita = 2;
    break;
    case 0xa0:
    carro->virar_direita = 0;
    break;
    case 0x26:
    carro->l = !carro->l;
    default:
    break;
  }
}

void receberMouse(struct Car * carro, struct Mouse *m){
  if(carro->pu == NULL)
    return;
  switch (m->event) {
    case LB_PRESSED:
      if(!carro->pu->activated){
        carro->pu->activated = true;
        carro->applyPowerUp = true;
        carro->pu->max_hold_time = carro->pu->max_util_time;
        //applyPowerUp(carro , carro->pu->id);
      }
      if(carro->pu->activated){
        //applyPowerUp(carro , carro->pu->id);
        carro->applyPowerUp = true;
      }
      carro->dir = v(m->pos.x - get_hres()/2,m->pos.y - get_vres()/2);
    break;
    case LB_RELEASED:
      carro->applyPowerUp = false;
    break;
    default:
    break;
  }

}

void applyPowerUp(struct Car * carro){
  if(carro->pu == NULL)
    return;
    //printf("A aplicar.\n" );
  switch(carro->pu->id){
    case 0:   //speed
      acelerar(&carro->gO,0.2);
      break;
    case 1: //turle
      insertItem(carro,1,carro->dir);
      break;
    case 2: //banana
      insertItem(carro,2,carro->dir);
      break;
    default:
      break;
  }
}

void updatePowerUp(struct Car * carro){
  if(carro->pu == NULL)
      return;
  if(carro->pu != NULL){
    if(!carro->pu->activated){
      carro->pu->max_hold_time--;
      if(carro->pu->max_hold_time == 0){
        free(carro->pu);
        carro->applyPowerUp = false;
        carro->pu = NULL;
        return;
      }
    }
    if(carro->pu->activated){
      carro->pu->max_util_time--;
      if(carro->pu->max_util_time == 0){
        free(carro->pu);
        carro->applyPowerUp = false;
        carro->pu = NULL;
        return;
      }
    }
  }

}

void carroLights(struct Car * carro, Camera *cm){
  struct Vector desl = cameraDeslocamento(cm);
  if(carro->l){
    float ang = carro->gO.ang;
    drawBitmapFade(carro->light->bmp ,carro->gO.pos.x  - desl.x - *carro->light->width/2 + (*carro->light->height/2 + *carro->sp->height/2) * cos(ang)  , carro->gO.pos.y - desl.y - *carro->light->height/2 + (*carro->light->height/2 + *carro->sp->height/2) * sin(ang), carro->gO.ang +PI/2  , 0, 200);

    //drawBitmapFade(carro->light->bmp ,carro->gO.pos.x  - desl.x + (*carro->light->width) * cos(carro->gO.ang) - *carro->light->height/2 * sin(carro->gO.ang)  , carro->gO.pos.y - desl.y - *carro->light->height/2 * cos(carro->gO.ang) /*- (*carro->light->width) * cos(carro->gO.ang) */, carro->gO.ang + PI/2 , 0, 200);
  }
}

void carroAi(struct Car * carro){
  double *sensores = carro->sensores;

  if(carro->pu != NULL ){
    if(carro->hold_pu == 0){
      carro->hold_pu = (int)rand()%180 + 180;
    }else if(carro->hold_pu > 0){
      carro->hold_pu--;
    }
    //Mouse m = msInit();
    //printf("%d\n", carro->hold_pu );
    if(carro->hold_pu > 1 && carro->hold_pu < 3){
      carro->hold_pu --;
      //carro->applyPowerUp = true;
      //m.event = LB_PRESSED;
      //receberMouse(carro,&m);
      //printf("apply\n");
      carro->dir = v((int)rand()%2000 + 1, (int)rand()%2000 + 1);
      applyPowerUp(carro);
    }else{
      //carro->applyPowerUp = false;
      //m.event = LB_RELEASED;
      //receberMouse(carro,&m);
    }
  }else{
    carro->hold_pu = 0;
  }


  if(sensores[2] < 200 && normaVector(&carro->gO.speed) > 4){
    carro->acelerar = false;
    carro->travar = true;
  }
  else{
    carro->acelerar = true;
    carro->travar = false;
  }

    if(sensores[1]<sensores[3]){
      carro->virar_direita = true;
      carro->virar_esquerda = false;
    }else if(sensores[1]>sensores[3]){
      carro->virar_direita = false;
      carro->virar_esquerda = true;
    }
    if(sensores[0] > 400 || sensores[4] > 400){
      if(sensores[0]<sensores[4]){
        carro->virar_direita = true;
        carro->virar_esquerda = false;
      }else if(sensores[0]>sensores[4]){
        carro->virar_direita = false;
        carro->virar_esquerda = true;
      }
    }
    if(sensores[1] <= *carro->sp->height * 1.3 || sensores[3]  <= *carro->sp->height * 1.3){
      carro->acelerar = false;
      carro->travar = true;
      if(sensores[0]<sensores[4]){
        carro->virar_direita = true;
        carro->virar_esquerda = false;
      }else{
        carro->virar_direita = false;
        carro->virar_esquerda = true;
      }
    }


}

void updateCarro(struct Car *carro){


  carro->gO.pre_ang = carro->gO.ang;

  if(carro->ai)
    carroAi(carro);

  if(!carro->banana_hit  && !carro->turtle_hit){
    carro->gO.angulo = true;
    if(carro->acelerar > 0){
      acelerar(&carro->gO,0.2);
    }
    if(carro->travar > 0){
      travar(&carro->gO);
    }
    if(carro->virar_direita > 0){
      rodarDireita(&carro->gO);
    }
    if(carro->virar_esquerda > 0){
      rodarEsquerda(&carro->gO);
    }
    if(carro->applyPowerUp == true){
      applyPowerUp(carro);
    }
  }
  if(carro->banana_hit){
    carro->banana_hit--;
    rodarEsquerda(&carro->gO);
  }
  if(carro->turtle_hit){
    carro->turtle_hit--;
  }


  updatePowerUp(carro);

  for(size_t i= 0; i < carro->n_items;++i ){
    updatePosition(&carro->items[i].gObj);
    carro->items[i].lifetime--;
    if(carro->items[i].lifetime == 0){
      removeItem(carro,i);
      i--;
    }

  }

  updatePosition(&carro->gO);

}

void hitItem(struct Car * carro , uint8_t id){
  switch (id) {
    case 0:  //turtle
    carro->turtle_hit = 180;
    carro->gO.speed = v(0,0);
    break;
    case 1:
    if(carro->ai){
      carro->banana_hit += 210;
    }else{
      carro->banana_hit += (int)rand()%50 + 210;
    }
    carro->gO.angulo = false;

    break;
    default:break;
  }

}

double readSensor(struct Car *car, double ang, struct QuadTreeObs* qt) {
  Vector desl = car->gO.pos;
  Vector step = v ( cos(car->gO.ang + ang) , sin(car->gO.ang + ang));
  multVectors(&step, 12);
  List l = listInit();

  for (unsigned int i = 0; i < 90; i++) {
    addVectors(&desl,&step);
    quadtreeObsQuery(qt, desl.x - 130, desl.y -130, 260, 260 ,&l);
    gameObject c = gameObjectCircle(desl.x, desl.y, 0, 0, 1, 0, false,0);
    while (!isEmpty(&l)) {
      struct Obstacle * obs = (Obstacle * )popFront(&l);
      if (collision(&obs->gameObs, &c)) {
        subVectors(&desl, &car->gO.pos);
        return normaVector(&desl);
      }
    }
  }
  subVectors(&desl, &car->gO.pos);
  return normaVector(&desl);
}

#define RADIANS(x) PI * x/180

void readSensores(struct Car* carro, struct QuadTreeObs* qt) {
  carro->sensores[0] = readSensor(carro, RADIANS(-60), qt);
  carro->sensores[1] = readSensor(carro, RADIANS(-25), qt);
  carro->sensores[2] = readSensor(carro, RADIANS(0), qt);
  carro->sensores[3] = readSensor(carro, RADIANS(25), qt);
  carro->sensores[4] = readSensor(carro, RADIANS(60), qt);
}





void insertItem(struct Car * carro , size_t id , Vector dir){
  if(carro->n_items == 0 && carro->items == NULL)
    carro->items = (Item *) malloc(sizeof(Item));
  else{
    carro->items = (Item *)realloc(carro->items,sizeof(Item) * (carro->n_items+1));
  }
  carro->items[carro->n_items] = itemInit(id-1, carro->gO.pos.x, carro->gO.pos.y, dir , &carro->gO);
  carro->n_items++;
}

void removeItem(struct Car * carro ,  size_t indice){
  if(indice >=  carro->n_items)
    return;
  for(size_t i = indice;i<carro->n_items-1;++i)
    carro->items[i] = carro->items[i+1];
  carro->n_items--;
  //realloc already frees the unused memory;
  carro->items = (Item *)realloc(carro->items,sizeof(Item) * (carro->n_items));
}















void freeCarro(struct Car* carro){
  if(carro->pu != NULL){
    free(carro->pu);
    carro->pu = NULL;
  }
  while(carro->n_items > 0){
    removeItem(carro,0);
  }
  if(carro->items != NULL){
    free(carro->items);
    carro->items = NULL;
  }
}
