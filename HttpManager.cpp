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

HttpManager::HttpManager(string server) : io_service(), socket(io_service)
{
  tcp::resolver resolver(io_service);
  tcp::resolver::query query(server, 22);
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

void HttpManager::sendHeader(string page)
{
  string message = "BLA";
  message += page;
  //cout << message << endl;
  boost::system::error_code ignored_error;
  boost::asio::write(socket, boost::asio::buffer(message),
          boost::asio::transfer_all(), ignored_error);
}
