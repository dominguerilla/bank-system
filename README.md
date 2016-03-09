#Bank-Client System#
A simulated client-bank interface that allows connected clients to start bank accounts, deposit and withdraw money.

Capable of handling multiple concurrent client connections. Note that all data is stored in memory on the machine running the server program.

##TO USE##
Both the server and client programs are only compatible with Linux.
1. Build the server by running 'make' in bank-system/server
2. Run './server', which will start it on port 5001
3. Build the client by running 'make' in bank-system/client
4. Run './client [HOST NAME] 5001'

###SERVER###
The server should display the list of all accounts and their balances every 20 seconds. If a client is currently using an account, it is specified in the list display. It also is the main display of every command that any client inputs.

###CLIENT###
The client is used to interface with the account database the server manages.
Commands:
  -"open [name]" : Opens an account under the name of [name], with an initial balance of $0.0.
  -"start [name]" : Starts an account session for the account opened under [name]. Necessary for using account specific commands. If another client is using the account, this client will not be able to connect. A client can start only one account at a time.
  -"credit [amount]" : Credits the account in session $[amount].
  -"debit [amount]" : Debits the account in session $[amount].
  -"balance" : Prints the balance of the account in session.
  -"finish" : Closes the account in session, allowing other clients to use the account.
  -"exit" : Exits the program, closing the account session if active.
