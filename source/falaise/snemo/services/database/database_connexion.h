/// \file snemo/services/database/databse_manager.h
/* Author(s)     :     Emmanuel Chauveau <chauveau@cenbg.in2p3.fr>
 * Creation date : 2020-01-20
 * Last modified : 2020-01-20
 *
 * License:
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public  License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Description:
 *
 *   Database connexion class
 *
 */

#ifndef DATABASE_CONNEXION_H
#define DATABASE_CONNEXION_H 1

#include <string>

namespace database {

  /// \brief Database connexion
  /// Main manager for connexion to DB server
  class connexion
  {
  public:
    
		// Constructors and destructors.
    connexion(const std::string& url = "",
							 const std::string& user = "",
							 const std::string& password = "");

    virtual ~connexion();
		
		void connect();

    // get_url
    // get_user
    // get_pw
    
		// /// Check to see table exists in connected database.
		// bool table_exists (const std::string& tableName);
		// void connect();
		// void disconnect();

		// bool open();
		// bool close(bool force = false);

		// TSQLServer* get_server();
    
	private:  
		// mysqlpp::Connection _database_server_;   
      
  };

} // end of namespace database

/***************
 * OCD support *
 ***************/
#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(database::connexion)

#endif // DATABASE_CONNEXION_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
