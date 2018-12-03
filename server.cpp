#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <stdlib.h>

int main(int argc, int argv[])
{
   srand (time (NULL));
   std::ifstream ifs;
   ifs.open("input", std::ifstream::in);
   std::cout << "running....\n";
   try{
      // Create the socket
      ServerSocket server(30000);
      
      while (true){
	 int random = rand()%5;
	 
	 ServerSocket new_sock;
	 server.accept(new_sock);
	 
	 // For multiple threading, you need to create
	 // a new thread here and pass new_sock to it.
	 // The thread will use new_sock to communicate
	 // with the client.

	 //moved this out so we only need one instance of the data variable
	 std::string data;
	 std::string data_c;// decimal value of the input
	 int parity=0;
	 int dec = 0;
	 std::string ack;

	 try{
	    int counter = 0;
	    while (!ifs.eof())
	    {
	       counter++;
	       ifs >> data;
	       int bin_len[data.length()*8];
	       //convert the input string to binary
	       for (int i = 0; i < data.length(); i++)
	       {
		  dec = data[i];
		  for (int j = 7; j > 0; j--)
		  {
		     bin_len[(i*8)+j] = dec % 2;
		     dec = dec / 2;
		  }
		  //place the 0 in the parity bit
		  bin_len[i*8] = 0;
	       }
	       for (int i = 0; i < (sizeof(bin_len)/sizeof(*bin_len)); i++)
	       {
		  //increment the parity for every 1 there is in the array
		  if (bin_len[i] == 1)
		     parity++;
		  
		  //if the parity mod 2 is 1, so odd,
		  //place a 1 in the parity spot of the first
		  //bit
		  if (parity % 2 == 1)
		     bin_len[0] = 1;
		  else if (parity % 2 == 0)
		     bin_len[0] = 0;
		  if (counter == random)
		  {
		     if (bin_len[0] == 1)
			bin_len[0] = 0;
		     else
			bin_len[0] = 1;
		  }
		  
	       }
	       //convert the array of ints to a int
	       
	       for (int i = 0; i < (sizeof(bin_len)/sizeof(*bin_len)); i++)
	       {
		  switch (bin_len[i])
		  {
		     case 0:
			data_c += '0';
			break;
		     case 1:
			data_c += '1';
			break;
		  }
	       }
	       new_sock << data;//send the data
	       new_sock >> data;//wait for a responce
	       std::cout << data << "\n";
	       new_sock << data_c;//send the parity bits
	       new_sock >> ack;//get the ack
	       if (ack == "ACK")
		  //got a ack
		  std::cout << "got the ack\n";
	       else
		  std::cout << "got a NAK\n";
	    }
	 }
	 catch(SocketException&){
	 }
      }
   }
   catch (SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }
  return 0;
}
