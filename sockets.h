#ifndef _SOCKETS_
#define _SOCKETS_

struct addrinfo* addStruct(const char*, const char*);

const char* helpMessage = "List of Commands:\n'hello' \t {HELLO} - Check if server is listening\n'quit' \t {GDBYE} - Disconnect from server\n'create' \t {CREAT} - Create new message box\n'delete' \t {DELBX} - Delete message box\n'open' \t {OPNBX} - Open message box\n'close' \t {CLSBX} - Close message box\n'next' \t {NXTMG} - Get next message\n'put' \t {PUTMG} - Add new message to box\n";

#endif