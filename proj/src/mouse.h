#pragma once


//int mouse_enable_data_reporting(void);

int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int();

int mouse_disable_data_rep();
int mouse_enable_data_rep();

int mouse_set_stream_mode();
int mouse_enbl_int();
int mouse_dsbl_int();
int mouse_request_packet();
int mouse_set_remote();

//void mouse_ih(void);

//Nao estava inicializada aqui, but...
//int mse_get_status (uint8_t *st);
