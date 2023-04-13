#include "picow_socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "socket_impl.h"
#include "pico/cyw43_arch.h"

//UDP Callbacks
void socket_recv_udp(void * arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t*addr, u16_t port){
    struct socket_impl *state = (struct socket_impl*)arg;

    if (p->tot_len > 0) {
        // Receive the buffer
        //memcpy(&state->buffer, p->payload, p->tot_len > MOBILE_MAX_TRANSFER_SIZE ? MOBILE_MAX_TRANSFER_SIZE : p->tot_len);
        state->buffer_len = pbuf_copy_partial(p, &state->buffer, p->tot_len > MOBILE_MAX_TRANSFER_SIZE ? MOBILE_MAX_TRANSFER_SIZE : p->tot_len, 0);
    }
    pbuf_free(p);

    char srv_ip[46];
    memset(srv_ip,0x00,sizeof(srv_ip));
    sprintf(srv_ip, "%u.%u.%u.%u", addr->addr&0xff, (addr->addr>>8)&0xff, (addr->addr>>16)&0xff, addr->addr>>24);
    ip4addr_aton(srv_ip, &pcb->remote_ip);
    pcb->remote_port = port;
}

//TCP Callbacks
err_t socket_connected_tcp(void *arg, struct tcp_pcb *pcb, err_t err) {
    if (err != ERR_OK) {
        printf("connect failed %d\n", err);
    }else{
         printf("TCP connected!\n");
    }
    return err;
}

void socket_err_tcp(void *arg, err_t err){
    struct socket_impl *state = (struct socket_impl*)arg;
}

err_t socket_accept_tcp(void *arg, struct tcp_pcb *pcb, err_t err){
    struct socket_impl *state = (struct socket_impl*)arg;
}

err_t socket_sent_tcp(void *arg, struct tcp_pcb *pcb, u16_t len){
    struct socket_impl *state = (struct socket_impl*)arg;
}

err_t socket_recv_tcp(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err){
    struct socket_impl *state = (struct socket_impl*)arg;
    
    cyw43_arch_lwip_check();
    if (p->tot_len > 0) {
        // Receive the buffer
        //memcpy(&state->buffer, p->payload, p->tot_len > MOBILE_MAX_TRANSFER_SIZE ? MOBILE_MAX_TRANSFER_SIZE : p->tot_len);
        state->buffer_len = pbuf_copy_partial(p, &state->buffer, p->tot_len > MOBILE_MAX_TRANSFER_SIZE ? MOBILE_MAX_TRANSFER_SIZE : p->tot_len, 0);
    }
    pbuf_free(p);
    return ERR_OK;
}