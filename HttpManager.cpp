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

  while(error && endpoint_iterator != end)
  {
    socket.close();
    socket.connect(*endpoint_iterator++, error);
  }

  if(error)
  {
    throw boost::system::system_error(error);
  }
}

bool HttpManager::send(string page)
{
  string fromcontent = str();
  string tocontent = "";

  unsigned int pos;
  for(pos = 0; pos < fromcontent.length(); pos++)
  {
    char ch = fromcontent.at(pos);
    if(ch == ' ')
    {
      tocontent += "%20";
    }
    else if(ch == '\n')
    {
      tocontent += "%0A";
    } 
    else if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || (ch == '=') || (ch == '&') || (ch == '_'))
    {
      tocontent += ch;
    }
  }

  boost::asio::streambuf request;
  ostream message(&request);
  message << "POST ";
  message << page;
  message << " HTTP/1.1\r\n";
  message << "Host: " + server + "\r\n";
  message << "Connection: close\r\n";
  message << "Content-Type: application/x-www-form-urlencoded\r\n";
  message << "Content-Length: ";
  message << tocontent.length(); 
  message << "\r\n\r\n";
  message << tocontent;
  message << "\r\n\r\n";
  cout << tocontent;

  boost::system::error_code error;
  boost::asio::write(socket, request, boost::asio::transfer_all(), error);
  if(error)
  {
    throw boost::system::system_error(error);
  }

  // Read the response status line.
  boost::asio::streambuf response;
  boost::asio::read_until(socket, response, "\r\n");

  // Check that response is OK.
  istream response_stream(&response);
  string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  string status_message;
  std::getline(response_stream, status_message);
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
  {
    throw boost::system::system_error(boost::asio::error::operation_not_supported, "Invalid response");
  }
  if (status_code != 200)
  {
    throw logic_error("Response returned with HTTP status code "+status_code);
  }

  // Read the response headers, which are terminated by a blank line.
  boost::asio::read_until(socket, response, "\r\n\r\n");

  // Process the response headers.
  string header;
  while(std::getline(response_stream, header) && header != "\r")
  {
    // will keinen Header
  }

  // Write whatever content we already have to output.
  stringstream returnmessage;
  if (response.size() > 0)
  {
    returnmessage << &response;
  }

  // Read until EOF, writing data to output as we go.
  while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
  {
    returnmessage << &response;
  }

  if (error != boost::asio::error::eof)
  {
    throw boost::system::system_error(error);
  }
  
  if(returnmessage.str() != "OK")
  {
    throw logic_error(returnmessage.str());
  }

  return true;
}

