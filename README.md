##### Linux Socket in C multiplethreads
- SA16225108
- Author: huangshuaishuai
- Specification:
   - 'make' First before each test
   - run the 'server' firstly
   - 'each message add a character to tag the target client it chatting with'
   - client0 and client1 chat in unicast
   - client broadcast to all the client connected to the server
- more specfication in comments will help you to understand

####### Need To Improvement for Tcp connection
- client read and write need 'mutex' or 'lock' to exclusive
- server need to a 'message buffer' or 'log' to cahche to message send to client who is off-line 
