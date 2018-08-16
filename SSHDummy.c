//SSHDummy v0.3
//using libssh
//Zack Mitchell AUG 2018
//https://github.com/substack/libssh/blob/master/examples/samplesshd.c
//http://api.libssh.org/master/libssh_tutor_guided_tour.html
//gcc SSHDummy.c -lssh -o SSHDummy


#define LIBSSH_STATIC 1
#include <libssh/libssh.h> 
#include <stdlib.h>
#include <stdio.h>
#include <libssh/server.h>
#define KEYS_FOLDER "/etc/ssh/"

void Hello()  {

  printf("SSHDummy v0.3\n\e[92mSoon with threads!!!:)\e[0m\n");
}

int main(int argc, char **argv)  {

  int SomeReturn, auth = 0;

  ssh_session  Session;
  ssh_bind     SSHBind;
  ssh_message  Message;
  ssh_channel  Channel = 0;
 
  Hello();
  
  SSHBind = ssh_bind_new();
  Session = ssh_new();
  
  //more test action VVVV
  //ssh_bind_options_set(SSHBind, SSH_BIND_OPTIONS_DSAKEY, KEYS_FOLDER "ssh_host_dsa_key");
  ssh_bind_options_set(SSHBind, SSH_BIND_OPTIONS_RSAKEY, KEYS_FOLDER "ssh_host_rsa_key");
  
  //ssh_bind_options_set JustPassword.
  
  if ( ssh_bind_listen(SSHBind) < 0 )  {
  
    printf("Error listening on socket: %s\n", ssh_get_error(SSHBind));
    exit(1);
  }
  
  if ( ssh_bind_accept(SSHBind, Session) == SSH_ERROR )  {
  
    printf("Error accepting connection: %s\n", ssh_get_error(SSHBind));
    exit(1);
  }
  
  if ( ssh_handle_key_exchange(Session) != SSH_OK )  { 
  
    printf("ssh_handle_key_exchange: %s\n", ssh_get_error(Session));
    exit(1);
  }
  
  //Test phase...
  //use this guys loop.
  
  
 do {
        Message=ssh_message_get(Session);
        if(!Message)
            break;
        switch(ssh_message_type(Message)){
            case SSH_REQUEST_AUTH:
                switch(ssh_message_subtype(Message)){
                    case SSH_AUTH_METHOD_PASSWORD:
                        printf("*************\nHost: \nUser: %s\nPass: %s\n",           //<<<<---this too.
                               ssh_message_auth_user(Message),
                               ssh_message_auth_password(Message));
                        if(0){							//<<<----tweaked this.
                               auth=1;
                               ssh_message_auth_reply_success(Message,0);
                               break;
                           }
                        // not authenticated, send default message
                    case SSH_AUTH_METHOD_NONE:
                    default:
                        ssh_message_auth_set_methods(Message,SSH_AUTH_METHOD_PASSWORD);
                        ssh_message_reply_default(Message);
                        break;
                }
                break;
            default:
                ssh_message_reply_default(Message);
        }
        ssh_message_free(Message);
} while (!auth);

/*
  ssh_disconnect(Session);
  ssh_bind_free(SSHBind);
  ssh_finalize();
*/
  return(0);
}
