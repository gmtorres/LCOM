#include "game.h"



static uint8_t key_read = 0x00;
uint8_t keyAsm = 0x81;
static uint8_t mouse_byte_read = 0x00;

typedef enum {MAIN_MENU , LEVEL_SELECTOR , COUNTDOWN , GAME , PAUSE , EXIT ,CONNECTING, WAITING_FOR_PLAYER, END_OF_RACE} gameState;


uint8_t date[6];

void rtc_ih(){

  uint32_t regC;
  sys_outb(RTC_ADDR_REG, RTC_REG_C);
  sys_inb(RTC_DATA_REG, &regC);
  read_rtc_time(date);
}


int sp_value = 0;
uint8_t w_n = 0;
bool w_ready = false;


uint8_t sp1_read = 0;

void sp1_int_handler(){
  uint32_t iir;
  uint8_t *ch = (uint8_t*)&sp_value;
  sys_inb(COM1 + IIR, &iir);
  if( !(iir & NO_INT) ) {
    switch( iir & IIR_INT_ID ) {
      case RECEIVED_DATA_AVAILABLE :

      sp1_receive_char(ch + 3 - w_n);
      //sp1_read_char();
      //*(ch + 3 - w_n) = sp1_read;
      w_n++;
      if(w_n == 4)
        w_ready = true;
      break;
      default:break;
    }
  }
}





void game(){


  gameState gameState = MAIN_MENU;


  uint8_t bit_no;


  int ipc_status;
  message msg;

  size_t r;


  int sp1_value = 0;

  bool server = false;
  bool multiplayer = false;


  void* v_adr = vg_init(0x117);
  if(v_adr == NULL){
    printf("Error adressing virtual memory for video card.\n");
    vg_set_text();
    gameState = EXIT;
    //return ;
  }
  initiateNumbers();


  Mouse pointer = msInit();
  SpriteBmp * aim = sBmpInit("/home/lcom/labs/proj/resources/images/utilities/aim.bmp");
  SpriteBmp * pnt = sBmpInit("/home/lcom/labs/proj/resources/images/utilities/pointer.bmp");

  setImage(&pointer , pnt);

  struct Pista *pista = NULL;

  SpriteBmp * pause_menu = sBmpInit("/home/lcom/labs/proj/resources/images/menus/pause_menu.bmp");
  SpriteBmp * main_menu = sBmpInit("/home/lcom/labs/proj/resources/images/menus/main_menu.bmp");
  SpriteBmp * connecting = sBmpInit("/home/lcom/labs/proj/resources/images/menus/connecting.bmp");
  SpriteBmp * waiting = sBmpInit("/home/lcom/labs/proj/resources/images/menus/waiting.bmp");
  SpriteBmp * level_selector = sBmpInit("/home/lcom/labs/proj/resources/images/menus/level_selector.bmp");
  SpriteBmp * end_of_race_menu = sBmpInit("/home/lcom/labs/proj/resources/images/menus/end_of_race_menu.bmp");

  uint32_t countdown = 180;



  mouse_enbl_int();

  sp1_config();

  keyboard_subscribe_int(&bit_no);
  int irq_set_kbd = BIT(bit_no);
  timer_subscribe_int(&bit_no);
  int irq_set_timer0 = BIT(bit_no);
  mouse_subscribe_int(&bit_no);
  int irq_set_mouse = BIT(bit_no);
  rtc_subscribe_int(&bit_no);
  int irq_set_rtc = BIT(bit_no);

  sp1_subscribe_int(&bit_no);
  int irq_set_sp1 = BIT(bit_no);

  write_rtc(11,0x16);

  int dois = 0;
  //static int t_connect = 0;

  //SpriteBmp * light = sBmpInit("/home/lcom/labs/proj/resources/images/light_yellow.bmp");

  while(gameState != EXIT) {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          if (msg.m_notify.interrupts & irq_set_timer0) {


            static int a = 0;
            a++;
          /*  if(a%60 == 0){

            printf("%x\t%c\t", sp1_value , (uint8_t)sp1_value );

            if(gameState == CONNECTING)
              printf("%s", "CONNECTING");
            if(gameState == WAITING_FOR_PLAYER)
              printf("%s", "WAITING_FOR_PLAYER");
            if(gameState == MAIN_MENU)
              printf("%s", "MAIN_MENU");
            if(gameState == LEVEL_SELECTOR)
                printf("%s", "LEVEL_SELECTOR");
            printf("\n");
          }*/

            switch(gameState) {
              case GAME:
                //printf("%d\t%d\n", (int)pista->carros[0].gO.pos.x ,(int)pista->carros[0].gO.pos.y );
                updatePista(pista);
                drawMouse(&pointer);
                //printf("%d\n" , checkEndOfRace(pista));
                if(checkEndOfRace(pista)){
                  //if(!multiplayer){
                    gameState = END_OF_RACE;
                    setImage(&pointer,pnt);
                    //pista->carros[0].ai = true;
                // }
                }
                if(multiplayer){
                  if(server){
                    if(a%5 == 0){
                      sp1_send_int(0x80000000);
                      sp1_send_int(((short)pista->carros[0].gO.pos.x << 16) | ((short)pista->carros[0].gO.pos.y));
                      sp1_send_int(0x80000001);
                      sp1_send_int(((short)((int)(pista->carros[0].gO.ang * 10000)%62831))<<16 | ((((signed char)(pista->carros[0].gO.speed.x * 8))&0x000000FF)<<8) | (((signed char)(pista->carros[0].gO.speed.y * 8))&0x000000FF));
                      //printf("%x\n", ((char)(pista->carros[0].gO.speed.x * 8))&0x000000FF);
                      //sp1_send_int( (((char)(pista->carros[0].gO.speed.x * 8))<<8));

                    }
                    if(a%6 == 0){
                      sp1_send_int(0x80000002);
                      sp1_send_int(((short)pista->carros[1].gO.pos.x << 16) | ((short)pista->carros[1].gO.pos.y));
                      sp1_send_int(0x80000003);
                      sp1_send_int(((short)((int)(pista->carros[1].gO.ang * 10000)%62831))<<16 | ((((signed char)(pista->carros[1].gO.speed.x * 8))&0x000000FF)<<8) | (((signed char)(pista->carros[1].gO.speed.y * 8))&0x000000FF));
                      //printf("%d\n", (signed char)(pista->carros[0].gO.speed.x * 1));
                    }
                  }
                }
              break;

              case CONNECTING:
              /*t_connect++;
              if(t_connect > 300){
                gameState = MAIN_MENU;
                t_connect = 0;
                sp1_value = 0;
                break;
              }*/
              sp1_send_char2('?');
              drawSpriteBmp(connecting,0,0);
              drawMouse(&pointer);
              break;

              case WAITING_FOR_PLAYER:
            /*  t_connect++;
              if(t_connect > 900){
                gameState = MAIN_MENU;
                t_connect = 0;
                sp1_value = 0;
                break;
              }*/
              //sp1_send_char2('y');
              drawSpriteBmp(waiting,0,0);
              drawMouse(&pointer);
              break;

              case LEVEL_SELECTOR:
              drawSpriteBmp(level_selector,0,0);
              drawMouse(&pointer);
              break;

              case MAIN_MENU:
              multiplayer = false;
              drawSpriteBmp(main_menu,0,0);
              drawMouse(&pointer);
              break;

              case COUNTDOWN:
              desenharPista(pista);
              draw_digit(countdown/60 , 400, 400,0);
              countdown--;
              if (!countdown) {
                gameState = GAME;
              }
              break;
              case PAUSE:
              desenharPista(pista);
              //applyMask(0x0000,190);
              //applyMask(0x01CC,190);
              drawSpriteBmp(pause_menu, 262, 184);
              setPadding(0);
              //draw_big_number(date[2], 20, 40 );
              //draw_big_number(date[1], 100, 40 );
              //draw_big_number(date[0], 180, 40 );
              draw_hour(20,40 , date[2] , date[1],date[0]);
              setPadding(10);
              drawMouse(&pointer);
              break;
              case END_OF_RACE:
                desenharPista(pista);
                drawSpriteBmp(end_of_race_menu, 300 , 180);
                unsigned int a = getPosition(pista);
                draw_digit(a,505,325,1);
                draw_ending_time(pista->ticks , 480,380);
                drawMouse(&pointer);
              break;
              default: break;
            }

            vg_show();


          }
          if (msg.m_notify.interrupts & irq_set_kbd) {
            static bool um = false;
            kbc_ih();
            static uint8_t bytes[2];
            if(dois == 1){
              //uint8_t bytes[2];
              bytes[0]= SET2_SCANCODE;
              bytes[1] = key_read;
              um = true;
              dois = 0;
              //getInputKbd(pista,bytes[1]);
            }
            else if(key_read == SET2_SCANCODE){
              dois = 1;
            }else{
              //uint8_t bytes[1];
              bytes[0] = key_read;
              dois = 0;
              um = false;
              //receberTecla(&car,bytes[0]);
              //kbd_print_scancode((key_read & CHECK_MSB) == 0x00 , 1 , bytes);
              //getInputKbd(pista,bytes[0]);
            }
            if (dois == 0) {
              switch (gameState) {
              case GAME:
                if (bytes[um] == ESC_KEY) {
                  gameState = PAUSE;
                  setImage(&pointer,pnt);
                  if(multiplayer){
                    sp1_send_char2(0x1B);
                  }
                }
                else {
                  getInputKbd(pista, bytes[um]);
                  if(multiplayer){
                    if(!server)
                    sp1_send_char2(bytes[um]);
                  }
                }
                break;
                case MAIN_MENU:
                  if(bytes[um] == ESC_KEY){
                    gameState = EXIT;
                  }
                break;
                case WAITING_FOR_PLAYER:
                case CONNECTING:
                case LEVEL_SELECTOR:
                case END_OF_RACE:
                if(bytes[um] == ESC_KEY){
                  gameState = MAIN_MENU;
                  sp1_value = 0;
                  setImage(&pointer,pnt);
                  freePista(pista);
                }
                break;
              default:
                break;
              }

            }
            if(gameState == EXIT)
              break;
            //kbd_print_scancode((key_read & CHECK_MSB) == 0x00 , 1 , bytes);
          }

          if (msg.m_notify.interrupts & irq_set_mouse) {

            //printf("a\n");z

            static struct packet pp;
            static uint8_t counter = 0;
            mouse_ih();
            if(counter == 0){
              if(check_bytes(mouse_byte_read,BIT(3)) == 0)
                continue;
              }
            pp.bytes[counter] = mouse_byte_read;
            uint8_t temp;
            switch (counter) {
              case 0:
              pp.y_ov = check_bytes(mouse_byte_read,BIT(7));
              pp.x_ov = check_bytes(mouse_byte_read,BIT(6));
              pp.mb = check_bytes(mouse_byte_read,BIT(2));
              pp.rb = check_bytes(mouse_byte_read,BIT(1));
              pp.lb = check_bytes(mouse_byte_read,BIT(0));
              break;
              case 1:
                temp = check_bytes(pp.bytes[0],BIT(4)) ?  ~(mouse_byte_read)+1 : mouse_byte_read;
                pp.delta_x = check_bytes(pp.bytes[0],BIT(4)) ? -temp : temp;
                if(pp.delta_x == 0 && check_bytes(pp.bytes[0],BIT(4)))
                  pp.delta_x = -256;
                break;
              case 2:
              temp = check_bytes(pp.bytes[0],BIT(5)) ?  ~(mouse_byte_read)+1 : mouse_byte_read;
              pp.delta_y = check_bytes(pp.bytes[0],BIT(5)) ? -temp : temp;
              if(pp.delta_y == 0 && check_bytes(pp.bytes[0],BIT(5)))
                pp.delta_y = -256;
              break;
              default: break;
            }
            counter ++;
            if(counter % 3 == 0){
              packetUpdate(&pp, &pointer);
              switch (gameState) {
                case GAME:
                getInputMouse(pista, &pointer);
                break;
                case PAUSE :
                if (pointer.event == LB_PRESSED) {
                  if (pointer.pos.x >= 292 - *pointer.sp->width/2 && pointer.pos.x <= 732 + - *pointer.sp->width/2 && pointer.pos.y >= 224  - *pointer.sp->height/2 && pointer.pos.y <= 369 + *pointer.sp->height/2) {
                    if(multiplayer){
                      sp1_send_char2('g');
                    }

                    gameState = GAME;
                    setImage(&pointer,aim);
                  }
                  else if (pointer.pos.x >= 292 - *pointer.sp->width/2 && pointer.pos.x <= 732 + - *pointer.sp->width/2 && pointer.pos.y >= 399 - *pointer.sp->height/2 && pointer.pos.y <= 544 + *pointer.sp->height/2) {
                    if(multiplayer){
                      sp1_send_char2('e');
                    }
                    gameState = MAIN_MENU;
                    sp1_value = 0;
                    setImage(&pointer,pnt);
                    freePista(pista);
                  }
                }
                break;
                case MAIN_MENU:
                if (pointer.event == LB_PRESSED) {
                  if (pointer.pos.x >= 320 - *pointer.sp->width/2 && pointer.pos.x <= 700 + - *pointer.sp->width/2 && pointer.pos.y >= 350  - *pointer.sp->height/2 && pointer.pos.y <= 440 + *pointer.sp->height/2) {
                    gameState = LEVEL_SELECTOR;
                  }
                  if (pointer.pos.x >= 320 - *pointer.sp->width/2 && pointer.pos.x <= 700 + - *pointer.sp->width/2 && pointer.pos.y >= 530  - *pointer.sp->height/2 && pointer.pos.y <= 600 + *pointer.sp->height/2) {
                    multiplayer = true;
                    //recebeu a pergunta do outro que estava á espera, entao aceita a conecção e vai esperar pela resposta
                    if(sp1_value == '?'){
                      sp1_value = 0;
                      gameState = WAITING_FOR_PLAYER;
                      sp1_send_char2('y');
                      sp1_send_char2('y');
                      server = false;
                    }else{
                      server = true;
                      gameState = CONNECTING;
                    }
                  }
                }
                break;
                case LEVEL_SELECTOR:
                if (pointer.event == LB_PRESSED) {
                  if (pointer.pos.x >= 225 - *pointer.sp->width/2 && pointer.pos.x <= 405 + - *pointer.sp->width/2 && pointer.pos.y >= 330  - *pointer.sp->height/2 && pointer.pos.y <= 505 + *pointer.sp->height/2) {
                    pista = pistaInit("/home/lcom/labs/proj/resources/pista3.txt" , multiplayer ,server);
                    getDate(pista,date);
                    gameState = COUNTDOWN;
                    countdown = 180;
                    setImage(&pointer,aim);
                    if(multiplayer){
                      sp1_send_char2('3');
                      //printf("Sent pista 3\n");
                    }
                  }
                  if (pointer.pos.x >= 590 - *pointer.sp->width/2 && pointer.pos.x <= 760 + - *pointer.sp->width/2 && pointer.pos.y >= 330  - *pointer.sp->height/2 && pointer.pos.y <= 505 + *pointer.sp->height/2) {
                    pista = pistaInit("/home/lcom/labs/proj/resources/pista4.txt" , multiplayer,server);
                    getDate(pista,date);
                    gameState = COUNTDOWN;
                    countdown = 180;
                    setImage(&pointer,aim);
                    if(multiplayer){
                      sp1_send_char2('4');
                    }
                  }
                }
                break;
                case END_OF_RACE:
                  if (pointer.event == LB_PRESSED) {
                    if (pointer.pos.x >= 382 - *pointer.sp->width/2 && pointer.pos.x <= 620 + - *pointer.sp->width/2 && pointer.pos.y >= 460 - *pointer.sp->height/2 && pointer.pos.y <= 520 + *pointer.sp->height/2) {
                      if(multiplayer){
                        sp1_send_char2('e');
                      }
                      gameState = MAIN_MENU;
                      sp1_value = 0;
                      setImage(&pointer,pnt);
                      freePista(pista);
                    }
                  }
                default: break;
              }
              //getInputMouse(pista , &pointer);
              //mouse_print_packet(&pp);
              counter = 0;
            }
          }

          if(msg.m_notify.interrupts & irq_set_rtc){

            rtc_ih();
            //printf("RTC\n");
            switch (gameState) {
              case COUNTDOWN:
              case PAUSE:
              case GAME:
                getDate(pista,date);
              break;
              default: break;
            }

          }
          if (msg.m_notify.interrupts & irq_set_sp1) {
            //printf("Sp1\n");
            sp1_int_handler();
            //printf("%c\n", sp1_value );
            if(w_ready == true){
              sp1_value = sp_value;
              w_ready = false;
              w_n = 0;
              //printf("%x\n", sp1_value );

              /*FLAGS
              0x00000000 -> esperar
              0x80000000 -> enviar posicão x do primeiro carro
              0x80000001 -> enviar posicão y do primeiro carro


              */
              static int flag = 0;



              switch (gameState) {
                case GAME:

                  if(multiplayer ){

                    if(sp1_value == 0x1B){
                      gameState = PAUSE;
                      setImage(&pointer,pnt);
                    }else{


                      if(!server){
                        //printf("%x\n",sp1_value );
                        if(sp1_value == (int)0x80000000 || sp1_value == (int)0x80000001 || sp1_value == (int)0x80000002 || sp1_value == (int)0x80000003){
                          flag = sp1_value;
                        }
                        else if(flag == (int)0x80000000){
                          pista->carros[1].gO.pos.x = sp1_value >> 16;
                          pista->carros[1].gO.pos.y = (short)sp1_value;
                          flag = 0;
                        }else if(flag == (int)0x80000001){
                          pista->carros[1].gO.ang = ((unsigned short)(sp1_value>>16))/10000.0;
                          pista->carros[1].gO.speed.x = ((double)((signed char)((sp1_value & 0x0000FF00) >> 8)))/8.0;
                          pista->carros[1].gO.speed.y = ((double)((signed char)((sp1_value & 0x000000FF))))/8.0;
                          flag = 0;
                          //printf("%x\n", ((sp1_value & 0x0000FF00) >> 8));
                          //printf("%d\n\n", pista->carros[1].gO.speed.y);
                        }
                        else if(flag == (int)0x80000002){
                          pista->carros[0].gO.pos.x = sp1_value >> 16;
                          pista->carros[0].gO.pos.y = (short)sp1_value;
                          flag = 0;
                        }else if(flag == (int)0x80000003){
                          pista->carros[0].gO.ang = ((unsigned short)(sp1_value>>16))/10000.0;
                          pista->carros[0].gO.speed.x = ((double)((signed char)((sp1_value & 0x0000FF00) >> 8)))/8.0;
                          pista->carros[0].gO.speed.y = ((double)((signed char)((sp1_value & 0x000000FF))))/8.0;
                          flag = 0;
                          //printf("%d\n", (int)pista->carros[0].gO.speed.x);
                          //printf("%d\n\n", (int)pista->carros[0].gO.speed.y);
                        }
                      }else if(server)
                        getInputSp1(pista,sp1_value);
                    }
                    break;


                  }



                  break;
                case CONNECTING:
                  if(sp1_value == 'y'){
                    sp1_value = 0;
                    gameState = LEVEL_SELECTOR;
                    server = true;
                    sp1_send_char2('k');
                  }
                  break;
                case WAITING_FOR_PLAYER :
                if(sp1_value == '3' || sp1_value == '4'){
                  if(sp1_value == '3')
                    pista = pistaInit("/home/lcom/labs/proj/resources/pista3.txt", multiplayer,server);
                  if(sp1_value == '4')
                    pista = pistaInit("/home/lcom/labs/proj/resources/pista4.txt" , multiplayer,server);
                  getDate(pista,date);
                  gameState = COUNTDOWN;
                  countdown = 180;
                  setImage(&pointer,aim);
                  sp1_value = 0;
                }
                break;
                case PAUSE :
                  if(multiplayer){
                    if(sp1_value == 'g'){
                      gameState = GAME;
                      setImage(&pointer,aim);
                    }
                    if(sp1_value == 'e'){
                      gameState = MAIN_MENU;
                      sp1_value = 0;
                      setImage(&pointer,pnt);
                    }
                  }
                default : break;
              }




            }


            //}

          }
          default:
          break;

        }
      }else{
        /**/
      }
    }
  mouse_unsubscribe_int();
  mouse_dsbl_int();
  timer_unsubscribe_int();
  keyboard_unsubscribe_int();
  rtc_unsubscribe_int();
  sp1_unsubscribe_int();
  vg_set_text();

  mouse_dsbl_int();

  sys_outb(0x64,0x60);
  sys_outb(0x60,minix_get_dflt_kbc_cmd_byte());


  freePista(pista);
  freeNumbers();

}

void (kbc_ih)(void){

  uint8_t kbd_status;
  kbd_get_status(&kbd_status);
  uint32_t key;
  if (!check_bytes(kbd_status,CHECK_RCV_ERROR) && !check_bytes(kbd_status,CHECK_MSB) ){
    sys_inb(KBD_OUT_BUF, &key);
    key_read = (uint8_t) key;
  }
}

void (mouse_ih)() {

  uint32_t ob;
  sys_inb (KBD_OUT_BUF, &ob);
  mouse_byte_read = (uint8_t) ob;
}
