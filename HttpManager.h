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
#ifndef _HTTPMANAGER_H_
#define _HTTPMANAGER_H_

#include <string>
#include <ostream>
using namespace std;

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

class HttpManager : public stringstream
{
  private:
    boost::asio::io_service io_service;
    tcp::socket socket;
    string server;
  public:
    HttpManager(string server);
    bool send(string page);
};

#endif

