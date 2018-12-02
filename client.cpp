#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

int main(int argc, int argv[])
{
   try{
      // Replace "localhost" with the hostname
      // that you're running your server.
      ClientSocket client_socket("cs.uleth.ca", 30000);
      std::string reply;
      std::string data;
      std::string data_c;
      std::string data_rep;
      int parity = 0;
      int dec = 0;
      // Usually in real applications, the following
      // will be put into a loop.
      // put in a for loop cause we have a max transmission of 1000 lines
      try {
	 for (int i = 0; i < 1000; i++)
	 {
	    //get the frame from the server
	    client_socket >> reply;
	    //create the parity for the frame
	    data = reply;
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
	       else
		  bin_len[0] = 0;
	    }
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
            //get the parity from the server
	    client_socket << "got it";
	    client_socket >> reply;
	    data_rep = reply;
	    if (data_c == data_rep)
	       client_socket << "ACK";
	    else
	       client_socket << "NAK";
	 }
      }
      catch(SocketException&){
      }
      std::cout << "We received this response from the server:\n\""
		<< reply << "\"\n";
   }
   catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\n";
   }

   return 0;
}
