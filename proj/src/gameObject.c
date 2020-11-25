#include "gameObject.h"

#define PI 3.12156

gameObject gameObjectRectangle(double x, double y , double vx, double vy, double width, double height, double ang, bool fixed , double atrito){
  struct gameObject gObj;
  gObj.type = 0;
  gObj.pos = v(x,y);
  gObj.speed = v(vx,vy);
  gObj.ang = ang;
  gObj.fixed = fixed;
  gObj.atrito = atrito; // 0.0011
  gObj.shape.r= rectangle(&gObj.pos, width, height, ang);
  gObj.angulo = true;
  return gObj;
}

gameObject gameObjectCircle(double x, double y , double vx, double vy, double r, double ang,bool fixed , double atrito){
  struct gameObject gObj;
  gObj.type = 1;
  gObj.pos = v(x,y);
  gObj.speed = v(vx,vy);
  gObj.ang = ang;
  gObj.fixed = fixed;
  gObj.atrito = atrito;
  gObj.angulo = false;
  gObj.shape.c = circle(&gObj.pos, r);
  return gObj;
}

void updatePosition (struct gameObject* gOb) {

  gOb->prev_pos = gOb->pos;
  //gOb->pre_ang = gOb->ang;

  struct Vector atrito = getAtrito(gOb);
  addVectors(&gOb->speed, &atrito);

  if(gOb->angulo){
    Vector lateral = v( -sin(gOb->ang) , cos(gOb->ang));
    multVectors(&lateral , prodEscalar(&gOb->speed , &lateral));


    if(normaVector(&lateral) > 0.32){
      multVectors(&lateral , 0.02);
    }
    subVectors(&gOb->speed , &lateral);
  }

  //printf("a\n");

  addVectors(&gOb->pos, &gOb->speed);

}

void gameObjectGoBack(struct gameObject * gO){
  Vector speed =  gO->speed;
  multVectors(&speed , 1/normaVector(&speed));
  subVectors(&gO->pos,&speed);
}

void acelerar (struct gameObject* gObj, double k) {
  struct Vector ac = v (cos(gObj->ang), sin(gObj->ang));
  multVectors(&ac,k);
  if(k!=0){
  gObj->prev_speed = gObj->speed;
  addVectors (&gObj->speed, &ac);
  }
}

void travar (struct gameObject* gObj) {
  if(normaVector(&gObj->speed) < 0.5){
    gObj->speed = v(0,0);
    return;
  }

  struct Vector acn = v ( gObj->speed.x, gObj->speed.y);
  multVectors(&acn,-0.02);
  gObj->prev_speed = gObj->speed;
  addVectors(&gObj->speed, &acn);
}

void rodarEsquerda(struct gameObject* gObj) {
  gObj->pre_ang = gObj->ang;
  gObj->ang -= 0.03;
}

void rodarDireita(struct gameObject* gObj) {
  gObj->pre_ang = gObj->ang;
  gObj->ang += 0.03;
}

Vector getAtrito(struct gameObject* gObj) {
  Vector atrito;
  atrito = gObj->speed;
  multVectors(&atrito, -normaVector(&atrito)*gObj->atrito);
  return atrito;
}

bool Overlap(double a, double b, double c) {
  if (b <= a || c >= a) {
    return true;
  }
  else return false;
}

double max(double a, double b){
  return (a > b) ? a : b;
}
double min(double a, double b){
  return (a < b) ? a : b;
}

bool collisionRectangles(struct gameObject* gObj1, struct gameObject* gObj2) {

  double cosseno = cos(gObj1->ang + PI/2);
  double seno = sin(gObj1->ang + PI/2 );

  double x1 = gObj1->pos.x , y1 = gObj1->pos.y;
  double w1 = gObj1->shape.r.width , h1 = gObj1->shape.r.height ;


  Vector points1[4] = { v(x1 + ( w1 * cosseno - h1 * seno)/2 , y1 + ( w1 * seno + h1 * cosseno)/2),
                        v(x1 + ( w1 * cosseno + h1 * seno)/2 , y1 + ( w1 * seno - h1 * cosseno)/2),
                        v(x1 + ( -w1 * cosseno + h1 * seno)/2 , y1 + ( -w1 * seno - h1 * cosseno)/2),
                        v(x1 + ( -w1 * cosseno - h1 * seno)/2 , y1 + ( -w1 * seno + h1 * cosseno)/2)
                      };

  cosseno = cos(gObj2->ang );
  seno = sin(gObj2->ang );

  double x2 = gObj2->pos.x , y2 = gObj2->pos.y;
  double w2 = gObj2->shape.r.width , h2 = gObj2->shape.r.height ;


  Vector points2[4] = { v(x2 + ( w2 * cosseno - h2 * seno)/2 , y2 + ( w2 * seno + h2 * cosseno)/2),
                        v(x2 + ( w2 * cosseno + h2 * seno)/2 , y2 + ( w2 * seno - h2 * cosseno)/2),
                        v(x2 + ( -w2 * cosseno + h2 * seno)/2 , y2 + ( -w2 * seno - h2 * cosseno)/2),
                        v(x2 + ( -w2 * cosseno - h2 * seno)/2 , y2 + ( -w2 * seno + h2 * cosseno)/2)
                      };


   for(int i = 0 ; i < 2 ;i ++){
     Vector *points = ( i == 0 ) ? points1 :  points2;
     for(int a = 0; a < 4 ; a++){
       int b = (a+1)%4;
       Vector p1 = points[a];
       Vector p2 = points[b];
       Vector normal = v(p2.y - p1.y , p1.x - p2.x);
       double minA = FLT_MAX;
       double maxA = FLT_MIN;
       for(int c = 0; c < 4;c++){
         double projected = normal.x * points1[c].x + normal.y * points1[c].y;
         if(projected < minA)
          minA = projected;
        if(projected > maxA)
          maxA = projected;
       }
        double minB = FLT_MAX;
        double maxB = FLT_MIN;
      for(int c = 0; c < 4;c++){
        double projected = normal.x * points2[c].x + normal.y * points2[c].y;
        if(projected < minB)
          minB = projected;
        if(projected > maxB)
          maxB = projected;
      }
     if(maxA < minB || maxB < minA)
        return false;
    }
  }
 return true;
}

Vector ballRotation(struct Vector posRect, struct Vector posCircle, double ang) {
  Vector ballinho;
  ballinho = posCircle;
  subVectors(&ballinho, &posRect);
  ballinho = multVectorMatriz(&ballinho, cos(ang), -sin(ang), sin(ang), cos(ang));

  addVectors(&ballinho, &posRect);
  return ballinho;

}



bool collisionRectangleCircle(struct gameObject* gObj1, struct gameObject* gObj2){
  struct Vector ball;
  struct Vector tlvRect = v(gObj1->pos.x -(gObj1->shape.r.width )/2, gObj1->pos.y - (gObj1->shape.r.height )/2);

  ball = ballRotation(gObj1->pos, gObj2->pos, -gObj1->ang);

  double DeltaX = ball.x - max(tlvRect.x, min(ball.x, tlvRect.x + gObj1->shape.r.width ));
  double DeltaY = ball.y - max(tlvRect.y, min(ball.y, tlvRect.y + gObj1->shape.r.height));
  return (DeltaX * DeltaX + DeltaY * DeltaY) < (gObj2->shape.c.r * gObj2->shape.c.r) ;
}

bool collisionCircles(struct gameObject* gObj1, struct gameObject* gObj2) {
  Vector temp = gObj1->pos;
  subVectors(&temp, &gObj2->pos);
  double norma = normaVector(&temp);
  if (norma <= (gObj1->shape.c.r + gObj2->shape.c.r)) {
    return true;
  }
  else return false;
}


bool collision (struct gameObject* gObj1, struct gameObject* gObj2) {
  if (gObj1->type == 0 && gObj2->type == 0) {
    return collisionRectangles(gObj1, gObj2);
  }
  else if (gObj1->type == 1 && gObj2->type == 0) {
    return collisionRectangleCircle(gObj2,gObj1);
  }
  else if (gObj1->type == 0 && gObj2->type == 1) {
    return collisionRectangleCircle(gObj1,gObj2);
  }
  else if (gObj1->type == 1 && gObj2->type == 1) {
    return collisionCircles(gObj1,gObj2);
  }
  return false;
}






void checkHandleCollision (struct gameObject* gObj1, struct gameObject* gObj2){

  if (gObj1->type == 0 && gObj2->type == 0) {
    checkHandleCollisionRectangles(gObj1, gObj2);
    return;
  }
else if (gObj1->type == 1 && gObj2->type == 0) {
    checkHandlecollisionRectangleCircle(gObj2,gObj1);
    return;
  }
  else if (gObj1->type == 0 && gObj2->type == 1) {
    checkHandlecollisionRectangleCircle(gObj1,gObj2);
    return;
  }/*
  else if (gObj1->type == 1 && gObj2->type == 1) {
    return collisionCircles(gObj1,gObj2);
  }*/

}
bool checkHandleCollisionRectangles(struct gameObject* gObj1, struct gameObject* gObj2){


  double cosseno = cos(gObj1->ang + PI/2);
  double seno = sin(gObj1->ang + PI/2 );

  double x1 = gObj1->pos.x , y1 = gObj1->pos.y;
  double w1 = gObj1->shape.r.width , h1 = gObj1->shape.r.height  ;


  Vector points1[4] = { v(x1 + ( w1 * cosseno - h1 * seno)/2 , y1 + ( w1 * seno + h1 * cosseno)/2),
                        v(x1 + ( w1 * cosseno + h1 * seno)/2 , y1 + ( w1 * seno - h1 * cosseno)/2),
                        v(x1 + ( -w1 * cosseno + h1 * seno)/2 , y1 + ( -w1 * seno - h1 * cosseno)/2),
                        v(x1 + ( -w1 * cosseno - h1 * seno)/2 , y1 + ( -w1 * seno + h1 * cosseno)/2)
                      };

  cosseno = cos(gObj2->ang );
  seno = sin(gObj2->ang );

  double x2 = gObj2->pos.x , y2 = gObj2->pos.y;
  double w2 = gObj2->shape.r.width  , h2 = gObj2->shape.r.height   ;


  Vector points2[4] = { v(x2 + ( w2 * cosseno - h2 * seno)/2 , y2 + ( w2 * seno + h2 * cosseno)/2),
                        v(x2 + ( w2 * cosseno + h2 * seno)/2 , y2 + ( w2 * seno - h2 * cosseno)/2),
                        v(x2 + ( -w2 * cosseno + h2 * seno)/2 , y2 + ( -w2 * seno - h2 * cosseno)/2),
                        v(x2 + ( -w2 * cosseno - h2 * seno)/2 , y2 + ( -w2 * seno + h2 * cosseno)/2)
                      };


   Vector mtv;
   double overlap = FLT_MAX;

   for(int i = 0 ; i < 2 ;i ++){
     Vector *points = ( i == 0 ) ? points1 :  points2;
     for(int a = 0; a < 4 ; a++){
       int b = (a+1)%4;
       Vector p1 = points[a];
       Vector p2 = points[b];
       Vector normal = v(p2.y - p1.y , p1.x - p2.x); //projetar na normal do eixo
       double minA = FLT_MAX;
       double maxA = FLT_MIN;
       for(int c = 0; c < 4;c++){
         double projected = normal.x * points1[c].x + normal.y * points1[c].y;
         if(projected < minA)
          minA = projected;
        if(projected > maxA)
          maxA = projected;
       }
        double minB = FLT_MAX;
        double maxB = FLT_MIN;
      for(int c = 0; c < 4;c++){
        double projected = normal.x * points2[c].x + normal.y * points2[c].y;
        if(projected < minB)
          minB = projected;
        if(projected > maxB)
          maxB = projected;
      }
     if(maxA < minB || maxB < minA){
        return false;
      }
      else{

        double o = fabs((minA < minB) ? minB - maxA : minA - maxB);
        if (o < overlap) {
          overlap = o;
          mtv = normal;
          Vector d = v(gObj1->pos.x - gObj2->pos.x , gObj1->pos.y - gObj2->pos.y);
          if(d.x * mtv.x + d.y * mtv.y < 0){
            mtv = v(-mtv.x , -mtv.y);
          }
        }
      }
    }
  }

  multVectors(&mtv , overlap);
  multVectors(&mtv , 1/normaVector(&mtv));

  Vector paral = v(-mtv.y , mtv.x);


  Vector p_a = gObj1->speed;
  Vector p_b = gObj2->speed;
  Vector v_rel = v(p_b.x - p_a.x , p_b.y - p_a.y);

  multVectors(&paral, 1+(paral.x * v_rel.x + paral.y * v_rel.y));

  double l = mtv.x * v_rel.x + mtv.y * v_rel.y;
  Vector vn = mtv;
  double c = 0.1;

  multVectors(&vn ,  l);
  multVectors(&vn , 1 + c);

  if(!gObj1->fixed){
    gObj1->pos = gObj1->prev_pos;
    gObj1->ang = gObj1->pre_ang;
    addVectors(&gObj1->pos , &mtv);
    addVectors(&gObj1->speed , &vn);
  }
  if(!gObj2->fixed){
    gObj2->pos = gObj2->prev_pos;
    gObj2->ang = gObj2->pre_ang;
    subVectors(&gObj2->pos , &mtv);
    subVectors(&gObj2->speed , &vn);
  }

  return true;
}

bool checkHandlecollisionRectangleCircle(struct gameObject* gObj1, struct gameObject* gObj2){

  struct Vector ball;
  struct Vector tlvRect = v(gObj1->pos.x -(gObj1->shape.r.width + 4)/2, gObj1->pos.y - (gObj1->shape.r.height + 4)/2);

  ball = ballRotation(gObj1->pos, gObj2->pos, -gObj1->ang );

  double DeltaX = ball.x - max(tlvRect.x, min(ball.x, tlvRect.x + gObj1->shape.r.width + 4));
  double DeltaY = ball.y - max(tlvRect.y, min(ball.y, tlvRect.y + gObj1->shape.r.height + 4));
  bool collision = (DeltaX * DeltaX + DeltaY * DeltaY) < (gObj2->shape.c.r * gObj2->shape.c.r) ;
  if(!collision)
    return false;

  Vector mtv = v(DeltaX , DeltaY);
  mtv = multVectorMatriz(&mtv, cos(gObj1->ang), -sin(gObj1->ang), sin(gObj1->ang), cos(gObj1->ang));

  multVectors(&mtv , -1/normaVector(&mtv));

  Vector p_a = gObj1->speed;
  Vector p_b = gObj2->speed;
  Vector v_rel = v(p_b.x - p_a.x , p_b.y - p_a.y);


  double l = mtv.x * v_rel.x + mtv.y * v_rel.y;
  Vector vn = mtv;
  double c = 1;

  multVectors(&vn ,  l);
  multVectors(&vn , 1 + c);

  if(!gObj1->fixed){
    gObj1->pos = gObj1->prev_pos;
    gObj1->ang = gObj1->pre_ang;
    addVectors(&gObj1->pos , &mtv);
    addVectors(&gObj1->pos , &mtv);
    addVectors(&gObj1->speed , &vn);
  }
  if(!gObj2->fixed){
    gObj2->pos = gObj2->prev_pos;
    gObj2->ang = gObj2->pre_ang;
    subVectors(&gObj2->pos , &mtv);
    subVectors(&gObj2->pos , &mtv);
    subVectors(&gObj2->speed , &vn);
  }

  return true;

}
