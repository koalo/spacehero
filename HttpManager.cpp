/* 
 * This file is part of Spacehero.
 * 
 * Spacehero is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Spacehero is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Spacehero.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "HttpManager.h"

#include <boost/array.hpp>
#include <iostream>
using namespace std;

HttpManager::HttpManager(string server)
  : io_service(), socket(io_service), server(server)
{
  tcp::resolver resolver(io_service);
  tcp::resolver::query query(server, "http");
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  tcp::resolver::iterator end;
  boost::system::error_code error = boost::asio::error::host_not_found;
  while (error && endpoint_iterator != end)
  {
      socket.close();
        socket.connect(*endpoint_iterator++, error);
  }
  if (error)
      throw boost::system::system_error(error);
}

bool HttpManager::send(string page)
{
  string fromlevel = str();
  string tolevel = "";

  unsigned int pos;
  for(pos = 0; pos < fromlevel.length(); pos++)
  {
    char ch = fromlevel.at(pos);
    if(ch == ' ')
    {
      tolevel += "%20";
    }
    else if(ch == '\n')
    {
      tolevel += "%0A";
    } 
    else if((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))
    {
      tolevel += ch;
    }
  }

  stringstream message;
  message << "POST ";
  message << page;
  message << " HTTP/1.1\r\n";
  message << "Host: " + server + "\r\n";
  message << "Connection: close\r\n";
  message << "Content-Type: application/x-www-form-urlencoded\r\n";
  message << "Content-Length: ";
  message << tolevel.length()+6; 
  message << "\r\n";
  message << "\r\nlevel=";
  message << tolevel;
  message << "\r\n\r\n";

  //const boost::regex levelname("G");
  //level = boost::regex_replace(level, levelname, "ALLE", boost::match_default | boost::format_sed);

  cout << message.str() << endl;
  boost::system::error_code ignored_error;
  boost::asio::write(socket, boost::asio::buffer(message.str()),
    boost::asio::transfer_all(), ignored_error);

  // TODO COULD LEAD TO DEAD!!!!!!
  stringstream resultstream;
  while(true)
  {
    boost::array<char, 128> buf;
    boost::system::error_code error;
    size_t len = socket.read_some(boost::asio::buffer(buf), error);
    if (error == boost::asio::error::eof)
      break; // Connection closed cleanly by peer.
    else if (error)
      throw boost::system::system_error(error); // Some other error.
    resultstream.write(buf.data(), len);
  }
 
  string result = resultstream.str();
  pos = result.find("\r\n\r\n");
  if(pos != std::string::npos)
  {
    cout << result.substr(pos+4) << endl;
  }  
  
  cout << tolevel << endl;

  
  /*if(result.substr(pos+4) == tolevel)
  {
    cout << "OK" << endl;
  } else {
    cout << "Failed" << endl;
  }*/
  return true;
}
