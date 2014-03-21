#define CHAT_PORT 5134
#define CHAT_MAXPEOPLE 1
#define CHAT_PACKETSIZE 256     /* Maximum length of a message */

/* The protocol between the chat client and server */
#define CHAT_HELLO  0   /* 0+Port+len+name */
#define CHAT_HELLO_PORT     1
#define CHAT_HELLO_NLEN     CHAT_HELLO_PORT+2
#define CHAT_HELLO_NAME     CHAT_HELLO_NLEN+1
#define CHAT_ADD    1   /* 1+N+IP+Port+len+name */
#define CHAT_ADD_SLOT       1
#define CHAT_ADD_HOST       CHAT_ADD_SLOT+1
#define CHAT_ADD_PORT       CHAT_ADD_HOST+4
#define CHAT_ADD_NLEN       CHAT_ADD_PORT+2
#define CHAT_ADD_NAME       CHAT_ADD_NLEN+1
#define CHAT_DEL    2   /* 2+N */
#define CHAT_DEL_SLOT       1
#define CHAT_DEL_LEN        CHAT_DEL_SLOT+1
#define CHAT_BYE    255 /* 255 */
#define CHAT_BYE_LEN        1

